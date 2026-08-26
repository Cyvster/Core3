#include "CustomSkillsConfig.h"

#include "engine/lua/Lua.h"

CustomSkillsConfig::CustomSkillsConfig() : Logger("CustomSkillsConfig") {
	setDefaults();
	load();
}

void CustomSkillsConfig::setDefaults() {
	for (int i = 0; i < CustomSkillsModifierType::COUNT; ++i) {
		modifierEnabled[i] = false;
		modifierCaps[i] = 0;
		modifierBadgeBonuses[i].setAllowOverwriteInsertPlan();
	}

	criticalChanceEnabled = true;
	// criticalChanceFallbackBonus stays 0: badge bonuses come exclusively
	// from config.lua (badges[] + badgeOverrides); nothing is seeded here.
	criticalChanceFallbackBonus = 0;
	criticalMultiplier = DEFAULT_CRITICAL_MULTIPLIER;
	customSummaryColor = "00FF00";
	rarityNamingEnabled = false;
	legendaryColor = "FF00FF";
	exceptionalColor = "0000FF";

	// BRIEF-034 defaults: FCT on; +15.00% scale per tier (1500 bp), +25.00%
	// extra scale on crits (2500 bp); white->yellow->orange tier escalation,
	// gold crit overlay; chat tag on.
	fctEnabled = true;
	fctScaleStepBp = DEFAULT_FCT_SCALE_STEP_BP;
	fctCritBonusBp = DEFAULT_FCT_CRIT_BONUS_BP;
	fctTier2Color = "FFFF00"; // yellow
	fctTier3Color = "FF9900"; // orange
	fctTier4Color = "FF0000"; // red
	fctCritColor  = "FFD700"; // gold
	chatTagEnabled = true;

	// BRIEF-036 defaults: repeat-craft OFF until the operator opts in;
	// practice-mode repeats allowed (matches vanilla, owner directive).

	// BRIEF-043 defaults: mission options ON (owner opt-in feature set);
	// list size 3 = vanilla datapad cap; descriptive titles on.
	missionOptionsEnabled = true;
	missionDirectionEnabled = true;
	missionDifficultyEnabled = true;
	missionListSize = DEFAULT_MISSION_LIST_SIZE;

	// BRIEF-050: account-shared structure lots (owner approved default ON).
	accountSharedLots = DEFAULT_ACCOUNT_SHARED_LOTS;

	// BRIEF-046/047/048 defaults (owner approved): credits-to-player ON,
	// attachment auto-naming ON, survey max range 2624m (vanilla 384).
	lootCreditsToTopDamager = true; // E02
	attachmentAutoName = true;      // E05
	surveyMaxRange = DEFAULT_SURVEY_MAX_RANGE; // C06

	descriptiveTitles = true;

	modifierEnabled[CustomSkillsModifierType::CRITICAL_CHANCE] = true;
	modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE] = 6000;
}

bool CustomSkillsConfig::isModifierEnabled(CustomSkillsModifierType::Type type) const {
	return type >= 0 && type < CustomSkillsModifierType::COUNT && modifierEnabled[type];
}

int CustomSkillsConfig::getModifierCap(CustomSkillsModifierType::Type type) const {
	return type >= 0 && type < CustomSkillsModifierType::COUNT ? modifierCaps[type] : 0;
}

const String& CustomSkillsConfig::getFctTierColor(int tier) const {
	switch (tier) {
	case 2:
		return fctTier2Color;
	case 3:
		return fctTier3Color;
	default:
		return fctTier4Color;
	}
}

int CustomSkillsConfig::getBadgeBonus(CustomSkillsModifierType::Type type, const String& badgeKey) const {
	if (!isModifierEnabled(type) || !modifierBadgeBonuses[type].contains(badgeKey))
		return 0;
	return modifierBadgeBonuses[type].get(badgeKey);
}

const VectorMap<String, int>& CustomSkillsConfig::getBadgeBonuses(CustomSkillsModifierType::Type type) const {
	return modifierBadgeBonuses[type];
}

