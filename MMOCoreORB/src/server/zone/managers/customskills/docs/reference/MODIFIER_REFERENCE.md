# Custom Skills — Modifier Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Complete registry of all 17 modifiers with configuration, units, badge assignments, and gameplay behavior.

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

---

## Configuration Conventions

- **Percentages**: Basis points (100 = 1.00%)
- **Multipliers**: Basis points (10000 = 1.00x)
- **Whole units**: Armor Penetration (levels), Defense Cap/SEA Cap (points)
- **Caps**: 0 = uncapped; applied after badge aggregation
- **Defaults**: All disabled except Critical Chance

---

## Offense

### Critical Chance (`CRITICAL_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `true` |
| **Default badge bonus** | 300 bp (3.00%) |
| **Default cap** | 10000 (100%) |
| **Eligible badges** | 12 ground combat mastery |
| **Config key** | `criticalChance` (special table) |

**Behavior**: Chance for landed attack to become custom critical hit. Checked before repeat-damage tiers.

**Default badges** (all 300 bp each):
```
combat_1hsword_master, combat_2hsword_master, combat_bountyhunter_master,
combat_brawler_master, combat_carbine_master, combat_commando_master,
combat_marksman_master, combat_pistol_master, combat_polearm_master,
combat_rifleman_master, combat_smuggler_master, combat_unarmed_master
```

---

### Critical Multiplier

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | 15000 (150.00%) |
| **Config key** | `criticalChance.multiplier` |

**Behavior**: Damage multiplier on custom critical hits. Displayed even when Critical Chance = 0.

---

### Double Attack Chance (`DOUBLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Combat spam label** | `(DOUBLE)` |
| **Eligible badges** | Warren (2), Theme Parks (4) = 6 total |

**Behavior**: Repeats finalized damage 2× total. Checked last (after Quad, Triple). Only one tier triggers per hit.

---

### Triple Attack Chance (`TRIPLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Combat spam label** | `(TRIPLE)` |
| **Eligible badges** | 12 combat mastery (200 bp each; BH 300 bp), Hero of Tatooine (5 × 1000 bp) = 17 total |

**Behavior**: Repeats finalized damage 3× total. Checked after Quad, before Double.

---

### Quad Attack Chance (`QUAD_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Combat spam label** | `(QUAD)` |
| **Eligible badges** | 12 combat mastery (200 bp each; BH 300 bp), Hero of Tatooine (5 × 500 bp) = 17 total |

**Behavior**: Repeats finalized damage 4× total. Checked first.

---

### Armor Penetration (`ARMOR_PENETRATION`)

| Property | Value |
|----------|-------|
| **Unit** | Whole armor levels |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 3 levels |
| **Eligible badges** | Corellian Corvette (9) = 9 total |

**Behavior**: Reduces defender effective armor by 1 level per point: Heavy → Medium → Light → None. Floor: None. Does not affect weapon Armor Piercing. Applied before native armor-piercing comparison.

---

## Defense & Equipment

### Defense Cap Increase (`DEFENSE_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Hero of Tatooine (5 × 5), Yavin IV Exar Kun (5), Lok (3 × 5), Dathomir easy (6 × 5) = 15 total |

**Behavior**: Raises native 125 hard cap on primary/secondary defense calculations. Does not affect SEA/tape above-cap contributions.

---

### Armor Degradation Reduction (`ARMOR_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Eligible badges** | Talus (4), Rori (4) = 8 total (1250 bp each) |

**Behavior**: Native eligible armor degradation = 100% event chance. This reduces that chance. Failed roll skips condition loss entirely. Covers worn armor, PSG, NPC armor, vehicle armor.

---

### Weapon Degradation Reduction (`WEAPON_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Eligible badges** | Dantooine (4), Endor (4) = 8 total (1250 bp each) |

**Behavior**: Native weapon degradation chance (base 5% + powerup adjustment) reduced multiplicatively:
```
effectiveChance = nativeChance × (10000 - reduction) / 10000
```
Condition loss amount unchanged on successful roll.

---

### SEA Cap Increase (`SEA_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Tatooine dangerous (4 × 15), Dathomir Sarlacc (15) = 5 total |

**Behavior**: Raises wearable (attachments/tapes) contribution cap above native +25. Does not raise normal defense cap. SEA/tape bonuses retain ability to exceed normal cap.

---

## Character & Utility

### Movement Speed (`MOVEMENT_SPEED`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (undecided) |
| **Eligible badges** | Naboo (4 × 2500) = 4 total |

**Behavior**: Multiplies final native speed in ALL travel modes (foot, mount, vehicle). Applied once — not added to mount then reapplied. Synchronized client (H13A) and server (H13B).

