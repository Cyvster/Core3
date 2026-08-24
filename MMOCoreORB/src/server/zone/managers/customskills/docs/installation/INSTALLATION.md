# Custom Skills Module -- Installation Guide

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Overview

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation
- ox-alpha (opencode/x-preview-f-free), 08232026 -- BRIEF-002: rewritten
  for single-tree layout (install onto other servers via module dir copy +
  patch)

Custom Skills is a SWGEmu Core3 module that adds badge-derived character bonuses across combat, crafting, gathering, movement, buffs, and experience systems. It provides a `/customskills` in-game menu showing character-specific bonus totals and their badge sources.

**Key principle**: The module's C++ defaults disable all non-critical modifiers with empty badge lists, but the shipped `config.lua` enables all 18 modifiers with full badge assignments. Editing `config.lua` changes behavior -- native Core3 behavior is preserved only if you disable a modifier or empty its badge list.

---

## How It Works (Quick Summary)

| Component | Purpose |
|-----------|---------|
| **C++ Module** (`customskills/`) | Core logic: modifier calculation, badge aggregation, gameplay hooks |
| **Lua Config** (`scripts/customskills/config.lua`) | Server owner controls: enable/disable modifiers, per-badge values, caps |
| **Core3 Hooks** (19 files patched) | Minimal delegations at lifecycle boundaries (combat, crafting, XP, movement, etc.) |
| **SUI Menu** (`/customskills`) | Displays acquired bonuses and badge-source breakdowns using same math as gameplay |

**Data flow**: Badge bitmasks (on `PlayerObject`) -- `CustomSkillsModifiers` (central authority) -- Gameplay hooks AND Menu display. Single source of truth.

---

## Prerequisites

- SWGEmu Core3 source checkout (matching the patch base revision)
- CMake 3.10+, C++17 compiler
- Git (for applying patches)

---

## Installation Steps

The module lives inside this Core3 repository. These steps install it onto a
DIFFERENT Core3 checkout (a vanilla server). For development here, no
installation is needed -- build and run.

### 1. Copy Module Files

```bash
# SRC = this repo's module dir
# MMOCoreORB/src/server/zone/managers/customskills/
# DST = /path/to/Core3/MMOCoreORB/src/server/zone/managers/customskills/
cp -r "$SRC" "$DST"
```

Then copy the two script files (sanctioned exceptions outside the module
dir):
- `MMOCoreORB/bin/scripts/commands/customSkills.lua`
- `MMOCoreORB/bin/scripts/customskills/config.lua`

### 2. Apply Core3 Integration Patch

```bash
cd /path/to/Core3
git apply --check --ignore-space-change <module>/integration/core3-hooks.patch
git apply --ignore-space-change <module>/integration/core3-hooks.patch
```