void CustomSkillsConfig::setUniformBadgeBonus(CustomSkillsModifierType::Type type, int value) {
	VectorMap<String, int>& bonuses = modifierBadgeBonuses[type];
	for (int i = 0; i < bonuses.size(); ++i) {
		String badge = bonuses.elementAt(i).getKey();
		bonuses.put(badge, value);
	}
}

void CustomSkillsConfig::loadModifier(LuaObject& modifiers, const String& name,
		CustomSkillsModifierType::Type type, bool defaultEnabled, int defaultBadgeBonus) {
	LuaObject modifier = modifiers.getObjectField(name);
	if (!modifier.isValidTable()) {
		modifier.pop();
		return;
	}

	modifierEnabled[type] = modifier.getBooleanField("enabled", defaultEnabled);
	modifierCaps[type] = static_cast<int>(modifier.getIntField("cap", modifierCaps[type]));
	if (modifierCaps[type] < 0)
		modifierCaps[type] = 0;
	int badgeBonus = static_cast<int>(modifier.getIntField("badgeBonus", defaultBadgeBonus));
	if (badgeBonus < 0)
		badgeBonus = 0;

	LuaObject badges = modifier.getObjectField("badges");
	if (badges.isValidTable()) {
		modifierBadgeBonuses[type].removeAll();
		for (int i = 1; i <= badges.getTableSize(); ++i) {
			String badge = badges.getStringAt(i);
			if (!badge.isEmpty())
				modifierBadgeBonuses[type].put(badge, badgeBonus);
		}
	}
	badges.pop();

	loadBadgeOverrides(modifier, type);

	modifier.pop();
}

// Note: an override whose key is absent from badges[] ADDS that key to the
// assignment map (by design -- overrides are the explicit per-badge source).
// Contract: overrides REPLACE the badgeBonus entry for that key -- they
// never stack with it. A badge listed in both badges[] and badgeOverrides
// contributes only its override value. One value per badge key, always.
void CustomSkillsConfig::loadBadgeOverrides(LuaObject& table, CustomSkillsModifierType::Type type) {
	LuaObject overrides = table.getObjectField("badgeOverrides");
	if (overrides.isValidTable()) {
		for (int i = 1; i <= overrides.getTableSize(); ++i) {
			LuaObject entry = overrides.getObjectAt(i);
			if (entry.isValidTable() && entry.getTableSize() == 2) {
				String key = entry.getStringAt(1);
				int value = entry.getIntAt(2);
				if (!key.isEmpty() && value >= 0)
					modifierBadgeBonuses[type].put(key, value);
			}
			entry.pop();
		}
	}
	overrides.pop();
}

