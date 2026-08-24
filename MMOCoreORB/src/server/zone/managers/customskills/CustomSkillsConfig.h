#ifndef CUSTOMSKILLSCONFIG_H_
#define CUSTOMSKILLSCONFIG_H_

#include "engine/engine.h"
#include "CustomSkillsModifierType.h"

class CustomSkillsConfig : public Singleton<CustomSkillsConfig>, public Object, public Logger {
public:
	static const int DEFAULT_CRITICAL_MULTIPLIER = 15000;

	CustomSkillsConfig();

	bool isCriticalChanceEnabled() const { return criticalChanceEnabled; }
	int getCriticalMultiplier() const { return criticalMultiplier; }
	const String& getCustomSummaryColor() const { return customSummaryColor; }
	const String& getCriticalCombatSpamLabel() const { return criticalCombatSpamLabel; }
	bool isRarityNamingEnabled() const { return rarityNamingEnabled; }
	const String& getLegendaryColor() const { return legendaryColor; }
	const String& getExceptionalColor() const { return exceptionalColor; }
	bool isModifierEnabled(CustomSkillsModifierType::Type type) const;
	int getModifierCap(CustomSkillsModifierType::Type type) const;
	int getBadgeBonus(CustomSkillsModifierType::Type type, const String& badgeKey) const;
	const VectorMap<String, int>& getBadgeBonuses(CustomSkillsModifierType::Type type) const;
	const String& getModifierCombatSpamLabel(CustomSkillsModifierType::Type type) const;

private:
	bool criticalChanceEnabled;
	int criticalChanceFallbackBonus;
	int criticalMultiplier;
	String customSummaryColor;
	String criticalCombatSpamLabel;
	bool rarityNamingEnabled;
	String legendaryColor;
	String exceptionalColor;
	bool modifierEnabled[CustomSkillsModifierType::COUNT];
	int modifierCaps[CustomSkillsModifierType::COUNT];
	VectorMap<String, int> modifierBadgeBonuses[CustomSkillsModifierType::COUNT];
	String modifierCombatSpamLabels[CustomSkillsModifierType::COUNT];

	void setDefaults();
	void load();
	void loadModifier(LuaObject& modifiers, const String& name, CustomSkillsModifierType::Type type,
		bool defaultEnabled, int defaultBadgeBonus);
	void loadBadgeOverrides(LuaObject& table, CustomSkillsModifierType::Type type);
	void setUniformBadgeBonus(CustomSkillsModifierType::Type type, int value);
};

#endif
