# Custom Skills -- Implementation Guide

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Complete hook inventory, service architecture, and build/deployment process for developers.

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation

---

## Hook Inventory (H01-H16)

| ID | Module Call | Modifiers | Core3 Entry Point | Status |
|----|-------------|-----------|-------------------|--------|
| **H01** | `CustomSkillsCombat::modifyLandedDamage(context)` | Critical Chance, Critical Multiplier, Double/Triple/Quad Attack | `CombatManager::applyDamage` wrapper | [OK] Implemented |
| **H02** | -- | None | -- | [X] Rejected (repeat-damage in H01) |
| **H03** | `CustomSkillsCombat::getEffectiveArmorRating(attacker, nativeRating)` | Armor Penetration | `CombatManager::getArmorReduction` (after armor layer known, before armor-piercing) | [OK] Implemented |
| **H04** | `CustomSkillsCombat::getDefenseCap(defender, nativeCap)` | Defense Cap Increase | Shared cap in `CombatManager` (primary & secondary) | [OK] Implemented |
| **H05** | `CustomSkillsDurability::shouldDegradeArmor(defender, nativeChance)` | Armor Degradation Reduction | Before armor/PSG `inflictDamage` in combat mitigation | [OK] Implemented |
| **H06** | `CustomSkillsDurability::shouldDegradeWeapon(user, nativeChance)` | Weapon Degradation Reduction | `WeaponObjectImplementation::decay` at chance roll | [OK] Implemented |
| **H07** | `CustomSkillsProgression::getExperienceMultiplier(player, xpType)` | Experience Bonus | `PlayerManagerImplementation::awardExperience` (inside `applyModifiers` calc) | [OK] Implemented |
| **H08** | `CustomSkillsCrafting::modifyPracticeExperience(crafter, nativeAmount)` | Practice XP Bonus | `CraftingSessionImplementation::createPrototype` (`createItem==false` branch) | [OK] Implemented |
| **H09A** | `CustomSkillsCrafting::getPersonalCraftingDuration(crafter, nativeSeconds)` | Crafting Speed | `CraftingSessionImplementation::startCreationTasks` (before scheduling) | [OK] Implemented |
| **H09B** | `CustomSkillsCrafting::getFactoryProductionDuration(activator, factory, schematic, nativeSeconds)` | Crafting Speed | `FactoryObjectImplementation::startFactory` (after native timer calc, before first task) | [OK] Implemented |
| **H10** | `CustomSkillsCrafting::getAmazingChanceBonus(crafter, phase)` | Amazing Success Chance | `SharedLabratory::calculateAssemblySuccess` & `CraftingManagerImplementation::calculateExperimentationSuccess` | [OK] Implemented |
| **H11** | `CustomSkillsCrafting::applyAmazingResults(crafter, phase, attrs, strength)` | Amazing Results | Resource/genetic lab assembly & experimentation (only on `AMAZINGSUCCESS`) | [OK] Implemented |
| **H12A** | `CustomSkillsBuffs::modifyInitialDuration(recipient, buff, nativeDuration)` | Buff Duration | `BuffImplementation::activate` (before `scheduleBuffEvent`) | [OK] Implemented |
| **H12B** | `CustomSkillsBuffs::modifyRenewedDuration(recipient, buff, nativeDuration)` | Buff Duration | `CreatureObjectImplementation::renewBuff` (before assigning renewal) | [OK] Implemented |
| **H13A** | `CustomSkillsMovement::getEffectiveRunSpeed(character, nativeSpeed, travelMode)` | Movement Speed | Client packet speed (`CreatureObjectMessage4`/`DeltaMessage4`) | [OK] Implemented |
| **H13B** | `CustomSkillsMovement::getEffectiveAllowedSpeed(character, nativeSpeed, travelMode)` | Movement Speed | `PlayerManagerImplementation::checkPlayerSpeedTest` (server validation) | [OK] Implemented |
| **H14A** | `CustomSkillsSkillMods::getEffectiveSkillMod(character, modName, nativeTotal, rawWearable)` | SEA Cap Increase | `CreatureObjectImplementation::getSkillMod` (after `SkillModList` aggregation) | [OK] Implemented |
| **H14B** | `CustomSkillsSkillMods::getEffectiveVisibleSkillMod(character, modName, nativeEntry, rawWearable)` | SEA Cap Increase | Character skill-mod delta/baseline prep & badge-cap refresh | [OK] Implemented |
| **H15A** | `CustomSkillsGathering::modifyForageQuantity(player, resource, nativeQuantity)` | Gathering Quantity | `ForageManagerImplementation::forageGiveResource` (after roll, before `harvestResourceToPlayer`) | [OK] Implemented |
| **H15B** | `CustomSkillsGathering::modifyMilkQuantity(player, creature, resource, nativeQuantity)` | Gathering Quantity | `MilkCreatureTask::giveMilkToPlayer` (after density adjustment, before harvest) | [OK] Implemented |
| **H16** | `CustomSkillsModifiers::applyRarityNaming(object, ...)` | Server Config: Rarity Naming | `LootManagerImplementation::setCustomObjectName` (after base name, before suffix) | [OK] Implemented |

