# Custom Skills -- Modifier Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Complete registry of all 18 modifiers with configuration, units, badge assignments, and gameplay behavior.

> **NOTE:** `bin/scripts/customskills/config.lua` is the authoritative source
> for all current modifier values (enabled state, badge assignments, caps,
> overrides). The per-modifier tables below are generated from it. If they
> disagree, trust `config.lua` and file an ERR entry.

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- BRIEF-003: count
  reconciliation, Critical Multiplier standalone entry, staleness banner
- hy3-free (opencode/hy3-free), 2026-08-23 -- BRIEF-004: refresh all
  per-modifier tables to config.lua; badgeOverrides/rarityNaming conventions;
  regenerate Maximum Theoretical Totals

---

## Configuration Conventions

- **Percentages**: Basis points (100 = 1.00%)
- **Multipliers**: Basis points (10000 = 1.00x)
- **Whole units**: Armor Penetration (levels), Defense Cap/SEA Cap (points)
- **Caps**: 0 = uncapped; applied after badge aggregation
- **Current config state**: in `config.lua`, all 18 modifiers are `enabled = true`
  with full badge assignments. The reference tables below reflect this. The
  module ships with safe C++ defaults; `config.lua` is the authoritative
  source of live values.
- **badgeOverrides**: a table of `{ "badgeKey", value }` pairs (on
  `criticalChance` directly, or under each `modifiers.<name>`) that override the
  default `badgeBonus` for the named badges only. Loaded by
  `CustomSkillsConfig::loadBadgeOverrides` (called from
  `CustomSkillsConfig::load()`), applied AFTER the `badges` list populates
  `modifierBadgeBonuses`. A badge listed in `badges` but not overridden uses
  the modifier's `badgeBonus`.
- **rarityNaming**: a server-config section (`enabled`, `legendaryColor`,
  `exceptionalColor`, six-character RGB hex) that switches item naming to
  color-only text instead of `(Exceptional)`/`(Legendary)` suffixes (see hook
  H16 / ARCHITECTURE.md). Configured independently of the badge modifiers.

---

## Offense

### Critical Chance (`CRITICAL_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 100 bp (1.00%) each via `badgeOverrides` (base `badgeBonus` 400 bp overridden) |
| **Cap (config.lua)** | 6000 (60.00%) |
| **Badges (config.lua)** | 12 milestone exploration badges |
| **Config key** | `criticalChance` (special table) |

**Behavior**: Chance for a landed attack to become a custom critical hit.
Checked before repeat-damage tiers. Both the combat roll
(`CustomSkillsCombat::applyDamage`) and the SUI menu read this value from the
config badge map (`CustomSkillsConfig::getBadgeBonuses(CRITICAL_CHANCE)` /
`getModifierTotal`) -- a single source of truth ([CS-3]); it is fully
configurable in `config.lua`.

**Badges** (all 100 bp each, via `badgeOverrides`):
```
count_5, count_10, count_25, count_50, count_75, count_100, count_125,
bdg_exp_10_badges, bdg_exp_20_badges, bdg_exp_30_badges, bdg_exp_40_badges, bdg_exp_45_badges
```

---

### Critical Multiplier (`CRITICAL_MULTIPLIER`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (multiplier) |
| **Config key** | `modifiers.criticalMultiplier` |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1250 bp (12.50%/badge) |
| **Cap (config.lua)** | 15000 bp (150% on top of the 150% base = 300% max) |
| **Badges (config.lua)** | 7 accumulation milestones + 5 exploration milestones = 12 total |

**Behavior**: Badge-driven damage multiplier applied on custom critical hits
(promoted from a `criticalChance.multiplier` sub-key to a standalone modifier
in commit c832b1c263). Stacks additively with the base:
```
critDamage = preArmorDamage x (baseMultiplier + badgeBonusTotal) / 10000
```
with `baseMultiplier = criticalChance.multiplier` (default 15000 = 150%);
current bonus cap 15000 -> 300% maximum crit damage
(CustomSkillsCombat.cpp H01; CustomSkillsModifiers.cpp:60-65).

---

### Double Attack Chance (`DOUBLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 2000 bp (20.00%) |
| **Cap (config.lua)** | 10000 (100%) |
| **Combat spam label** | `(DOUBLE)` |
| **Badges (config.lua)** | Warren (2), Theme Parks (4) = 6 total |

**Behavior**: Repeats finalized damage 2x total. Checked last (after Quad, Triple). Only one tier triggers per hit.

---

### Triple Attack Chance (`TRIPLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 200 bp (2.00%); BH mastery overridden to 300 bp |
| **Cap (config.lua)** | 7500 (75%) |
| **Combat spam label** | `(TRIPLE)` |
| **Badges (config.lua)** | 12 combat mastery (11 x 200 bp, BH x 300 bp via badgeOverride), 5 POI (rabidbeast, prisonbreak, twoliars, factoryliberation, heromark) x 200 bp = 17 total |

**Behavior**: Repeats finalized damage 3x total. Checked after Quad, before Double.

---

