# Custom Skills -- Architecture & Implementation Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Purpose

The single technical reference for the Custom Skills module: architecture,
hook inventory, runtime services, menu system, configuration, extension
procedures, and verification. Pure lookup data lives in
[MODIFIER_REFERENCE.md](MODIFIER_REFERENCE.md) and
[BADGE_REFERENCE.md](BADGE_REFERENCE.md). Build/run/test operations for the
Core3 platform live in [CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md).

Design *principles* are stated once in the master document ([CS-1..10]);
this document references them and does not restate them.

**Factual corrections** go through [../rules/errata.md](../rules/errata.md).

---

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- initial creation of predecessor
  documents (ARCHITECTURE.md, IMPLEMENTATION_GUIDE.md, MENU_SYSTEM.md)
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- consolidation into a
  single reference during documentation compression
- hy3-free (opencode/hy3-free), 2026-08-23 -- BRIEF-005 single-source rule for
  badge-backed modifiers; noted `getCriticalChance` reads the config badge map

---

## Physical Layout

```
MMOCoreORB/src/server/zone/managers/customskills/
|-- combat/           # CustomSkillsCombat.{h,cpp}
|-- buffs/            # CustomSkillsBuffs.{h,cpp}
|-- crafting/         # CustomSkillsCrafting.{h,cpp}
|-- durability/       # CustomSkillsDurability.{h,cpp}
|-- gathering/        # CustomSkillsGathering.{h,cpp}
|-- movement/         # CustomSkillsMovement.{h,cpp}
|-- progression/      # CustomSkillsProgression.{h,cpp}
|-- skillmods/        # CustomSkillsSkillMods.{h,cpp}
|-- CustomSkillsCommand.{h,cpp}       # /customskills command entry
|-- CustomSkillsConfig.{h,cpp}        # Singleton config loader
|-- CustomSkillsModifiers.{h,cpp}     # Central modifier authority
|-- CustomSkillsModifierType.h        # Modifier type enum
|-- CustomSkillsMenu.{h,cpp}          # C++ SUI menu
|-- CustomSkillsSuiCallback.{h,cpp}   # SUI event handling
`-- MENU_DESIGN.md                    # Menu design notes
```

The module lives inside the Core3 repository; this directory IS the
distribution source. Runtime-required files outside it:
`MMOCoreORB/bin/scripts/customskills/config.lua` and
`MMOCoreORB/bin/scripts/commands/customSkills.lua` (see [PROC R6.5]
containment rule).

---

## Data Flow

```
,-----------------,
|  Badge Bitmasks |  (on PlayerObject -- Core3 native persistence)
|  (140 bits)     |
`--------+--------`
         v
,----------------------------------------------,
|         CustomSkillsModifiers                |  <-- Central Authority [CS-3]
|  * getModifierTotal(player, type)            |
|  * getBadgeModifier(badgeKey, type)          |
|  * isModifierEnabled(type)                   |
|  * formatPercent/formatModifierBonus()       |
`--------+-------------------------------------`
         v
,----------------------------------------------,
|         CustomSkillsConfig (Singleton)       |
|  * Loads scripts/customskills/config.lua     |
|  * Provides: enabled[], caps[], badgeBonuses |
|  * Cached for combat performance             |
`----------------------------------------------`
         |
   ,-----+----+----------+----------+---------,
   v          v          v          v         v
 Combat  Durability  Progression Crafting  Buffs   (Runtime Services)
 Movement SkillMods  Gathering                     (query Modifiers)
   |          |          |          |         |
   v          v          v          v         v
 Core3 Hooks <------------------------------> Menu (CustomSkillsMenu)
   |                                            |
   `------------------+-------------------------`
                      v
             ,------------------,
             |   Same Values    |  <-- Single Source of Truth [CS-3]
             `------------------`
