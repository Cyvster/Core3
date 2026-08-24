# Custom Skills Module — Installation Guide

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Overview

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

Custom Skills is a SWGEmu Core3 module that adds badge-derived character bonuses across combat, crafting, gathering, movement, buffs, and experience systems. It provides a `/customskills` in-game menu showing character-specific bonus totals and their badge sources.

**Key principle**: All non-critical modifiers ship **disabled** with empty badge lists. Installing preserves native Core3 behavior until you configure bonuses.

---

## How It Works (Quick Summary)

| Component | Purpose |
|-----------|---------|
| **C++ Module** (`customskills/`) | Core logic: modifier calculation, badge aggregation, gameplay hooks |
| **Lua Config** (`scripts/customskills/config.lua`) | Server owner controls: enable/disable modifiers, per-badge values, caps |
| **Core3 Hooks** (19 files patched) | Minimal delegations at lifecycle boundaries (combat, crafting, XP, movement, etc.) |
| **SUI Menu** (`/customskills`) | Displays acquired bonuses and badge-source breakdowns using same math as gameplay |

**Data flow**: Badge bitmasks (on `PlayerObject`) — `CustomSkillsModifiers` (central authority) — Gameplay hooks AND Menu display. Single source of truth.

---

## Prerequisites

- SWGEmu Core3 source checkout (matching the patch base revision)
- CMake 3.10+, C++17 compiler
- Git (for applying patches)

---

## Installation Steps

### 1. Copy Module Files

```bash
# From customskills-mod root
cp -r package/MMOCoreORB/ /path/to/Core3/MMOCoreORB/
```

This adds:
- C++ module: `MMOCoreORB/src/server/zone/managers/customskills/`
- Command script: `MMOCoreORB/bin/scripts/commands/customSkills.lua`
- Configuration: `MMOCoreORB/bin/scripts/customskills/config.lua`

### 2. Apply Core3 Integration Patch

```bash
cd /path/to/Core3
git apply --check --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
git apply --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
```

**If `--check` fails**: The target Core3 revision differs. Port hunks manually using `MANIFEST.md` (lists all 19 integration files) and `docs/reference/ARCHITECTURE.md` (describes each hook's purpose). Do not force the patch.

### 3. Reconfigure & Build

```bash
# From Core3 root
cmake -B build  # or your standard configure command
cmake --build build --target MMOCoreORB
```

The recursive source glob will pick up the new `.cpp` files automatically.

### 4. Deploy & Restart

- Deploy the built binary (`MMOCoreORB`/`core3`)
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
    badgeBonus = 300,        -- 3.00% per combat mastery badge (basis points)
    multiplier = 15000,      -- 150.00% critical damage
    combatSpamLabel = "(CRIT)",
}
```

### Generic Modifiers (all disabled by default)

Each modifier table accepts:
- `enabled` — `true`/`false`
- `badgeBonus` — basis points per badge (or whole levels for armor penetration, defense cap, SEA cap)
- `cap` — maximum total (0 = uncapped)
- `badges` — list of badge keys (from `BadgeList`)
- `combatSpamLabel` — optional, for repeat-damage tiers only

```lua
modifiers = {
    doubleAttackChance  = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(DOUBLE)", badges = {} },
    tripleAttackChance  = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(TRIPLE)",  badges = {} },
    quadAttackChance    = { enabled = false, badgeBonus = 0, cap = 10000, combatSpamLabel = "(QUAD)",    badges = {} },
    armorPenetration    = { enabled = false, badgeBonus = 0, cap = 3, badges = {} },           -- whole levels
    defenseCapIncrease  = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },           -- whole points
    armorDegradeReduction = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
    weaponDegradeReduction = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
    seaCapIncrease      = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },           -- whole points
    movementSpeed       = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
    buffDuration        = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
    experienceMultiplier = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },          -- multiplier in bp
    practiceExperienceBonus = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },
    craftingSpeed       = { enabled = false, badgeBonus = 0, cap = 0, badges = {} },           -- multiplier in bp
    amazingSuccessChance = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
    amazingResults      = { enabled = false, badgeBonus = 0, cap = 10000, badges = {} },
    gatheringQuantity   = { enabled = false, badgeBonus = 20000, cap = 0, badges = {} },
}
```

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
[BADGE_REFERENCE.md](../reference/BADGE_REFERENCE.md) for the full catalog.
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
| Visible — grantable | `show = 0` badges can still grant modifiers if explicitly listed in config |

---

## Verification Checklist

1. **Startup**: No errors loading `customSkills.lua` or `CustomSkillsCommand`
2. **Command**: `/customskills` opens menu on a normal player character
3. **Navigation**: Open every category, use Back/Cancel, reopen repeatedly, test 2+ characters simultaneously
4. **Critical Chance**: Each combat mastery badge shows +3.00%, max 36.00% for all 12
5. **Combat test**: Critical attacks deal 150% of pre-armor damage
6. **Before enabling new modifier**: Run focused tests from `docs/reference/ARCHITECTURE.md`

---

## Removal

```bash
# 1. Remove module files
rm -rf MMOCoreORB/src/server/zone/managers/customskills/

# 2. Remove scripts
rm MMOCoreORB/bin/scripts/commands/customSkills.lua
rm -rf MMOCoreORB/bin/scripts/customskills/

# 3. Reverse patch
cd /path/to/Core3
git apply --check --reverse --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
git apply --reverse --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch

# 4. Rebuild & restart
```

If reverse check fails (Core3 files edited since), remove Custom Skills hunks manually using `MANIFEST.md` — do not force.

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

- **User Guide**: `../user-guide/USER_GUIDE.md` — Player-facing mechanics
- **Architecture & Implementation**: `../reference/ARCHITECTURE.md` — Hooks, services, menu, verification matrix
- **Modifier Reference**: `../reference/MODIFIER_REFERENCE.md` — All 17 modifiers with config
- **Badge Reference**: `../reference/BADGE_REFERENCE.md` — Full badge catalog
- **Core3 platform**: `../reference/CORE3_CODE_REFERENCE.md` + `../reference/CORE3_USER_GUIDE.md`

