/*
 * CustomSkillManager.cpp
 */

#include "CustomSkillManager.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/managers/customskill/CustomSkillSuiCallback.h"

const String CustomSkillManager::SCREENPLAY = "customSkills";

CustomSkillManager::CustomSkillManager()
	: Logger("CustomSkillManager") {
	setLogging(false);
	loadDefaultDefinitions();
}

void CustomSkillManager::loadDefaultDefinitions() {
	definitions.add(CustomSkillDefinition("custom_skill_1", "Custom Skill 1", "Placeholder custom skill.", 10));
	definitions.add(CustomSkillDefinition("custom_skill_2", "Custom Skill 2", "Placeholder custom skill.", 10));
	definitions.add(CustomSkillDefinition("custom_skill_3", "Custom Skill 3", "Placeholder custom skill.", 10));
	definitions.add(CustomSkillDefinition("custom_skill_4", "Custom Skill 4", "Placeholder custom skill.", 10));
	definitions.add(CustomSkillDefinition("custom_skill_5", "Custom Skill 5", "Placeholder custom skill.", 10));
}

void CustomSkillManager::openMenu(CreatureObject* player) {
	if (player == nullptr || player->getPlayerObject() == nullptr)
		return;

	PlayerObject* ghost = player->getPlayerObject();

	// Only close an existing copy of our menu. We do not touch other SUI windows.

	ManagedReference<SuiListBox*> box = new SuiListBox(player, 0, SuiListBox::HANDLETWOBUTTON);
	box->setPromptTitle("Custom Skills");
	box->setPromptText("Select a custom skill to view its current level.");
	box->setOkButton(true, "@ok");
	box->setCancelButton(true, "@cancel");
	box->setCallback(new CustomSkillSuiCallback(player->getZoneServer()));

	for (int i = 0; i < definitions.size(); ++i) {
		const CustomSkillDefinition& definition = definitions.get(i);
		String label = definition.name + "  (" + String::valueOf(getSkillLevel(player, definition.id)) + "/" + String::valueOf(definition.maxLevel) + ")";
		box->addMenuItem(label, i);
	}

	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());
}

int CustomSkillManager::getSkillLevel(CreatureObject* player, const String& skillId) const {
	if (player == nullptr || player->getPlayerObject() == nullptr)
		return 0;

	String value = player->getPlayerObject()->getScreenPlayData(SCREENPLAY, skillId);

	if (value.isEmpty())
		return 0;

	try {
		return Integer::valueOf(value);
	} catch (...) {
		return 0;
	}
}

void CustomSkillManager::setSkillLevel(CreatureObject* player, const String& skillId, int level) const {
	if (player == nullptr || player->getPlayerObject() == nullptr)
		return;

	const CustomSkillDefinition* definition = nullptr;

	for (int i = 0; i < definitions.size(); ++i) {
		if (definitions.get(i).id == skillId) {
			definition = &definitions.get(i);
			break;
		}
	}

	if (definition == nullptr)
		return;

	level = Math::max(0, Math::min(level, definition->maxLevel));
	player->getPlayerObject()->setScreenPlayData(SCREENPLAY, skillId, String::valueOf(level));
}

const CustomSkillDefinition* CustomSkillManager::getDefinition(int index) const {
	if (index < 0 || index >= definitions.size())
		return nullptr;

	return &definitions.get(index);
}

int CustomSkillManager::getDefinitionCount() const {
	return definitions.size();
}