```

---

## Core Classes

### CustomSkillsModifierType.h

```cpp
enum Type {
    CRITICAL_CHANCE, CRITICAL_MULTIPLIER,
    DOUBLE_ATTACK_CHANCE, TRIPLE_ATTACK_CHANCE, QUAD_ATTACK_CHANCE,
    ARMOR_PENETRATION, DEFENSE_CAP_INCREASE, ARMOR_DEGRADE_REDUCTION, WEAPON_DEGRADE_REDUCTION,
    SEA_CAP_INCREASE, MOVEMENT_SPEED, BUFF_DURATION, EXPERIENCE_MULTIPLIER,
    PRACTICE_EXPERIENCE_BONUS, CRAFTING_SPEED, AMAZING_SUCCESS_CHANCE,
    AMAZING_RESULTS, GATHERING_QUANTITY, COUNT
};
```

### CustomSkillsConfig (Singleton + Logger)

- **Lifecycle**: constructed once at startup -> `setDefaults()` -> `load()`
- **Defaults**: all modifiers disabled; Critical Chance enabled (300 bp/badge,
  15000 multiplier)
- **load()**: parses `scripts/customskills/config.lua` via the `Lua` object
- **API**: `isModifierEnabled()`, `getModifierCap()`, `getBadgeBonus()`,
  `getBadgeBonuses()`, `getModifierCombatSpamLabel()`
- **Cache**: gameplay reads config values directly -- restart required on
  config change; missing/invalid values -> safe defaults + server-log warning

```cpp
// CustomSkillsConfig::load()
Lua lua; lua.init();
lua.runFile("scripts/customskills/config.lua");
LuaObject root = lua.getGlobalObject("customSkillsConfig");

customSummaryColor = root.getStringField("customSummaryColor", "00FF00");

// criticalChance table (special case)
criticalChanceEnabled = critical.getBooleanField("enabled", true);
criticalChancePerCombatBadge = critical.getIntField("badgeBonus", 300);
criticalMultiplier = critical.getIntField("multiplier", 15000);