**If `--check` fails**: The target Core3 revision differs. Port hunks manually using `MANIFEST.md` (lists all integration files) and `docs/customskills/CODE_REFERENCE.md` (describes each hook's purpose). Do not force the patch.

### 3. Reconfigure & Build

```bash
# From Core3 root
cmake -B build  # or your standard configure command
cmake --build build --target core3
```

The recursive source glob will pick up the new `.cpp` files automatically.

### 4. Deploy & Restart

- Deploy the built binary (`core3`)
- Deploy updated scripts from `MMOCoreORB/bin/scripts/`
- Restart the server

---

## Configuration

Edit **before** starting Core3:

```
MMOCoreORB/bin/scripts/customskills/config.lua
```

### Critical Chance (enabled by default)

```lua
criticalChance = {
    enabled = true,
    cap = 6000,             -- 60.00% maximum
    badgeBonus = 400,        -- default per-badge rate (basis points)
    multiplier = 15000,      -- 150.00% critical damage
    combatSpamLabel = "(CRIT)",
    badges = {
        "count_5", "count_10", "count_25", "count_50", "count_75",
        "count_100", "count_125",
        "bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges",
        "bdg_exp_40_badges", "bdg_exp_45_badges",
    },
    badgeOverrides = {
        { "count_5", 100 }, { "count_10", 100 }, { "count_25", 100 },
        { "count_50", 100 }, { "count_75", 100 }, { "count_100", 100 },
        { "count_125", 100 },
        { "bdg_exp_10_badges", 100 }, { "bdg_exp_20_badges", 100 },
        { "bdg_exp_30_badges", 100 }, { "bdg_exp_40_badges", 100 },
        { "bdg_exp_45_badges", 100 },
    },
}
```

### Generic Modifiers

Each modifier table accepts:
- `enabled` -- `true`/`false`
- `badgeBonus` -- basis points per badge (or whole levels for armor penetration, defense cap, SEA cap)
- `cap` -- maximum total (0 = uncapped)
- `badges` -- list of badge keys (from `BadgeList`)
- `badgeOverrides` -- optional `{ "badgeKey", value }` pairs overriding `badgeBonus` per badge
- `combatSpamLabel` -- optional, for repeat-damage tiers only

```lua
modifiers = {
    doubleAttackChance  = { enabled = true, badgeBonus = 2000, cap = 10000, combatSpamLabel = "(DOUBLE)", badges = { "warren_compassion", "warren_hero", "bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge", "bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge" } },
    tripleAttackChance  = { enabled = true, badgeBonus = 200, cap = 7500, combatSpamLabel = "(TRIPLE)", badges = { "combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master", "combat_brawler_master", "combat_carbine_master", "combat_commando_master", "combat_marksman_master", "combat_pistol_master", "combat_polearm_master", "combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master", "poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark" }, badgeOverrides = { { "combat_bountyhunter_master", 300 } } },
    quadAttackChance    = { enabled = true, badgeBonus = 200, cap = 5000, combatSpamLabel = "(QUAD)", badges = { "combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master", "combat_brawler_master", "combat_carbine_master", "combat_commando_master", "combat_marksman_master", "combat_pistol_master", "combat_polearm_master", "combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master", "poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark" }, badgeOverrides = { { "combat_bountyhunter_master", 300 } } },
    armorPenetration    = { enabled = true, badgeBonus = 1, cap = 3, badges = { "bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin", "bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin", "bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin" } },
    defenseCapIncrease  = { enabled = true, badgeBonus = 5, cap = 0, badges = { "poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark", "exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton", "exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2", "bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison", "exp_yav_temple_exar_kun" }, badgeOverrides = { { "poi_rabidbeast", 10 }, { "poi_prisonbreak", 10 }, { "poi_twoliars", 10 }, { "poi_factoryliberation", 10 }, { "poi_heromark", 10 } } },
    armorDegradeReduction = { enabled = true, badgeBonus = 1250, cap = 10000, badges = { "bdg_exp_tal_creature_village", "bdg_exp_tal_imp_base", "bdg_exp_tal_imp_vs_reb_battle", "bdg_exp_tal_aqualish_cave", "bdg_exp_ror_kobala_spice_mine", "bdg_exp_ror_rebel_outpost", "bdg_exp_ror_imp_camp", "bdg_exp_ror_imp_hyperdrive_fac" } },
    weaponDegradeReduction = { enabled = true, badgeBonus = 1250, cap = 10000, badges = { "exp_dan_jedi_temple", "exp_dan_rebel_base", "bdg_exp_dan_dantari_village1", "bdg_exp_dan_dantari_village2", "bdg_exp_end_ewok_tree_village", "bdg_exp_end_ewok_lake_village", "bdg_exp_end_dulok_village", "bdg_exp_end_imp_outpost" } },
    seaCapIncrease      = { enabled = true, badgeBonus = 15, cap = 0, badges = { "exp_tat_tusken_pool", "exp_tat_krayt_skeleton", "exp_tat_sarlacc_pit", "exp_tat_krayt_graveyard", "exp_dat_sarlacc" } },
    movementSpeed       = { enabled = true, badgeBonus = 2500, cap = 0, badges = { "exp_nab_gungan_sacred_place", "bdg_exp_nab_theed_falls_bottom", "bdg_exp_nab_deeja_falls_top", "bdg_exp_nab_amidalas_sandy_beach" } },
    buffDuration        = { enabled = true, badgeBonus = 2500, cap = 0, badges = { "exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut", "exp_yav_temple_woolamander", "exp_yav_temple_blueleaf", "science_combatmedic_master", "science_doctor_master", "science_medic_master", "social_dancer_master", "social_entertainer_master", "social_imagedesigner_master", "social_musician_master", "social_politician_master" }, badgeOverrides = { { "social_dancer_master", 2000 }, { "social_entertainer_master", 2000 }, { "social_imagedesigner_master", 2000 }, { "social_musician_master", 2000 }, { "social_politician_master", 2000 } } },
    experienceMultiplier = { enabled = true, badgeBonus = 10000, cap = 0, badges = { "exp_cor_agrilat_swamp", "bdg_exp_cor_rebel_hideout", "bdg_exp_cor_rogue_corsec_base", "bdg_exp_cor_tyrena_theater", "bdg_exp_cor_bela_vistal_fountain" } },
    practiceExperienceBonus = { enabled = true, badgeBonus = 10000, cap = 0, badges = { "exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut", "exp_yav_temple_woolamander", "exp_yav_temple_blueleaf", "social_dancer_master", "social_entertainer_master", "social_imagedesigner_master", "social_musician_master", "social_politician_master" } },
    craftingSpeed       = { enabled = true, badgeBonus = 1000, cap = 0, badges = { "crafting_architect_master", "crafting_armorsmith_master", "crafting_artisan_master", "crafting_chef_master", "crafting_droidengineer_master", "crafting_merchant_master", "crafting_shipwright", "crafting_tailor_master", "crafting_weaponsmith_master", "science_doctor_master" } },
    amazingSuccessChance = { enabled = true, badgeBonus = 500, cap = 10000, badges = { "exp_yav_temple_exar_kun", "exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton", "exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2", "bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison" } },
    amazingResults      = { enabled = true, badgeBonus = 1000, cap = 10000, badges = { "exp_yav_temple_exar_kun", "exp_lok_volcano", "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton", "exp_dat_tarpit", "exp_dat_escape_pod", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2", "bdg_exp_dat_crashed_ship", "bdg_exp_dat_imp_prison" } },
    gatheringQuantity   = { enabled = true, badgeBonus = 2500, cap = 0, badges = { "exp_tat_escape_pod", "exp_tat_lars_homestead", "exp_tat_bens_hut", "exp_yav_temple_woolamander", "exp_yav_temple_blueleaf", "outdoors_bio_engineer_master", "outdoors_creaturehandler_master", "outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master", "science_combatmedic_master", "science_doctor_master", "science_medic_master" }, badgeOverrides = { { "outdoors_bio_engineer_master", 11500 }, { "outdoors_creaturehandler_master", 11500 }, { "outdoors_ranger_master", 11500 }, { "outdoors_scout_master", 11500 }, { "outdoors_squadleader_master", 11500 }, { "science_combatmedic_master", 10000 }, { "science_doctor_master", 10000 }, { "science_medic_master", 10000 } } },
}
```

### Per-Badge Overrides (`badgeOverrides`)

A `badgeOverrides` table of `{ "badgeKey", value }` pairs overrides the
modifier's `badgeBonus` for the named badges only. Badges listed under
`badges` but not overridden use the modifier's `badgeBonus`. Loaded by
`CustomSkillsConfig::loadBadgeOverrides`, applied after the `badges` list
populates the modifier's badge map.

### Rarity Naming (`rarityNaming`)

```lua
rarityNaming = {
    enabled = true,
    legendaryColor = "FF00FF",   -- six-character RGB hex (purple)
    exceptionalColor = "0000FF", -- six-character RGB hex (blue)
}
```

When enabled, loot items use color-only display names instead of appending
`(Exceptional)`/`(Legendary)` suffixes (hook H16). This also fixes quest
turn-in compatibility, since quest systems match items by name and suffixes
break the match.

### Enabling a Modifier (worked example)

```lua
modifiers = {
    doubleAttackChance  = { enabled = true, badgeBonus = 2000, cap = 10000,
                            combatSpamLabel = "(DOUBLE)", badges = {
                                "warren_compassion", "warren_hero",
                                "bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
                                "bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge"
                            }},
}
```

Badge keys must exist in `BadgeList` (check `badge_map.iff`); see
[CODE_REFERENCE.md Appendix B](../customskills/CODE_REFERENCE.md) for the full badge catalog.
Restart Core3 after editing.

### Root Settings

```lua
customSummaryColor = "00FF00"   -- RGB hex for menu modifier text color
```

**Important**: Configuration is cached for combat performance. **Restart Core3 after any change**. Missing/invalid values use safe defaults and log warnings.

---

## Badge Assignment Rules

| Rule | Description |
|------|-------------|
| Use BadgeList keys | Never hard-code indices (fragile across TRE changes) |
| Exclude pilot/JTL badges | No bonuses until JTL is available on your server |
| Exclude admin/event badges | See `EXCLUDED_BADGES.md` for full list |
| Visible -- grantable | `show = 0` badges can still grant modifiers if explicitly listed in config |

---

## Verification Checklist

1. **Startup**: No errors loading `customSkills.lua` or `CustomSkillsCommand`
2. **Command**: `/customskills` opens menu on a normal player character
3. **Navigation**: Open every category, use Back/Cancel, reopen repeatedly, test 2+ characters simultaneously
4. **Critical Chance**: Each milestone exploration badge (via `badgeOverrides`) shows +1.00%, max +12.00% for all 12 (combat masteries no longer grant Critical Chance)
5. **Combat test**: Critical attacks deal 150% of pre-armor damage
6. **Before enabling new modifier**: Run focused tests from `docs/customskills/CODE_REFERENCE.md`

---

## Removal

```bash
# 1. Remove module files (includes docs/ -- back up if wanted)
rm -rf MMOCoreORB/src/server/zone/managers/customskills/

# 2. Remove scripts
rm MMOCoreORB/bin/scripts/commands/customSkills.lua
rm -rf MMOCoreORB/bin/scripts/customskills/

# 3. Reverse patch
cd /path/to/Core3
git apply --check --reverse --ignore-space-change <module-backup>/integration/core3-hooks.patch
git apply --reverse --ignore-space-change <module-backup>/integration/core3-hooks.patch

# 4. Rebuild & restart
```

If reverse check fails (Core3 files edited since), remove Custom Skills hunks manually using `MANIFEST.md` -- do not force.

---

## Troubleshooting

| Symptom | Likely Cause | Fix |
|---------|--------------|-----|
| Patch `--check` fails | Core3 revision mismatch | Port hunks manually per `MANIFEST.md` |
| Command not found | `commands.lua` not updated | Verify patch applied; check `customSkills.lua` deployed |
| Menu shows no bonuses | Badges not in `config.lua` | Add badge keys to modifier's `badges` list |
| Changes not reflected | Config cached | Restart Core3 after editing `config.lua` |
| Build errors | Missing `.cpp` in glob | Re-run CMake configure step |

---

## Related Documentation

- **User Guide**: `../customskills/USER_GUIDE.md` -- Player-facing mechanics
- **Architecture & Implementation**: `../customskills/CODE_REFERENCE.md` -- Hooks, services, menu, verification matrix
- **Modifier Specification**: `../customskills/CODE_REFERENCE.md` -> Appendix A -- all 18 modifiers with config
- **Badge Catalog**: `../customskills/CODE_REFERENCE.md` -> Appendix B
- **Core3 platform (emulator guides)**: `../swgemu/CODE_REFERENCE.md` + `../swgemu/USER_GUIDE.md`