---

### Buff Duration (`BUFF_DURATION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Tatooine easy (3 × 2500), Yavin IV Woolamander/Blueleaf (2 × 2500), Science mastery (3 × 2500), Social mastery (5 × 2000) = 13 total |

**Behavior**: Increases initial duration of eligible buff families only:
- **Included**: Medical, Performance, Food/Drink, Spice-up, positive Jedi/Force
- **Excluded**: Negative buffs, spice downers, debuffs, states, traps, DoTs, cooldown/control markers, skill-item, innate, Squad Leader, concealment, gallop, vehicle buffs

Explicit renewals (H12B) apply current bonus to new native duration. DB reload/internal reschedule preserves stored duration (no re-multiplication).

---

### Experience Bonus (`EXPERIENCE_MULTIPLIER`)

| Property | Value |
|----------|-------|
| **Unit** | Multiplier (basis points) |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Corellia (5 × 10000) = 5 total |

**Behavior**: Multiplies ALL positive XP awards on normal modifier path:
```
finalXP = base × species × buff × local × global × customMultiplier
```
Does not affect: XP deductions, conversion costs, `applyModifiers == false` awards.
Composes multiplicatively: 100 XP × 2x server × 5x character = 1000 XP (10x total).

---

### Practice Mode Experience Bonus (`PRACTICE_EXPERIENCE_BONUS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Tatooine easy (3 × 10000), Yavin IV Woolamander/Blueleaf (2 × 10000), Social mastery (5 × 10000) = 10 total |

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
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Crafting mastery (9 × 1000), Doctor mastery (1000) = 10 total |

**Behavior**:
- **Personal**: `nativeDuration / multiplier`, clamp ≥1s. Uses crafter's current bonus.
- **Factory**: Snapshots **activator's** multiplier at run start. Persists across logout/restart. Badge changes mid-run don't affect current run. Stop/restart to recapture.

---

### Crafting Amazing Success Chance (`AMAZING_SUCCESS_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (percentage points) |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Eligible badges** | Yavin IV Exar Kun (500), Lok (3 × 500), Yavin IV Woolamander/Blueleaf (2 × 500) = 6 total |

**Behavior**: Adds bp to native Amazing Success probability (assembly & experimentation). Does NOT multiply native chance. Same policy at both roll sites. Clamped to configured cap. Target: max-bonus character reaches ≥50% final chance.

---

### Amazing Results (`AMAZING_RESULTS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (strength %) |
| **Default enabled** | `false` |
| **Default badge bonus** | 0 |
| **Default cap** | 10000 (100%) |
| **Eligible badges** | Yavin IV Exar Kun (1000), Lok (3 × 1000), Yavin IV Woolamander/Blueleaf (2 × 1000) = 6 total |

**Behavior**: On actual `AMAZINGSUCCESS`, for each affected attribute:
```
enhanced = nativeResult + ((schematicCap - nativeResult) × strength / 10000)
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
| **Default enabled** | `false` |
| **Default badge bonus** | 20000 (200%) |
| **Default cap** | 0 (uncapped) |
| **Eligible badges** | Outdoors mastery (5 × 11500), Science mastery (3 × 10000), Tatooine easy (3 × 2500), Yavin IV Woolamander/Blueleaf (2 × 2500) = 13 total |

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

| Modifier | Unit | Per Badge | Max Badges | Max Total |
|----------|------|-----------|------------|-----------|
| Critical Chance | bp | 400 | 12 | 6000 (60%) |
| Critical Multiplier | bp | 1250 | 12 | 15000 bonus (300% total) |
| Double Attack | bp | 2000 | 6 | 10000 (100%) |
| Triple Attack | bp | 200 | 17 | 7500 (75%) |
| Quad Attack | bp | 200 | 17 | 5000 (50%) |
| Armor Penetration | levels | 1 | 9 | 9 (eff. max 3) |
| Defense Cap | points | 5 | 15 | 100 |
| SEA Cap | points | 15 | 5 | 75 |
| Armor Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Weapon Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Movement Speed | bp | 2500 | 4 | 10000 (100%) |
| Buff Duration | bp | 2500 | 11 | 30000 (300%) |
| Experience Bonus | multiplier bp | 10000 | 5 | 50000 (500%) |
| Practice XP Bonus | bp | 10000 | 8 | 100000 (1000%) |
| Crafting Speed | multiplier bp | 1000 | 10 | 10000 (100%) |
| Amazing Success | bp | 500 | 10 | 5000 (50%) |
| Amazing Results | bp | 1000 | 10 | 10000 (100%) |
| Gathering Quantity | bp | 11500 | 13 | 100000 (1000%) |

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
