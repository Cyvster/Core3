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

	// BRIEF-034: consolidated-strike floating combat text + chat tag knobs.
	bool isFctEnabled() const { return fctEnabled; }
	int getFctScaleStepBp() const { return fctScaleStepBp; }
	int getFctCritBonusBp() const { return fctCritBonusBp; }
	const String& getFctTierColor(int tier) const; // tier 2..4
	const String& getFctCritColor() const { return fctCritColor; }
	bool isChatTagEnabled() const { return chatTagEnabled; }
	const String& getCustomSummaryColor() const { return customSummaryColor; }
	bool isRarityNamingEnabled() const { return rarityNamingEnabled; }
	const String& getLegendaryColor() const { return legendaryColor; }
	const String& getExceptionalColor() const { return exceptionalColor; }
	bool isModifierEnabled(CustomSkillsModifierType::Type type) const;
	int getModifierCap(CustomSkillsModifierType::Type type) const;
	int getBadgeBonus(CustomSkillsModifierType::Type type, const String& badgeKey) const;
	const VectorMap<String, int>& getBadgeBonuses(CustomSkillsModifierType::Type type) const;

private:
	bool criticalChanceEnabled;
	int criticalChanceFallbackBonus;
	int criticalMultiplier;
	String customSummaryColor;
	bool rarityNamingEnabled;
	String legendaryColor;
	String exceptionalColor;

	// BRIEF-034: consolidated-strike FCT + chat tag state.
	bool fctEnabled;
	int fctScaleStepBp;   // basis points of scale added per tier above base
	int fctCritBonusBp;   // extra basis points of scale on critical hits
	String fctTier2Color; // hex RRGGBB for Double tier flytext
	String fctTier3Color; // hex RRGGBB for Triple tier flytext
	String fctTier4Color; // hex RRGGBB for Quad tier flytext
	String fctCritColor;  // hex RRGGBB overlay color when the hit crits
	bool chatTagEnabled;

	static const int DEFAULT_FCT_SCALE_STEP_BP = 1500;
	static const int DEFAULT_FCT_CRIT_BONUS_BP = 2500;

	bool modifierEnabled[CustomSkillsModifierType::COUNT];
	int modifierCaps[CustomSkillsModifierType::COUNT];
	VectorMap<String, int> modifierBadgeBonuses[CustomSkillsModifierType::COUNT];

	void setDefaults();
	void load();
	void loadModifier(LuaObject& modifiers, const String& name, CustomSkillsModifierType::Type type,
		bool defaultEnabled, int defaultBadgeBonus);
	void loadBadgeOverrides(LuaObject& table, CustomSkillsModifierType::Type type);
	void setUniformBadgeBonus(CustomSkillsModifierType::Type type, int value);
};

#endif
