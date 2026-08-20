/*
 * CustomSkillsCommand.h
 */

#ifndef CUSTOMSKILLSCOMMAND_H_
#define CUSTOMSKILLSCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "CustomSkillsMenu.h"

class CustomSkillsCommand : public QueueCommand {
public:
	CustomSkillsCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const override {
		if (creature == nullptr)
			return GENERALERROR;

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (creature->getPlayerObject() == nullptr)
			return GENERALERROR;

		CustomSkillsMenu::open(creature);
		return SUCCESS;
	}
};

#endif // CUSTOMSKILLSCOMMAND_H_