// modifiers table -- generic loader per modifier
loadModifier(modifiers, "doubleAttackChance", DOUBLE_ATTACK_CHANCE, false, 0);
// ... etc
```

Generic modifier config fields: `enabled`, `badgeBonus`, `cap`, `badges[]`,
`combatSpamLabel` (repeat-damage only).

### CustomSkillsModifiers (Static API)

- **Central authority** -- all modifier queries route here ([CS-3])
- **Badge aggregation**: iterates config's badge map per modifier, checks
  `PlayerObject::hasBadge()`
- **Caps**: applies `config->getModifierCap(type)` if > 0 (0 = uncapped),
  after aggregation
- **Single source of truth ([CS-3])**: every badge-driven value -- gameplay
  hooks AND the SUI menu -- resolves from
  `CustomSkillsConfig::getBadgeBonuses(type)` / `getModifierTotal()`. C++ must
  never hardcode a modifier's badge key list or per-badge rates; doing so
  re-creates the ERR-005 (combat/menu divergence) failure.
- **Combat helpers**: `isCriticalChanceEnabled()`, `getCriticalChance()`
  (reads the config badge map -- same source as the menu; no hardcoded badge
  list), `getCriticalMultiplier()`
- **Formatting**: `formatPercent(bp)`, `colorizeCriticalText()`,
  `formatModifierBonus(type, value)`
- **Badge change notification**: `notifyBadgeAwarded(player)` refreshes
  visible skill mods & run speed

---

## Hook Inventory (H01-H16)

| ID | Module Call | Modifiers | Core3 Entry Point | Status |
|----|-------------|-----------|-------------------|--------|
| **H01** | `CustomSkillsCombat::modifyLandedDamage(context)` | Critical Chance/Multiplier, Double/Triple/Quad Attack | `CombatManager::applyDamage` wrapper | Implemented |
| **H02** | -- | None | -- | Rejected (repeat-damage handled by H01) |
| **H03** | `CustomSkillsCombat::getEffectiveArmorRating(attacker, nativeRating)` | Armor Penetration | `CombatManager::getArmorReduction` (after armor layer known, before armor-piercing) | Implemented |
| **H04** | `CustomSkillsCombat::getDefenseCap(defender, nativeCap)` | Defense Cap Increase | Shared cap in `CombatManager` (primary & secondary) | Implemented |
| **H05** | `CustomSkillsDurability::shouldDegradeArmor(defender, nativeChance)` | Armor Degradation Reduction | Before armor/PSG `inflictDamage` in combat mitigation | Implemented |
| **H06** | `CustomSkillsDurability::shouldDegradeWeapon(user, nativeChance)` | Weapon Degradation Reduction | `WeaponObjectImplementation::decay` at chance roll | Implemented |
| **H07** | `CustomSkillsProgression::getExperienceMultiplier(player, xpType)` | Experience Bonus | `PlayerManagerImplementation::awardExperience` (inside `applyModifiers` calc) | Implemented |
| **H08** | `CustomSkillsCrafting::modifyPracticeExperience(crafter, nativeAmount)` | Practice XP Bonus | `CraftingSessionImplementation::createPrototype` (`createItem==false` branch) | Implemented |
| **H09A** | `CustomSkillsCrafting::getPersonalCraftingDuration(crafter, nativeSeconds)` | Crafting Speed | `CraftingSessionImplementation::startCreationTasks` (before scheduling) | Implemented |
| **H09B** | `CustomSkillsCrafting::getFactoryProductionDuration(activator, factory, schematic, nativeSeconds)` | Crafting Speed | `FactoryObjectImplementation::startFactory` (after native timer calc) | Implemented |
| **H10** | `CustomSkillsCrafting::getAmazingChanceBonus(crafter, phase)` | Amazing Success Chance | `SharedLabratory::calculateAssemblySuccess` & `calculateExperimentationSuccess` | Implemented |
| **H11** | `CustomSkillsCrafting::applyAmazingResults(crafter, phase, attrs, strength)` | Amazing Results | Resource/genetic lab assembly & experimentation (only on `AMAZINGSUCCESS`) | Implemented |
| **H12A** | `CustomSkillsBuffs::modifyInitialDuration(recipient, buff, nativeDuration)` | Buff Duration | `BuffImplementation::activate` (before `scheduleBuffEvent`) | Implemented |
| **H12B** | `CustomSkillsBuffs::modifyRenewedDuration(recipient, buff, nativeDuration)` | Buff Duration | `CreatureObjectImplementation::renewBuff` (before renewal) | Implemented |
| **H13A** | `CustomSkillsMovement::getEffectiveRunSpeed(character, nativeSpeed, travelMode)` | Movement Speed | Client packet speed (`CreatureObjectMessage4`/`DeltaMessage4`) | Implemented |
| **H13B** | `CustomSkillsMovement::getEffectiveAllowedSpeed(character, nativeSpeed, travelMode)` | Movement Speed | `PlayerManagerImplementation::checkPlayerSpeedTest` (server validation) | Implemented |
| **H14A** | `CustomSkillsSkillMods::getEffectiveSkillMod(character, modName, nativeTotal, rawWearable)` | SEA Cap Increase | `CreatureObjectImplementation::getSkillMod` (after aggregation) | Implemented |
| **H14B** | `CustomSkillsSkillMods::getEffectiveVisibleSkillMod(character, modName, nativeEntry, rawWearable)` | SEA Cap Increase | Character skill-mod delta/baseline prep & badge-cap refresh | Implemented |
| **H15A** | `CustomSkillsGathering::modifyForageQuantity(player, resource, nativeQuantity)` | Gathering Quantity | `ForageManagerImplementation::forageGiveResource` (after roll) | Implemented |
| **H15B** | `CustomSkillsGathering::modifyMilkQuantity(player, creature, resource, nativeQuantity)` | Gathering Quantity | `MilkCreatureTask::giveMilkToPlayer` (after density adjustment) | Implemented |
| **H16** | `CustomSkillsModifiers::applyRarityNaming(object, excMod, legendaryModifier, exceptionalModifier)` | Server Config: Rarity Naming | `LootManagerImplementation::setCustomObjectName` -- after per-item modifiers are computed, BEFORE native suffix selection; returns true to skip the vanilla `(Exceptional)`/`(Legendary)` suffix and use color-only naming (`rarityNaming` config: enabled, legendaryColor FF00FF, exceptionalColor 0000FF) | Implemented |

Hook pattern: minimal generic delegation in the Core3 file; module-owned
service does all logic ([CS-2]).

### Consolidation Decisions

| Decision | Rationale |
|----------|-----------|
| H02 rejected | Double/Triple/Quad are damage repeats via H01, not separate attack executions |
| H01/H03/H04 separate hooks | Damage, armor selection, defense capping occur at different lifecycle points |
| H05/H06 shared policy, separate hooks | Armor & weapon degradation use different entry points & native chances |
| H08 separate from H07 | Avoids changing the widely-used `awardExperience` signature |
| H12A/H12B shared classifier | Same eligibility & calculation for initial + explicit renewal |
| H13A/H13B same calculation | Client speed + server validation MUST match ([CS-8]) |
| H14A/H14B same wearable-cap calc | Server mechanics + client-visible values must stay identical ([CS-8]) |

---

## Runtime Services

Each service is a static class with focused methods -- no singleton state;
pure functions of inputs.

| Service | Key Methods | Hooks |
|---------|-------------|-------|
| `CustomSkillsCombat` | `modifyLandedDamage()`, `getEffectiveArmorRating()`, `getDefenseCap()` | H01, H03, H04 |
| `CustomSkillsDurability` | `shouldDegradeArmor()`, `shouldDegradeWeapon()` | H05, H06 |
| `CustomSkillsProgression` | `getExperienceMultiplier()` | H07 |
| `CustomSkillsCrafting` | `modifyPracticeExperience()`, `getPersonalCraftingDuration()`, `getFactoryProductionDuration()`, `getAmazingChanceBonus()`, `applyAmazingResults()` | H08, H09A/B, H10, H11 |
| `CustomSkillsBuffs` | `getDuration()` (initial & renewal), eligibility classifier | H12A, H12B |
| `CustomSkillsMovement` | `getSpeed()` (client & server), `getEffectiveRunSpeed()`, `getEffectiveAllowedSpeed()` | H13A, H13B |
| `CustomSkillsSkillMods` | `getSkillMod()`, `getVisibleSkillMod()`, `getVisibleSkillModList()` | H14A, H14B |
| `CustomSkillsGathering` | `modifyForageQuantity()`, `modifyMilkQuantity()` | H15A, H15B |

---

## Runtime Ordering Details

### Combat Pipeline

```
1. Native landed hit resolved
2. Custom critical chance + multiplier applied
3. One repeat-damage tier checked (Quad -> Triple -> Double); one tier max
4. Final custom damage -> native damage pipeline
5. Armor Penetration: reduces defender armor level BEFORE native
   armor-piercing compare (Heavy->Medium->Light->None; floor None;
   does not touch weapon AP)
