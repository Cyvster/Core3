#ifndef CUSTOMSKILLSBUFFS_H_
#define CUSTOMSKILLSBUFFS_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/buffs/Buff.h"

class CustomSkillsBuffs {
public:
	static float getDuration(CreatureObject* recipient, Buff* buff, float nativeDuration);

private:
	static bool isEligible(Buff* buff);
};

#endif // CUSTOMSKILLSBUFFS_H_
