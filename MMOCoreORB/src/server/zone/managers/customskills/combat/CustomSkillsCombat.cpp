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

		int result = combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
				damageMultiplier, poolsToDamage, hitLocation, data);

		CustomSkillsConfig* config = CustomSkillsConfig::instance();

		// Tiered FCT escalation: only when the strike actually escalated
		// (repeats > 1) or critted -- base hits keep the vanilla flytext
		// untouched so nothing double-renders at scale 1.0.
		if ((repeats > 1 || critical) && config->isFctEnabled() && defender != nullptr
				&& !defender->isVehicleObject()) {
			float scale = 1.0f + (repeats - 1) * (config->getFctScaleStepBp() / 10000.f);
			if (critical)
				scale += config->getFctCritBonusBp() / 10000.f;

			const String& tierColor =
					critical ? config->getFctCritColor() : config->getFctTierColor(repeats);
			uint8 r = 0xFF, g = 0xFF, b = 0xFF;
			parseRgb(tierColor, r, g, b);

			ShowFlyText* fly = new ShowFlyText(defender, "combat_effects",
					hitLocationEntry(hitLocation), r, g, b, scale); // flags byte 5, as vanilla
			creo->sendMessage(fly);
		}

		// Chat tag: second spam line to the ATTACKER ONLY carrying just the
		// tier glyph ("x2"/"x3"/"x4"), colored by tier (yellow -> yellow ->
		// red). Minimal add-on; vanilla spam line stays as-is.
		if (repeats > 1 && config->isChatTagEnabled()) {
			byte tagColor = (repeats >= 4 || critical) ? 10 : 11; // 10=red, 11=yellow
			String tag = "x" + String::valueOf(repeats);
			CombatSpam* spam = new CombatSpam(creo, UnicodeString(tag), tagColor);
			creo->sendMessage(spam);
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
