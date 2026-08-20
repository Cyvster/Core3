#include "CustomSkillsCombat.h"

#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/player/PlayerObject.h"

int CustomSkillsCombat::applyDamage(const CombatManager* combatManager, TangibleObject* attacker, WeaponObject* weapon,
		CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier,
		int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data) {
	if (combatManager == nullptr)
		return 0;

	// A multiplier below one represents a blocked attack in the vanilla combat
	// pipeline. Critical hits apply only to successful, unblocked player attacks.
	if (attacker != nullptr && attacker->isPlayerCreature() && damage > 0 && damageMultiplier >= 1.f) {
		PlayerObject* ghost = attacker->asCreatureObject()->getPlayerObject();
		int criticalChance = CustomSkillsModifiers::getCriticalChance(ghost);

		if (criticalChance > 0 && System::random(9999) < criticalChance) {
			int criticalMultiplier = CustomSkillsModifiers::getCriticalMultiplier(ghost);
			damage = static_cast<int>((static_cast<int64>(damage) * criticalMultiplier) / 10000);

			// Use the client's combat-spam channel so the indicator is visible with
			// the attack feedback without requiring client files.
			UnicodeString criticalMessage("(CRIT)");
			attacker->asCreatureObject()->sendCustomCombatSpam(criticalMessage, 11);
		}
	}

	return combatManager->applyVanillaDamage(attacker, weapon, defender, defenderHitList, damage,
		damageMultiplier, poolsToDamage, hitLocation, data);
}
