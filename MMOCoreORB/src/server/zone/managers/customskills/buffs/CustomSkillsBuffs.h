#ifndef CUSTOMSKILLSBUFFS_H_
#define CUSTOMSKILLSBUFFS_H_

class Buff;
class CreatureObject;

class CustomSkillsBuffs {
public:
	static float getDuration(CreatureObject* recipient, Buff* buff, float nativeDuration);

private:
	static bool isEligible(Buff* buff);
};

#endif // CUSTOMSKILLSBUFFS_H_
