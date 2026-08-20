#include "CustomSkillsModifiers.h"

#include "server/zone/managers/player/BadgeList.h"
#include "server/zone/objects/player/PlayerObject.h"

namespace {
	const char* const combatProfessionBadges[] = {
		"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
		"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
		"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
		"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master"
	};

	static_assert(sizeof(combatProfessionBadges) / sizeof(combatProfessionBadges[0]) == 12,
		"Critical Chance must be assigned to exactly 12 combat profession badges");
}

bool CustomSkillsModifiers::isCombatProfessionBadge(const String& badgeKey) {
	for (const char* key : combatProfessionBadges) {
		if (badgeKey == key)
			return true;
	}

	return false;
}

int CustomSkillsModifiers::getBadgeCriticalChance(const String& badgeKey) {
	return isCombatProfessionBadge(badgeKey) ? CRITICAL_CHANCE_PER_COMBAT_BADGE : 0;
}

int CustomSkillsModifiers::getCriticalChance(PlayerObject* ghost) {
	return getCriticalChance(ghost, combatProfessionBadges, sizeof(combatProfessionBadges) / sizeof(combatProfessionBadges[0]));
}

int CustomSkillsModifiers::getCriticalChance(PlayerObject* ghost, const char* const* badgeKeys, int count) {
	if (ghost == nullptr || badgeKeys == nullptr || count <= 0)
		return 0;

	const BadgeList* badgeList = BadgeList::instance();
	int chance = 0;

	for (int i = 0; i < count; ++i) {
		const Badge* badge = badgeList->get(badgeKeys[i]);
		if (badge != nullptr && ghost->hasBadge(badge->getIndex()))
			chance += getBadgeCriticalChance(badge->getKey());
	}

	return Math::min(chance, 10000);
}

int CustomSkillsModifiers::getCriticalMultiplier(PlayerObject* ghost) {
	return ghost == nullptr ? 10000 : BASE_CRITICAL_MULTIPLIER;
}

String CustomSkillsModifiers::formatPercent(int basisPoints) {
	int absolute = basisPoints < 0 ? -basisPoints : basisPoints;
	String fraction = String::valueOf(absolute % 100);
	if (fraction.length() < 2)
		fraction = "0" + fraction;

	return (basisPoints < 0 ? "-" : "") + String::valueOf(absolute / 100) + "." + fraction + "%";
}
