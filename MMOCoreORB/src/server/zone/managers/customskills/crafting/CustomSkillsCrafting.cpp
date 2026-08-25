#include "CustomSkillsCrafting.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"

int CustomSkillsCrafting::modifyPracticeExperience(CreatureObject* crafter, int nativeAmount) {
	if (nativeAmount <= 0)
		return nativeAmount;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::PRACTICE_EXPERIENCE_BONUS);
	return static_cast<int>((static_cast<int64>(nativeAmount) * (10000 + bonus)) / 10000);
}

int CustomSkillsCrafting::divideDuration(CreatureObject* crafter, int nativeSeconds) {
	// Durations below 1 second are clamped to 1 (documented edge behavior).
	if (nativeSeconds <= 1)
		return 1;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::CRAFTING_SPEED);
	int multiplier = 10000 + bonus;
	int64 scaled = static_cast<int64>(nativeSeconds) * 10000;
	return Math::max(1, static_cast<int>((scaled + multiplier - 1) / multiplier));
}

int CustomSkillsCrafting::getPersonalCraftingDuration(CreatureObject* crafter, int nativeSeconds) {
	return divideDuration(crafter, nativeSeconds);
}

int CustomSkillsCrafting::getFactoryProductionDuration(CreatureObject* activator, int nativeSeconds) {
	return divideDuration(activator, nativeSeconds);
}

int CustomSkillsCrafting::getAmazingChanceBonus(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_SUCCESS_CHANCE);
}

int CustomSkillsCrafting::getAmazingResultsStrength(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_RESULTS);
}

bool CustomSkillsCrafting::shouldPromoteAmazingFailure(CreatureObject* crafter, int nativeChanceBasisPoints) {
	const int bonus = getAmazingChanceBonus(crafter);
	const int nativeChance = Math::min(Math::max(nativeChanceBasisPoints, 0), 10000);
	const int finalChance = Math::min(10000, nativeChance + bonus);
	if (finalChance <= nativeChance || nativeChance >= 10000)
		return false;

	// This roll happens only after the native roll failed. Convert the desired
	// percentage-point increase into its conditional probability so the final
	// chance is nativeChance + configured bonus, not a multiplier.
	const int conditionalChance = static_cast<int>((static_cast<int64>(finalChance - nativeChance) * 10000) /
			(10000 - nativeChance));
	return System::random(9999) < conditionalChance;
}

void CustomSkillsCrafting::applyAmazingResults(CreatureObject* crafter, CraftingValues* values, int result,
		int amazingResult, const String& selectedGroup) {
	if (values == nullptr || result != amazingResult)
		return;

	const int strength = Math::min(getAmazingResultsStrength(crafter), 10000);
	if (strength <= 0)
		return;

	const float factor = strength / 10000.f;
	for (int i = 0; i < values->getTotalExperimentalAttributes(); ++i) {
		const String& attribute = values->getAttribute(i);
		if (!selectedGroup.isEmpty() && values->getAttributeGroup(attribute) != selectedGroup)
			continue;

		const float current = values->getCurrentPercentage(attribute);
		const float improved = current + ((1.f - current) * factor);
		values->setCurrentPercentage(attribute, improved, Math::max(values->getMaxPercentage(attribute), improved));
	}
	values->recalculateValues(selectedGroup.isEmpty());
}

// =============================================================================
// ERR-018)
//
// luaStringData map -- zero IDL changes, survives restarts; see CODE_REFERENCE):
//                         name + "#" + serial (component slots)
//                           re-applied by the player via the experiment UI --
//                           surfaced to them as a reminder message on pre-fill
//
// BRIEF-042 hardening notes:
//  * Resource/component scans RECURSE into containers inside inventory
//    (backpacks, satchels) depth-limited -- matches vanilla's
//    isASubChildOf(crafter) acceptance semantics.
//  * Partial stacks: total availability across ALL matching stacks is summed
//    before anything is consumed; if it cannot fill the slot, nothing is
//    drained and a shortfall message naming the resource is sent.
// =============================================================================

#include "server/zone/ZoneServer.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/draftschematic/DraftSchematic.h"
#include "templates/crafting/draftslot/DraftSlot.h"
#include "server/zone/objects/tangible/tool/CraftingTool.h"
#include "server/zone/objects/player/sessions/crafting/CraftingSession.h"
#include "server/zone/objects/tangible/tool/CraftingStation.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/IngredientSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ResourceSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ComponentSlot.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/customskills/CustomSkillsConfig.h"

static const int CS36_MAX_CONTAINER_DEPTH = 4;

// stale higher-indexed slot keys from a previous recipe can't linger.
namespace {
// Recursive candidate collection under `root` (depth-limited). Appends:
//  resource containers matching spawnName, or
//  tangible components matching template (+serial when non-empty).
} // namespace