6. Defense Cap Increase: raises native 125 cap ONLY; SEA/tape above-cap
   contributions preserved
```

### Experience Stacking

```
Final XP = base x speciesMod x buffMod x localMod x globalExpMod x customMultiplier

- customMultiplier = 1.00x (10000 bp) when disabled [CS-9]
- Applies ONLY to positive awards on normal modifier path
- applyModifiers == false -> skipped
- Practice XP: native 5% applied FIRST, then custom practice bonus,
  then general multiplier via central award path
- Overflow: keep multiplied award well below 2^31 (int32 chain)
```

### Crafting Details

| Phase | Hook | Behavior |
|-------|------|----------|
| Personal speed | H09A | `nativeDuration / multiplier`, clamp >= 1s; crafter's current bonus |
| Factory speed | H09B | Snapshots activator's multiplier at run start; persists across logout/restart; stop/restart to recapture |
| Amazing Chance | H10 | Adds bp to native probability (assembly & experimentation); clamp to cap; does NOT multiply native chance; target: max-bonus char reaches >= 50% final |
| Amazing Results | H11 | On actual AMAZINGSUCCESS: `enhanced = nativeResult + ((schematicCap - nativeResult) x strength / 10000)`; raises resource ceiling only enough to retain. Assembly: all applicable attrs; experimentation: selected row only. 0% = native; 100% = poor resources can yield perfect attributes |

### Buff Duration Eligibility

**Included**: MEDICAL, PERFORMANCE, FOOD (incl. drinks), spice-up, positive
JEDI/Force.
**Excluded**: negative buffs, spice downers, debuffs, states, traps, DoTs,
cooldown/control markers, skill-item, innate, Squad Leader, concealment,
gallop, vehicle buffs.
**Implementation**: explicit classifier (CRC allowlist/exclusions) -- buff
type alone is insufficient. Explicit renewals (H12B) apply current bonus to
the new native duration; DB reload/internal reschedule preserves stored
duration (no re-multiplication).

### Movement Synchronization

| Hook | Purpose |
|------|---------|
| H13A | Client packet speed -- feeds CREO4 baseline/delta |
| H13B | Server validation -- `PlayerManager::checkPlayerSpeedTest` |

Both MUST use the same calculation ([CS-8]). Mount/vehicle validation reads
parent speed directly -- H13A alone would desync client/server. Applied once
to the final native speed in all travel modes (foot/mount/vehicle), not
reapplied per hop.

### SEA Cap Implementation

Native `SkillModManager` caps WEARABLE source at +/-25; `SkillModList` has no
character context, so:

- **H14A (server)**: `getSkillMod()` -> difference between native-clamped and
  custom-capped wearable contribution added to native total
- **H14B (client)**: `getVisibleSkillMod()` -> same difference applied to the
  visible entry
- **Result**: only SEA/tape contribution raised above +25; all other source
  caps preserved; server = client visible ([CS-8])

### Gathering Quantity

Bonuses add together, then multiply the native quantity:
0 badges = 1x, each badge adds its bp on top of 10000 base. Applied AFTER
native calculation (forage roll / milk density), rounded down, never below
native. Discrete loot (food, bait, medical components, rare items, live
creatures) is never duplicated.

---

## SUI Menu System

- **C++ owned** (`CustomSkillsMenu`, `CustomSkillsSuiCallback`) -- not a Lua
  screenplay. Rationale: avoids Lua as a second calculation path; queries the
  same typed API as gameplay hooks ([CS-3])
- **Entry**: `CustomSkillsCommand` -> Lua bridge `CustomSkills:openMenu(pPlayer)`
  -> C++ menu creation
- **Character-scoped**: SUI page stored in the invoking character's
  PlayerObject SUI map ([CS-7])
- **Multi-window**: multiple `/customskills` windows supported simultaneously

### Main Page Layout

```
Custom Skills

