#include "CustomSkillsCombat.h"

#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/customskills/CustomSkillsConfig.h"
#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/CombatSpam.h"
#include "server/zone/packets/object/ShowFlyText.h"

namespace {
// BRIEF-034: parse a six-character RGB hex string ("RRGGBB") into bytes.
bool parseRgb(const String& hex, uint8& r, uint8& g, uint8& b) {
	unsigned int value = 0;
	if (hex.length() != 6 || sscanf(hex.toCharArray(), "%06x", &value) != 1)
		return false;
	r = static_cast<uint8>((value >> 16) & 0xFF);
	g = static_cast<uint8>((value >> 8) & 0xFF);
	b = static_cast<uint8>(value & 0xFF);
	return true;
}

// Same stf entries vanilla showHitLocationFlyText uses (CombatManager.cpp).
const char* hitLocationEntry(uint8 location) {
	switch (location) {
	case CombatManager::HIT_HEAD:
		return "hit_head";
	case CombatManager::HIT_LARM:
		return "hit_larm";
	case CombatManager::HIT_RARM:
		return "hit_rarm";
	case CombatManager::HIT_LLEG:
		return "hit_lleg";
	case CombatManager::HIT_RLEG:
		return "hit_rleg";
	default:
		return "hit_body";
	}
}
} // namespace

int CustomSkillsCombat::applyDamage(const CombatManager* combatManager, TangibleObject* attacker, WeaponObject* weapon,
		CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier,
		int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data) {
	if (combatManager == nullptr)
		return 0;

	if (attacker != nullptr && attacker->isPlayerCreature() && damage > 0 && damageMultiplier >= 1.f) {
		CreatureObject* creo = attacker->asCreatureObject();
		bool critical = false;
		int criticalChance = CustomSkillsModifiers::getCriticalChance(creo->getPlayerObject());

		if (criticalChance > 0 && System::random(9999) < criticalChance) {
			critical = true;
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

		// BRIEF-034: consolidated strike. Instead of N x applyVanillaDamage,
		// the tier multiplies the finalized hit ONCE and the damage is
		// applied in a single application. Owner-accepted deviation vs the
		// old per-hit armor math: one big hit mitigates differently than
		// several small ones, but totals are equivalent by design.
		damage = static_cast<int>(static_cast<int64>(damage) * repeats);

		bool escalated = repeats > 1 || critical;

		int result = combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
				damageMultiplier, poolsToDamage, hitLocation, data);

		return result;

		return result;
	}

	// Non-player attackers (and zero-damage hits): straight vanilla path.
	return combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
		damageMultiplier, poolsToDamage, hitLocation, data);
}

int CustomSkillsCombat::applyTanoTargetDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender,
		DefenderHitList* defenderHitList, int damage, int poolsToDamage) {
	if (attacker == nullptr || defender == nullptr)
		return damage;

	// BRIEF-042 item B: lairs / TANO defenders previously bypassed every
	// custom tier roll because player-vs-TANO goes through the OTHER
	// applyDamage overload (CreatureObject* attacker / TangibleObject*
	// defender), which had no hook at all. Roll crit + repeat tiers here and
	// multiply the single consolidated hit before vanilla inflicts it.
	if (!attacker->isPlayerCreature() || damage <= 0)
		return damage;

	bool critical = false;
	int criticalChance = CustomSkillsModifiers::getCriticalChance(attacker->getPlayerObject());

	if (criticalChance > 0 && System::random(9999) < criticalChance) {
		critical = true;
		int criticalMultiplier = CustomSkillsModifiers::getCriticalMultiplier(attacker);
		damage = static_cast<int>((static_cast<int64>(damage) * criticalMultiplier) / 10000);
	}

	int doubleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE,
			CustomSkillsModifiers::getModifierTotal(attacker, CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE));
	int tripleTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE,
			CustomSkillsModifiers::getModifierTotal(attacker, CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE));
	int quadTotal = CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::QUAD_ATTACK_CHANCE,
			CustomSkillsModifiers::getModifierTotal(attacker, CustomSkillsModifierType::QUAD_ATTACK_CHANCE));

	int repeats = 1;
	if (doubleTotal > 0 && System::random(9999) < doubleTotal) {
		repeats = 2;
		if (tripleTotal > 0 && System::random(9999) < tripleTotal) {
			repeats = 3;
			if (quadTotal > 0 && System::random(9999) < quadTotal)
				repeats = 4;
		}
	}

	damage = static_cast<int>(static_cast<int64>(damage) * repeats);

	// FCT reverted to vanilla (owner decision 08252026): no escalated flytext.

	return damage;
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
