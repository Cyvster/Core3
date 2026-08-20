#include "CustomSkillsCombat.h"

#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/player/PlayerObject.h"

int CustomSkillsCombat::applyDamage(const CombatManager* combatManager, TangibleObject* attacker, WeaponObject* weapon,
		CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier,
		int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data) {
	if (combatManager == nullptr)
		return 0;

	if (attacker != nullptr && attacker->isPlayerCreature() && damage > 0 && damageMultiplier >= 1.f) {
		PlayerObject* ghost = attacker->asCreatureObject()->getPlayerObject();
		int criticalChance = CustomSkillsModifiers::getCriticalChance(ghost);

		if (criticalChance > 0 && System::random(9999) < criticalChance) {
			int criticalMultiplier = CustomSkillsModifiers::getCriticalMultiplier(ghost);
			damage = static_cast<int>((static_cast<int64>(damage) * criticalMultiplier) / 10000);

			UnicodeString criticalMessage("(CRIT)");
			attacker->asCreatureObject()->sendCustomCombatSpam(criticalMessage, 11);
		}
	}

	return combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
		damageMultiplier, poolsToDamage, hitLocation, data);
}

int CustomSkillsCombat::getDefenseCap(CreatureObject* defender, int nativeCap) {
	if (defender == nullptr)
		return nativeCap;

	const int bonus = CustomSkillsModifiers::getModifierTotal(defender, CustomSkillsModifierType::DEFENSE_CAP_INCREASE);
	if (bonus <= 0)
		return nativeCap;
	return nativeCap + bonus;
}

int CustomSkillsCombat::getEffectiveArmorRating(CreatureObject* attacker, int nativeArmor) {
	if (attacker == nullptr || nativeArmor <= 0)
		return nativeArmor;

	const int penetration = CustomSkillsModifiers::getModifierTotal(attacker, CustomSkillsModifierType::ARMOR_PENETRATION);
	if (penetration <= 0)
		return nativeArmor;
	return Math::max(0, nativeArmor - penetration);
}
