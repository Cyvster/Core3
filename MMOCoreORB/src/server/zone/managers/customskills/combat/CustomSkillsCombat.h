#ifndef CUSTOMSKILLSCOMBAT_H_
#define CUSTOMSKILLSCOMBAT_H_

#include "engine/engine.h"
#include "engine/core/Core.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/managers/combat/DefenderHitList.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"

class CombatManager;
class CreatureAttackData;

class CustomSkillsCombat {
public:
	class ChatTagInfo {
	public:
		int repeats = 1;
		int critical = 0;
	};

	static int applyDamage(const CombatManager* combatManager, TangibleObject* attacker, WeaponObject* weapon,
			CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier,
			int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data);

	static int getDefenseCap(CreatureObject* defender, int nativeCap);
	static int getEffectiveArmorRating(TangibleObject* attacker, int nativeArmor);

	// BRIEF-042 item B: tier rolls for the CreatureObject-attacker /
	// TangibleObject-defender applyDamage overload (lairs, turrets, other
	// non-creature targets). Returns the consolidated (tier-multiplied)
	// damage for vanilla to inflict.
	static int applyTanoTargetDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender,
			DefenderHitList* defenderHitList, int damage, int poolsToDamage);
};

#endif
