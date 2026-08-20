-- Custom Skills module configuration.
-- Percentage values use basis points: 100 = 1.00%.
-- Restart Core3 after changing this file.

customSkillsConfig = {
	-- RGB color used for custom modifier text in SUI menus.
	customSummaryColor = "00FF00",

	criticalChance = {
		enabled = true,

		-- Critical Chance granted by each acquired combat profession badge.
		badgeBonus = 300,

		-- Critical damage multiplier. 15000 = 150.00% damage.
		multiplier = 15000,

		-- Compact label sent when this effect activates.
		-- This is temporary development feedback and will be removed later.
		combatSpamLabel = "(CRIT)",
	},

	-- Generic badge-backed modifiers. Empty badge lists are intentional: assign
	-- real obtainable badge keys when enabling a modifier for your server.
	-- cap = 0 means uncapped. Values use basis points except whole-point/level
	-- modifiers (armor penetration, defense cap, and SEA cap).
	modifiers = {
		doubleAttackChance = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(DOUBLE)", badges = {} },
		tripleAttackChance = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(TRIPLE)", badges = {} },
		quadAttackChance = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(QUAD)", badges = {} },
		armorPenetration = { enabled = false, badgeBonus = 0, cap = 3, badges = {} },
		defenseCapIncrease = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		armorDegradeReduction = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
		weaponDegradeReduction = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
		seaCapIncrease = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		movementSpeed = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		buffDuration = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		experienceMultiplier = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		practiceExperienceBonus = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		craftingSpeed = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
		amazingSuccessChance = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
		amazingResults = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
		gatheringQuantity = { enabled = false, badgeBonus = 20000, cap = 0, badges = {} },
	},
}
