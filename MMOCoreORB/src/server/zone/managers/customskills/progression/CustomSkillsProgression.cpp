#include "CustomSkillsProgression.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"

int CustomSkillsProgression::getExperienceMultiplier(CreatureObject* player) {
	return 10000 + CustomSkillsModifiers::getModifierTotal(player, CustomSkillsModifierType::EXPERIENCE_MULTIPLIER);
}
