#ifndef CUSTOMSKILLSPROGRESSION_H_
#define CUSTOMSKILLSPROGRESSION_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CustomSkillsProgression {
public:
	static int getExperienceMultiplier(CreatureObject* player);
};

#endif
