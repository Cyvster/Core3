/*
 * RepeatCraftCommand.h -- BRIEF-036 repeat-craft assisted pre-fill.
 *
 * Starts a fresh crafting session on a tool holding a repeat recipe
 * snapshot, auto-selects the snapshotted schematic and auto-fills
 * ingredient slots from live inventory, leaving the normal crafting
 * window open at the resource screen for the player to confirm.
 * Never creates the prototype itself.
 */

#ifndef REPEATCRAFTCOMMAND_H_
#define REPEATCRAFTCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/customskills/crafting/CustomSkillsCrafting.h"

class RepeatCraftCommand : public QueueCommand {
public:
	RepeatCraftCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const override {
		if (creature == nullptr || !creature->isPlayerCreature())
			return GENERALERROR;

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		return CustomSkillsCrafting::doRepeatCraft(creature, target);
	}
};

#endif // REPEATCRAFTCOMMAND_H_
