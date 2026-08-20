#ifndef CUSTOMSKILLSCOMBAT_H_
#define CUSTOMSKILLSCOMBAT_H_

#include "engine/engine.h"
#include "server/zone/managers/combat/CombatManager.h"

class CustomSkillsCombat {
public:
	static int applyDamage(const CombatManager* combatManager, TangibleObject* attacker, WeaponObject* weapon,
		CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier,
		int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data);
};

#endif
