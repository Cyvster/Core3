#include "CustomSkillsCrafting.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"

int CustomSkillsCrafting::modifyPracticeExperience(CreatureObject* crafter, int nativeAmount) {
	if (nativeAmount <= 0)
		return nativeAmount;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::PRACTICE_EXPERIENCE_BONUS);
	return static_cast<int>((static_cast<int64>(nativeAmount) * (10000 + bonus)) / 10000);
}

int CustomSkillsCrafting::divideDuration(CreatureObject* crafter, int nativeSeconds) {
	// Durations below 1 second are clamped to 1 (documented edge behavior).
	if (nativeSeconds <= 1)
		return 1;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::CRAFTING_SPEED);
	int multiplier = 10000 + bonus;
	int64 scaled = static_cast<int64>(nativeSeconds) * 10000;
	return Math::max(1, static_cast<int>((scaled + multiplier - 1) / multiplier));
}

int CustomSkillsCrafting::getPersonalCraftingDuration(CreatureObject* crafter, int nativeSeconds) {
	return divideDuration(crafter, nativeSeconds);
}

int CustomSkillsCrafting::getFactoryProductionDuration(CreatureObject* activator, int nativeSeconds) {
	return divideDuration(activator, nativeSeconds);
}

int CustomSkillsCrafting::getAmazingChanceBonus(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_SUCCESS_CHANCE);
}

int CustomSkillsCrafting::getAmazingResultsStrength(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_RESULTS);
}

bool CustomSkillsCrafting::shouldPromoteAmazingFailure(CreatureObject* crafter, int nativeChanceBasisPoints) {
	const int bonus = getAmazingChanceBonus(crafter);
	const int nativeChance = Math::min(Math::max(nativeChanceBasisPoints, 0), 10000);
	const int finalChance = Math::min(10000, nativeChance + bonus);
	if (finalChance <= nativeChance || nativeChance >= 10000)
		return false;

	// This roll happens only after the native roll failed. Convert the desired
	// percentage-point increase into its conditional probability so the final
	// chance is nativeChance + configured bonus, not a multiplier.
	const int conditionalChance = static_cast<int>((static_cast<int64>(finalChance - nativeChance) * 10000) /
		(10000 - nativeChance));
	return System::random(9999) < conditionalChance;
}

void CustomSkillsCrafting::applyAmazingResults(CreatureObject* crafter, CraftingValues* values, int result,
		int amazingResult, const String& selectedGroup) {
	if (values == nullptr || result != amazingResult)
		return;

	const int strength = Math::min(getAmazingResultsStrength(crafter), 10000);
	if (strength <= 0)
		return;

	const float factor = strength / 10000.f;
	for (int i = 0; i < values->getTotalExperimentalAttributes(); ++i) {
		const String& attribute = values->getAttribute(i);
		if (!selectedGroup.isEmpty() && values->getAttributeGroup(attribute) != selectedGroup)
			continue;

		const float current = values->getCurrentPercentage(attribute);
		const float improved = current + ((1.f - current) * factor);
		values->setCurrentPercentage(attribute, improved, Math::max(values->getMaxPercentage(attribute), improved));
	}
	values->recalculateValues(selectedGroup.isEmpty());
}

// =============================================================================
// BRIEF-036: repeat-craft assisted pre-fill
//
// Snapshot format (stored on the CraftingTool via TangibleObject's persistent
// luaStringData map -- zero IDL changes, survives restarts; see CODE_REFERENCE):
//   cs36.schematicCrc  -> client object CRC of the draft schematic
//   cs36.slotCount     -> number of ingredient slots in the snapshot
//   cs36.slot.<i>.type -> resource spawn name (resource slots) or template
//                         name + "#" + serial (component slots)
//   cs36.slot.<i>.qty  -> quantity required by that slot
//   cs36.exp           -> experiment "row points row points ..." string
// =============================================================================

#include "server/zone/ZoneServer.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/tangible/tool/CraftingTool.h"
#include "server/zone/objects/player/sessions/crafting/CraftingSession.h"
#include "server/zone/objects/tangible/tool/CraftingStation.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/IngredientSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ResourceSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ComponentSlot.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/customskills/CustomSkillsConfig.h"

static const char* CS36_PREFIX = "cs36.";

static void cs36Put(TangibleObject* tool, const String& key, const String& value) {
	tool->setLuaStringData(CS36_PREFIX + key, value);
}

static String cs36Get(TangibleObject* tool, const String& key) {
	return tool->getLuaStringData(CS36_PREFIX + key);
}

