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
		modifierCombatSpamLabels[i] = "";
	}

	criticalChanceEnabled = true;
	criticalChancePerCombatBadge = DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE;
	criticalMultiplier = DEFAULT_CRITICAL_MULTIPLIER;
	customSummaryColor = "00FF00";
	criticalCombatSpamLabel = "(CRIT)";
	modifierEnabled[CustomSkillsModifierType::CRITICAL_CHANCE] = true;
	modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE] = 6000;

	const char* combatBadges[] = {
		"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
		"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
		"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
		"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master"
	};
	for (const char* badge : combatBadges)
		modifierBadgeBonuses[CustomSkillsModifierType::CRITICAL_CHANCE].put(badge, DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE);
}

bool CustomSkillsConfig::isModifierEnabled(CustomSkillsModifierType::Type type) const {
	return type >= 0 && type < CustomSkillsModifierType::COUNT && modifierEnabled[type];
}

int CustomSkillsConfig::getModifierCap(CustomSkillsModifierType::Type type) const {
	return type >= 0 && type < CustomSkillsModifierType::COUNT ? modifierCaps[type] : 0;
}

int CustomSkillsConfig::getBadgeBonus(CustomSkillsModifierType::Type type, const String& badgeKey) const {
	if (!isModifierEnabled(type) || !modifierBadgeBonuses[type].contains(badgeKey))
		return 0;
	return modifierBadgeBonuses[type].get(badgeKey);
}

const VectorMap<String, int>& CustomSkillsConfig::getBadgeBonuses(CustomSkillsModifierType::Type type) const {
	return modifierBadgeBonuses[type];
}

const String& CustomSkillsConfig::getModifierCombatSpamLabel(CustomSkillsModifierType::Type type) const {
	return modifierCombatSpamLabels[type];
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
	modifierCombatSpamLabels[type] = modifier.getStringField("combatSpamLabel", modifierCombatSpamLabels[type].toCharArray());

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

		int badgeBonus = critical.getIntField("badgeBonus", DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE);
		if (badgeBonus >= 0 && badgeBonus <= 10000)
			criticalChancePerCombatBadge = badgeBonus;
		else
			warning("criticalChance.badgeBonus must be from 0 to 10000; using default");
		setUniformBadgeBonus(CustomSkillsModifierType::CRITICAL_CHANCE, criticalChancePerCombatBadge);

		int cap = critical.getIntField("cap", modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE]);
		if (cap >= 0)
			modifierCaps[CustomSkillsModifierType::CRITICAL_CHANCE] = cap;

		LuaObject ccBadges = critical.getObjectField("badges");
		if (ccBadges.isValidTable()) {
			for (int i = 1; i <= ccBadges.getTableSize(); ++i) {
				String badge = ccBadges.getStringAt(i);
				if (!badge.isEmpty())
					modifierBadgeBonuses[CustomSkillsModifierType::CRITICAL_CHANCE].put(badge, criticalChancePerCombatBadge);
			}
		}
		ccBadges.pop();

		loadBadgeOverrides(critical, CustomSkillsModifierType::CRITICAL_CHANCE);

		int multiplier = critical.getIntField("multiplier", DEFAULT_CRITICAL_MULTIPLIER);
		if (multiplier >= 10000)
			criticalMultiplier = multiplier;
		else
			warning("criticalChance.multiplier must be at least 10000; using default");

		String spamLabel = critical.getStringField("combatSpamLabel", "(CRIT)");
		if (!spamLabel.isEmpty())
			criticalCombatSpamLabel = spamLabel;
	} else {
		warning("customSkillsConfig.criticalChance is missing or invalid; using defaults");
	}
	critical.pop();

	LuaObject modifiers = root.getObjectField("modifiers");
	if (modifiers.isValidTable()) {
		loadModifier(modifiers, "doubleAttackChance", CustomSkillsModifierType::DOUBLE_ATTACK_CHANCE, false, 0);
		loadModifier(modifiers, "tripleAttackChance", CustomSkillsModifierType::TRIPLE_ATTACK_CHANCE, false, 0);
		loadModifier(modifiers, "quadAttackChance", CustomSkillsModifierType::QUAD_ATTACK_CHANCE, false, 0);
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
		loadModifier(modifiers, "gatheringQuantity", CustomSkillsModifierType::GATHERING_QUANTITY, false, 20000);
	}
	modifiers.pop();
	root.pop();
}
