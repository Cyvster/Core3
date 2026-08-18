/*
 * CustomSkillSuiCallback.h
 */
#ifndef CUSTOMSKILLSUICALLBACK_H_
#define CUSTOMSKILLSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/managers/customskill/CustomSkillManager.h"

class CustomSkillSuiCallback : public SuiCallback {
public:
	CustomSkillSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) override {
		if (player == nullptr || suiBox == nullptr || args == nullptr || !suiBox->isListBox())
			return;

		// Cancel was pressed.
		if (eventIndex == 1)
			return;

		if (args->size() < 2)
			return;

		bool otherPressed = Bool::valueOf(args->get(0).toString());
		int index = Integer::valueOf(args->get(1).toString());

		if (otherPressed)
			return;

		CustomSkillManager* manager = CustomSkillManager::instance();
		const CustomSkillDefinition* definition = manager->getDefinition(index);

		if (definition == nullptr)
			return;

		int level = manager->getSkillLevel(player, definition->id);

		player->sendSystemMessage(definition->name + " - Level " + String::valueOf(level) + "/" + String::valueOf(definition->maxLevel));
		player->sendSystemMessage(definition->description);
	}
};

#endif /* CUSTOMSKILLSUICALLBACK_H_ */
