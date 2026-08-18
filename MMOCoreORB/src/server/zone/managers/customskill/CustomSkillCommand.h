/*
 * CustomSkillCommand.h
 */
#ifndef CUSTOMSKILLCOMMAND_H_
#define CUSTOMSKILLCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/customskill/CustomSkillManager.h"

class CustomSkillCommand : public QueueCommand {
public:
	CustomSkillCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const override {
		CustomSkillManager::instance()->openMenu(creature);
		return SUCCESS;
	}

	String getSyntax() const override {
		return String("/customskills");
	}
};

#endif /* CUSTOMSKILLCOMMAND_H_ */