Badges
Bonuses
Server Config
```

The top-level menu has three entries:

- **Badges** -- browse badges by category, with per-badge option to set as
  favorite or inspect its detail page.
- **Bonuses** -- browse accumulated bonuses grouped by Combat, Utility, and
  Crafting (see Modifier Hierarchy). Only non-zero totals are shown.
- **Server Config** -- server-side toggle state, including the Rarity Naming
  detail page (see below).

Accumulated Bonuses shows only non-zero totals from acquired badges -- never
total-possible values. Disabled modifiers are omitted from the active summary
(or shown inactive per config).

### Modifier Hierarchy

The **Bonuses** category groups modifiers into three sub-trees:

```
Custom Skills > Bonuses
  Combat
    Critical Chance / Critical Multiplier
    Double / Triple / Quad Attack Chance
    Armor Penetration / Defense Cap Increase
  Utility
    Armor Degradation Reduction / Weapon Degradation Reduction
    SEA Cap Increase / Movement Speed
    Buff Duration / Experience Bonus / Gathering Quantity
  Crafting
    Practice Mode Experience Bonus / Crafting Speed
    Amazing Success Chance / Amazing Results
```

Display rules: stable configured display order (not badge index);
zero-valued categories hidden unless config requests them; disabled mechanics
visible but marked inactive.

### Modifier Detail Page

```
Custom Skills > Bonuses > Combat > Critical Chance

