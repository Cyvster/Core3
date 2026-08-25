# Custom Skills -- Architecture & Implementation Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Purpose

The single technical reference for the Custom Skills module: architecture,
hook inventory, runtime services, menu system, configuration, extension
procedures, and verification. Pure lookup data lives in
[# Appendix A](#appendix-a--modifier-specification) (modifiers) and [# Appendix B](#appendix-b--badge-catalog) (badges). Build/run/test operations for the
Core3 platform live in [../../swgemu/CODE_REFERENCE.md](../../swgemu/CODE_REFERENCE.md)
(Part II).

Design *principles* are stated once in the master document ([CS-1..10]);
this document references them and does not restate them.

**Factual corrections** go through [../rules/errata.md](../rules/errata.md).


## Contributors

**Nemotron 3.5 Lightning Free (AI)** -- initial creation of predecessor
 documents (ARCHITECTURE.md, IMPLEMENTATION_GUIDE.md, MENU_SYSTEM.md)  
ox-alpha (opencode/x-preview-f-free), 08232026 -- consolidation into a
 single reference during documentation compression  
hy3-free (opencode/hy3-free), 08232026 -- BRIEF-005 single-source rule for
 badge-backed modifiers; noted `getCriticalChance` reads the config badge map  
hy3-free (opencode/hy3-free), 08242026 -- BRIEF-014 doc removals (combat
 spam label references)  


**Last reconciled:** 08242026 by hy3-free (opencode/hy3-free) -- BRIEF-014
combat-spam-label doc removals.


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


## Core Classes

### CustomSkillsModifierType.h

```cpp
class CustomSkillsModifierType {
public:
    enum Type {
        CRITICAL_CHANCE, CRITICAL_MULTIPLIER,
        DOUBLE_ATTACK_CHANCE, TRIPLE_ATTACK_CHANCE, QUAD_ATTACK_CHANCE,
        ARMOR_PENETRATION, DEFENSE_CAP_INCREASE, ARMOR_DEGRADE_REDUCTION, WEAPON_DEGRADE_REDUCTION,
        SEA_CAP_INCREASE, MOVEMENT_SPEED, BUFF_DURATION, EXPERIENCE_MULTIPLIER,
        PRACTICE_EXPERIENCE_BONUS, CRAFTING_SPEED, AMAZING_SUCCESS_CHANCE,
        AMAZING_RESULTS, GATHERING_QUANTITY, COUNT
    };
};
```

> **GOTCHA (verified 08242026, ERR-010):** `CustomSkillsModifierType` is a
> **class**, NOT a namespace. Its enumerators are scoped as
> `CustomSkillsModifierType::CRITICAL_CHANCE`, etc. `using namespace
> CustomSkillsModifierType;` is a **compile error** -- every enumerator must be
> written with the explicit `CustomSkillsModifierType::` prefix. The menu
> (`CustomSkillsMenu::getAcquiredCount`, `addPageItems`) qualifies them this
> way; do not "simplify" to a `using` directive.

### CustomSkillsConfig (Singleton + Logger)

**Lifecycle**: constructed once at startup -> `setDefaults()` -> `load()`
  
**Defaults**: all modifiers disabled; Critical Chance enabled (300 bp/badge,
 15000 multiplier)  
**load()**: parses `scripts/customskills/config.lua` via the `Lua` object
  
**API**: `isModifierEnabled()`, `getModifierCap()`, `getBadgeBonus()`,
 `getBadgeBonuses()`  
**Cache**: gameplay reads config values directly -- restart required on
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

Generic modifier config fields: `enabled`, `badgeBonus`, `cap`, `badges[]`.

### CustomSkillsModifiers (Static API)

**Central authority** -- all modifier queries route here ([CS-3])
  
**Badge aggregation**: iterates config's badge map per modifier, checks
 `PlayerObject::hasBadge()`  
**Caps**: applies `config->getModifierCap(type)` if > 0 (0 = uncapped),
 after aggregation  
**Single source of truth ([CS-3])**: every badge-driven value -- gameplay
 hooks AND the SUI menu -- resolves from `CustomSkillsConfig::getBadgeBonuses(type)` / `getModifierTotal()`. C++ must never hardcode a modifier's badge key list or per-badge rates; doing so re-creates the ERR-005 (combat/menu divergence) failure.  
**Combat helpers**: `isCriticalChanceEnabled()`, `getCriticalChance()`
 (reads the config badge map -- same source as the menu; no hardcoded badge list), `getCriticalMultiplier()`  
**Formatting**: `formatPercent(bp)`, `colorizeCriticalText()`,
 `formatModifierBonus(type, value)`  
**Badge change notification**: `notifyBadgeAwarded(player)` refreshes
 visible skill mods & run speed  


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

**H14A (server)**: `getSkillMod()` -> difference between native-clamped and
 custom-capped wearable contribution added to native total  
**H14B (client)**: `getVisibleSkillMod()` -> same difference applied to the
 visible entry  
**Result**: only SEA/tape contribution raised above +25; all other source
 caps preserved; server = client visible ([CS-8])  

### Gathering Quantity

Bonuses add together, then multiply the native quantity:
0 badges = 1x, each badge adds its bp on top of 10000 base. Applied AFTER
native calculation (forage roll / milk density), rounded down, never below
native. Discrete loot (food, bait, medical components, rare items, live
creatures) is never duplicated.


## SUI Menu System

**C++ owned** (`CustomSkillsMenu`, `CustomSkillsSuiCallback`) -- not a Lua
 screenplay. Rationale: avoids Lua as a second calculation path; queries the same typed API as gameplay hooks ([CS-3])  
**Entry**: `CustomSkillsCommand` -> Lua bridge `CustomSkills:openMenu(pPlayer)`
 -> C++ menu creation  
**Character-scoped**: SUI page stored in the invoking character's
 PlayerObject SUI map ([CS-7])  
**Multi-window**: multiple `/customskills` windows supported simultaneously
  
**Bonus count helper**: `CustomSkillsMenu::countOwnedBonuses(player,
 CustomSkillsModifierType::Type)` walks `config.lua` badge keys for a modifier (via `CustomSkillsConfig::getBadgeBonuses(type)`) and counts how many the player actually owns (used by `getAcquiredCount` and `addBonusItems`). Both helpers are declared in `CustomSkillsMenu.h`; the enum arg is the **class**- scoped `CustomSkillsModifierType::Type` (see gotcha above).  
**Per-category modifier total**: `CustomSkillsMenu::countModifier(player,
 const char* const* keys, int count, CustomSkillsModifierType::Type)` is the per-page helper called by `getModifierTotal()` via the `MOD_LEAF` macro. It iterates a category's badge `keys[]`, and for each owned badge sums the matching basis-point value from `getBadgeBonuses(type)` (mirrors the gameplay `CustomSkillsModifiers::getModifierTotal` pattern). Reconstructed in ERR-011 when its definition turned out missing from the generated `.cpp`.  

### Main Page Layout

```
Custom Skills

Badges
Bonuses
Server Config
```

The top-level menu has three entries:

**Badges** -- browse badges by category, with per-badge option to set as
 favorite or inspect its detail page.  
**Bonuses** -- browse accumulated bonuses grouped by Combat, Utility, and
 Crafting (see Modifier Hierarchy). Only non-zero totals are shown.  
**Server Config** -- server-side toggle state, including the Rarity Naming
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
... (12 milestone exploration badges @ 100bp uniform)
```

Total + source rows come from `CustomSkillsModifiers` (same as gameplay).
Source rows sort by contribution magnitude (desc), then display name.
Critical Chance is sourced from milestone exploration badges only (combat
masteries no longer grant it -- see [ERR-005]).

### Server Config

The **Server Config** top-level category exposes server-side state:

**Mod Options** -- when `rarityNaming` is enabled, reveals a **Rarity Naming**
 detail page showing the configured legendary/exceptional color state.  
**SWGEMU Options** -- other server toggles.
  

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

Accumulated Bonuses: recursive total from acquired descendant badges
  
Category entries show acquired descendant count: `Tatooine (2)`
  
Badge rows: `O` (green, owned) or `X` (red, unowned) prefix -- only the
 marker colored  
Bonus suffix `(+X% Modifier)` in module summary color; omitted if no bonus
  

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
Validate `pPlayer` is a player creature in every entry point; validate ghost
 before reading character data  
Treat cancel, missing args, negative rows, unknown actions as no-op
  
Store stable action IDs in SUI row data (never dispatch on display text);
 explicit action table only  

### Color Scheme

| Element | Color |
|---------|-------|
| Owned badge marker (`O`) | Green (`\#00FF00` via `customSummaryColor`) |
| Unowned badge marker (`X`) | Red |
| Modifier bonus text | `customSummaryColor` (default `00FF00`) |
| Badge names / category text | Default client text color |


## Adding a New Modifier (Authoritative Checklist)

(1) **Enum**: add to `CustomSkillsModifierType.h` (before `COUNT`)
  
(2) **Name**: add to `CustomSkillsModifiers::getModifierName()`
  
(3) **Formatting**: add to `formatModifierBonus()` (bp vs whole units)
  
(4) **Defaults**: add in `CustomSkillsConfig::setDefaults()`
  
(5) **Config loader**: add `loadModifier()` call in `CustomSkillsConfig::load()`
  
(6) **Service**: create a new service class or extend an existing one
  
(7) **Hook(s)**: add Core3 delegation per the hook inventory; update
 `integration/core3-hooks.patch`  
(8) **Badge assignments**: update `config.lua` (+ Appendix B for new
 badges); respect master-doc exclusions (pilot/JTL, admin/event)  
(9) **Menu**: add category/page in `CustomSkillsMenu` and the SUI section above
  
(10) **Docs** ([PROC R6.6]): CODE_REFERENCE.md Appendix A entry, MANIFEST.md,
 INSTALLATION.md config reference if schema changed  


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

##Appendix A -- Modifier Specification

Complete registry of all 18 modifiers: configuration conventions, units, badge assignments, caps, and gameplay behavior. Tables document the SHIPPED DEFAULTS; a given server's config.lua is the authoritative source of its live values.

## Configuration Conventions

**Percentages**: Basis points (100 = 1.00%)
  
**Multipliers**: Basis points (10000 = 1.00x)
  
**Whole units**: Armor Penetration (levels), Defense Cap/SEA Cap (points)
  
**Caps**: 0 = uncapped; applied after badge aggregation
  
**Shipped defaults**: the module ships with all 18 modifiers enabled and full badge assignments. The tables below document these defaults -- they are not live values. A server owner can change anything (enabled flags, badge lists, caps, badgeBonus values); on any given server, its config.lua is the single authoritative source of live values.
  
**Uniform badgeBonus (default convention)**: each modifier uses ONE `badgeBonus` value applied uniformly to every badge in its `badges` list. The shipped defaults carry ZERO active `badgeOverrides`. To customize a single badge, an owner uncomments the inactive `badgeOverrides` placeholder block under that modifier (loaded by `CustomSkillsConfig::loadBadgeOverrides` from `CustomSkillsConfig::load()`), where each `{ "badgeKey", value }` entry REPLACES the uniform `badgeBonus` for that one badge -- it never stacks with it. A badge listed in `badges` but absent from an active `badgeOverrides` uses the modifier's `badgeBonus`.
  
**rarityNaming**: a server-config section (`enabled`, `legendaryColor`,
 `exceptionalColor`, six-character RGB hex) that switches item naming to color-only text instead of `(Exceptional)`/`(Legendary)` suffixes (see hook H16 (Part I)). Configured independently of the badge modifiers.  


## Offense

### Critical Chance (`CRITICAL_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 100 bp (1.00%) uniform -- all 60 badges equal, no overrides active by default |
| **Cap (default)** | 6000 (60.00%) |
| **Badges (default)** | 60 achievement badges (see list below) |
| **Config key** | `criticalChance` (special table) |

**Behavior**: Chance for a landed attack to become a custom critical hit.
Checked before repeat-damage tiers. Both the combat roll
(`CustomSkillsCombat::applyDamage`) and the SUI menu read this value from the
config badge map (`CustomSkillsConfig::getBadgeBonuses(CRITICAL_CHANCE)` /
`getModifierTotal`) -- a single source of truth ([CS-3]); it is fully
configurable in `config.lua`.

**History**: before BRIEF-007 the C++ constructor seeded the 12 combat
mastery badges at 300 bp via `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE`
(the ERR-005-era hardcode that survived cleanup). Removed; the config map
is now the only source of badge values, and an empty/missing config yields
zero bonuses rather than silent defaults. The combat spam label feature was
removed 08242026 by owner decision after BRIEF-010.
-- hy3-free (opencode/hy3-free)

**Badges** (all 60 at a uniform 100 bp each via `badgeBonus`; full set =
6000 bp = cap):

| Group | Keys |
|-------|------|
| Accumulation milestones (7) | `count_5` .. `count_125` |
| Exploration milestones (5) | `bdg_exp_10_badges` .. `bdg_exp_45_badges` |
| Combat profession masteries (12) | `combat_*_master` |
| Hero of Tatooine (5) | `poi_rabidbeast`, `poi_prisonbreak`, `poi_twoliars`, `poi_factoryliberation`, `poi_heromark` |
| Warren (2) | `warren_compassion`, `warren_hero` |
| Theme Parks (4) | `bdg_thm_park_jabba/imperial/rebel/nym_badge` |
| Corellian Corvette (9) | `bdg_corvette_*` (9 faction/mission combos) |
| Exploration -- dangerous sites (5) | `exp_tat_tusken_pool`, `exp_tat_krayt_skeleton`, `exp_tat_sarlacc_pit`, `exp_tat_krayt_graveyard`, `exp_dat_sarlacc` |
| Exploration -- Jedi sites (3) | `exp_tat_bens_hut`, `exp_yav_temple_exar_kun`, `exp_dan_jedi_temple` |
| Outdoors masteries (5) | `outdoors_bio_engineer/creaturehandler/ranger/scout/squadleader_master` |
| Science masteries (3) | `science_combatmedic/doctor/medic_master` |


### Critical Multiplier (`CRITICAL_MULTIPLIER`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (multiplier) |
| **Config key** | `modifiers.criticalMultiplier` |
| **Default** | `true` |
| **Badge bonus (default)** | 1250 bp (12.50%/badge) |
| **Cap (default)** | 15000 bp (150% on top of the 150% base = 300% max) |
| **Badges (default)** | 7 accumulation milestones + 5 exploration milestones = 12 total |

**Behavior**: Badge-driven damage multiplier applied on custom critical hits
(promoted from a `criticalChance.multiplier` sub-key to a standalone modifier
in commit c832b1c263). Stacks additively with the base:
```
critDamage = preArmorDamage x (baseMultiplier + badgeBonusTotal) / 10000
```
with `baseMultiplier = criticalChance.multiplier` (default 15000 = 150%);
current bonus cap 15000 -> 300% maximum crit damage
(Part I hook H01).


### Double Attack Chance (`DOUBLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 2000 bp (20.00%) |
| **Cap (default)** | 10000 (100%) |
| **Badges (default)** | Warren (2), Theme Parks (4) = 6 total |

**Behavior**: Sequential upgrade chain start. Rolled first off the landed hit;
success upgrades the attack to tier 2 and enables the Triple roll. First
failed stage ends the chain.
**Consolidated strike (BRIEF-034)**: tiers no longer apply N hits. The tier
multiplies the finalized damage ONCE (`damage *= repeats`) and
`applyVanillaDamage` runs a single time — one bigger hit, same total as the
old repeat chain by design. Armor mitigation is IDENTICAL either way: every
mitigation stage is a multiplicative percentage (force armor :2502, PSG
:2562, standard armor :2591, NPC armor :2475), and percentages commute with
the tier multiplier — verified CombatManager.cpp getArmorReduction chain. Escalated strikes get tiered
flytext + `xN` chat tag (see "Consolidated Strike Presentation").
**Implemented**: yes (BRIEF-015; delivery reworked BRIEF-034).


### Triple Attack Chance (`TRIPLE_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 500 bp (5.00%) uniform |
| **Cap (default)** | 8500 (85%) |
| **Badges (default)** | 12 combat masteries + 5 Hero of Tatooine POI = 17 total |

**Behavior**: Rolled only after Double succeeds in the same chain; success
upgrades to tier 3 and enables the Quad roll. Delivered as a consolidated
strike (damage x3, single application — BRIEF-034).
**Implemented**: yes (BRIEF-015; delivery reworked BRIEF-034).


### Quad Attack Chance (`QUAD_ATTACK_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 300 bp (3.00%) uniform |
| **Cap (default)** | 5100 (51%) |
| **Badges (default)** | 12 combat masteries + 5 Hero of Tatooine POI = 17 total |

**Behavior**: Rolled only after Triple succeeds in the same chain; success
upgrades to tier 4 (highest). Delivered as a consolidated strike
(damage x4, single application — BRIEF-034).
**Implemented**: yes (BRIEF-015; delivery reworked BRIEF-034).


### Consolidated Strike Presentation (`consolidatedStrike`, BRIEF-034)

| Property | Value |
|----------|-------|
| **Config key** | `customSkillsConfig.consolidatedStrike` |
| **Knobs** | `fctEnabled` (bool, default true); `fctScaleStepBp` (int, 1500 = +15.00% flytext scale per tier above base; 1000 bp = 10.00%); `fctCritBonusBp` (int, 2500 = +25.00% extra on crits); `tier2Color`/`tier3Color`/`tier4Color`/`critColor` (hex RRGGBB; defaults FFFF00 yellow / FF9900 orange / FF0000 red / FFD700 gold); `chatTagEnabled` (bool, true) |

**Behavior** (`CustomSkillsCombat.cpp:applyDamage`): after crit roll and the
Double->Triple->Quad chain, damage is multiplied by the tier ONCE and applied
via a single `applyVanillaDamage` call. When repeats > 1 or the hit critted,
a `ShowFlyText` (packets/object/ShowFlyText.h, flags byte 5 as vanilla) is
sent to the attacker using the vanilla hit-location stf entry
(`combat_effects/hit_<location>`) with escalated scale
(1.0 + (tier-1)*step + critBonus) and tier color (crit overlays gold).
Base hits leave vanilla flytext untouched. When repeats > 1 a second
custom-unicode `CombatSpam` line ("x2"/"x3"/"x4", colored byte 11 yellow /
10 red) goes to the ATTACKER only; the normal damage spam is unchanged.
Loader: CustomSkillsConfig.cpp `load()` under the `consolidatedStrike`
table; defaults in `setDefaults()`.


### Armor Penetration (`ARMOR_PENETRATION`)

| Property | Value |
|----------|-------|
| **Unit** | Whole armor levels |
| **Default** | `true` |
| **Badge bonus (default)** | 1 level |
| **Cap (default)** | 3 levels |
| **Badges (default)** | Corellian Corvette (9) = 9 total |

**Behavior**: Reduces defender effective armor by 1 level per point: Heavy -> Medium -> Light -> None. Floor: None. Does not affect weapon Armor Piercing. Applied before native armor-piercing comparison.


## Defense & Equipment

### Defense Cap Increase (`DEFENSE_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Default** | `true` |
| **Badge bonus (default)** | 20 points |
| **Cap (default)** | 100 points |
| **Badges (default)** | Hero of Tatooine only: 5 POI badges x 20 points = 100 total (cap exact) |

**Behavior**: Raises native 125 hard cap on primary/secondary defense calculations. Does not affect SEA/tape above-cap contributions.


### Armor Degradation Reduction (`ARMOR_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 1250 bp (12.50%) |
| **Cap (default)** | 10000 (100%) |
| **Badges (default)** | Talus (4), Rori (4) = 8 total (1250 bp each) |

**Behavior**: Native eligible armor degradation = 100% event chance. This reduces that chance. Failed roll skips condition loss entirely. Covers worn armor, PSG, NPC armor, vehicle armor.


### Weapon Degradation Reduction (`WEAPON_DEGRADE_REDUCTION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 1250 bp (12.50%) |
| **Cap (default)** | 10000 (100%) |
| **Badges (default)** | Dantooine (4), Endor (4) = 8 total (1250 bp each) |

**Behavior**: Native weapon degradation chance (base 5% + powerup adjustment) reduced multiplicatively:
```
effectiveChance = nativeChance x (10000 - reduction) / 10000
```
Condition loss amount unchanged on successful roll.


### SEA Cap Increase (`SEA_CAP_INCREASE`)

| Property | Value |
|----------|-------|
| **Unit** | Points |
| **Default** | `true` |
| **Badge bonus (default)** | 20 points |
| **Cap (default)** | 100 points |
| **Badges (default)** | Tatooine dangerous (4 x 20), Dathomir Sarlacc (20) = 5 total (cap exact) |

**Behavior**: Raises wearable (attachments/tapes) contribution cap above native +25. Does not raise normal defense cap. SEA/tape bonuses retain ability to exceed normal cap.


## Character & Utility

### Movement Speed (`MOVEMENT_SPEED`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 2500 bp (25.00%) |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Naboo (4 x 2500) = 4 total |

**Behavior**: Multiplies final native speed in ALL travel modes (foot, mount, vehicle). Applied once -- not added to mount then reapplied. Synchronized client (H13A) and server (H13B).


### Buff Duration (`BUFF_DURATION`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 2500 bp (25.00%) uniform |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Tatooine easy (3), Yavin IV Woolamander/Blueleaf (2), Science masteries (3), Social masteries (5) = 13 total |

**Behavior**: Increases initial duration of eligible buff families only:
**Included**: Medical, Performance, Food/Drink, Spice-up, positive Jedi/Force
  
**Excluded**: Negative buffs, spice downers, debuffs, states, traps, DoTs, cooldown/control markers, skill-item, innate, Squad Leader, concealment, gallop, vehicle buffs
  

Explicit renewals (H12B) apply current bonus to new native duration. DB reload/internal reschedule preserves stored duration (no re-multiplication).


### Experience Bonus (`EXPERIENCE_MULTIPLIER`)

| Property | Value |
|----------|-------|
| **Unit** | Multiplier (basis points) |
| **Default** | `true` |
| **Badge bonus (default)** | 10000 bp (100.00%) |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Corellia (5 x 10000) = 5 total |

**Behavior**: Multiplies ALL positive XP awards on normal modifier path:
```
finalXP = base x species x buff x local x global x customMultiplier
```
Does not affect: XP deductions, conversion costs, `applyModifiers == false` awards.
Composes multiplicatively: 100 XP x 2x server x 5x character = 1000 XP (10x total).


### Practice Mode Experience Bonus (`PRACTICE_EXPERIENCE_BONUS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 10000 bp (100.00%) |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Tatooine easy (3 x 10000), Yavin IV Woolamander/Blueleaf (2 x 10000), Social mastery (5 x 10000) = 10 total |

**Behavior**: Applies when `createItem == false` (practice mode). Order:
(1) Base crafting XP
  
(2) Core3 native 5% practice increase
  
(3) Custom practice bonus (multiplies)
  
(4) General Experience Bonus (H07) via central award path
  


## Crafting

### Crafting Speed (`CRAFTING_SPEED`)

| Property | Value |
|----------|-------|
| **Unit** | Speed multiplier (basis points) |
| **Default** | `true` |
| **Badge bonus (default)** | 1000 bp (10.00%) |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Crafting mastery (9 x 1000), Doctor mastery (1000) = 10 total |

**Behavior**:
**Personal**: `nativeDuration / multiplier`, clamp >=1s. Uses crafter's current bonus.
  
**Factory**: Snapshots **activator's** multiplier at run start. Persists across logout/restart. Badge changes mid-run don't affect current run. Stop/restart to recapture.
  


### Crafting Amazing Success Chance (`AMAZING_SUCCESS_CHANCE`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (percentage points) |
| **Default** | `true` |
| **Badge bonus (default)** | 500 bp (5.00%) |
| **Cap (default)** | 10000 (100%) |
| **Badges (default)** | Yavin IV Exar Kun (500); Lok: volcano/imp outpost/kimogila skeleton (3 x 500); Dathomir: tarpit/escape pod/misty falls x2/crashed ship/imp prison (6 x 500) = 10 total |

**Behavior**: Adds bp to native Amazing Success probability (assembly & experimentation). Does NOT multiply native chance. Same policy at both roll sites. Clamped to configured cap. Target: max-bonus character reaches >=50% final chance.


### Amazing Results (`AMAZING_RESULTS`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points (strength %) |
| **Default** | `true` |
| **Badge bonus (default)** | 1000 bp (10.00%) |
| **Cap (default)** | 10000 (100%) |
| **Badges (default)** | same 10-badge set as Amazing Success Chance, each at 1000 bp = 10 total |

**Behavior**: On actual `AMAZINGSUCCESS`, for each affected attribute:
```
enhanced = nativeResult + ((schematicCap - nativeResult) x strength / 10000)
```
Raises resource-derived ceiling only enough to retain enhanced value.
**Assembly**: All applicable initial attributes
  
**Experimentation**: Selected row only
  
**Non-amazing**: Fully resource-limited (unchanged)
  
0% = native Core3 behavior; 100% = poor resources can yield perfect attributes
  


## Gathering

### Gathering Quantity (`GATHERING_QUANTITY`)

| Property | Value |
|----------|-------|
| **Unit** | Basis points |
| **Default** | `true` |
| **Badge bonus (default)** | 10000 bp (100.00%) uniform |
| **Cap (default)** | 0 (uncapped) |
| **Badges (default)** | Outdoors masteries (5), Science masteries (3), Tatooine easy (3), Yavin IV Woolamander/Blueleaf (2) = 13 total |

**Behavior**: Bonuses add together, then multiply native quantity:
```
0 badges: 100% (1x)
1 badge:  300% (3x)
2 badges: 500% (5x)
```
Applied AFTER native calculation (forage roll / milk density adjustment). Rounded down to whole unit. Never below native.
**Does not affect**: Discrete loot (food, bait, medical components, rare items, live creatures).


## Maximum Theoretical Totals (All 111 Eligible Badges)

Values are the sum of each modifier's configured badge bonuses (uniform badgeBonus, no active overrides), capped at the configured cap where one applies. `config.lua` is authoritative.

| Modifier | Unit | Per-Badge (config) | Max Badges | Max Total (capped) |
|----------|------|--------------------|------------|--------------------|
| Critical Chance | bp | 100 | 60 | 6000 (60% -- exact) |
| Critical Multiplier | bp | 1250 | 12 | 15000 bonus (300% total) |
| Double Attack | bp | 2000 | 6 | 10000 (100%) |
| Triple Attack | bp | 500 | 17 | 8500 (85% -- exact) |
| Quad Attack | bp | 300 | 17 | 5100 (51% -- exact) |
| Armor Penetration | levels | 1 | 9 | 3 (eff. max 3) |
| Defense Cap | points | 20 | 5 (Hero of Tatooine only) | 100 pts |
| SEA Cap | points | 20 | 5 | 100 pts |
| Armor Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Weapon Degrade Red. | bp | 1250 | 8 | 10000 (100%) |
| Movement Speed | bp | 2500 | 4 | 10000 (100%) |
| Buff Duration | bp | 2500 | 13 | 32500 (325%) |
| Experience Bonus | multiplier bp | 10000 | 5 | 50000 (500%) |
| Practice XP Bonus | bp | 10000 | 10 | 100000 (1000%) |
| Crafting Speed | multiplier bp | 1000 | 10 | 10000 (100%) |
| Amazing Success | bp | 500 | 10 | 5000 (50%) |
| Amazing Results | bp | 1000 | 10 | 10000 (100%) |
| Gathering Quantity | bp | 10000 | 13 | 130000 (+1300%) |


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


##Appendix B -- Badge Catalog

Complete badge catalog from badge_map.iff: inventory, menu organization, eligibility rules, assignments, and integration requirements.


## Badge Inventory Summary

| Core3 Enum Type | Table Strings | Records | Show=1 | Show=0 |
|-----------------|---------------|---------|--------|--------|
| `ACCUMULATION` | `accumulation` | 12 | 7 | 5 |
| `INTEREST` | `interest` | 5 | 5 | 0 |
| `EXPLORATION` | `exploration_easy`, `exploration_dangerous`, `exploration_jedi` | 45 | 45 | 0 |
| `MASTER` | `master` | 43 | 43 | 0 |
| `EVENT` | `events` | 20 | 3 | 17 |
| `CONTENT` | `content` | 15 | 11 | 4 |
| **Total** | | **140** | **124** | **16** |

Indices: contiguous 0-139, no duplicates
  
Columns: index, stable key, music, client category, show flag, type
  
**Always use stable keys** -- never hard-code indices (fragile across TRE changes)
  


## Exact Key Inventory

### Accumulation (12)

| Index | Key | Notes |
|-------|-----|-------|
| 0 | `count_5` | |
| 1 | `count_10` | |
| 2 | `count_25` | |
| 3 | `count_50` | |
| 4 | `count_75` | |
| 5 | `count_100` | |
| 6 | `count_125` | |
| 100 | `bdg_exp_10_badges` | Exploration milestone |
| 101 | `bdg_exp_20_badges` | Exploration milestone |
| 102 | `bdg_exp_30_badges` | Exploration milestone |
| 103 | `bdg_exp_40_badges` | Exploration milestone |
| 104 | `bdg_exp_45_badges` | Exploration milestone |


### Interest (5) -- Hero of Tatooine Progression

| Index | Key |
|-------|-----|
| 7 | `poi_rabidbeast` |
| 8 | `poi_prisonbreak` |
| 9 | `poi_twoliars` |
| 10 | `poi_factoryliberation` |
| 11 | `poi_heromark` |


### Exploration (45)

#### Jedi (3)
| Index | Key |
|-------|-----|
| 12 | `exp_tat_bens_hut` |
| 23 | `exp_yav_temple_exar_kun` |
| 30 | `exp_dan_jedi_temple` |

#### Dangerous (5)
| Index | Key |
|-------|-----|
| 13 | `exp_tat_tusken_pool` |
| 14 | `exp_tat_krayt_skeleton` |
| 16 | `exp_tat_sarlacc_pit` |
| 18 | `exp_tat_krayt_graveyard` |
| 26 | `exp_dat_sarlacc` |

#### Easy (37)
| Index | Key |
|-------|-----|
| 15 | `exp_tat_escape_pod` |
| 17 | `exp_tat_lars_homestead` |
| 19 | `exp_nab_gungan_sacred_place` |
| 20 | `exp_cor_agrilat_swamp` |
| 21 | `exp_yav_temple_woolamander` |
| 22 | `exp_yav_temple_blueleaf` |
| 24 | `exp_lok_volcano` |
| 25 | `exp_dat_tarpit` |
| 27 | `exp_dat_escape_pod` |
| 28 | `exp_dat_misty_falls_1` |
| 29 | `exp_dat_misty_falls_2` |
| 31 | `exp_dan_rebel_base` |
| 75 | `bdg_exp_nab_theed_falls_bottom` |
| 76 | `bdg_exp_nab_deeja_falls_top` |
| 77 | `bdg_exp_nab_amidalas_sandy_beach` |
| 78 | `bdg_exp_cor_rebel_hideout` |
| 79 | `bdg_exp_cor_rogue_corsec_base` |
| 80 | `bdg_exp_cor_tyrena_theater` |
| 81 | `bdg_exp_cor_bela_vistal_fountain` |
| 82 | `bdg_exp_dat_crashed_ship` |
| 83 | `bdg_exp_dat_imp_prison` |
| 84 | `bdg_exp_dan_dantari_village1` |
| 85 | `bdg_exp_dan_dantari_village2` |
| 86 | `bdg_exp_end_ewok_tree_village` |
| 87 | `bdg_exp_end_ewok_lake_village` |
| 88 | `bdg_exp_end_dulok_village` |
| 89 | `bdg_exp_end_imp_outpost` |
| 90 | `bdg_exp_tal_creature_village` |
| 91 | `bdg_exp_tal_imp_base` |
| 92 | `bdg_exp_tal_imp_vs_reb_battle` |
| 93 | `bdg_exp_tal_aqualish_cave` |
| 94 | `bdg_exp_ror_kobala_spice_mine` |
| 95 | `bdg_exp_ror_rebel_outpost` |
| 96 | `bdg_exp_ror_imp_camp` |
| 97 | `bdg_exp_ror_imp_hyperdrive_fac` |
| 98 | `bdg_exp_lok_imp_outpost` |
| 99 | `bdg_exp_lok_kimogila_skeleton` |


### Master (43)

#### Ground Professions (33)
| Index | Key |
|-------|-----|
| 42 | `combat_1hsword_master` |
| 43 | `combat_2hsword_master` |
| 44 | `combat_bountyhunter_master` |
| 45 | `combat_brawler_master` |
| 46 | `combat_carbine_master` |
| 47 | `combat_commando_master` |
| 48 | `combat_marksman_master` |
| 49 | `combat_pistol_master` |
| 50 | `combat_polearm_master` |
| 51 | `combat_rifleman_master` |
| 52 | `combat_smuggler_master` |
| 53 | `combat_unarmed_master` |
| 54 | `crafting_architect_master` |
| 55 | `crafting_armorsmith_master` |
| 56 | `crafting_artisan_master` |
| 57 | `crafting_chef_master` |
| 58 | `crafting_droidengineer_master` |
| 59 | `crafting_merchant_master` |
| 60 | `crafting_tailor_master` |
| 61 | `crafting_weaponsmith_master` |
| 62 | `outdoors_bio_engineer_master` |
| 63 | `outdoors_creaturehandler_master` |
| 64 | `outdoors_ranger_master` |
| 65 | `outdoors_scout_master` |
| 66 | `outdoors_squadleader_master` |
| 67 | `science_combatmedic_master` |
| 68 | `science_doctor_master` |
| 69 | `science_medic_master` |
| 70 | `social_dancer_master` |
| 71 | `social_entertainer_master` |
| 72 | `social_imagedesigner_master` |
| 73 | `social_musician_master` |
| 74 | `social_politician_master` |

#### Space Professions (10)
| Index | Key |
|-------|-----|
| 129 | `crafting_shipwright` |
| 130 | `pilot_rebel_navy_naboo` |
| 131 | `pilot_rebel_navy_corellia` |
| 132 | `pilot_rebel_navy_tatooine` |
| 133 | `pilot_imperial_navy_naboo` |
| 134 | `pilot_imperial_navy_corellia` |
| 135 | `pilot_imperial_navy_tatooine` |
| 136 | `pilot_neutral_naboo` |
| 137 | `pilot_neutral_corellia` |
| 138 | `pilot_neutral_tatooine` |


### Events (20)

| Index | Key | Show | Notes |
|-------|-----|------|-------|
| 32 | `event_project_dead_eye_1` | 0 | Project Dead Eye |
| 33 | `acc_good_samaritan` | 0 | Admin accolade |
| 34 | `acc_fascinating_background` | 0 | Admin accolade |
| 35 | `acc_brave_soldier` | 0 | Admin accolade |
| 36 | `acc_interesting_personage` | 0 | Admin accolade |
| 37 | `acc_professional_demeanor` | 0 | Admin accolade |
| 40 | `event_coa2_rebel` | 0 | Cries of Alderaan 2 |
| 41 | `event_coa2_imperial` | 0 | Cries of Alderaan 2 |
| 109 | `event_coa3_rebel` | 0 | Cries of Alderaan 3 |
| 110 | `event_coa3_imperial` | 0 | Cries of Alderaan 3 |
| 111 | `bdg_library_trivia` | 1 | Librarian |
| 121 | `bdg_racing_agrilat_swamp` | 1 | Racing |
| 122 | `bdg_racing_keren_city` | 1 | Racing |
| 123 | `bdg_racing_mos_espa` | 1 | Racing |
| 124 | `bdg_accolade_live_event` | 0 | Live event |
| 125 | `bdg_racing_lok_marathon` | 0 | Lok marathon |
| 126 | `bdg_racing_narmle_memorial` | 0 | Narmle memorial |
| 127 | `bdg_racing_nashal_river` | 0 | Nashal river |
| 128 | `destroy_deathstar` | 0 | Death Star event |
| 139 | `bdg_accolade_home_show` | 0 | Home show |


### Content (15)

| Index | Key | Notes |
|-------|-----|-------|
| 38 | `warren_compassion` | Warren |
| 39 | `warren_hero` | Warren |
| 105 | `bdg_thm_park_jabba_badge` | Theme Park: Jabba |
| 106 | `bdg_thm_park_imperial_badge` | Theme Park: Imperial |
| 107 | `bdg_thm_park_rebel_badge` | Theme Park: Rebel |
| 108 | `bdg_thm_park_nym_badge` | Theme Park: Nym |
| 112 | `bdg_corvette_imp_destroy` | Corvette: Imperial Destroy |
| 113 | `bdg_corvette_imp_rescue` | Corvette: Imperial Rescue |
| 114 | `bdg_corvette_imp_assassin` | Corvette: Imperial Assassin |
| 115 | `bdg_corvette_neutral_destroy` | Corvette: Neutral Destroy |
| 116 | `bdg_corvette_neutral_rescue` | Corvette: Neutral Rescue |
| 117 | `bdg_corvette_neutral_assassin` | Corvette: Neutral Assassin |
| 118 | `bdg_corvette_reb_destroy` | Corvette: Rebel Destroy |
| 119 | `bdg_corvette_reb_rescue` | Corvette: Rebel Rescue |
| 120 | `bdg_corvette_reb_assassin` | Corvette: Rebel Assassin |


## Menu Organization (Player-Facing Groups)

Core3's 6 internal types -> 4 menu groups:

```
Badges
  Milestone Badges          (Accumulation: 7 count_* + 5 bdg_exp_*_badges)
  Exploration
    Milestone Exploration   (bdg_exp_10/20/30/40/45_badges)
    Tatooine (7)            -> 3 easy, 4 dangerous
    Naboo (4)
    Corellia (5)
    Yavin IV (3)
    Lok (3)
    Dathomir (7)            -> 6 easy, 1 dangerous
    Dantooine (4)
    Endor (4)
    Talus (4)
    Rori (4)
  Profession
    Combat (12)
    Crafting (9)            -> 8 ground + Shipwright
    Outdoors (5)
    Science (3)
    Social (5)
    Pilot (9)               -> EXCLUDED (no JTL)
  Quest
    Hero of Tatooine (5)    -> Interest: poi_*
    Warren (2)              -> Content: warren_*
    Theme Parks (4)         -> Content: bdg_thm_park_*
    Corellian Corvette (9)  -> Content: bdg_corvette_*
  Event
    Cries of Alderaan (5)   -> Events: event_coa2/3_*
    Accolades (7)           -> EXCLUDED (admin)
    Librarian (1)           -> bdg_library_trivia
    Racing (6)              -> 3 visible, 3 EXCLUDED
    Death Star (1)          -> EXCLUDED
```


## Eligibility for Bonuses

| Status | Count | Badges |
|--------|-------|--------|
| **Eligible** | 111 | All except pilot/JTL + admin/event exclusions |
| **Excluded: Pilot/JTL** | 9 | All 9 `pilot_*_master` badges |
| **Excluded: Admin/Event** | 20 | All accolades, live events, most racing, Death Star, Cries of Alderaan |

**Total badge_map.iff**: 140 = 111 eligible + 9 pilot + 20 excluded


## Excluded Badge List (Must Not Grant Modifiers)

### Pilot / JTL (9) -- No bonuses until JTL available
```
pilot_imperial_navy_corellia
pilot_imperial_navy_naboo
pilot_imperial_navy_tatooine
pilot_neutral_corellia
pilot_neutral_naboo
pilot_neutral_tatooine
pilot_rebel_navy_corellia
pilot_rebel_navy_naboo
pilot_rebel_navy_tatooine
```

### Admin / Special Event (20) -- Not part of normal progression
```
acc_brave_soldier
acc_fascinating_background
acc_good_samaritan
acc_interesting_personage
acc_professional_demeanor
bdg_accolade_home_show
bdg_accolade_live_event
bdg_library_trivia
bdg_racing_agrilat_swamp
bdg_racing_keren_city
bdg_racing_lok_marathon
bdg_racing_mos_espa
bdg_racing_narmle_memorial
bdg_racing_nashal_river
destroy_deathstar
event_coa2_imperial
event_coa2_rebel
event_coa3_imperial
event_coa3_rebel
event_project_dead_eye_1
```


## Visibility Policy

| Rule | Description |
|------|-------------|
| `show = 1` | May appear in normal badge browsing |
| `show = 0` | Hidden from general browser by default |
| Hidden + enabled in config | Still shown in `/customskills` modifier breakdown (so totals stay explained) |
| Visibility != ownership | `show` flag is metadata only; does not determine if badge is owned |


## Badge-to-Modifier Assignments (menu grouping in Part I; gameplay assignments below)

### Milestone Badges (12) -- +12.5% Crit Multiplier, +1% Crit Chance each
`count_5` through `count_125` (7)
  
`bdg_exp_10/20/30/40/45_badges` (5)
  

### Exploration Planets (45)

| Planet | Badges | Bonus (per badge) |
|--------|--------|-------------------|
| **Tatooine easy** (3) | `exp_tat_escape_pod`, `exp_tat_lars_homestead`, `exp_tat_bens_hut` | +100% Practice XP, +25% Buff Duration, +25% Gathering |
| **Tatooine dangerous** (4) | `exp_tat_tusken_pool`, `exp_tat_krayt_skeleton`, `exp_tat_sarlacc_pit`, `exp_tat_krayt_graveyard` | +15 SEA Cap |
| **Naboo** (4) | `exp_nab_gungan_sacred_place`, `bdg_exp_nab_theed_falls_bottom`, `bdg_exp_nab_deeja_falls_top`, `bdg_exp_nab_amidalas_sandy_beach` | +25% Movement Speed (100% max) |
| **Corellia** (5) | `exp_cor_agrilat_swamp`, `bdg_exp_cor_rebel_hideout`, `bdg_exp_cor_rogue_corsec_base`, `bdg_exp_cor_tyrena_theater`, `bdg_exp_cor_bela_vistal_fountain` | +100% Experience (500% max) |
| **Yavin IV** (3) | `exp_yav_temple_exar_kun`: +5 Def Cap, +5% Amazing Success, +10% Amazing Results | |
| | `exp_yav_temple_woolamander`: +100% Practice XP, +25% Gathering, +25% Buff Duration | |
| | `exp_yav_temple_blueleaf`: +100% Practice XP, +25% Gathering, +25% Buff Duration | |
| **Lok** (3) | All: +5 Def Cap, +5% Amazing Success, +10% Amazing Results | |
| **Dathomir easy** (6) | `exp_dat_tarpit`, `exp_dat_escape_pod`, `exp_dat_misty_falls_1`, `exp_dat_misty_falls_2`, `bdg_exp_dat_crashed_ship`, `bdg_exp_dat_imp_prison` | +5 Def Cap, +5% Amazing Success, +10% Amazing Results |
| **Dathomir dangerous** (1) | `exp_dat_sarlacc` | +15 SEA Cap |
| **Dantooine** (4) | All: +12.5% Weapon Degradation Reduction | |
| **Endor** (4) | All: +12.5% Weapon Degradation Reduction | |
| **Talus** (4) | All: +12.5% Armor Degradation Reduction | |
| **Rori** (4) | All: +12.5% Armor Degradation Reduction | |

### Profession Mastery (43)

| Family | Badges | Bonus |
|--------|--------|-------|
| **Combat** (12) | All 12 ground combat mastery | +4% Crit Chance, +2% Triple, +2% Quad (BH: +3%/+3%) |
| **Crafting** (9) | 8 ground + Shipwright | +10% Crafting Speed |
| **Outdoors** (5) | All 5 | +115% Gathering Quantity |
| **Science** (3) | All 3 | +100% Gathering, +25% Buff Duration (Doctor: +10% Crafting Speed) |
| **Social** (5) | All 5 | +20% Buff Duration, +100% Practice XP |
| **Pilot** (9) | All 9 | EXCLUDED (no JTL) |

### Quest (20)

| Line | Badges | Bonus |
|------|--------|-------|
| **Hero of Tatooine** (5) | `poi_rabidbeast`, `poi_prisonbreak`, `poi_twoliars`, `poi_factoryliberation`, `poi_heromark` | +10% Triple, +5% Quad, +10 Def Cap |
| **Warren** (2) | `warren_compassion`, `warren_hero` | +20% Double Attack |
| **Theme Parks** (4) | `bdg_thm_park_jabba/imperial/rebel/nym_badge` | +20% Double Attack |
| **Corellian Corvette** (9) | All 9 faction/mission combos | +1 Armor Penetration |

### Event

| Group | Badges | Bonus |
|-------|--------|-------|
| **Cries of Alderaan** (5) | `event_coa2/3_imperial/rebel`, `event_project_dead_eye_1` | (configured per server) |
| **Accolades** (7) | All `acc_*` + `bdg_accolade_*` | EXCLUDED |
| **Librarian** (1) | `bdg_library_trivia` | (configured per server) |
| **Racing** (6) | 3 visible: `bdg_racing_agrilat/keren/mos_espa` | (configured per server) |
| | 3 excluded: `bdg_racing_lok/narmle/nashal` | EXCLUDED |
| **Death Star** (1) | `destroy_deathstar` | EXCLUDED |


## Runtime Resolution (reference)

```cpp
// CustomSkillsModifiers::getModifierTotal()
const BadgeList* badgeList = BadgeList::instance();
const Badge* badge = badgeList->get(badgeKey);  // stable key -> index
if (badge && ghost->hasBadge(badge->getIndex()))
    total += bonus;
```
## Client Category Mapping (Reference)

| Client Category | Core3 Types Included | Records |
|-----------------|---------------------|---------|
| 0 | Accumulation (12) | 7 shown, 5 hidden |
| 1 | Interest (5) + some Content | 6 shown |
| 2 | Exploration (45) + most Content | 54 shown |
| 3 | Master (43) | 43 shown |
| 4 | Event (14) | 3 shown, 11 hidden |
| 5 | Event (11) | 11 shown |

*Client category is separate from Core3 type enum -- used for client-side badge browser only.*

## SWGEMU Options Viewer (BRIEF-026)

`CustomSkillsMenu::appendSwgemuOptions` (CustomSkillsMenu.cpp) renders a
curated read-only list of ~20 notable Core3 options on the
Server Config > SWGEMU Options page. Registry is a static `SwgemuOpt[]`
table in that function: `{label, configKey, type('b'/'i'/'s'), restart}`.
To add an option: append a row (defaults come from CONFIG_OPTIONS.md);
secrets/credentials are excluded by policy -- never add DBPass, DBSecret,
APIToken, or similar. Bools render green ENABLED / red DISABLED; rows with
restart=true show a gray "(restart required)" suffix. Values are read live
via ConfigManager::instance() getters, so [dyn] options reflect hot-reloads.

## Hidden Config Options (server-configurable, undocumented keys)

Full inventory of every option ConfigManager reads lives in
[CONFIG_OPTIONS.md](CONFIG_OPTIONS.md) (BRIEF-023): 159 keys with type,
default, consumer file:line, and read-behavior tags ([startup] vs [dyn]
hot-reload). Highlights: the owner seed set (PlayerManager
DisableGroupVisibility/WipeFillingOnClone/GalaxyWideGrouping/AdvancedWaypoints,
PlayerCreationManager.MaxCharactersPerGalaxy, JTL.JTLEnabled) are all live
reads absent from shipped config.lua.

Gotchas from the inventory:
- `PlayerManager.accountVictimList` is read WITHOUT the `Core3.` prefix
  (PlayerManagerImplementation.cpp:6914) -- top-level table key.
- `Core3.JTL.LaunchFromDevice` is a dead read (getter exists, no consumers).
- ConfigManager hot-reloads [dyn] options on configVersion bump; [startup]
  options need a restart.
- Per-account overlay: getters invoked with an accountID resolve
  `Core3.AccountFlags.<accountID>.<name>` first (ConfigManager.h:198-207).

Adding a new entry to the menu viewer (BRIEF-026): register it in the
SWGEMU Options registry with {label, configKey, type, group, restartFlag};
exclude secrets per ERR-014/BRIEF-026 rules.

## Config Overlay Mechanism (research, BRIEF-027)

ConfigManager::loadConfigData() (src/conf/ConfigManager.cpp:23-93) loads
exactly TWO lua files into ONE shared lua_State: conf/config.lua
(hard-required, :33) then optional conf/config-local.lua (:38-47, loaded only
if present). No third/include file exists in stock code. Merge rule is
LAST-ASSIGNMENT-WINS: both chunks mutate the same global `Core3` table before
a single parse pass (`clearConfigData()` at :55 wipes the map;
parseConfigData("Core3") at :60 flattens it to dotted keys). Every key replace
bumps configVersion (updateItem, :640), which auto-refreshes all cached
getters (ConfigManager.h:278-355 pattern). Hot-reload re-runs loadConfigData()
in full -- ServerCore::processConfig() at ServerCore.cpp:1128-1131 -- so any
additional file loaded there re-applies on every reload too. A mod overlay
file therefore needs only a `File::setReadOnly()` + `lua.runFile()` block
mirroring :38-47; Lua screenplays CANNOT set ConfigManager keys at runtime
(DirectorManager registers read mirrors only: isJtlEnabled()
DirectorManager.cpp:5107-5108, isCovertOvertSystem() :5016 -- no setter
binding). The mod's own CustomSkillsConfig::load() uses a private throwaway
Lua state (CustomSkillsConfig.cpp:110-127) and cannot reach the config state.
Recommended precedence for a mod-managed conf/mod-overrides.lua: ship fully
commented out (sparse override = operator values always win); mod-forced lines
loaded last would otherwise beat config-local by last-write-wins.

### IMPLEMENTED (BRIEF-027-IMPL, 08252026)

Route A is implemented. ConfigManager.cpp loadConfigData() now runs THREE
files in one shared lua_State, in this order:

1. conf/config.lua (hard-required) -- loadConfigData() :33
2. conf/config-local.lua (optional, File::setReadOnly guard) -- :38-47
3. conf/mod-overrides.lua (optional, same setReadOnly pattern) -- :49-62;
   logs "Loaded conf/mod-overrides.lua" / "Did not find
   conf/mod-overrides.lua" at INFO.

All three run BEFORE clearConfigData()/parseConfigData (:55+), inside the lua
lifetime of loadConfigData(), so a single parse pass sees the merged Core3
table and LAST-WRITE-WINS holds across the whole chain. processConfig()
(ServerCore.cpp:1128-1131) re-runs loadConfigData() on hot-reload, so the
overlay re-applies every reload too.

Precedence rules:
- Commented-out mod-overrides lines change nothing; config.lua /
  config-local.lua operator values always win while the file stays sparse.
- An UNCOMMENTED line in mod-overrides.lua wins over everything loaded
  before it (both stock files), by last-write-wins.
- To regain control, re-comment or delete the line and hot-reload; the value
  reverts to the operator's config-local.lua setting.
- Secrets/dead-reads/naming-trap keys are excluded from the shipped example
  per ERR-014/BRIEF-026 policy.

Shipped template: bin/conf/mod-overrides.lua.example -- fully commented,
self-documenting (effect/type/default/dyn-vs-restart tag per option),
~40 curated class-(b) options covering the owner seed set plus notable
gameplay toggles. Copy to bin/conf/mod-overrides.lua to activate.

## Floating Combat Text (ShowFlyText)
Packet: packets/object/ShowFlyText.h -- ObjectControllerMessage 0x1B/0x1BD;
payload = long targetID, ascii stf FILE, int spacer, ascii stf ENTRY, float
SCALE ("1.0 broadcasted, 0 none", :20), byte R/G/B, byte FLAGS hardcoded 5
(:31; comment :25-30 lists 0x1 on-target-only, 0x2 chat?, 0x4 unknown).
Wrapper: SceneObjectImplementation.cpp:1862 showFlyText(file,aux,r,g,b,
isPrivate) -- public broadcasts at scale 1.0; private sends scale 0 to self.
IDL: SceneObject.idl:904 (Lua-callable).
Usage inventory: every caller in src uses default scale; only explicit
scale is CombatManager::showHitLocationFlyText (:2909-2943) -- hit_head
blue/hit_body+arms red/legs green all 1.0f, sent to attacker ONLY (:2942).
doMiss/doCounterAttack/doBlock/doDodge white/green via wrapper
(CombatManager.cpp:2865-2906). Buff flytext proves dynamic stf strings work
(BuffImplementation.cpp:121,168). No flag variation anywhere in tree.
Verdicts: SIZE YES (server float per message; slider interaction needs live
test); COLOR YES (RGB fully server-controlled); TEXT PARTIAL (stf
file+entry only -- no raw text/digits in packet; custom tables need client
stf patch); POSITION NO (no offset field); EFFECTS NO (no animation field;
client-mod territory).
Hook point: CustomSkillsCombat::applyDamage already owns post-mitigation
totals and is a CombatManager friend (CombatManager.h:21,24; delegation
CombatManager.cpp:1419-1421) -- build ShowFlyText directly there for full
scale/color control. showHitLocationFlyText is non-virtual const
(CombatManager.h:273); replicate rather than fork.
NOT feasible server-side: literal damage digits, positional offsets,
shake/motion effects, resizing client-generated numeric damage flytext
(that path is CombatSpam-driven client-side; NGE-era opcode 0x45A
ShowCombatText absent from this tree).
Open tests: rendered size formula (slider x float?), flag 0x2 semantics,
whether scale 0 means private render or invisible.

## Menu Performance Profile (R6.9, BRIEF-033)

Per-open cost of CustomSkillsMenu::open (CustomSkillsMenu.cpp:49) -- modeled,
not profiled; full model in docs/briefs/_033_findings.md.

Call graph: CustomSkillsCommand.h:30 -> open() -> getPromptText
(CustomSkillsMenu.cpp:288; 18 summary lines via 18x getModifierTotal, or 20
live ConfigManager reads on the SWGEMU_OPTIONS page via appendSwgemuOptions
:242-286) -> addPageItems (:363). Leaf badge rows do 1 BadgeList::get +
1 hasBadge + 1 StringIdManager lookup (:76) + an 18-type VectorMap scan per
row (:83-87); bonus pages add a 17-type cross-modifier re-scan per row
(:175-181). generateMessage serializes every row into one SuiCreatePageMessage
(SuiListBoxImplementation.cpp:14); each row deploys one SuiListBoxMenuItem
(:102-105).

Costs: ConfigManager get* = ReadLocker + hash find (~150-400 ns;
ConfigManager.cpp:479-499, shared readers don't contend). BadgeList::get ~100 ns.
StringId lookup ~0.5-2 us. No DB hits and no Lua per open -- badges are in-memory
(hasBadge), bonuses pre-parsed once at startup (CustomSkillsConfig.cpp:110).

Headline numbers:
- Typical open (<=12 rows): ~60-150 us server CPU; worst page (SWGEMU options)
  ~80-180 us. Whole-tree hypothetical (140 badge rows counted at
  CustomSkillsMenu.cpp:16-42): ~1.5-3 ms.
- One menu open ~= 2-4 unobserved melee swings (CombatManager::doCombatAction
  CombatManager.cpp:213 runs accuracy/mitigation + CombatAction broadcast to all
  nearby observers, :284-285/:693-721), ~= 0.5-1 swing in a crowded fight.
- 1000 simultaneous opens ~= 100-150 ms total CPU across the task worker pool;
  NEGLIGIBLE. Menu work shares workers with combat ticks but would need
  >10k opens/sec to threaten starvation.

Scaling cliffs (500/1000/1500 rows): server build time stays trivial (~20 ms
at 1500); the FIRST cliff is client-side listbox rendering around 1000-1500
rows in one SUI page -- SuiListBoxImplementation imposes no row cap, so the
server ships it intact and the client chokes. Keep pages lazy/paged (already
true) with a soft cap ~250 rows/page if content grows.

## Crafting Session Lifecycle (R6.9, BRIEF-035)

Durable facts about inventory-tool crafting (research only; no code changed).

**Objects**: one `CraftingSession`
(`objects/player/sessions/crafting/CraftingSessionImplementation.cpp`) per
craft, created by `requestcraftingsession` (RequestCraftingSessionCommand.h:120).
Tool states: READY/WORKING/FINISHED (CraftingTool.idl:41-43); tool holds max
one prototype (CraftingToolImplementation.cpp:56).

**State machine** (`state` field): 1 = schematic list -> 2 = resource screen
(`selectDraftSchematic`) -> 3/4 = assembly done w/ or w/o experimentation
(`initialAssembly`, station + exp-rows/factory check at :841) -> 5 =
customization done (`customization()` :1330) -> 6 final. Every transition is a
client queue command / ObjectController packet (0x106 experiment, 0x107/0x108
ingredient add/remove, 0x15A customization; registered ZonePacketHandler.cpp:207-210;
stage commands NextCraftingStage/CreatePrototype/SelectDraftSchematic).

**Session end is unconditional**: `createPrototype()` (:1372) awards XP,
schedules `CreateObjectTask` (timer = complexity*2 through
CustomSkillsCrafting::getPersonalCraftingDuration), then always calls
`cancelSession()` (:1415). Resources are consumed at assembly and never
recovered on success (:952; critical failure re-slots via
`synchronizedUIListen` :959). `CreateObjectTask` transfers the prototype to
inventory and sets tool READY, or parks it FINISHED if inventory full.

**Repeat-craft implication**: sessions are strictly one-shot; "repeat" must be
an assisted pre-fill of a fresh session (snapshot draft-schematic CRC +
per-slot resource names + exp row/point pairs + customization data), not a
server-side loop. Factory mass-production precedent:
FactoryObjectImplementation::createNewObject() loops on persisted
ManufactureSchematic + stored prototype with manufactureLimit countdown.
Practice XP is xp*1.05 with identical resource cost -- repeats don't change
XP-per-resource ratio but do remove inter-craft time when Crafting Speed is
stacked (clamp >=1s), so auto-repeat should exclude practice mode.

## Repeat-Craft Assisted Pre-Fill (BRIEF-036)

Builds on the BRIEF-035 lifecycle findings above: sessions are one-shot
(`createPrototype` -> `cancelSession`), so "repeat" is an assisted pre-fill of
a FRESH session -- never a server-side loop and never an auto-created item.

**Command**: `/repeatcraft` (`RepeatCraftCommand.h`, registered in
CommandConfigManager.cpp `registerSpecialCommands` + CommandConfigManager3.cpp,
scripted in `bin/scripts/commands/repeatCraft.lua`). Target a crafting tool or
run bare (first inventory tool holding a recipe). Requires the feature enabled.

**Snapshot ("RepeatRecipe") storage choice**: stored ON THE CRAFTING TOOL via
TangibleObject's existing persistent `luaStringData` VectorMap<string,string>
(TangibleObject.idl:49, `setLuaStringData`/`getLuaStringData`/`deleteLuaStringData`)
under `cs36.*` keys -- ZERO IDL changes, no new object, survives server restart,
one recipe per tool, overwritten each successful craft. Keys:
`schematicCrc` (draft schematic client CRC), `slotCount`,
`slot.<i>.type` (resource spawn name, or template string + "#" + serial for
component slots), `slot.<i>.qty`, `exp` (experimentation "row points ..." string).
Written by `CustomSkillsCrafting::storeRepeatRecipe` from the success branch of
`CraftingSessionImplementation::createPrototype` (~:1408; gated by
`repeatAllowPractice` when the craft was practice). The session records its last
experimentation allocation in the transient `lastExpAttempt` field.

**Pre-fill flow** (`CustomSkillsCrafting::doRepeatCraft`):
1. Resolve tool + snapshot; reject FINISHED/BUSY tools with vanilla messages.
2. Cancel any active CRAFTING session (mirrors RequestCraftingSessionCommand),
   create a NEW CraftingSession, `initializeSession(tool, nearbyStation)`.
3. Re-resolve the snapshotted schematic by client CRC inside the freshly
   filtered `currentSchematicList` (new IDL accessors
   `getCurrentSchematicListSize`/`getCurrentSchematic`). Missing => discard
   snapshot + notice + cancelSession.
4. `selectDraftSchematic(index)`, then validate slot count/quantities against
   the snapshot; mismatch => discard snapshot + notice.
5. Auto-fill each empty slot from live inventory via `session->addIngredient`
   (resource slots match exact spawn name; component slots match template +
   serial). Any missing/insufficient resource leaves that slot EMPTY with a
   system message naming the resource.
6. STOPS at state 2 (resource screen): the normal crafting window stays open
   pre-filled; the player assembles/experiments/customizes/creates as usual.
   The prototype is NEVER created automatically.

**Window flow note (same-window requirement)**: the repeat starts a fresh
session while the old one is already closed (vanilla always closes it after
createPrototype), so from the player's view the crafting window re-opens
pre-filled at the resource screen rather than staying pixel-identical on
screen; the stock client has no server-side way to keep one window instance
alive across sessions. This is the closest achievable to "craft, hit repeat" --
no tool/inventory hopping is involved.

**Config** (`customSkillsConfig.repeatCraft`, loaded in CustomSkillsConfig.cpp):
`repeatEnabled` bool DEFAULT FALSE (whole feature off until operator opts in);
`repeatAllowPractice` bool DEFAULT TRUE (practice crafts refresh the recipe;
matches vanilla per owner directive). NO rate caps, NO anti-farming knobs --
owner decision.

## Mod Integration Gotchas (R6.9, 08252026 live-test + build lessons)

CombatManager has TWO applyDamage overloads: player-vs-CreatureObject
(:490) delegates to CustomSkillsCombat::applyDamage; player-vs-TangibleObject
(lairs, turrets -- :377 -> :1630) does NOT. Tier/escalation logic must cover
both paths when porting combat behavior.

Vanilla hit-location flytext (showHitLocationFlyText :2936, called :1614)
renders scale 1.0 pool colors on every hit at the same anchor as any mod
flytext; the client stacks them and vanilla wins visually. Escalated text
must suppress the vanilla call or render alone to be visible.
Broadcast via defender's observers (vanilla sends attacker-only).

QueueCommand result codes GENERALERROR=1 / SUCCESS=0 are static members of
QueueCommand (commands/QueueCommand.h:60-61), not globals -- code outside a
QueueCommand subclass must include the header and qualify
QueueCommand::GENERALERROR.

Never forward-declare IDL-generated classes (ManufactureSchematic,
CraftingTool, ...) in the global namespace in mod headers: IDL headers
declare them inside server::zone::objects::* namespaces; both visible in one
TU yields ambiguous-type build errors. Include the real header instead.
