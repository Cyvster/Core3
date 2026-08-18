/*
 * CustomSkillsCommand.h
 * Self-contained custom skill menu command.
 */
#ifndef CUSTOMSKILLSCOMMAND_H_
#define CUSTOMSKILLSCOMMAND_H_

#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/SuiCallback.h"

class CustomSkillsSuiCallback : public SuiCallback {
public:
	CustomSkillsSuiCallback(ZoneServer* server) : SuiCallback(server) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) override {
		if (player == nullptr || eventIndex == 1 || args == nullptr || args->size() == 0)
			return;

		int index = Integer::valueOf(args->get(0).toString());

		if (index < 0 || index > 4)
			return;

		static const char* names[] = {
			"Custom Skill 1",
			"Custom Skill 2",
			"Custom Skill 3",
			"Custom Skill 4",
			"Custom Skill 5"
		};

		player->sendSystemMessage("Selected " + String(names[index]) + ". Level: 0/10");
	}
};

class CustomSkillsCommand : public QueueCommand {
public:
	CustomSkillsCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const override {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		PlayerObject* ghost = creature->getPlayerObject();

		if (ghost == nullptr)
			return GENERALERROR;

		ManagedReference<SuiListBox*> box = new SuiListBox(creature, SuiWindowType::NONE, SuiListBox::HANDLETWOBUTTON);
		box->setCallback(new CustomSkillsSuiCallback(server->getZoneServer()));
		box->setPromptTitle("Custom Skills");
		box->setPromptText("Select a custom skill to view its information.");
		box->setOkButton(true, "@ok");
		box->setCancelButton(true, "@cancel");

		box->addMenuItem("Custom Skill 1 - Level 0/10");
		box->addMenuItem("Custom Skill 2 - Level 0/10");
		box->addMenuItem("Custom Skill 3 - Level 0/10");
		box->addMenuItem("Custom Skill 4 - Level 0/10");
		box->addMenuItem("Custom Skill 5 - Level 0/10");

		ghost->addSuiBox(box);
		creature->sendMessage(box->generateMessage());

		return SUCCESS;
	}
};

#endif // CUSTOMSKILLSCOMMAND_H_
