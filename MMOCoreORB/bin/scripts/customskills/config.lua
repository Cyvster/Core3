-- =============================================================================
-- Custom Skills Module Configuration
-- =============================================================================
-- HOW VALUES WORK:
--
--   Percentage-based settings use BASIS POINTS (bp).
--       100 bp  = 1.00%
--       1000 bp = 10.00%
--       10000 bp = 100.00% (the full amount)
--
--   To convert a percentage you want into basis points, multiply by 100.
--   Example: you want each badge to give 2.5% movement speed -> 2.5 * 100 = 250.
--
--   EXCEPTIONS (whole units, NOT basis points):
--       armorPenetration   -> whole armor levels (Heavy/Medium/Light/None)
--       defenseCapIncrease -> whole defense points
--       seaCapIncrease     -> whole skill-attachment points
--
-- AFTER EDITING THIS FILE:
--   Restart the Core3 server. Config is loaded once at startup; changes do
--   NOT hot-reload. Players can verify live values with /customskills in game.
--
-- BADGE KEYS:
--   Badge keys must match BadgeList entries exactly (case-sensitive). Never
--   hard-code numeric badge indices -- keys are stable across data-archive
--   changes, indices are not.
-- =============================================================================

customSkillsConfig = {

	-- ---------------------------------------------------------------------------
	-- customSummaryColor : RGB hex color for bonus text in the /customskills menu
	-- Possible values  : any six-character RGB hex string ("RRGGBB")
	-- Default          : "00FF00" (green)
	-- What it does     : colors the "+X% ..." modifier text players see on menu
	--                    pages so bonuses stand out from regular label text.
	-- Common choices   : "00FF00" green | "FFFF00" yellow | "00FFFF" cyan |
	--                    "FF00FF" magenta | "FFFFFF" white
	-- ---------------------------------------------------------------------------
	customSummaryColor = "00FF00",

	-- ---------------------------------------------------------------------------
	-- rarityNaming : how crafted-item rarity is displayed
	-- What it does : when enabled, Exceptional/Legendary crafted items show their
	--                rarity as colored item-name text instead of a "(Exceptional)"
	--                or "(Legendary)" suffix appended to the name.
	-- Why you care : quest turn-ins that match items by EXACT NAME break when a
	--                suffix is present. Color-only naming keeps names clean, so
	--                those quests work. Set enabled = false to restore the
	--                original SWGEmu suffix behavior if your server has no such
	--                quests or prefers the classic look.
	-- Fields       :
	--   enabled          true = color-only naming, false = vanilla suffixes
	--   legendaryColor   six-char RGB hex, applied to Legendary items
	--   exceptionalColor six-char RGB hex, applied to Exceptional items
	-- Defaults     : enabled = true, legendary "FF00FF" (purple),
	--                exceptional "0000FF" (blue)
	-- ---------------------------------------------------------------------------
	rarityNaming = {
		enabled = true,
		legendaryColor = "FF00FF",   -- purple for legendary
		exceptionalColor = "0000FF", -- blue for exceptional
	},

	-- ---------------------------------------------------------------------------
	-- consolidatedStrike : Double/Triple/Quad presentation (BRIEF-034)
	-- What it does       : Double/Triple/Quad hits are delivered as ONE bigger
	--                      hit (damage x tier) instead of several repeat hits.
	--                      These knobs control how that strike is PRESENTED:
	--                      a scaled/colored flytext on the defender and an
	--                      "x2"/"x3"/"x4" tag line in the attacker's combat spam.
	-- Fields             :
	--   fctEnabled     true = escalated flytext on tier/crit hits.
	--                  Base hits (no tier, no crit) keep the normal vanilla
	--                  hit-location flytext; this only fires on escalations.
	--   fctScaleStepBp extra flytext SIZE per tier above base, in basis points.
	--                  1000 bp = +10.00% scale per tier. 1500 = Double 1.15x,
	--                  Triple 1.30x, Quad 1.50x. Relative multiplier, so it
	--                  respects the client's own text-size slider.
	--   fctCritBonusBp additional scale added when the hit is also CRITICAL.
	--                  2500 bp = +25.00%. Stacks with fctScaleStepBp.
	--   tier2Color     six-char RGB hex for the Double-tier flytext.
	--   tier3Color     six-char RGB hex for the Triple-tier flytext.
	--   tier4Color     six-char RGB hex for the Quad-tier flytext.
	--   critColor      six-char RGB hex used INSTEAD of the tier color when
	--                  the strike is a critical (crit overlays the tiers).
	--   chatTagEnabled true = send a second combat-spam line to the attacker
	--                  only, showing "x2"/"x3"/"x4" colored by tier
	--                  (yellow/yellow/red). The normal damage spam line is
	--                  unchanged.
	-- Defaults         : enabled, +15.00%/tier (+25.00% crit), white->yellow->
	--                  orange->red escalation with gold crits, chat tag on.
	-- REAL EXAMPLE (commented out): subtler escalation, no chat tag:
	--
	-- consolidatedStrike = {
	--     fctEnabled = true,
	--     fctScaleStepBp = 1000,  -- 1000 bp = +10.00% size per tier
	--     fctCritBonusBp = 2000,  -- 2000 bp = +20.00% size on crits
	--     tier2Color = "FFFF88",
	--     tier3Color = "FFAA44",
	--     tier4Color = "FF4444",
	--     critColor  = "FFEE00",
	--     chatTagEnabled = false,
	-- },
	-- ---------------------------------------------------------------------------
	consolidatedStrike = {
		fctEnabled = true,
		fctScaleStepBp = 1500,   -- 1500 bp = +15.00% flytext size per tier
		fctCritBonusBp = 2500,   -- 2500 bp = +25.00% extra size on crits

		tier2Color = "FFFF00",   -- yellow (Double)
		tier3Color = "FF9900",   -- orange (Triple)
		tier4Color = "FF0000",   -- red    (Quad)
		critColor  = "FFD700",   -- gold   (critical overlay)

		chatTagEnabled = true,
	},

	-- ---------------------------------------------------------------------------
	-- missions : mission terminal direction/difficulty options (BRIEF-043)
	-- What it does : lets players pick, from the mission terminal radial menu,
	--                a compass DIRECTION for destroy missions and a difficulty
	--                LEVEL RANGE tier. Choices persist per character.
	-- Why you care : replaces cyvster2's Lua menus with pure C++ SUIs; choices
	--                are read ONCE per mission-list refresh (no stutter).
	-- Fields       :
	--   missionOptionsEnabled  master switch for ALL mission options
	--                          (radial rows + overrides). false = fully vanilla.
	--   directionOptionEnabled show "Mission Direction" radial row + menu;
	--                          missions spawn within +/-5 degrees of the chosen
	--                          heading instead of a random 360-degree heading.
	--   difficultyOptionEnabled show "Mission Difficulty" radial row + menu;
	--                          chosen tier replaces the player/group level when
	--                          sizing destroy-mission difficulty.
	--   missionListSize        how many ACCEPTED missions fit in the datapad.
	--                          3 = vanilla default. Raise it (e.g. 6) to allow
	--                          more simultaneous missions.
	--   descriptiveTitles      true = destroy missions get readable titles like
	--                          "CL25 Destroy the giant sand beetle lair."
	--                          instead of the StringId table title.
	-- Defaults     : all enabled; list size 3 (vanilla); titles on.
	-- REAL EXAMPLE (commented out): bigger mission bag, no direction option:
	--
	-- missions = {
	--     missionOptionsEnabled = true,
	--     directionOptionEnabled = false,
	--     difficultyOptionEnabled = true,
	--     missionListSize = 6,
	--     descriptiveTitles = true,
	-- },
	-- ---------------------------------------------------------------------------
	missions = {
		missionOptionsEnabled = true,
		directionOptionEnabled = true,
		difficultyOptionEnabled = true,
		missionListSize = 3,           -- vanilla cap; owner may raise (e.g. 6)
		descriptiveTitles = true,
	},


	-- ---------------------------------------------------------------------------
	-- structures : structure placement / ownership (BRIEF-050)
	-- What it does : ACCOUNT-SHARED LOTS. The lot pool becomes
	--                (lots per character) x (characters on the account), shared
	--                across ALL characters on that account. Any character can
	--                place up to the whole pool; every character's structures
	--                count against it. Additionally, when any character places
	--                a structure, ALL other characters on the account are
	--                granted ADMIN permission on it automatically.
	-- Why you care : one character no longer blocks another from placing;
	--                families/mules share one pool like NGE-era SWG.
	-- Performance  : lot usage is cached per account and updated incrementally
	--                on place/destroy/transfer -- never rescanned per query,
	--                so this does NOT reintroduce the cyvster2 stutter.
	-- Fields       :
	--   accountSharedLots  true  = shared pool + auto-ADMIN for account chars
	--                      false = vanilla behavior (10 lots, owner only)
	-- Default       : true (owner approved).
	-- REAL EXAMPLE (commented out): restore vanilla lots:
	--
	-- structures = { accountSharedLots = false },
	-- ---------------------------------------------------------------------------
	structures = {
		accountSharedLots = true,
	},

	-- ---------------------------------------------------------------------------

	-- ---------------------------------------------------------------------------
	-- loot : loot-table QoL knobs (E02/E05)
	-- Fields       :
	--   creditsToTopDamager  true = NPC credit drops skip the corpse and are
	--                        paid DIRECTLY to the top-damage player (system
	--                        message confirms the amount). Event mobs are
	--                        always excluded. false = vanilla corpse credits.
	--   attachmentAutoName   true = newly looted Armor/Clothing Attachments
	--                        are renamed "[AA] <statname>: <value>" /
	--                        "<[CA]> <statname>: <value>" from their highest
	--                        skill mod, so you can triage attachments without
	--                        opening each one.
	-- Defaults     : both true (owner approved).
	-- REAL EXAMPLE (commented out): restore vanilla loot behavior:
	--
	-- loot = { creditsToTopDamager = false, attachmentAutoName = false },
	-- ---------------------------------------------------------------------------
	loot = {
		creditsToTopDamager = true,
		attachmentAutoName = true,
	},

	-- ---------------------------------------------------------------------------
	-- surveying : survey tool reach (C06)
	-- What it does : extends the survey tool's selectable range ladder above
	--                the vanilla 384m cap. Tiers keep the vanilla skill gates
	--                (surveying 20/35/55/75/100/120) and scale from 64m up to
	--                maxRange in 64m-granular steps; sample grid grows to
	--                6x6 points above 1024m.
	-- Fields       :
	--   maxRange  maximum selectable range in METERS (whole units).
	--             Vanilla value was 384. Setting it back to 384 restores the
	--             exact vanilla tiers (64/128/192/256/320/384 x3-5pts).
	-- Default      : 2624 (owner approved). Accepted range: 64 - 8192.
	-- REAL EXAMPLE (commented out): vanilla survey ranges:
	--
	-- surveying = { maxRange = 384 },
	-- ---------------------------------------------------------------------------
	surveying = {
		maxRange = 2624,           -- meters; vanilla was 384
	},

	-- ---------------------------------------------------------------------------

	-- ---------------------------------------------------------------------------
	-- criticalChance : chance for a landed attack to become a CRITICAL hit
	-- What it does   : each acquired badge in the list below adds badgeBonus to
	--                  the character's crit chance (in basis points). When an
	--                  attack lands, the engine rolls against this chance; on
	--                  success the hit becomes a critical and damage is boosted
	--                  by the criticalChance.multiplier below (and further by
	--                  the criticalMultiplier modifier in the modifiers table).
	-- Fields         :
	--   enabled    true = feature active, false = fully disabled (no rolls)
	--   cap        maximum total crit chance in bp; total is clamped here.
	--              6000 = 60.00% ceiling. Set 0 for no cap.
	--   badgeBonus bp granted PER badge in the badges list (uniform for all).
	--              100 = +1.00% per badge.
	--   multiplier crit DAMAGE boost in bp, added on top of the native base:
	--              finalCritDamage = preArmorDamage x (baseMultiplier + bonus).
	--              15000 = +150.00% (i.e., 2.5x total with the native 150%).
	--   badges     which badges grant the bonus. Full default set of 60 x 100 bp
	--              reaches the 6000 cap exactly.
	-- ---------------------------------------------------------------------------
	criticalChance = {
		enabled = true,
		cap = 6000,

		-- Per-badge crit chance. 100 bp = +1.00% per badge.
		badgeBonus = 100,

		-- Crit damage multiplier. 15000 bp = +150.00% damage on crits.
		multiplier = 15000,

		-- 60 achievement badges at +1.00% (100 bp) each = 60.00% total,
		-- exactly reaching the 6000 cap.
		badges = {
			-- Accumulation milestones (7): total badges earned
			"count_5", "count_10", "count_25", "count_50", "count_75", "count_100", "count_125",
			-- Exploration milestones (5): total exploration badges found
			"bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges", "bdg_exp_40_badges", "bdg_exp_45_badges",
			-- Combat profession masteries (12)
			"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
			"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
			"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
			"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
			-- Hero of Tatooine quest line (5)
			"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			-- Warren quest line (2)
			"warren_compassion", "warren_hero",
			-- Theme park completions (4): Jabba, Imperial, Rebel, Nym
			"bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
			"bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge",
			-- Corellian Corvette missions (9): Imp/Neutral/Reb x destroy/rescue/assassin
			"bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin",
			"bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin",
			"bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin",
			-- Dangerous-site exploration (5)
			"exp_tat_tusken_pool", "exp_tat_krayt_skeleton", "exp_tat_sarlacc_pit", "exp_tat_krayt_graveyard", "exp_dat_sarlacc",
			-- Jedi-site exploration (3)
			"exp_tat_bens_hut", "exp_yav_temple_exar_kun", "exp_dan_jedi_temple",
			-- Outdoors profession masteries (5)
			"outdoors_bio_engineer_master", "outdoors_creaturehandler_master",
			"outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master",
			-- Science profession masteries (3)
			"science_combatmedic_master", "science_doctor_master", "science_medic_master",
		},

		-- -------------------------------------------------------------------------
		-- OPTIONAL per-badge overrides (badgeOverrides)
		-- -------------------------------------------------------------------------
		-- By default every badge in the list above grants the same badgeBonus.
		-- If you want INDIVIDUAL badges to be worth more or less, uncomment the
		-- badgeOverrides block below and edit it.
		--
		-- RULES:
		--   * Each entry is { "badge_key", value } where value REPLACES
		--     badgeBonus for that one badge (it does NOT stack on top of it).
		--   * A badge listed in badges but absent from overrides simply uses
		--     badgeBonus.
		--   * Keys must exist in the badges list above to have any effect.
		--   * Works identically inside every modifiers.<name> block below.
		--
		-- REAL EXAMPLE (commented out -- safe to enable):
		-- Makes Bounty Hunter mastery worth triple (+3.00%) while every other
		-- combat mastery stays at the uniform +1.00%:
		--
		-- badgeOverrides = {
		--     { "combat_bountyhunter_master", 300 },
		-- },
		--
		-- MULTI-BADGE EXAMPLE (commented out):
		-- Hero of Tatooine badges worth double (+2.00% each), everything else
		-- stays at +1.00%:
		--
		-- badgeOverrides = {
		--     { "poi_rabidbeast", 200 },
		--     { "poi_prisonbreak", 200 },
		--     { "poi_twoliars", 200 },
		--     { "poi_factoryliberation", 200 },
		--     { "poi_heromark", 200 },
		-- },
		-- -------------------------------------------------------------------------
	},

	-- ===========================================================================
	-- GENERIC MODIFIERS
	-- ===========================================================================
	-- Every block below follows the same schema:
	--
	--   enabled    true/false -- master switch for this modifier
	--   badgeBonus basis points granted PER badge (or whole units for the three
	--              noted exceptions: armorPenetration/defenseCapIncrease/
	--              seaCapIncrease). REMEMBER: 1000 bp = 10.00%.
	--   cap        maximum summed total, in the same units. Total is clamped
	--              after all owned badges are summed. 0 = UNCAPPED.
	--   badges     badge keys that grant the bonus.
	--   badgeOverrides  optional per-badge replacements (see the worked examples
	--              in the criticalChance block above -- same syntax everywhere).
	--
	-- Disabled modifiers still appear in /customskills menus but are marked
	-- inactive and have zero gameplay effect.
	-- ===========================================================================
	modifiers = {

		-- -----------------------------------------------------------------------
		-- doubleAttackChance : chance an attack strikes TWICE
		-- What it does       : rolled first in the repeat chain (Double -> Triple
		--                      -> Quad). On success the hit resolves twice and a
		--                      Triple roll follows. Only ONE tier fires per hit.
		-- Current math       : 6 badges x 2000 bp (20.00%) = 12000, clamped to
		--                      the 10000 (100%) cap.
		-- -----------------------------------------------------------------------
		doubleAttackChance = {
			enabled = true,
			badgeBonus = 2000,   -- 2000 bp = +20.00% per badge
			cap = 10000,         -- 10000 = 100.00% (always double at max)

			badges = {
				-- Warren quest line (2)
				"warren_compassion", "warren_hero",
				-- Theme park completions (4)
				"bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
				"bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge",
			},
		},

		-- -----------------------------------------------------------------------
		-- criticalMultiplier : extra DAMAGE dealt BY critical hits
		-- What it does       : adds to the crit-damage multiplier from the
		--                      criticalChance block. Stacks additively with the
		--                      base: total crit damage = preArmorDamage x
		--                      ((15000 base + this total) / 10000).
		-- Current math       : 12 badges x 1250 bp (12.50%) = 15000 = cap exact.
		--                      Max total crit = 300% of normal damage.
		-- -----------------------------------------------------------------------
		criticalMultiplier = {
			enabled = true,
			badgeBonus = 1250,   -- 1250 bp = +12.50% crit damage per badge
			cap = 15000,         -- +150.00% bonus on top of the 150% base

			badges = {
				-- Accumulation milestones (7)
				"count_5", "count_10", "count_25", "count_50", "count_75", "count_100", "count_125",
				-- Exploration milestones (5)
				"bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges", "bdg_exp_40_badges", "bdg_exp_45_badges",
			},
		},

		-- -----------------------------------------------------------------------
		-- tripleAttackChance : chance an attack strikes THREE times
		-- What it does       : only rolled after Double succeeds in the same
		--                      chain. Success upgrades the hit to 3 strikes and
		--                      enables the Quad roll.
		-- Current math       : 17 badges x 500 bp (5.00%) = 8500 = cap exact.
		-- -----------------------------------------------------------------------
		tripleAttackChance = {
			enabled = true,
			badgeBonus = 500,    -- 500 bp = +5.00% per badge
			cap = 8500,          -- 8500 = 85.00%

			badges = {
				-- Combat profession masteries (12)
				"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
				"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
				"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
				"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
				-- Hero of Tatooine (5)
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
		},

		-- -----------------------------------------------------------------------
		-- quadAttackChance : chance an attack strikes FOUR times (highest tier)
		-- What it does     : only rolled after Triple succeeds. Final stage of
		--                    the chain; one tier max per landed hit.
		-- Current math     : 17 badges x 300 bp (3.00%) = 5100 = cap exact.
		-- -----------------------------------------------------------------------
		quadAttackChance = {
			enabled = true,
			badgeBonus = 300,    -- 300 bp = +3.00% per badge
			cap = 5100,          -- 5100 = 51.00%

			badges = {
				-- Combat profession masteries (12) - same set as Triple
				"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
				"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
				"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
				"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master",
				-- Hero of Tatooine (5)
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
		},

		-- -----------------------------------------------------------------------
		-- armorPenetration : lowers the TARGET's effective armor level on hit
		-- What it does     : each point drops defender armor one level
		--                    (Heavy -> Medium -> Light -> None). Floor is None;
		--                    never affects YOUR weapon's Armor Piercing rating.
		-- UNITS            : WHOLE LEVELS, not basis points.
		-- Faction note     : Corvette badges are faction-paired -- one character
		--                    can hold only one faction side plus neutral, so the
		--                    realistic max is lower than the listed total.
		-- Current math     : 9 badges x 1 = 9 levels, clamped to cap 3.
		-- -----------------------------------------------------------------------
		armorPenetration = {
			enabled = true,
			badgeBonus = 1,      -- 1 armor level per badge (whole units!)
			cap = 3,             -- maximum 3 levels dropped

			badges = {
				-- Corellian Corvette missions (9): Imperial / Neutral / Rebel
				"bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin",
				"bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin",
				"bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin",
			},
		},

		-- -----------------------------------------------------------------------
		-- defenseCapIncrease : raises the hard cap on primary/secondary DEFENSE
		-- What it does       : the engine caps defense at 125; this raises that
		--                      ceiling by the accumulated amount. Does NOT affect
		--                      attachment/tape (SEA) bonuses -- those are handled
		--                      by seaCapIncrease below.
		-- UNITS              : WHOLE POINTS, not basis points.
		-- Current math       : 5 badges x 20 pts = 100 pts = cap exact.
		-- -----------------------------------------------------------------------
		defenseCapIncrease = {
			enabled = true,
			badgeBonus = 20,     -- 20 defense points per badge (whole units!)
			cap = 100,           -- maximum +100 defense points

			badges = {
				-- Hero of Tatooine quest line (5)
				"poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark",
			},
		},

		-- -----------------------------------------------------------------------
		-- armorDegradeReduction : chance worn armor AVOIDS losing condition
		-- What it does          : native armor degradation is a 100% event chance
		--                         on qualifying hits; this subtracts from it.
		--                         At 10000 bp (100%) armor never degrades from
		--                         combat. Covers player-worn armor, PSGs, NPC
		--                         armor, and vehicle armor.
		-- Current math          : 8 badges x 1250 bp (12.50%) = 10000 = cap exact.
		-- -----------------------------------------------------------------------
		armorDegradeReduction = {
			enabled = true,
			badgeBonus = 1250,   -- 1250 bp = 12.50% degradation avoidance per badge
			cap = 10000,         -- 10000 = 100% (armor never degrades)

			badges = {
				-- Talus exploration (4)
				"bdg_exp_tal_creature_village", "bdg_exp_tal_imp_base",
				"bdg_exp_tal_imp_vs_reb_battle", "bdg_exp_tal_aqualish_cave",
				-- Rori exploration (4)
				"bdg_exp_ror_kobala_spice_mine", "bdg_exp_ror_rebel_outpost",
				"bdg_exp_ror_imp_camp", "bdg_exp_ror_imp_hyperdrive_fac",
			},
		},

		-- -----------------------------------------------------------------------
		-- weaponDegradeReduction : reduces the WEAPON degradation event chance
		-- What it does           : multiplies the native degrade chance down:
		--                          effective = nativeChance x (10000 - total)/10000.
		--                          Amount of condition lost per event unchanged.
		-- Current math           : 8 badges x 1250 bp = 100% reduction = cap exact
		--                          (weapons never degrade from use at max).
		-- -----------------------------------------------------------------------
		weaponDegradeReduction = {
			enabled = true,
			badgeBonus = 1250,   -- 1250 bp = 12.50% reduction per badge
			cap = 10000,         -- 100% reduction maximum

			badges = {
				-- Dantooine exploration (4)
				"exp_dan_jedi_temple", "exp_dan_rebel_base",
				"bdg_exp_dan_dantari_village1", "bdg_exp_dan_dantari_village2",
				-- Endor exploration (4)
				"bdg_exp_end_ewok_tree_village", "bdg_exp_end_ewok_lake_village",
				"bdg_exp_end_dulok_village", "bdg_exp_end_imp_outpost",
			},
		},

		-- -----------------------------------------------------------------------
		-- seaCapIncrease : raises the cap on Skill Enhancement Attachment (SEA/tape)
		-- What it does   : native engine caps wearable attachment bonuses at +25
		--                  per stat; this raises that ceiling by the accumulated
		--                  amount so tapes can exceed +25. Independent of
		--                  defenseCapIncrease above.
		-- UNITS          : WHOLE POINTS, not basis points.
		-- Current math   : 5 badges x 20 pts = 100 pts = cap exact.
		-- -----------------------------------------------------------------------
		seaCapIncrease = {
			enabled = true,
			badgeBonus = 20,     -- 20 SEA points per badge (whole units!)
			cap = 100,           -- attachments may reach +100 total

			badges = {
				-- Dangerous-site exploration: Tatooine (4) + Dathomir (1)
				"exp_tat_tusken_pool", "exp_tat_krayt_skeleton",
				"exp_tat_sarlacc_pit", "exp_tat_krayt_graveyard",
				"exp_dat_sarlacc",
			},
		},

		-- -----------------------------------------------------------------------
		-- movementSpeed : multiplies final run speed in ALL travel modes
		-- What it does  : applies once to foot, mount, and vehicle speed alike
		--                 (never stacked per-hop), synced between client display
		--                 and server validation.
		-- Current math  : 4 badges x 2500 bp (25.00%) = +100% speed (2x total).
		-- -----------------------------------------------------------------------
		movementSpeed = {
			enabled = true,
			badgeBonus = 2500,   -- 2500 bp = +25.00% speed per badge
			cap = 0,             -- uncapped

			badges = {
				-- Naboo exploration (4)
				"exp_nab_gungan_sacred_place", "bdg_exp_nab_theed_falls_bottom",
				"bdg_exp_nab_deeja_falls_top", "bdg_exp_nab_amidalas_sandy_beach",
			},
		},

		-- -----------------------------------------------------------------------
		-- buffDuration : extends duration of BENEFICIAL buffs
		-- What it does : multiplies initial duration of eligible buffs (medical/
		--                entertainer/food-drink/spice-up/positive Force). Explicit
		--                renewals also apply the bonus; DB reloads do not re-
		--                multiply. Debuffs/damage-over-time/innates/Squad Leader
		--                buffs are NEVER extended.
		-- Current math : 13 badges x 2500 bp (25.00%) = +325% buff duration.
		-- -----------------------------------------------------------------------
		buffDuration = {
			enabled = true,
			badgeBonus = 2500,   -- 2500 bp = +25.00% duration per badge
			cap = 0,             -- uncapped

			badges = {
				-- Tatooine easy exploration (3)
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				-- Yavin IV exploration (2)
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				-- Science profession masteries (3)
				"science_combatmedic_master", "science_doctor_master", "science_medic_master",
				-- Social profession masteries (5)
				"social_dancer_master", "social_entertainer_master", "social_imagedesigner_master",
				"social_musician_master", "social_politician_master",
			},
		},

		-- -----------------------------------------------------------------------
		-- experienceMultiplier : multiplies ALL positive XP earned
		-- What it does         : applies to combat, crafting, harvest, quest XP --
		--                        anything positive on the normal award path.
		--                        Stacks MULTIPLICATIVELY with server rates and
		--                        buffs (e.g., 100 base x 2x server x 5x badges =
		--                        1000 XP total, not 700). Does not affect XP
		--                        deductions or conversion costs.
		-- Current math         : 5 badges x 10000 bp (100%) = +500% (6x total XP).
		-- -----------------------------------------------------------------------
		experienceMultiplier = {
			enabled = true,
			badgeBonus = 10000,  -- 10000 bp = +100.00% XP per badge
			cap = 0,             -- uncapped

			badges = {
				-- Corellia exploration (5)
				"exp_cor_agrilat_swamp", "bdg_exp_cor_rebel_hideout",
				"bdg_exp_cor_rogue_corsec_base", "bdg_exp_cor_tyrena_theater",
				"bdg_exp_cor_bela_vistal_fountain",
			},
		},

		-- -----------------------------------------------------------------------
		-- practiceExperienceBonus : boosts XP from PRACTICE crafting sessions
		-- What it does            : applies when crafting in practice mode (no
		--                           item produced). Order of operations: native
		--                           5% practice bonus FIRST, then this bonus,
		--                           then the general experienceMultiplier.
		-- Current math            : 10 badges x 10000 bp = +1000% practice XP.
		-- -----------------------------------------------------------------------
		practiceExperienceBonus = {
			enabled = true,
			badgeBonus = 10000,  -- 10000 bp = +100.00% practice XP per badge
			cap = 0,             -- uncapped

			badges = {
				-- Tatooine easy exploration (3)
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				-- Yavin IV exploration (2)
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				-- Social profession masteries (5)
				"social_dancer_master", "social_entertainer_master", "social_imagedesigner_master",
				"social_musician_master", "social_politician_master",
			},
		},

		-- -----------------------------------------------------------------------
		-- craftingSpeed : shortens crafting session AND factory run times
		-- What it does  : personal crafting duration = nativeDuration /
		--                 multiplier (minimum 1 second). Factory runs snapshot
		--                 the STARTING player's bonus; the run persists through
		--                 logout/restart and ignores mid-run badge changes --
		--                 stop/restart the factory to pick up new values.
		-- Current math  : 10 badges x 1000 bp (10.00%) = +100% (2x faster).
		-- -----------------------------------------------------------------------
		craftingSpeed = {
			enabled = true,
			badgeBonus = 1000,   -- 1000 bp = +10.00% crafting speed per badge
			cap = 0,             -- uncapped

			badges = {
				-- Crafting profession masteries (8 ground schools + Shipwright)
				"crafting_architect_master", "crafting_armorsmith_master", "crafting_artisan_master",
				"crafting_chef_master", "crafting_droidengineer_master", "crafting_merchant_master",
				"crafting_shipwright", "crafting_tailor_master", "crafting_weaponsmith_master",
				-- Doctor mastery rounds out the set (10 total)
				"science_doctor_master",
			},
		},

		-- -----------------------------------------------------------------------
		-- amazingSuccessChance : bonus to the crafting AMAZING SUCCESS roll
		-- What it does         : adds percentage points to the natural Amazing
		--                        Success probability during assembly and
		--                        experimentation. Adds directly (does not
		--                        multiply the native roll), clamped to cap.
		-- Current math         : 10 badges x 500 bp (5.00%) = +50% final chance
		--                        (intentionally under the 10000 cap to leave
		--                        tuning headroom).
		-- -----------------------------------------------------------------------
		amazingSuccessChance = {
			enabled = true,
			badgeBonus = 500,    -- 500 bp = +5.00% chance per badge
			cap = 10000,         -- absolute ceiling 100%

			badges = {
				-- Yavin IV (1): Exar Kun's temple
				"exp_yav_temple_exar_kun",
				-- Lok exploration (3)
				"exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton",
				-- Dathomir exploration (6)
				"exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2",
				"bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison",
			},
		},

		-- -----------------------------------------------------------------------
		-- amazingResults : quality boost WHEN an Amazing Success actually fires
		-- What it does   : on an Amazing Success, each affected attribute lands
		--                  closer to the schematic cap:
		--                  enhanced = nativeResult + ((cap - nativeResult) x
		--                             strength / 10000).
		--                  Assembly affects ALL attributes; experimentation
		--                  affects only the selected line. Without an Amazing
		--                  Success, results stay resource-capped as vanilla.
		-- Current math   : 10 badges x 1000 bp (10.00%) = 100% strength = cap
		--                  exact (poor resources can yield perfect attributes).
		-- -----------------------------------------------------------------------
		amazingResults = {
			enabled = true,
			badgeBonus = 1000,   -- 1000 bp = +10.00% strength per badge
			cap = 10000,         -- 100% strength maximum

			badges = {
				-- Same 10-badge exploration set as amazingSuccessChance
				"exp_yav_temple_exar_kun",
				"exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton",
				"exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2",
				"bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison",
			},
		},

		-- -----------------------------------------------------------------------
		-- gatheringQuantity : multiplies resources gained from foraging & milking
		-- What it does      : badge bonuses ADD together, then multiply the
		--                     native yield: total = native x (1 + sum/10000).
		--                     Rounded down, never below the native amount.
		--                     Discrete loot (food, bait, medical components,
		--                     rare items, live creatures) is NEVER duplicated.
		-- Current math      : 13 badges x 10000 bp (100%) = +1300% (~14x yields).
		-- -----------------------------------------------------------------------
		gatheringQuantity = {
			enabled = true,
			badgeBonus = 10000,  -- 10000 bp = +100% yield per badge
			cap = 0,             -- uncapped

			badges = {
				-- Tatooine easy exploration (3)
				"exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut",
				-- Yavin IV exploration (2)
				"exp_yav_temple_woolamander", "exp_yav_temple_blueleaf",
				-- Outdoors profession masteries (5)
				"outdoors_bio_engineer_master", "outdoors_creaturehandler_master",
				"outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master",
				-- Science profession masteries (3)
				"science_combatmedic_master", "science_doctor_master", "science_medic_master",
			},
		},
	},
}