### Quad Attack Chance (`QUAD_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 200 bp (2.00%); BH mastery overridden to 300 bp |
| **Cap (config.lua)** | 5000 (50%) |
| **Combat spam label** | `(QUAD)` |
| **Badges (config.lua)** | 12 combat mastery (11 x 200 bp, BH x 300 bp via badgeOverride), 5 POI (rabidbeast, prisonbreak, twoliars, factoryliberation, heromark) x 200 bp = 17 total |

**Behavior**: Repeats finalized damage 4x total. Checked first.

---

### Armor Penetration (`ARMOR_PENETRATION`)

| Property | Value |
|----------|-------|
| **Unit** | Whole armor levels |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1 level |
| **Cap (config.lua)** | 3 levels |
| **Badges (config.lua)** | Corellian Corvette (9) = 9 total |

**Behavior**: Reduces defender effective armor by 1 level per point: Heavy -> Medium -> Light -> None. Floor: None. Does not affect weapon Armor Piercing. Applied before native armor-piercing comparison.

---

## Defense & Equipment

### Defense Cap Increase (`DEFENSE_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 5 points |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | 5 POI (rabidbeast, prisonbreak, twoliars, factoryliberation, heromark) x 5, Yavin IV Exar Kun (5), Lok (3 x 5), Dathomir easy (6 x 5) = 15 total |

**Behavior**: Raises native 125 hard cap on primary/secondary defense calculations. Does not affect SEA/tape above-cap contributions.

---

### Armor Degradation Reduction (`ARMOR_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1250 bp (12.50%) |
| **Cap (config.lua)** | 10000 (100%) |
| **Badges (config.lua)** | Talus (4), Rori (4) = 8 total (1250 bp each) |

**Behavior**: Native eligible armor degradation = 100% event chance. This reduces that chance. Failed roll skips condition loss entirely. Covers worn armor, PSG, NPC armor, vehicle armor.

---

### Weapon Degradation Reduction (`WEAPON_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1250 bp (12.50%) |
| **Cap (config.lua)** | 10000 (100%) |
| **Badges (config.lua)** | Dantooine (4), Endor (4) = 8 total (1250 bp each) |

**Behavior**: Native weapon degradation chance (base 5% + powerup adjustment) reduced multiplicatively:
```
effectiveChance = nativeChance x (10000 - reduction) / 10000
```
Condition loss amount unchanged on successful roll.

---

### SEA Cap Increase (`SEA_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 15 points |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Tatooine dangerous (4 x 15), Dathomir Sarlacc (15) = 5 total |

**Behavior**: Raises wearable (attachments/tapes) contribution cap above native +25. Does not raise normal defense cap. SEA/tape bonuses retain ability to exceed normal cap.

---

## Character & Utility

### Movement Speed (`MOVEMENT_SPEED`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 2500 bp (25.00%) |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Naboo (4 x 2500) = 4 total |

**Behavior**: Multiplies final native speed in ALL travel modes (foot, mount, vehicle). Applied once -- not added to mount then reapplied. Synchronized client (H13A) and server (H13B).

---

### Buff Duration (`BUFF_DURATION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 2500 bp (25.00%); Social mastery overridden to 2000 bp |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Tatooine easy (3 x 2500), Yavin IV Woolamander/Blueleaf (2 x 2500), Science mastery (3 x 2500), Social mastery (5 x 2000) = 13 total |

**Behavior**: Increases initial duration of eligible buff families only:
- **Included**: Medical, Performance, Food/Drink, Spice-up, positive Jedi/Force
- **Excluded**: Negative buffs, spice downers, debuffs, states, traps, DoTs, cooldown/control markers, skill-item, innate, Squad Leader, concealment, gallop, vehicle buffs

Explicit renewals (H12B) apply current bonus to new native duration. DB reload/internal reschedule preserves stored duration (no re-multiplication).

---

### Experience Bonus (`EXPERIENCE_MULTIPLIER`)

| Property | Value |
|----------|-------|
| **Unit** | Multiplier (basis points) |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 10000 bp (100.00%) |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Corellia (5 x 10000) = 5 total |

**Behavior**: Multiplies ALL positive XP awards on normal modifier path:
```
finalXP = base x species x buff x local x global x customMultiplier
```
Does not affect: XP deductions, conversion costs, `applyModifiers == false` awards.
Composes multiplicatively: 100 XP x 2x server x 5x character = 1000 XP (10x total).

---

### Practice Mode Experience Bonus (`PRACTICE_EXPERIENCE_BONUS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 10000 bp (100.00%) |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Tatooine easy (3 x 10000), Yavin IV Woolamander/Blueleaf (2 x 10000), Social mastery (5 x 10000) = 10 total |

**Behavior**: Applies when `createItem == false` (practice mode). Order:
1. Base crafting XP
2. Core3 native 5% practice increase
3. Custom practice bonus (multiplies)
4. General Experience Bonus (H07) via central award path

---

## Crafting

### Crafting Speed (`CRAFTING_SPEED`)

| Property | Value |
|----------|-------|
| **Unit** | Speed multiplier (basis points) |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1000 bp (10.00%) |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Crafting mastery (9 x 1000), Doctor mastery (1000) = 10 total |

