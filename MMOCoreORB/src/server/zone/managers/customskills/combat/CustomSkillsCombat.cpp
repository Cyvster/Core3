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

			CustomSkillsConfig* config = CustomSkillsConfig::instance();
			if (config->isCombatSpamLabelsEnabled()) {
				const String& label = config->getCriticalCombatSpamLabel();
				if (!label.isEmpty())
					creo->sendCustomCombatSpam(UnicodeString(label), 11);
			}
		}

		// Attack repeat tiers (ERR-009): Quad > Triple > Double, one tier
		// only. Multiplies the crit-adjusted damage; armor mitigation is
		// proportional, so this is equivalent to repeating the finalized
		// hit. Chances are the capped config totals in basis points.
		int quadTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::QUAD_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::QUAD_ATTACK_CHANCE));
		int tripleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE));
		int doubleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE,
				CustomSkillsModifiers::getModifierTotal(creo, CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE));

		int repeatMultiplier = 1;
		if (quadTotal > 0 && System::random(9999) < quadTotal)
			repeatMultiplier = 4;
		else if (tripleTotal > 0 && System::random(9999) < tripleTotal)
			repeatMultiplier = 3;
		else if (doubleTotal > 0 && System::random(9999) < doubleTotal)
			repeatMultiplier = 2;

		if (repeatMultiplier > 1)
			damage = static_cast<int>(static_cast<int64>(damage) * repeatMultiplier);
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
