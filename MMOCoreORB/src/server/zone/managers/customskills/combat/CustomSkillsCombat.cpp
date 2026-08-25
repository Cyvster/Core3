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

		// BRIEF-042 item A: when our escalation fires AND flytext is enabled,
		// suppress vanilla's scale-1.0 pool-colored hit-location text for this
		// hit so ours is the only one rendered at that anchor. Strictly
		// scoped: base hits keep vanilla text exactly as before.
		bool suppressVanillaFlyText = false;
		if (escalated && defender != nullptr && !defender->isVehicleObject()) {
			suppressVanillaFlyText = true; // provisional; cleared below if FCT off

			auto configCheck = CustomSkillsConfig::instance();
			if (!configCheck->isFctEnabled())
				suppressVanillaFlyText = false;
		}

		combatManager->setSuppressHitLocationFlyText(suppressVanillaFlyText);
		int result = combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
				damageMultiplier, poolsToDamage, hitLocation, data);
		combatManager->setSuppressHitLocationFlyText(false);

		if (escalated && defender != nullptr && !defender->isVehicleObject()) {
			CustomSkillsConfig* config = CustomSkillsConfig::instance();

			// Tiered FCT escalation (BRIEF-042 item A): broadcast to the
			// DEFENDER's observers -- everyone watching the fight sees it,
			// matching vanilla's audience semantics (ERR-016 fix). Scale and
			// color step up by tier so escalated hits visually dominate.
			if (config->isFctEnabled()) {
				float scale = 1.0f + (repeats - 1) * (config->getFctScaleStepBp() / 10000.f);
				if (critical)
					scale += config->getFctCritBonusBp() / 10000.f;

				const String& tierColor =
						critical ? config->getFctCritColor() : config->getFctTierColor(repeats);
				uint8 r = 0xFF, g = 0xFF, b = 0xFF;
				parseRgb(tierColor, r, g, b);

				ShowFlyText* fly = new ShowFlyText(defender, "combat_effects",
						hitLocationEntry(hitLocation), r, g, b, scale); // flags byte 5, as vanilla

				// Broadcast from the defender so all fight observers receive
				// the packet (defender included); attacker gets a private
				// copy in case they are somehow not in the defender's close
				// range set.
				defender->broadcastMessage(fly, true);

				ChatTagInfo tagInfo;
				tagInfo.repeats = repeats;
				tagInfo.critical = critical ? 1 : 0;

				// Chat tag stays ATTACKER-ONLY per BRIEF-034 design (second
				// spam line with the tier glyph). Deferred to next tick to
				// avoid sending packets while holding combat locks.
				Reference<CreatureObject*> strongAttacker = creo;
				Core::getTaskManager()->executeTask([strongAttacker, tagInfo]() {
					if (strongAttacker == nullptr || !strongAttacker->isPlayerCreature())
						return;

					PlayerObject* ghost = strongAttacker->getPlayerObject();
					if (ghost == nullptr)
						return;

					byte tagColor = (tagInfo.repeats >= 4 || tagInfo.critical) ? 10 : 11; // 10=red, 11=yellow
					String tag = "x" + String::valueOf(tagInfo.repeats);
					CombatSpam* spam = new CombatSpam(strongAttacker.get(), UnicodeString(tag), tagColor);
					strongAttacker->sendMessage(spam);
				}, "CsStrikeChatTagLambda");
			}
		}

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

	// Tiered FCT on lair/TANO hits too -- same visibility treatment as the
	// creature-defender path (suppress flag covers the creature path only,
	// but this overload never showed hit-location text anyway).
	if ((repeats > 1 || critical) && CustomSkillsConfig::instance()->isFctEnabled()) {
		CustomSkillsConfig* config = CustomSkillsConfig::instance();

		float scale = 1.0f + (repeats - 1) * (config->getFctScaleStepBp() / 10000.f);
		if (critical)
			scale += config->getFctCritBonusBp() / 10000.f;

		const String& tierColor =
				critical ? config->getFctCritColor() : config->getFctTierColor(repeats);
		uint8 r = 0xFF, g = 0xFF, b = 0xFF;
		parseRgb(tierColor, r, g, b);

		ShowFlyText* fly = new ShowFlyText(defender, "combat_effects",
				critical ? "critical_hit" : "hit_body", r, g, b, scale);
		defender->broadcastMessage(fly, true);
	}

	return damage;
}
