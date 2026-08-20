#ifndef CUSTOMSKILLSMOVEMENT_H_
#define CUSTOMSKILLSMOVEMENT_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CustomSkillsMovement {
public:
	static float getSpeed(CreatureObject* player, float nativeSpeed);
};

#endif // CUSTOMSKILLSMOVEMENT_H_
