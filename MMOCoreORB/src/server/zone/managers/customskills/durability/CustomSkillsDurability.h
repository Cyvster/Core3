#ifndef CUSTOMSKILLSDURABILITY_H_
#define CUSTOMSKILLSDURABILITY_H_

class CreatureObject;

class CustomSkillsDurability {
public:
	static bool shouldDegradeArmor(CreatureObject* wearer);
	static bool shouldDegradeWeapon(CreatureObject* user, int nativeChancePercent);
};

#endif
