/*
 * CustomSkillsMissions.cpp -- BRIEF-043 mission terminal direction/difficulty
 * options. All feature logic lives here; Core3 touches are 1-5 line
 * delegations (see MissionManagerImplementation.cpp / MissionTerminal
 * Implementation.cpp).
 *
 * cyvster2 keys reused verbatim so returning players keep their choices:
 *   mission_level_choice / levelChoice      (int tier value, 0 = Reset)
 *   mission_direction_choice / directionChoice (compass deg, -1 = Reset)
 */

#include "CustomSkillsMissions.h"

#include "../CustomSkillsConfig.h"

#include "server/zone/ZoneServer.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/mission/MissionObject.h"
#include "server/templates/mobile/LairTemplate.h"

#include <mutex>

namespace {
	// Ten tiers verbatim from cyvster2 mission_level_choice.lua (owner
	// decision #3: do not renumber or trim). levelSelect 0 = Reset.
	struct LevelTier { const char* label; int level; };
	const LevelTier LEVEL_TIERS[] = {
		{ "Reset Level Range", 0 },
		{ "Easiest",   1 },
		{ "Mid 1",     2 },
		{ "Mid 2",     12 },
		{ "Mid 3",     25 },
		{ "High 1",    35 },
		{ "High 2",    45 },
		{ "High 3",    60 },
		{ "Hard",      135 },
		{ "Hardest",   200 },
	};
	const int LEVEL_TIER_COUNT = sizeof(LEVEL_TIERS) / sizeof(LEVEL_TIERS[0]);
	const int HIGHEST_LEVEL_TIER = LEVEL_TIER_COUNT - 1;

	// Compass rows verbatim from cyvster2 mission_direction_choice.lua.
	// dirSelect -1 = Reset; degrees follow SWG's world-heading convention.
	struct DirectionRow { const char* label; int heading; };
	const DirectionRow DIRECTION_ROWS[] = {
		{ "Reset mission direction", -1 },
		{ "North",      0 },
		{ "North East", 315 },
		{ "East",       270 },
		{ "South East", 225 },
		{ "South",      180 },
		{ "South West", 135 },
		{ "West",       90 },
		{ "North West", 45 },
	};
	const int DIRECTION_ROW_COUNT = sizeof(DIRECTION_ROWS) / sizeof(DIRECTION_ROWS[0]);

	const char* const DIRECTION_MENU_PROMPT =
		"Use this menu to choose the direction your destroy missions spawn in. "
		"After choosing, use the mission terminal to get a selection of missions.\n\n"
		"When you want the normal random spread again, choose Reset.";

	const char* const LEVEL_MENU_PROMPT =
		"Use this menu to select a mission level range to aim for. After you have "
		"chosen a range, use the mission terminal to get a selection of missions.\n\n"
		"If no missions are shown, it is because no missions in that level range "
		"exist for this planet. You will need to choose another range.\n\n"
		"When you want to go back to 'normal' offering of missions for your skill "
		"level, just choose Reset Level Range.";
}

// --- cached per-player choices -------------------------------------------

class CustomSkillsMissions::Choices {
public:
	bool haveLevel = false;
	int levelChoice = 0; // >0 = override active

	bool haveDirection = false;
	int directionChoice = -1; // >=0 = override active

	void load(server::zone::objects::creature::CreatureObject* player);
};

void CustomSkillsMissions::Choices::load(server::zone::objects::creature::CreatureObject* player) {
	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == nullptr)
		return;

	// Empty-string guards: ScreenPlayData returns "" before any choice;
	// Integer/Float::valueOf("") throws (cyvster2 defect 1).
	String levelData = ghost->getScreenPlayData("mission_level_choice", "levelChoice");
	if (!levelData.isEmpty()) {
		int parsed = Integer::valueOf(levelData);
		if (parsed > 0) {
			haveLevel = true;
			levelChoice = parsed;
		}
	}

	String dirData = ghost->getScreenPlayData("mission_direction_choice", "directionChoice");
	if (!dirData.isEmpty()) {
		int parsed = Integer::valueOf(dirData);
		haveDirection = true;
		directionChoice = parsed; // may be -1 (Reset) -- explicit presence
	}
}

namespace {
	// Keyed by player objectID, populated once per populateMissionList call.
	// Single-threaded executor semantics + clearChoices() after each populate
	// mean entries never outlive their request; the mutex only guards the map
	// itself against terminal-select vs populate interleaving on tick edges.
	std::mutex& choicesMutex() {
		static std::mutex m;
		return m;
	}

	HashMap<uint64, CustomSkillsMissions::Choices*>& choicesMap() {
		static HashMap<uint64, CustomSkillsMissions::Choices*> map;
		return map;
	}