---

## Consolidation Decisions

| Decision | Rationale |
|----------|-----------|
| H02 rejected | Double/Triple/Quad are damage repeats via H01, not complete attack executions |
| H01/H03/H04 separate hooks | Damage, armor selection, defense capping occur at different lifecycle points |
| H05/H06 shared policy, separate hooks | Armor & weapon degradation use different entry points & native chances |
| H08 separate from H07 | Avoids changing widely-used `awardExperience` signature; stacking policy defined once |
| H12A/H12B shared classifier | Same eligibility & calculation for initial + explicit renewal |
| H13A/H13B same calculation | Client speed + server validation MUST match to prevent desync |
| H14A/H14B same wearable-cap calc | Server mechanics + client-visible values must stay identical |

---

## Service Architecture

### Configuration Authority
```
CustomSkillsConfig (Singleton)
|---- Loads scripts/customskills/config.lua at startup
|---- Cached for combat performance (restart on change)
|---- Defaults: all disabled, Critical Chance enabled
`---- API: isModifierEnabled(), getModifierCap(), getBadgeBonus(), getBadgeBonuses()
```

### Central Modifier Authority
```
CustomSkillsModifiers (Static API)
|---- getModifierTotal(player, type) -- aggregates badge bonuses, applies caps
|---- getBadgeModifier(badgeKey, type) -- single badge contribution
|---- isModifierEnabled(type) -- config proxy
|---- formatPercent(), formatModifierBonus(), colorizeCriticalText()
|---- notifyBadgeAwarded(player) -- refreshes skill mods & run speed
`---- Combat helpers: getCriticalChance(), getCriticalMultiplier()
```

### Runtime Services (Hook Targets)

Each service is a static class with focused methods. No singleton state -- pure functions of inputs.

| Service | Key Design |
|---------|------------|
| `CustomSkillsCombat` | Context struct for H01; armor rating reduction before piercing; defense cap replacement |
| `CustomSkillsDurability` | Shared multiplicative chance reduction; separate hooks for armor/weapon |
| `CustomSkillsProgression` | Multiplier applied only to positive XP on normal path |
| `CustomSkillsCrafting` | Factory snapshots activator; amazing results interpolates toward cap & raises ceiling |
| `CustomSkillsBuffs` | Explicit eligibility classifier (CRC allowlist); renewal snapshots; no re-scale on reload |
| `CustomSkillsMovement` | Single `getSpeed()` used by both H13A (client) & H13B (server) |
| `CustomSkillsSkillMods` | Delta between native-clamped & custom-capped wearable added to total; same for visible |
| `CustomSkillsGathering` | Post-calculation multiplication; floor at native; discrete loot excluded |

---

## Build Process

### Prerequisites
- Core3 source tree with CMake build system
- C++17 compiler
- Git (for patch application)

### Standard Build

```bash
# 1. Apply patch (once per Core3 revision)
cd /path/to/Core3
git apply --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch

# 2. Configure (picks up new .cpp files via recursive glob)
cmake -B build

# 3. Build
cmake --build build --target MMOCoreORB -j$(nproc)

# 4. Deploy
# Copy binary + updated scripts from MMOCoreORB/bin/scripts/
```

### Clean Build Verification

Per DESIGN.md goal #10: **Verify on clean Core3 base before release.**

```bash
# Fresh checkout
git clone <core3-repo> Core3-clean
cd Core3-clean

# Install module
cp -r /path/to/customskills-mod/package/MMOCoreORB/ MMOCoreORB/
git apply --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch

# Build & test
cmake -B build && cmake --build build --target MMOCoreORB
# Run server, verify /customskills works
```

---

## Adding a New Modifier (Checklist)

