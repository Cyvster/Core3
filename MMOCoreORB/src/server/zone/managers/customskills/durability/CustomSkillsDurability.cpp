#include "CustomSkillsDurability.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"

bool CustomSkillsDurability::shouldDegradeArmor(CreatureObject* wearer) {
	int reduction = CustomSkillsModifiers::getModifierTotal(wearer, CustomSkillsModifierType::ARMOR_DEGRADE_REDUCTION);
	if (reduction <= 0)
		return true;
	return System::random(9999) < Math::max(0, 10000 - reduction);
}

bool CustomSkillsDurability::shouldDegradeWeapon(CreatureObject* user, int nativeChancePercent) {
	int reduction = CustomSkillsModifiers::getModifierTotal(user, CustomSkillsModifierType::WEAPON_DEGRADE_REDUCTION);
	if (reduction <= 0)
		return System::random(9999) < nativeChancePercent * 100;
	int effectiveChance = Math::max(0, nativeChancePercent * (10000 - reduction) / 100);
	return System::random(9999) < effectiveChance;
}
