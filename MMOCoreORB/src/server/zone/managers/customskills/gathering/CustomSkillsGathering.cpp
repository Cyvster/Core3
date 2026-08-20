#include "CustomSkillsGathering.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"

int CustomSkillsGathering::modifyQuantity(CreatureObject* player, int nativeQuantity) {
	if (nativeQuantity <= 0)
		return nativeQuantity;
	int bonus = CustomSkillsModifiers::getModifierTotal(player, CustomSkillsModifierType::GATHERING_QUANTITY);
	int modified = static_cast<int>((static_cast<int64>(nativeQuantity) * (10000 + bonus)) / 10000);
	return Math::max(nativeQuantity, modified);
}

int CustomSkillsGathering::modifyForageQuantity(CreatureObject* player, int nativeQuantity) {
	return modifyQuantity(player, nativeQuantity);
}

int CustomSkillsGathering::modifyMilkQuantity(CreatureObject* player, int nativeQuantity) {
	return modifyQuantity(player, nativeQuantity);
}
