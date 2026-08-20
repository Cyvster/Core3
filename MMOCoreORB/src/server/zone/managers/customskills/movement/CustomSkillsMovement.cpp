#include "CustomSkillsMovement.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"

float CustomSkillsMovement::getSpeed(CreatureObject* player, float nativeSpeed) {
	if (player == nullptr || nativeSpeed <= 0)
		return nativeSpeed;

	const int bonus = CustomSkillsModifiers::getModifierTotal(player, CustomSkillsModifierType::MOVEMENT_SPEED);
	if (bonus <= 0)
		return nativeSpeed;
	return nativeSpeed * (10000.f + bonus) / 10000.f;
}
