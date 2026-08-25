/*
 * CustomSkillsCommand.h
 */

#ifndef CUSTOMSKILLSCOMMAND_H_
#define CUSTOMSKILLSCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "CustomSkillsMenu.h"
#include "crafting/CustomSkillsCrafting.h"

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

		// BRIEF-042 item D: /customskills repeatcraft [toolObjectID]
		// subcommand form replaces the standalone /repeatcraft command.
		StringTokenizer tokenizer(arguments.toString());
		tokenizer.setDelimeter(" ");

		if (tokenizer.hasMoreTokens()) {
			String sub;
			tokenizer.getStringToken(sub);
			sub = sub.toLowerCase();

			if (sub == "repeatcraft") {
				uint64 toolID = 0;

				if (tokenizer.hasMoreTokens())
					toolID = tokenizer.getUnsignedLongToken();
				else if (target != 0)
					toolID = target;

				return CustomSkillsCrafting::doRepeatCraft(creature, toolID);
			}

			creature->sendSystemMessage("Usage: /customskills [repeatcraft [tool id]]");
			return GENERALERROR;
		}

		CustomSkillsMenu::open(creature);
		return SUCCESS;
	}
};

#endif // CUSTOMSKILLSCOMMAND_H_