**Behavior**:
- **Personal**: `nativeDuration / multiplier`, clamp >=1s. Uses crafter's current bonus.
- **Factory**: Snapshots **activator's** multiplier at run start. Persists across logout/restart. Badge changes mid-run don't affect current run. Stop/restart to recapture.

---

### Crafting Amazing Success Chance (`AMAZING_SUCCESS_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (percentage points) |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 500 bp (5.00%) |
| **Cap (config.lua)** | 10000 (100%) |
| **Badges (config.lua)** | Yavin IV Exar Kun (500), Lok (3 x 500), Yavin IV Woolamander/Blueleaf (2 x 500) = 6 total |

**Behavior**: Adds bp to native Amazing Success probability (assembly & experimentation). Does NOT multiply native chance. Same policy at both roll sites. Clamped to configured cap. Target: max-bonus character reaches >=50% final chance.

---

### Amazing Results (`AMAZING_RESULTS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (strength %) |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (config.lua)** | 1000 bp (10.00%) |
| **Cap (config.lua)** | 10000 (100%) |
| **Badges (config.lua)** | Yavin IV Exar Kun (1000), Lok (3 x 1000), Yavin IV Woolamander/Blueleaf (2 x 1000) = 6 total |

**Behavior**: On actual `AMAZINGSUCCESS`, for each affected attribute:
```
enhanced = nativeResult + ((schematicCap - nativeResult) x strength / 10000)
```
Raises resource-derived ceiling only enough to retain enhanced value.
- **Assembly**: All applicable initial attributes
- **Experimentation**: Selected row only
- **Non-amazing**: Fully resource-limited (unchanged)
- 0% = native Core3 behavior; 100% = poor resources can yield perfect attributes

---

## Gathering

### Gathering Quantity (`GATHERING_QUANTITY`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Enabled (config.lua)** | `true` |
| **Badge bonus (base, config.lua)** | 2500 bp (25.00%); Outdoors mastery overridden to 11500 bp, Science mastery to 10000 bp |
| **Cap (config.lua)** | 0 (uncapped) |
| **Badges (config.lua)** | Outdoors mastery (5 x 11500), Science mastery (3 x 10000), Tatooine easy (3 x 2500), Yavin IV Woolamander/Blueleaf (2 x 2500) = 13 total |

**Behavior**: Bonuses add together, then multiply native quantity:
```
0 badges: 100% (1x)
1 badge:  300% (3x)
2 badges: 500% (5x)
```
Applied AFTER native calculation (forage roll / milk density adjustment). Rounded down to whole unit. Never below native.
**Does not affect**: Discrete loot (food, bait, medical components, rare items, live creatures).

---

## Maximum Theoretical Totals (All 111 Eligible Badges)

Values are the sum of each modifier's configured badge bonuses (including
badgeOverrides), capped at the configured cap where one applies. `config.lua`
is authoritative.

| Modifier | Unit | Per-Badge (config) | Max Badges | Max Total (capped) |
|----------|------|--------------------|------------|--------------------|
| Critical Chance | bp | 100 (via badgeOverrides) | 12 | 1200 (12%) |
| Critical Multiplier | bp | 1250 | 12 | 15000 bonus (300% total) |
| Double Attack | bp | 2000 | 6 | 10000 (100%) |
| Triple Attack | bp | 200 (BH 300) | 17 | 3500 (35%) |
| Quad Attack | bp | 200 (BH 300) | 17 | 3500 (35%) |
| Armor Penetration | levels | 1 | 9 | 3 (eff. max 3) |
| Defense Cap | points | 5 | 15 | 75 |
| SEA Cap | points | 15 | 5 | 75 |
| Armor Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Weapon Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Movement Speed | bp | 2500 | 4 | 10000 (100%) |
| Buff Duration | bp | 2500 (Social 2000) | 13 | 30000 (300%) |
| Experience Bonus | multiplier bp | 10000 | 5 | 50000 (500%) |
| Practice XP Bonus | bp | 10000 | 10 | 100000 (1000%) |
| Crafting Speed | multiplier bp | 1000 | 10 | 10000 (100%) |
| Amazing Success | bp | 500 | 6 | 3000 (30%) |
| Amazing Results | bp | 1000 | 6 | 6000 (60%) |
| Gathering Quantity | bp | 2500 (Outdoors 11500, Science 10000) | 13 | 100000 (1000%) |

---

## Badge Pool Summary

| Category | Eligible Badges |
|----------|-----------------|
| Milestone Badges (Accumulation) | 7 |
| Exploration Milestones | 5 |
| Exploration Planets | 45 |
| Combat Mastery | 12 |
| Crafting Mastery | 9 |
| Outdoors Mastery | 5 |
| Science Mastery | 3 |
| Social Mastery | 5 |
| Quest (Hero, Warren, Theme Parks, Corvette) | 20 |
| **Total** | **111** |

Excluded: 9 Pilot/JTL, 20 admin/event badges (see `EXCLUDED_BADGES.md`)