void CustomSkillsConfig::load() {
	Lua lua;
	lua.init();

	if (!lua.runFile("scripts/customskills/config.lua")) {
		warning("Unable to read scripts/customskills/config.lua; using defaults");
		return;
	}

	LuaObject root = lua.getGlobalObject("customSkillsConfig");
	if (!root.isValidTable()) {
		warning("customSkillsConfig is missing or invalid; using defaults");
		return;
	}

	String summaryColor = root.getStringField("customSummaryColor", "00FF00");
	if (summaryColor.length() == 6)
		customSummaryColor = summaryColor;
	else
		warning("customSummaryColor must be a six-character RGB hex value; using default");

	LuaObject critical = root.getObjectField("criticalChance");
	if (critical.isValidTable()) {
		criticalChanceEnabled = critical.getBooleanField("enabled", true);
		modifierEnabled[CustomSkillsModifierType::CRITICAL_CHANCE] = criticalChanceEnabled;

		// Explicit 0 default: a listed badge without an override grants
		// nothing. There is intentionally no non-zero code-side fallback.
		int badgeBonus = critical.getIntField("badgeBonus", 0);
		if (badgeBonus >= 0 && badgeBonus <= 10000)
			criticalChanceFallbackBonus = badgeBonus;
		else
			warning("criticalChance.badgeBonus must be from 0 to 10000; treating as 0");
		setUniformBadgeBonus(CustomSkillsModifierType::CRITICAL_CHANCE, criticalChanceFallbackBonus);

		int cap = critical.getIntField("cap", modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE]);
		if (cap < 0)
			cap = 0;
		modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE] = cap;

		LuaObject ccBadges = critical.getObjectField("badges");
		if (ccBadges.isValidTable()) {
			for (int i = 1; i <= ccBadges.getTableSize(); ++i) {
				String badge = ccBadges.getStringAt(i);
				if (!badge.isEmpty())
					modifierBadgeBonuses[CustomSkillsModifierType::CRITICAL_CHANCE].put(badge, criticalChanceFallbackBonus);
			}
		}
		ccBadges.pop();

		loadBadgeOverrides(critical, CustomSkillsModifierType::CRITICAL_CHANCE);

		int multiplier = critical.getIntField("multiplier", DEFAULT_CRITICAL_MULTIPLIER);
		if (multiplier >= 10000)
			criticalMultiplier = multiplier;
		else
			warning("criticalChance.multiplier must be at least 10000; using default");
	} else {
		warning("customSkillsConfig.criticalChance is missing or invalid; using defaults");
	}
	critical.pop();

	LuaObject rarity = root.getObjectField("rarityNaming");
	if (rarity.isValidTable()) {
		rarityNamingEnabled = rarity.getBooleanField("enabled", false);

		String legendary = rarity.getStringField("legendaryColor", "FF00FF");
		if (legendary.length() == 6)
			legendaryColor = legendary;
		else
			warning("rarityNaming.legendaryColor must be a six-character RGB hex value; using default");

		String exceptional = rarity.getStringField("exceptionalColor", "0000FF");
		if (exceptional.length() == 6)
			exceptionalColor = exceptional;
		else
			warning("rarityNaming.exceptionalColor must be a six-character RGB hex value; using default");
	}
	rarity.pop();

	// BRIEF-034: consolidated-strike FCT + chat tag knobs.
	LuaObject fct = root.getObjectField("consolidatedStrike");
	if (fct.isValidTable()) {
		fctEnabled = fct.getBooleanField("fctEnabled", true);

		int scaleStep = static_cast<int>(fct.getIntField("fctScaleStepBp", DEFAULT_FCT_SCALE_STEP_BP));
		if (scaleStep >= 0)
			fctScaleStepBp = scaleStep;
		else
			warning("consolidatedStrike.fctScaleStepBp must be non-negative; using default");

		int critBonus = static_cast<int>(fct.getIntField("fctCritBonusBp", DEFAULT_FCT_CRIT_BONUS_BP));
		if (critBonus >= 0)
			fctCritBonusBp = critBonus;
		else
			warning("consolidatedStrike.fctCritBonusBp must be non-negative; using default");

		struct FctColorField { const char* key; String* target; };
		FctColorField colorFields[] = {
			{ "tier2Color", &fctTier2Color },
			{ "tier3Color", &fctTier3Color },
			{ "tier4Color", &fctTier4Color },
			{ "critColor",  &fctCritColor },
		};
		for (int i = 0; i < 4; ++i) {
			String value = fct.getStringField(colorFields[i].key, "");
			if (value.length() == 6)
				*(colorFields[i].target) = value;
			else
				warning(String("consolidatedStrike.") + colorFields[i].key + " must be a six-character RGB hex value; using default");
		}

		chatTagEnabled = fct.getBooleanField("chatTagEnabled", true);
	}
	fct.pop();

	// BRIEF-043: mission terminal direction/difficulty options.
	LuaObject missions = root.getObjectField("missions");
	if (missions.isValidTable()) {
		missionOptionsEnabled = missions.getBooleanField("missionOptionsEnabled", true);
		missionDirectionEnabled = missions.getBooleanField("directionOptionEnabled", true);
		missionDifficultyEnabled = missions.getBooleanField("difficultyOptionEnabled", true);

		int listSize = static_cast<int>(missions.getIntField("missionListSize", DEFAULT_MISSION_LIST_SIZE));
		if (listSize > 0)
			missionListSize = listSize;
		else
			warning("missions.missionListSize must be positive; using default");

		descriptiveTitles = missions.getBooleanField("descriptiveTitles", true);
	}
	missions.pop();

	// BRIEF-050: account-shared structure lots.
	LuaObject structures = root.getObjectField("structures");
	if (structures.isValidTable()) {
		accountSharedLots = structures.getBooleanField("accountSharedLots", DEFAULT_ACCOUNT_SHARED_LOTS);
	}
	structures.pop();

	// BRIEF-046/047/048: loot + survey QoL knobs.
	LuaObject loot = root.getObjectField("loot");
	if (loot.isValidTable()) {
		lootCreditsToTopDamager = loot.getBooleanField("creditsToTopDamager", true); // E02
		attachmentAutoName = loot.getBooleanField("attachmentAutoName", true);       // E05
	}
	loot.pop();

	// C06: surveying.maxRange, in meters. Clamp to the vanilla floor of 64
	// (smallest vanilla tier) and a sane 8192 ceiling.
	LuaObject surveying = root.getObjectField("surveying");
	if (surveying.isValidTable()) {
		int maxRange = static_cast<int>(surveying.getIntField("maxRange", DEFAULT_SURVEY_MAX_RANGE));
		if (maxRange >= 64 && maxRange <= 8192)
			surveyMaxRange = maxRange;
		else
			warning("surveying.maxRange must be between 64 and 8192 meters; using default");
	}
	surveying.pop();



	LuaObject modifiers = root.getObjectField("modifiers");
	if (modifiers.isValidTable()) {
		loadModifier(modifiers, "doubleAttackChance", CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE, false, 0);
		loadModifier(modifiers, "tripleAttackChance", CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE, false, 0);
		loadModifier(modifiers, "quadAttackChance", CustomSkillsModifierType::QUAD_ATTACK_CHANCE, false, 0);
		loadModifier(modifiers, "criticalMultiplier", CustomSkillsModifierType::CRITICAL_MULTIPLIER, false, 0);
		loadModifier(modifiers, "armorPenetration", CustomSkillsModifierType::ARMOR_PENETRATION, false, 0);
		loadModifier(modifiers, "defenseCapIncrease", CustomSkillsModifierType::DEFENSE_CAP_INCREASE, false, 0);
		loadModifier(modifiers, "armorDegradeReduction", CustomSkillsModifierType::ARMOR_DEGRADE_REDUCTION, false, 0);
		loadModifier(modifiers, "weaponDegradeReduction", CustomSkillsModifierType::WEAPON_DEGRADE_REDUCTION, false, 0);
		loadModifier(modifiers, "seaCapIncrease", CustomSkillsModifierType::SEA_CAP_INCREASE, false, 0);
		loadModifier(modifiers, "movementSpeed", CustomSkillsModifierType::MOVEMENT_SPEED, false, 0);
		loadModifier(modifiers, "buffDuration", CustomSkillsModifierType::BUFF_DURATION, false, 0);
		loadModifier(modifiers, "experienceMultiplier", CustomSkillsModifierType::EXPERIENCE_MULTIPLIER, false, 0);
		loadModifier(modifiers, "practiceExperienceBonus", CustomSkillsModifierType::PRACTICE_EXPERIENCE_BONUS, false, 0);
		loadModifier(modifiers, "craftingSpeed", CustomSkillsModifierType::CRAFTING_SPEED, false, 0);
		loadModifier(modifiers, "amazingSuccessChance", CustomSkillsModifierType::AMAZING_SUCCESS_CHANCE, false, 0);
		loadModifier(modifiers, "amazingResults", CustomSkillsModifierType::AMAZING_RESULTS, false, 0);
		static constexpr int DEFAULT_GATHERING_QUANTITY_BONUS = 20000;
		loadModifier(modifiers, "gatheringQuantity", CustomSkillsModifierType::GATHERING_QUANTITY, false, DEFAULT_GATHERING_QUANTITY_BONUS);
	}
	modifiers.pop();
	root.pop();
}
