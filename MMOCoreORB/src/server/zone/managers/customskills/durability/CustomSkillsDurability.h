#ifndef CUSTOMSKILLSDURABILITY_H_
#define CUSTOMSKILLSDURABILITY_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CustomSkillsDurability {
public:
	static bool shouldDegradeArmor(CreatureObject* wearer);
	static bool shouldDegradeWeapon(CreatureObject* user, int nativeChancePercent);
};

#endif