void CustomSkillsCrafting::storeRepeatRecipe(CraftingTool* tool,
		ManufactureSchematic* schematic, const String& expAttempt) {
	if (tool == nullptr || schematic == nullptr)
		return;

	Reference<DraftSchematic*> draft = schematic->getDraftSchematic();
	if (draft == nullptr)
		return;

	cs36Put(tool, "schematicCrc", String::valueOf(draft->getClientObjectCRC()));
	cs36Put(tool, "slotCount", String::valueOf(schematic->getSlotCount()));

	for (int i = 0; i < schematic->getSlotCount(); ++i) {
		IngredientSlot* slot = schematic->getSlot(i);
		if (slot == nullptr)
			continue;

		String key = "slot." + String::valueOf(i) + ".";
		String value;

		ResourceSlot* resSlot = dynamic_cast<ResourceSlot*>(slot);
		if (resSlot != nullptr && resSlot->getCurrentSpawn() != nullptr) {
			value = resSlot->getCurrentSpawn()->getName(); // exact spawn name
		} else {
			ComponentSlot* compSlot = dynamic_cast<ComponentSlot*>(slot);
			TangibleObject* proto = compSlot != nullptr ? compSlot->getPrototype() : nullptr;
			if (proto != nullptr) {
				value = proto->getObjectTemplate()->getFullTemplateString() + "#" + proto->getSerialNumber();
			}
		}

		cs36Put(tool, key + "type", value);
		cs36Put(tool, key + "qty", String::valueOf(slot->getQuantityNeeded()));
	}

	cs36Put(tool, "exp", expAttempt);
}

