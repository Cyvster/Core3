#ifndef CUSTOMSKILLSGATHERING_H_
#define CUSTOMSKILLSGATHERING_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CustomSkillsGathering {
public:
	static int modifyForageQuantity(CreatureObject* player, int nativeQuantity);
	static int modifyMilkQuantity(CreatureObject* player, int nativeQuantity);

private:
	static int modifyQuantity(CreatureObject* player, int nativeQuantity);
};

#endif
