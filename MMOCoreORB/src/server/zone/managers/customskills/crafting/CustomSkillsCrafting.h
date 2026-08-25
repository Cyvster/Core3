#ifndef CUSTOMSKILLSCRAFTING_H_
#define CUSTOMSKILLSCRAFTING_H_

#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/tangible/tool/CraftingTool.h"

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

	// BRIEF-036: repeat-craft assisted pre-fill.
	static void storeRepeatRecipe(CraftingTool* tool, ManufactureSchematic* schematic,
		const String& expAttempt);
	static void clearRepeatRecipe(CraftingTool* tool);
	static int doRepeatCraft(CreatureObject* player, uint64 targetID);

private:
	static int divideDuration(CreatureObject* crafter, int nativeSeconds);
};

#endif