int CustomSkillsCrafting::doRepeatCraft(CreatureObject* player, uint64 targetID) {
	if (!CustomSkillsConfig::instance()->isRepeatEnabled()) {
		player->sendSystemMessage("Repeat-craft is disabled on this server.");
		return QueueCommand::GENERALERROR;
	}

	auto zoneServer = player->getZoneServer();
	if (zoneServer == nullptr)
		return QueueCommand::GENERALERROR;

	ManagedReference<SceneObject*> target = zoneServer->getObject(targetID);

	CraftingTool* tool = nullptr;
	if (target != nullptr && target->isCraftingTool()) {
		tool = cast<CraftingTool*>(target.get());
	} else {
		// No explicit target: find the first crafting tool in inventory that
		// holds a stored snapshot.
		auto inventory = player->getSlottedObject("inventory");
		if (inventory != nullptr) {
			for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
				auto obj = inventory->getContainerObject(i);
				if (obj != nullptr && obj->isCraftingTool()) {
					auto candidate = cast<CraftingTool*>(obj.get());
					if (candidate != nullptr && !cs36Get(candidate, "schematicCrc").isEmpty()) {
						tool = candidate;
						break;
					}
				}
			}
		}
	}

	if (tool == nullptr || !tool->isASubChildOf(player)) {
		player->sendSystemMessage("Repeat-craft: no crafting tool found.");
		return QueueCommand::GENERALERROR;
	}

	const String crcStr = cs36Get(tool, "schematicCrc");
	if (crcStr.isEmpty()) {
		player->sendSystemMessage("Repeat-craft: this tool has no stored recipe. Craft something first.");
		return QueueCommand::GENERALERROR;
	}

	if (tool->isFinished()) {
		player->sendSystemMessage("@system_msg:crafting_tool_full");
		return QueueCommand::GENERALERROR;
	}

	if (tool->isBusy()) {
		player->sendSystemMessage("@system_msg:crafting_tool_creating_prototype");
		return QueueCommand::GENERALERROR;
	}

	uint32 crc = (uint32)strtoul(crcStr.toCharArray(), nullptr, 10);

	// Resolve a nearby crafting station for this tool type, as vanilla does.
	auto playerMan = zoneServer->getPlayerManager();
	CraftingStation* station =
		playerMan != nullptr ? playerMan->getNearbyCraftingStation(player, tool->getToolType()) : nullptr;

	// Cancel any active crafting session first (mirrors RequestCraftingSessionCommand).
	Reference<CraftingSession*> oldSession =
		player->getActiveSession(SessionFacadeType::CRAFTING).castTo<CraftingSession*>();
	if (oldSession != nullptr) {
		Locker slocker(oldSession);
		oldSession->cancelSession();
	}

	ManagedReference<CraftingSession*> session = new CraftingSession(player);
	Locker locker(tool);

	if (!session->initializeSession(tool, station)) {
		player->sendSystemMessage("@ui_craft@err_no_crafting_tool");
		return QueueCommand::GENERALERROR;
	}

	// Auto-select the snapshotted schematic by CRC within the freshly
	// filtered session list.
	int index = -1;
	for (int i = 0; i < session->getCurrentSchematicListSize(); ++i) {
		DraftSchematic* draft = session->getCurrentSchematic(i);
		if (draft != nullptr && draft->getClientObjectCRC() == crc) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		// Schematic no longer known / wrong tool tabs / complexity mismatch:
		// discard the stale snapshot with notice (brief requirement).
		player->sendSystemMessage("Repeat-craft: schematic no longer available -- stored recipe discarded.");
		tool->deleteLuaStringData(CS36_PREFIX + String("schematicCrc"));
		Locker slock(session);
		session->cancelSession();
		return QueueCommand::GENERALERROR;
	}

	session->selectDraftSchematic(index);

	// ---- Pre-fill: re-validate every slot against live inventory ----------
	ManagedReference<ManufactureSchematic*> manf = session->getSchematic();
	if (manf == nullptr) {
		return QueueCommand::GENERALERROR;
	}

	int snapshotSlots = Integer::valueOf(cs36Get(tool, "slotCount"));
	bool mismatch = false;

	if (manf->getSlotCount() != snapshotSlots) {
		mismatch = true;
	} else {
		for (int i = 0; i < manf->getSlotCount() && !mismatch; ++i) {
			IngredientSlot* slot = manf->getSlot(i);
			if (slot == nullptr || slot->getQuantityNeeded()
					!= Integer::valueOf(cs36Get(tool, "slot." + String::valueOf(i) + ".qty"))) {
				mismatch = true;
			}
		}
	}

	if (mismatch) {
		// Schematic changed shape since the snapshot: discard with notice.
		player->sendSystemMessage("Repeat-craft: recipe no longer matches this schematic -- stored recipe discarded.");
		tool->deleteLuaStringData(CS36_PREFIX + String("schematicCrc"));
		Locker slock(session);
		session->cancelSession();
		return QueueCommand::GENERALERROR;
	}

	// Session::addIngredient resolves the crafted-components satchel itself.

	for (int i = 0; i < manf->getSlotCount(); ++i) {
		IngredientSlot* slot = manf->getSlot(i);
		if (slot == nullptr || slot->isFull())
			continue;

		String wantType = cs36Get(tool, "slot." + String::valueOf(i) + ".type");
		if (wantType.isEmpty())
			continue; // leave empty; player fills manually

		ManagedReference<TangibleObject*> found = nullptr;

		if (slot->isResourceSlot()) {
			// Find matching resource containers on the player.
			auto inventory = player->getSlottedObject("inventory");
			if (inventory != nullptr) {
				for (int j = 0; j < inventory->getContainerObjectsSize() && found == nullptr; ++j) {
					auto obj = inventory->getContainerObject(j);
					if (obj == nullptr || !obj->isResourceContainer())
						continue;
					auto rc = cast<ResourceContainer*>(obj.get());
					if (rc != nullptr && rc->getSpawnName() == wantType && rc->getQuantity() > 0) {
						found = rc;
					}
				}
			}
		} else {
			// Component slot: match server template + serial.
			String serial = "";
			int hashPos = wantType.lastIndexOf("#");
			if (hashPos != -1) {
				serial = wantType.subString(hashPos + 1);
				wantType = wantType.subString(0, hashPos);
			}
			auto inventory = player->getSlottedObject("inventory");
			if (inventory != nullptr) {
				for (int j = 0; j < inventory->getContainerObjectsSize() && found == nullptr; ++j) {
					auto obj = inventory->getContainerObject(j);
					if (obj == nullptr || !obj->isTangibleObject())
						continue;
					if (obj->getObjectTemplate()->getFullTemplateString() != wantType)
						continue;
					auto tanoObj = cast<TangibleObject*>(obj.get());
					if (!serial.isEmpty() && tanoObj->getSerialNumber() != serial)
						continue;
					found = tanoObj;
				}
			}
		}

		if (found == nullptr) {
			// Missing / insufficient resource: leave the slot EMPTY and say
			// which resource is short (brief requirement).
			player->sendSystemMessage("Repeat-craft: missing or insufficient resource, slot left empty: " + wantType);
			continue;
		}

		Locker ingLock(found, player);
		session->addIngredient(found, i, 0);
	}

	return QueueCommand::SUCCESS;
}
