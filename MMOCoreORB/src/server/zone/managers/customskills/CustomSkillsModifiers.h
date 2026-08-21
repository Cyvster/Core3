#ifndef CUSTOMSKILLSMODIFIERS_H_
#define CUSTOMSKILLSMODIFIERS_H_

#include "engine/engine.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "CustomSkillsModifierType.h"

class CustomSkillsModifiers {
public:
	static bool isCriticalChanceEnabled();
	static int getBadgeCriticalChance(const String& badgeKey);
	static int getCriticalChance(PlayerObject* ghost);
	static int getCriticalChance(PlayerObject* ghost, const char* const* badgeKeys, int count);
	static int getCriticalMultiplier(CreatureObject* player);
	static String formatPercent(int basisPoints);
	static String colorizeCriticalText(const String& text);
	static int getModifierTotal(CreatureObject* player, CustomSkillsModifierType::Type type);
	static int getBadgeModifier(const String& badgeKey, CustomSkillsModifierType::Type type);
	static bool isModifierEnabled(CustomSkillsModifierType::Type type);
	static int applyModifierCap(CustomSkillsModifierType::Type type, int value);
	static String getModifierName(CustomSkillsModifierType::Type type);
	static String formatModifierBonus(CustomSkillsModifierType::Type type, int value);
	static void notifyBadgeAwarded(CreatureObject* player);

private:
	static bool isCombatProfessionBadge(const String& badgeKey);
};

#endif
