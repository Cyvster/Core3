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
		CreatureObject* creo = attacker->asCreatureObject();
		int criticalChance = CustomSkillsModifiers::getCriticalChance(creo->getPlayerObject());

		if (criticalChance > 0 && System::random(9999) < criticalChance) {
			int criticalMultiplier = CustomSkillsModifiers::getCriticalMultiplier(creo);
			damage = static_cast<int>((static_cast<int64>(damage) * criticalMultiplier) / 10000);
		}

		// Attack repeat tiers (ERR-009): sequential upgrade chain off the
		// landed hit -- Double -> Triple -> Quad. Each stage rolls only if
		// the previous succeeded; the first failure ends the chain.
		// Chances are the capped config totals in basis points.
		int doubleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE));
		int tripleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE));
		int quadTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::QUAD_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::QUAD_ATTACK_CHANCE));

		int repeats = 1;
		if (doubleTotal > 0 && System::random(9999) < doubleTotal) {
			repeats = 2;
			if (tripleTotal > 0 && System::random(9999) < tripleTotal) {
				repeats = 3;
				if (quadTotal > 0 && System::random(9999) < quadTotal)
					repeats = 4;
			}
		}

		// Resend the already-calculated hit. Each application runs the full
		// vanilla path (armor, spam, observers), so multi-hits are real hits.
		// Stop early if the defender dies or is incapacitated mid-sequence.
		int result = combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
				damageMultiplier, poolsToDamage, hitLocation, data);
		for (int i = 1; i < repeats; ++i) {
			if (defender == nullptr || defender->isDead() || defender->isIncapacitated())
				break;
			result = combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
					damageMultiplier, poolsToDamage, hitLocation, data);
		}
		return result;
	}

	// Non-player attackers (and zero-damage hits): straight vanilla path.
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

int CustomSkillsCombat::getEffectiveArmorRating(TangibleObject* attacker, int nativeArmor) {
	if (attacker == nullptr || nativeArmor <= 0)
		return nativeArmor;

	if (!attacker->isPlayerCreature())
		return nativeArmor;

	const int penetration = CustomSkillsModifiers::getModifierTotal(attacker->asCreatureObject(), CustomSkillsModifierType::ARMOR_PENETRATION);
	if (penetration <= 0)
		return nativeArmor;
	return Math::max(0, nativeArmor - penetration);
}
