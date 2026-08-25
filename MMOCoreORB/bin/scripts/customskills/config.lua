-- Custom Skills module configuration.
-- Percentage values use basis points: 100 = 1.00%.
-- Restart Core3 after changing this file.
-- badgeOverrides uses { "badgeKey", value } pairs to override per-badge values.

customSkillsConfig = {
	-- RGB color used for custom modifier text in SUI menus.
	customSummaryColor = "00FF00",

	-- Show combat spam labels like (CRIT) after qualifying hits. Labels are
	-- sent as their own combat message (client limitation: a label cannot
	-- be appended onto the native hit line).
	combatSpamLabelsEnabled = true,

	-- Rarity naming: color-only display instead of appending "(Exceptional)"/"(Legendary)" to item names.
	-- When enabled, item names use colored text instead of suffixes, which also fixes quest turn-in
	-- compatibility (quest systems match items by name, so suffixes break the match).
	-- Set enabled = true to use color-only naming, or false to use the original SWGEmu suffix system.
	-- Color values are six-character RGB hex codes (e.g., "FF00FF" for purple, "00FFFF" for cyan).
	rarityNaming = {
		enabled = true,
		legendaryColor = "FF00FF",   -- purple for legendary
		exceptionalColor = "0000FF", -- blue for exceptional
	},

	criticalChance = {
		enabled = true,
		cap = 6000,

		-- Fallback for any listed badge that lacks an explicit override
		-- below. Kept at 0: every badge declares its enough value via
		-- badgeOverrides, so nothing is granted silently.
		badgeBonus = 100,

		-- Critical damage multiplier. 15000 = 150.00% damage.
		multiplier = 15000,

		-- Compact label sent when this effect activates.
		combatSpamLabel = "(CRIT)",

		-- 60 achievement badges: +1% (100 bp) crit chance each, declared
		-- per badge in badgeOverrides. Full set reaches the 60% cap.
		badges = {
			-- Accumulation milestones (7)
			"count_5", "count_10", "count_25", "count_50", "count_75", "count_100", "count_125",
			-- Exploration milestones (5)
			"bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges", "bdg_exp_40_badges", "bdg_exp_45_badges",
			-- Combat profession masteries (12)
			"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
			"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
			"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
			"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
			-- Hero of Tatooine (5)
			"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			-- Warren (2)
			"warren_compassion", "warren_hero",
			-- Theme Parks (4)
			"bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
			"bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge",
			-- Corellian Corvette (9)
			"bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin",
			"bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin",
			"bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin",
			-- Exploration: dangerous sites (5)
			"exp_tat_tusken_pool", "exp_tat_krayt_skeleton", "exp_tat_sarlacc_pit", "exp_tat_krayt_graveyard", "exp_dat_sarlacc",
			-- Exploration: Jedi sites (3)
			"exp_tat_bens_hut", "exp_yav_temple_exar_kun", "exp_dan_jedi_temple",
			-- Outdoors masteries (5)
			"outdoors_bio_engineer_master", "outdoors_creaturehandler_master",
			"outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master",
			-- Science masteries (3)
			"science_combatmedic_master", "science_doctor_master", "science_medic_master",
		},
		-- Placeholder: { "badge_key", basisPointValue }
		-- Example: { "count_5", 100 }
	},

	-- Generic badge-backed modifiers.
	-- cap = 0 means uncapped. Values use basis points except whole-point/level
	-- modifiers (armor penetration, defense cap, and SEA cap).
	modifiers = {
		doubleAttackChance = {
			enabled = true,
			badgeBonus = 2000,
			cap = 10000,
			combatSpamLabel = "(DOUBLE)",
			badges = {
				"warren_compassion", "warren_hero",
				"bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
				"bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "warren_compassion", 2000 }
		},

		-- 12 milestone badges: +12.5% Crit Multiplier each (150% bonus on top of 150% base = 300% max).
		criticalMultiplier = {
			enabled = true,
			badgeBonus = 1250,
			cap = 15000,
			badges = {
				"count_5", "count_10", "count_25", "count_50", "count_75", "count_100", "count_125",
				"bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges", "bdg_exp_40_badges", "bdg_exp_45_badges",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "count_5", 1250 }
		},

		tripleAttackChance = {
			enabled = true,
			badgeBonus = 500,
			cap = 8500,
			combatSpamLabel = "(TRIPLE)",
			badges = {
				"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
				"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
				"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
				"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "combat_1hsword_master", 500 }
		},

		quadAttackChance = {
			enabled = true,
			badgeBonus = 300,
			cap = 5100,
			combatSpamLabel = "(QUAD)",
			badges = {
				"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
				"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
				"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
				"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "combat_1hsword_master", 300 }
		},

		armorPenetration = {
			enabled = true,
			badgeBonus = 1,
			cap = 3,
			badges = {
				"bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin",
				"bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin",
				"bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "bdg_corvette_imp_destroy", 1 }
		},

		defenseCapIncrease = {
			enabled = true,
			badgeBonus = 20,
			cap = 100,
			badges = {
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "poi_rabidbeast", 20 }
		},

		armorDegradeReduction = {
			enabled = true,
			badgeBonus = 1250,
			cap = 10000,
			badges = {
				"bdg_exp_tal_creature_village", "bdg_exp_tal_imp_base",
				"bdg_exp_tal_imp_vs_reb_battle", "bdg_exp_tal_aqualish_cave",
				"bdg_exp_ror_kobala_spice_mine", "bdg_exp_ror_rebel_outpost",
				"bdg_exp_ror_imp_camp", "bdg_exp_ror_imp_hyperdrive_fac",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "bdg_exp_tal_creature_village", 1250 }
		},

		weaponDegradeReduction = {
			enabled = true,
			badgeBonus = 1250,
			cap = 10000,
			badges = {
				"exp_dan_jedi_temple", "exp_dan_rebel_base",
				"bdg_exp_dan_dantari_village1", "bdg_exp_dan_dantari_village2",
				"bdg_exp_end_ewok_tree_village", "bdg_exp_end_ewok_lake_village",
				"bdg_exp_end_dulok_village", "bdg_exp_end_imp_outpost",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_dan_jedi_temple", 1250 }
		},

		seaCapIncrease = {
			enabled = true,
			badgeBonus = 20,
			cap = 100,
			badges = {
				"exp_tat_tusken_pool", "exp_tat_krayt_skeleton",
				"exp_tat_sarlacc_pit", "exp_tat_krayt_graveyard",
				"exp_dat_sarlacc",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_tat_tusken_pool", 20 }
		},

		movementSpeed = {
			enabled = true,
			badgeBonus = 2500,
			cap = 0,
			badges = {
				"exp_nab_gungan_sacred_place", "bdg_exp_nab_theed_falls_bottom",
				"bdg_exp_nab_deeja_falls_top", "bdg_exp_nab_amidalas_sandy_beach",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_nab_gungan_sacred_place", 2500 }
		},

		buffDuration = {
			enabled = true,
			badgeBonus = 2500,
			cap = 0,
			badges = {
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				"science_combatmedic_master", "science_doctor_master", "science_medic_master",
				"social_dancer_master", "social_entertainer_master", "social_imagedesigner_master",
				"social_musician_master", "social_politician_master",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_tat_escape_pod", 2500 }
		},

		experienceMultiplier = {
			enabled = true,
			badgeBonus = 10000,
			cap = 0,
			badges = {
				"exp_cor_agrilat_swamp", "bdg_exp_cor_rebel_hideout",
				"bdg_exp_cor_rogue_corsec_base", "bdg_exp_cor_tyrena_theater",
				"bdg_exp_cor_bela_vistal_fountain",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_cor_agrilat_swamp", 10000 }
		},

		practiceExperienceBonus = {
			enabled = true,
			badgeBonus = 10000,
			cap = 0,
			badges = {
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				"social_dancer_master", "social_entertainer_master", "social_imagedesigner_master",
				"social_musician_master", "social_politician_master",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_tat_escape_pod", 10000 }
		},

		craftingSpeed = {
			enabled = true,
			badgeBonus = 1000,
			cap = 0,
			badges = {
				"crafting_architect_master", "crafting_armorsmith_master", "crafting_artisan_master",
				"crafting_chef_master", "crafting_droidengineer_master", "crafting_merchant_master",
				"crafting_shipwright", "crafting_tailor_master", "crafting_weaponsmith_master",
				"science_doctor_master",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "crafting_architect_master", 1000 }
		},

		amazingSuccessChance = {
			enabled = true,
			badgeBonus = 500,
			cap = 10000,
			badges = {
				"exp_yav_temple_exar_kun",
				"exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton",
				"exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2",
				"bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_yav_temple_exar_kun", 500 }
		},

		amazingResults = {
			enabled = true,
			badgeBonus = 1000,
			cap = 10000,
			badges = {
				"exp_yav_temple_exar_kun",
				"exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton",
				"exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2",
				"bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_yav_temple_exar_kun", 1000 }
		},

		gatheringQuantity = {
			enabled = true,
			badgeBonus = 10000,
			cap = 0,
			badges = {
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				"outdoors_bio_engineer_master", "outdoors_creaturehandler_master",
				"outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master",
				"science_combatmedic_master", "science_doctor_master", "science_medic_master",
			},
			-- Placeholder: { "badge_key", basisPointValue }
			-- Example: { "exp_tat_escape_pod", 10000 }
		},
	},
}