	const CustomSkillsMissions::Choices& emptyChoices() {
		static CustomSkillsMissions::Choices EMPTY;
		return EMPTY;
	}
}

// --- config --------------------------------------------------------------

bool CustomSkillsMissions::isEnabled() {
	return CustomSkillsConfig::instance()->isMissionOptionsEnabled();
}

bool CustomSkillsMissions::isDirectionOptionEnabled() {
	return isEnabled() && CustomSkillsConfig::instance()->isMissionDirectionEnabled();
}

bool CustomSkillsMissions::isDifficultyOptionEnabled() {
	return isEnabled() && CustomSkillsConfig::instance()->isMissionDifficultyEnabled();
}

// --- hooks ---------------------------------------------------------------

const CustomSkillsMissions::Choices& CustomSkillsMissions::getChoices(
		server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isEnabled())
		return emptyChoices();

	std::lock_guard<std::mutex> locker(choicesMutex());

	Choices** entry = choicesMap().getPointer(player->getObjectID());
	if (entry != nullptr)
		return **entry;

	choicesMap().put(player->getObjectID(), new Choices());

	entry = choicesMap().getPointer(player->getObjectID());
	(*entry)->load(player);

	return **entry;
}

void CustomSkillsMissions::cacheChoices(server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isEnabled())
		return;

	getChoices(player); // populates the cache for this request
}

void CustomSkillsMissions::clearChoices(uint64 playerObjectID) {
	std::lock_guard<std::mutex> locker(choicesMutex());

	Choices** entry = choicesMap().getPointer(playerObjectID);
	if (entry != nullptr) {
		delete *entry;
		choicesMap().remove(playerObjectID);
	}
}

int CustomSkillsMissions::getDifficultyOverride(server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isDifficultyOptionEnabled())
		return -1;

	return getChoices(player).levelChoice; // <=0 means no override
}

int CustomSkillsMissions::getLevelOverride(server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isDifficultyOptionEnabled())
		return -1;

	return getChoices(player).levelChoice; // <=0 means no override
}

float CustomSkillsMissions::getMissionHeading(server::zone::objects::creature::CreatureObject* player,
		float fallbackHeading) {
	if (player == nullptr || !isDirectionOptionEnabled())
		return fallbackHeading;

	int dirChoice = getChoices(player).directionChoice;
	if (dirChoice < 0)
		return fallbackHeading;

	// Symmetric deviation around the chosen heading (cyvster2 used a biased
	// 60/40 sign roll; design doc 1.2 item 4 fixes this to +/- N degrees).
	float heading = (float)dirChoice + (float)(System::random(11) - 5);

	if (heading >= 360.f)
		heading -= 360.f;
	else if (heading < 0.f)
		heading += 360.f;

	return heading;
}

int CustomSkillsMissions::getMissionListSize() {
	CustomSkillsConfig* config = CustomSkillsConfig::instance();
	if (!config->isMissionOptionsEnabled())
		return 3;

	int size = config->getMissionListSize();
	return size > 0 ? size : 3;
}

bool CustomSkillsMissions::shouldUseDescriptiveTitles() {
	return isEnabled() && CustomSkillsConfig::instance()->isDescriptiveMissionTitlesEnabled();
}

void CustomSkillsMissions::applyDescriptiveTitle(server::zone::objects::mission::MissionObject* mission,
		int diffDisplay, const server::templates::mobile::LairTemplate* lairTemplate) {
	if (!shouldUseDescriptiveTitles() || mission == nullptr || lairTemplate == nullptr)
		return;

	String groupSuffix;
	if (lairTemplate->getMobType() == LairTemplate::NPC)
		groupSuffix = " camp.";
	else
		groupSuffix = " lair.";

	const VectorMap<String, int>* mobiles = lairTemplate->getMobiles();
	String mobileName = "mysterious";

	if (mobiles != nullptr && mobiles->size() > 0)
		mobileName = mobiles->elementAt(0).getKey();

	mission->setMissionTitle("CL" + String::valueOf(diffDisplay),
			" Destroy the " + mobileName.replaceAll("_", " ") + groupSuffix);
}

// --- persistence ---------------------------------------------------------

void CustomSkillsMissions::persistChoice(server::zone::objects::creature::CreatureObject* player,
		bool directionMenu, int value) {
	if (player == nullptr)
		return;

	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == nullptr)
		return;

	if (directionMenu) {
		ghost->setScreenPlayData("mission_direction_choice", "directionChoice", String::valueOf(value));
	} else {
		ghost->setScreenPlayData("mission_level_choice", "levelChoice", String::valueOf(value));
	}

	// Keep the cache coherent if a regeneration happens before the next
	// populate re-caches (defensive; normally clearChoices handles it).
	{
		std::lock_guard<std::mutex> locker(choicesMutex());
		Choices** entry = choicesMap().getPointer(player->getObjectID());
		if (entry != nullptr) {
			delete *entry;
			choicesMap().remove(player->getObjectID());
		}
	}
}

