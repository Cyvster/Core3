#ifndef CUSTOMSKILLSMOVEMENT_H_
#define CUSTOMSKILLSMOVEMENT_H_

class CreatureObject;

class CustomSkillsMovement {
public:
	static float getSpeed(CreatureObject* player, float nativeSpeed);
};

#endif // CUSTOMSKILLSMOVEMENT_H_