1. **Enum**: Add to `CustomSkillsModifierType.h` (before `COUNT`)
2. **Name**: Add to `CustomSkillsModifiers::getModifierName()`
3. **Formatting**: Add to `formatModifierBonus()` (bp vs whole units)
4. **Defaults**: Add in `CustomSkillsConfig::setDefaults()`
5. **Config loader**: Add `loadModifier()` call in `CustomSkillsConfig::load()`
6. **Service**: Create new service class or extend existing
7. **Hook(s)**: Add Core3 delegation per hook inventory
8. **Badge assignments**: Update `MENU_SYSTEM.md + BADGE_REFERENCE.md` + `config.lua`
9. **Menu**: Add category/page in `CustomSkillsMenu` / `MENU_DESIGN.md`
10. **Docs**: Update `MODIFIER_REFERENCE.md` status, `MANIFEST.md` if new files

---

## Testing & Verification Matrix

Operational testing capabilities (unit-test suite, `testScreenPlays` smoke
mode, database requirements, no-CI constraints) are documented in
[CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md). The matrix below defines the
in-game behavioral checks for this module:

From `IMPLEMENTATION_GUIDE.md`:

| Test | Description |
|------|-------------|
| Disabled/zero modifier | Reproduces native output exactly |
| Menu = runtime | All configured values shown by `/customskills` equal effective values |
| Negative XP / `applyModifiers==false` | Unchanged |
| Practice mode | Retains native 5% with zero custom bonus |
| Factory speed | Survives logout/restart; changes only on new run |
| Non-amazing crafting | Remains resource-capped |
| Buff exclusions | Downers/debuffs/excluded families retain native durations |
| Movement sync | Client speed = server validation in all travel modes; no double on mount transitions |
| SEA overflow | Server mechanics = client-visible values |
| Forage discrete loot | Never duplicated by Gathering Quantity |

### Per-Modifier Focused Tests

| Modifier | Key Tests |
|----------|-----------|
| Movement | Foot, mount, vehicle; mount/dismount transitions; reconnect |
| Buff Duration | Activation, explicit renewal, DB reload, recursive linked buffs |
| Factory | Stop/restart captures new bonus; logout/restart preserves timer |
| Amazing Results | Assembly (all attrs) vs experimentation (selected row); ceiling raise |
| SEA Cap | Server calc = client visible; badge gain/loss refreshes visible |
| Forage/Milk | Round down; never below native; discrete loot unaffected |

---

## Patch Management

### Applying
```bash
git apply --check --ignore-space-change /path/to/core3-hooks.patch  # validate
git apply --ignore-space-change /path/to/core3-hooks.patch           # apply
```

### Reversing
```bash
git apply --check --reverse --ignore-space-change /path/to/core3-hooks.patch
git apply --reverse --ignore-space-change /path/to/core3-hooks.patch
```

### If Check Fails
- Target Core3 revision differs
- **Do not force** -- port hunks manually using `MANIFEST.md` (lists all 19 files)
- Each hunk is a small, generic delegation -- easy to port

### Patch Contents
- Command registration (3 files)
- Combat damage/armor/defense (CombatManager.h/.cpp)
- Crafting amazing (2 files)
- Factory activation (IDL + impl)
- Player XP & speed (PlayerManagerImplementation.cpp)
- Foraging (ForageManagerImplementation.cpp)
- Creature skill mods & buffs (CreatureObjectImplementation.cpp)
- Buff activation/renewal (BuffImplementation.cpp)
- Weapon degradation (WeaponObjectImplementation.cpp)
- Milk (MilkCreatureTask.h)
- SkillModList helpers (SkillModList.h)
- Packet sync (CreatureObjectMessage4.h, CreatureObjectDeltaMessage4.h)

---

## Configuration Reload

- **No hot reload** -- config cached for combat performance
- **Restart required** after any `config.lua` change
- Missing/invalid values -> safe defaults + server warning (not error)

---

## Debugging Tips

| Issue | Check |
|-------|-------|
| Modifier not applying | `isModifierEnabled(type)` in config; badge keys match `BadgeList` exactly |
| Menu != combat | Both use `CustomSkillsModifiers::getModifierTotal()` -- verify same `PlayerObject` |
| Factory speed wrong | Activator snapshot at start; restart factory after badge change |
| Movement desync | H13A & H13B use same `CustomSkillsMovement::getSpeed()` |
| SEA cap not visible | H14B refreshes on badge change; `CustomSkillsSkillMods::refreshVisibleSkillMods()` |
| Buff not extended | Eligibility classifier (CRC allowlist); check `CustomSkillsBuffs::isEligibleBuff()` |