Total                              +12.00%
Milestone: 5 Badges                +1.00%
Milestone: 10 Badges               +1.00%
Milestone: 25 Badges               +1.00%
... (12 milestone exploration badges @ 100bp via badgeOverrides)
```

Total + source rows come from `CustomSkillsModifiers` (same as gameplay).
Source rows sort by contribution magnitude (desc), then display name.
Critical Chance is sourced from milestone exploration badges only (combat
masteries no longer grant it -- see [ERR-005]).

### Server Config

The **Server Config** top-level category exposes server-side state:

- **Mod Options** -- when `rarityNaming` is enabled, reveals a **Rarity Naming**
  detail page showing the configured legendary/exceptional color state.
- **SWGEMU Options** -- other server toggles.

This page is display-only; it reflects `config.lua` state loaded at server
start (see Configuration).

### Badge Hierarchy

```
Badges
  Milestone Badges
  Exploration
    Milestone Exploration
    Corellia / Dantooine / Dathomir / Endor / Lok / Naboo /
    Rori / Talus / Tatooine / Yavin IV
  Profession
  Quest
  Event
```

| Category | Contents | Order |
|----------|----------|-------|
| Milestone Badges | 7 accumulation milestones + 5 exploration milestones | Numeric threshold ascending |
| Exploration | Milestone Exploration first, then 10 planets | Planet alpha; badges within planet alpha by display name |
| Profession | 6 families (Combat, Crafting, Outdoors, Science, Social, Pilot) | Configured family order; professions alpha within family |
| Quest | Hero of Tatooine, Warren, Theme Parks, Corellian Corvette | Configured line order; story/progression within line |
| Event | Cries of Alderaan, Accolades, Librarian, Racing, Death Star | Configured group order; chronological within multi-stage |

### Badge Page Layout

```
Badges > Exploration > Tatooine

Accumulated Bonuses
+3% Critical Chance
+2 Melee Defense

O  Ben Kenobi's Old Home  (+1% Critical Chance)
X  Pool Beneath Fort Tusken  (+1% Critical Chance)
O  Lars Homestead  (+2% Critical Chance, +2 Melee Defense)
```

- Accumulated Bonuses: recursive total from acquired descendant badges
- Category entries show acquired descendant count: `Tatooine (2)`
- Badge rows: `O` (green, owned) or `X` (red, unowned) prefix -- only the
  marker colored
- Bonus suffix `(+X% Modifier)` in module summary color; omitted if no bonus

### Aggregation Rules

| Level | Aggregates |
|-------|------------|
| Planet page | Its acquired badges |
| Exploration | All exploration milestones + all planets |
| Profession / Quest / Event | All families / lines / groups respectively |
| Badges (root) | All badge categories |

Only acquired badges contribute. Totals come from the shared modifier
service -- the menu never independently adds display strings ([CS-3]).

### Navigation & Safety

| Control | Behavior |
|---------|----------|
| OK (select row) | Opens subpage |
| Back | Returns one level |
| Cancel | Exits completely |
| Refresh (badge lists) | Reopens current page (OK without valid selection also refreshes) |

Stock `Script.listBox` limitation: server event fires on button submit, not
row-selection change. Single-click navigation/right-click Back/double-click
reliability require client-side modifications (deferred).

Safety requirements:
- Validate `pPlayer` is a player creature in every entry point; validate ghost
  before reading character data
- Treat cancel, missing args, negative rows, unknown actions as no-op
- Store stable action IDs in SUI row data (never dispatch on display text);
  explicit action table only

### Color Scheme

| Element | Color |
|---------|-------|
| Owned badge marker (`O`) | Green (`\#00FF00` via `customSummaryColor`) |
| Unowned badge marker (`X`) | Red |
| Modifier bonus text | `customSummaryColor` (default `00FF00`) |
| Badge names / category text | Default client text color |
| Combat spam labels | Native colors untouched |

Development labels `(CRIT)/(DOUBLE)/(TRIPLE)/(QUAD)` are temporary
(`combatSpamLabel` in config.lua); remove after dev.

---

## Adding a New Modifier (Authoritative Checklist)

