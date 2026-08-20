#ifndef CUSTOMSKILLSCRAFTING_H_
#define CUSTOMSKILLSCRAFTING_H_

class CreatureObject;
class CraftingValues;

class CustomSkillsCrafting {
public:
	static int modifyPracticeExperience(CreatureObject* crafter, int nativeAmount);
	static int getPersonalCraftingDuration(CreatureObject* crafter, int nativeSeconds);
	static int getFactoryProductionDuration(CreatureObject* activator, int nativeSeconds);
	static int getAmazingChanceBonus(CreatureObject* crafter);
	static int getAmazingResultsStrength(CreatureObject* crafter);
	static bool shouldPromoteAmazingFailure(CreatureObject* crafter, int nativeChanceBasisPoints);
	static void applyAmazingResults(CreatureObject* crafter, CraftingValues* values, int result,
		int amazingResult, const String& selectedGroup = "");

private:
	static int divideDuration(CreatureObject* crafter, int nativeSeconds);
};

#endif