// --- SUI callbacks -------------------------------------------------------

class MissionOptionsMenuSelectCallback : public SuiCallback {
private:
	bool directionMenu;

public:
	MissionOptionsMenuSelectCallback(ZoneServer* server, bool isDirectionMenu)
		: SuiCallback(server), directionMenu(isDirectionMenu) {
	}

	void run(server::zone::objects::creature::CreatureObject* player, SuiBox* suiBox, uint32 eventIndex,
			Vector<UnicodeString>* args) override {
		if (player == nullptr || suiBox == nullptr || !suiBox->isListBox())
			return;

		SuiListBox* listBox = cast<SuiListBox*>(suiBox);

		// Cancel or malformed submission: just close.
		if (eventIndex == 1 || args == nullptr || args->size() == 0)
			return;

		int selection = Integer::valueOf(args->get(0).toString());
		if (selection < 0 || selection >= listBox->getMenuSize())
			return;

		if (directionMenu && selection < DIRECTION_ROW_COUNT) {
			int heading = DIRECTION_ROWS[selection].heading;
			CustomSkillsMissions::persistChoice(player, true, heading);

			if (heading == 0)
				player->sendSystemMessage("Mission direction has been reset to normal.");
			else
				player->sendSystemMessage(String("You have selected missions to the ")
					+ DIRECTION_ROWS[selection].label
					+ ". This choice will remain active until you change or reset it.");
		} else if (!directionMenu && selection < LEVEL_TIER_COUNT) {
			int tierLevel = LEVEL_TIERS[selection].level;
			CustomSkillsMissions::persistChoice(player, false, tierLevel);

			if (tierLevel == 0)
				player->sendSystemMessage(String("You have selected: ") + LEVEL_TIERS[selection].label
					+ ".  You may now choose missions suited to your own skill/group level.");
			else
				player->sendSystemMessage(String("You have selected: ") + LEVEL_TIERS[selection].label
					+ ".  These missions would be considered suitable for a player or group of level "
					+ String::valueOf(tierLevel) + ".");
		}
	}
};

// --- option menus --------------------------------------------------------

void CustomSkillsMissions::showDirectionMenu(server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isDirectionOptionEnabled())
		return;

	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == nullptr)
		return;

	ManagedReference<SuiListBox*> box =
		new SuiListBox(player, SuiWindowType::NONE, SuiListBox::HANDLETWOBUTTON);
	box->setCallback(new MissionOptionsMenuSelectCallback(player->getZoneServer(), true));
	box->setCancelButton(true, "@cancel");
	box->setOkButton(true, "@ok");
	box->setPromptTitle("Mission direction");
	box->setPromptText(DIRECTION_MENU_PROMPT);

	int current = getChoices(player).directionChoice;

	for (int i = 0; i < DIRECTION_ROW_COUNT; ++i) {
		String row = DIRECTION_ROWS[i].label;
		if (current >= 0 ? DIRECTION_ROWS[i].heading == current : DIRECTION_ROWS[i].heading == -1)
			row += " (current)";
		box->addMenuItem(row);
	}

	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());
}

void CustomSkillsMissions::showDifficultyMenu(server::zone::objects::creature::CreatureObject* player) {
	if (player == nullptr || !isDifficultyOptionEnabled())
		return;

	PlayerObject* ghost = player->getPlayerObject();
	if (ghost == nullptr)
		return;

	ManagedReference<SuiListBox*> box =
		new SuiListBox(player, SuiWindowType::NONE, SuiListBox::HANDLETWOBUTTON);
	box->setCallback(new MissionOptionsMenuSelectCallback(player->getZoneServer(), false));
	box->setCancelButton(true, "@cancel");
	box->setOkButton(true, "@ok");
	box->setPromptTitle("Mission level selection");
	box->setPromptText(LEVEL_MENU_PROMPT);

	int current = getChoices(player).levelChoice;
	(void)HIGHEST_LEVEL_TIER; // full ten-tier list is offered verbatim

	for (int i = 0; i < LEVEL_TIER_COUNT; ++i) {
		String row = LEVEL_TIERS[i].label;
		if ((current > 0 ? LEVEL_TIERS[i].level == current : LEVEL_TIERS[i].level == 0))
			row += " (current)";
		box->addMenuItem(row);
	}

	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());
}