1. **Enum**: add to `CustomSkillsModifierType.h` (before `COUNT`)
2. **Name**: add to `CustomSkillsModifiers::getModifierName()`
3. **Formatting**: add to `formatModifierBonus()` (bp vs whole units)
4. **Defaults**: add in `CustomSkillsConfig::setDefaults()`
5. **Config loader**: add `loadModifier()` call in `CustomSkillsConfig::load()`
6. **Service**: create a new service class or extend an existing one
7. **Hook(s)**: add Core3 delegation per the hook inventory; update
   `integration/core3-hooks.patch`
8. **Badge assignments**: update `config.lua` (+ BADGE_REFERENCE.md for new
   badges); respect master-doc exclusions (pilot/JTL, admin/event)
9. **Menu**: add category/page in `CustomSkillsMenu` and the SUI section above
10. **Docs** ([PROC R6.6]): MODIFIER_REFERENCE.md entry, MANIFEST.md,
    INSTALLATION.md config reference if schema changed

---

## Patch Management

The patch exists for installing the module onto OTHER Core3 checkouts; this
repository already contains all edits. Apply / verify / reverse on a target
checkout:

```bash
git apply --check --ignore-space-change <module>/integration/core3-hooks.patch
git apply --ignore-space-change <module>/integration/core3-hooks.patch
git apply --check --reverse --ignore-space-change <module>/integration/core3-hooks.patch
git apply --reverse --ignore-space-change <module>/integration/core3-hooks.patch
```

If check fails, the target revision differs -- port hunks manually using
[../../MANIFEST.md](../../MANIFEST.md); never force. Each hunk is a small
generic delegation. Platform build details:
[CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md). Full install/remove procedure:
[../installation/INSTALLATION.md](../installation/INSTALLATION.md).

No hot reload: config is cached; restart after any `config.lua` change.

---

## Testing & Verification Matrix

Platform capabilities (unit-test suite, `testScreenPlays`, database
requirements): [CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md). The matrix below
defines behavioral checks for this module:

| Test | Description |
|------|-------------|
| Disabled/zero modifier | Reproduces native output exactly ([CS-9]) |
| Menu = runtime | All `/customskills` values equal effective values ([CS-3]) |
| Negative XP / `applyModifiers==false` | Unchanged |
| Practice mode | Retains native 5% with zero custom bonus |
| Factory speed | Survives logout/restart; changes only on new run |
| Non-amazing crafting | Remains resource-capped |
| Buff exclusions | Downers/debuffs/excluded families retain native durations |
| Movement sync | Client speed = server validation in all travel modes; no double-apply on mount transitions |
| SEA overflow | Server mechanics = client-visible values |
| Forage discrete loot | Never duplicated by Gathering Quantity |

Per-modifier focused tests:

| Modifier | Key Tests |
|----------|-----------|
| Movement | Foot, mount, vehicle; mount/dismount transitions; reconnect |
| Buff Duration | Activation, explicit renewal, DB reload, recursive linked buffs |
| Factory | Stop/restart captures new bonus; logout/restart preserves timer |
| Amazing Results | Assembly (all attrs) vs experimentation (selected row); ceiling raise |
| SEA Cap | Server calc = client visible; badge gain/loss refreshes visible |
| Forage/Milk | Round down; never below native; discrete loot unaffected |

---

## Debugging Tips

| Symptom | Check |
|---------|-------|
| Modifier not applying | `isModifierEnabled(type)` in config; badge keys match BadgeList exactly |
| Menu != combat | Both use `CustomSkillsModifiers::getModifierTotal()` -- verify same PlayerObject |
| Factory speed wrong | Activator snapshot at start; restart factory after badge change |
| Movement desync | H13A & H13B must use same `CustomSkillsMovement::getSpeed()` |
| SEA cap not visible | H14B refreshes on badge change; `refreshVisibleSkillMods()` |
| Buff not extended | Eligibility classifier (CRC allowlist); check `isEligibleBuff()` |

Core3-level gotchas: [CORE3_CODE_REFERENCE.md](CORE3_CODE_REFERENCE.md).
