#ifndef CUSTOMSKILLSMODIFIERS_H_
#define CUSTOMSKILLSMODIFIERS_H_

#include "engine/engine.h"
#include "server/zone/objects/player/PlayerObject.h"

class CustomSkillsModifiers {
public:
	static const int CRITICAL_CHANCE_PER_COMBAT_BADGE = 300;
	static const int BASE_CRITICAL_MULTIPLIER = 15000;

	static int getBadgeCriticalChance(const String& badgeKey);
	static int getCriticalChance(PlayerObject* ghost);
	static int getCriticalChance(PlayerObject* ghost, const char* const* badgeKeys, int count);
	static int getCriticalMultiplier(PlayerObject* ghost);
	static String formatPercent(int basisPoints);

private:
	static bool isCombatProfessionBadge(const String& badgeKey);
};

#endif
