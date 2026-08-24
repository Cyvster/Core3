# SWGEmu Core3 — Code Guide for AI Researchers

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Purpose

This guide documents patterns, conventions, and key systems in the SWGEmu Core3 codebase to help AI agents (and developers) navigate and understand the code efficiently. **Update this guide when you discover useful patterns not yet documented.**

---

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

---

## Codebase Layout

```
Core3/
├── MMOCoreORB/                    # Main server code
│   ├── src/
│   │   ├── server/
│   │   │   ├── zone/              # Zone server (gameplay logic)
│   │   │   │   ├── managers/      # System managers (combat, crafting, player, etc.)
│   │   │   │   ├── objects/       # Game objects (creature, player, installation, etc.)
│   │   │   │   ├── packets/       # Network packets (baselines, deltas, messages)
│   │   │   │   └── ...
│   │   │   └── ...
│   │   └── ...
│   │   └── utils/engine3/         # Core engine (git submodule: swgemu/engine3)
│   └── bin/scripts/               # Lua scripts (commands, config, screenplays)
├── docker/
├── linux/
├── wsl2/
└── ...
```

**Note**: `engine/` is not a top-level directory; it lives at `MMOCoreORB/utils/engine3/` as a git submodule (see `.gitmodules`).

---

## Key Patterns & Conventions

### 1. Singleton Managers

Most zone systems use `Singleton<T>` pattern:

```cpp
class CombatManager : public Singleton<CombatManager>, public Logger {
public:
    static CombatManager* instance() { return Singleton<CombatManager>::instance(); }
    // ...
};
```

**Usage**: `CombatManager::instance()->method()`

### 2. Reference Counting (ManagedReference)

All game objects use `ManagedReference<T*>` for automatic reference counting:

```cpp
ManagedReference<CreatureObject*> creature = player->getTarget();
```

**Rules**:
- Never store raw pointers to game objects long-term
- `ManagedReference` handles addRef/release automatically
- Use `_this.getReferenceUnsafeStaticCast()` in object methods to get self-reference

### 3. Locking Pattern (Locker)

Thread safety via `Locker` RAII:

```cpp
void CreatureObjectImplementation::someMethod() {
    Locker locker(this);  // Locks the object's mutex
    // ... critical section ...
}  // Auto-unlocks on scope exit

// Cross-object locking (avoid deadlock: consistent order)
Locker clocker(creature, target);  // Locks both in consistent order
```

**Critical**: Always lock parent before child; use `Locker(a, b)` for multiple.

### 4. Logger Integration

All managers inherit `Logger`:

```cpp
class CustomSkillsConfig : public Singleton<CustomSkillsConfig>, public Logger {
    // ...
    void load() {
        info("Loading Custom Skills config");  // info()
        warning("Config missing, using defaults");  // warning()
        error("Failed to load config");  // error()
    }
};
```

### 5. String Handling

Custom `String` class (Unicode-aware):

```cpp
String name = "test";
name == "test"           // operator==
name.length()            // length()
name.toLowerCase()       // case conversion
String::valueOf(123)     // int to String
```

### 6. Vector/Map Containers

Engine provides `Vector<T>`, `VectorMap<K,V>`, `SortedVector<T>`:

```cpp
Vector<int> list;
list.add(1);
list.size();
list.get(0);

VectorMap<String, int> map;
map.put("key", 123);
map.contains("key");
map.get("key");
map.remove("key");
```

---

## Core Systems Reference

### Combat System

| File | Purpose |
|------|---------|
| `CombatManager.h/.cpp` | Central combat logic: applyDamage, armor reduction, defense calc |
| `CombatQueueCommand.h` | Base class for combat commands |
| `CreatureAttackData.h` | Attack data container (weapon, pools, etc.) |
| `DefenderHitList.h` | Hit results for defenders |

**Combat Flow (applyDamage entry point)**:

1. **Hit/defense calculation** — `getDefenderDefenseModifier()` computes defender's defense skill (includes weapon-mediated mods from `defenderDefenseModifiers` / `defenderSecondaryDefenseModifiers`).
2. **Armor reduction** — Per-pool inside damage application: `getArmorReduction()` called for health/action/mind pools separately. Includes toughness (pre-armor), armor/PSG/Force armor, food mitigation (`mitigate_damage`), state defenses (generic skill-mod-based resist rolls).
3. **Damage application** — After all mitigations, final damage applied to pools.

**Mitigation layers** (application order; all percentage-based):

1. **Toughness** — `<weapon>_toughness` variants + `jedi_toughness`: multiplicative pre-armor reduction, fully silent (`getDefenderToughnessModifier()`). Weapon-mediated (defender's held weapon determines variant); force damage mitigated only by `jedi_toughness`; lightsaber-type bypasses `jedi_toughness`; DoT ticks skip toughness.
2. **Armor / PSG / Force armor & shield** — `getArmorReduction()` (per pool).
3. **Food mitigation** — `mitigate_damage`: post-armor % per pool, hard cap 100. Only mitigation that emits chat spam (`@combat_effects:mitigate_damage`).
4. **State defenses** — `knockdown_defense`, `dizzy_defense`, `stun_defense`, `blind_defense`, `intimidate_defense`, `posture_change_down_defense` — not hardcoded in C++; command scripts declare `defenderStateDefenseModifiers` list, resist roll sums defender `getSkillMod()` values generically.

### Player System

| File | Purpose |
|------|---------|
| `PlayerManagerImplementation.cpp` | XP awards, badge granting, speed validation, buff management |
| `PlayerObject.h/.cpp` | Player-specific data (badges, skills, UI state, screenplay data) |
| `BadgeList.h` | Global badge registry (loads from badge_map.iff) |

**Badge check**: `ghost->hasBadge(badgeIndex)` where `ghost = player->getPlayerObject()`

### Experience (XP) System

| File | Purpose |
|------|---------|
| `PlayerManagerImplementation.cpp` | `awardExperience()` — central positive-XP multiplier chain (~line 2605) |
| `PlayerObjectImplementation.cpp` | `addExperience()` — storage + cap clamp (~line 727); `hasCappedExperience()` is an exact-equality check |
| `SkillManager.cpp` | `updateXpLimits()` (~line 788) — rebuilds the per-player cap map on login and every skill change |

**Multiplier chain** (positive awards, `applyModifiers == true`):
`amount × speciesModifier × buffMultiplier (xp_increase ÷ 100) × localMultiplier × globalExpMultiplier × customSkills (basis points ÷ 10000)` → `(int)` cast → `addExperience()`.

**Cap enforcement**:
- Clamp-on-award with partial grant; reaching a cap never blocks later awards.
- Cap value = highest `xpCap` among owned skill boxes (`skills.iff` column 14), else the type's `xp_limits.iff` default, else literal `2000`. `prestige_*` types are forced to `INT_MAX` (uncapped).
- Lowering a cap pulls existing XP down on next login/skill change; raising one never grants retroactive XP.

**Overflow safety** (XP is signed int32 end-to-end: memory, ObjectDatabase persistence, PLAY8 packets, client):
- A float product above ~2^31 makes the `(int)` cast undefined behavior (x86 typically yields `INT_MIN`, so an extreme multiplier produces a *negative* award that deletes the XP type). Keep `amount × full product` well below 2^31.
- `currentXP + award` is summed before clamping: cap + worst-case single award must stay ≤ 2,147,483,647.
- Practical ceiling: caps ≤ 100M are comfortably safe; ≤ 500M still leaves >1.6B headroom. The multiplier product binds before the cap does.
- Fixed behaviors: jedi clone loss = 5% of jedi cap; `jedi_general` negative floor −10,000,000.

### Crafting System

| File | Purpose |
|------|---------|
| `CraftingManagerImplementation.cpp` | Experimentation, assembly success calculation |
| `SharedLabratory.cpp` | Assembly success (shared by all labs) |
| `CraftingSessionImplementation.cpp` | Crafting session state, prototype creation, practice mode |
| `FactoryObjectImplementation.cpp` | Factory production, timer management |

**Amazing success**: `AMAZINGSUCCESS` constant = **0** (see `CraftingManager.idl:37`). Native chance unverified — "~5%" noted as placeholder pending measurement.

### Buff System

| File | Purpose |
|------|---------|
| `BuffImplementation.cpp` | Buff activation, renewal, duration, modifiers |
| `Buff.idl` / `Buff.h` | Buff data (CRC, duration, attribute modifiers) |
| `CreatureObjectImplementation.cpp` | `renewBuff()`, `addBuff()`, skill mod integration |

**Buff families**: Identified by CRC; type checking via CRC lists.
**Skill modifier API on Buff** (`Buff.idl`): `getSkillModifierValue(name)`, `setSkillModifier(name, value)`, `getSkillModifiers()` — **no `getSkillMod()` method exists**.

### Movement System

| File | Purpose |
|------|---------|
| `CreatureObjectImplementation.cpp` | `getRunSpeed()`, `updateRunSpeed()` |
| `PlayerManagerImplementation.cpp` | `checkPlayerSpeedTest()` (server validation) |
| `CreatureObjectMessage4.h` | Baseline packet (run speed at 0x07) |
| `CreatureObjectDeltaMessage4.h` | Delta packet (run speed updates) |

**Client/Server sync**: Both must compute same effective speed (H13A/H13B pattern)

### Skill Mods

| File | Purpose |
|------|---------|
| `SkillModManager.h/.cpp` | Global skill mod caps, registration |
| `SkillModList.h/.cpp` | Per-creature skill mod aggregation |
| `CreatureObjectImplementation.cpp` | `getSkillMod()`, `addSkillMod()`, visible skill mods |

**Source groups** (`SkillModManager.h:43-58`; per-group clamps in `bin/scripts/managers/skill_mod_manager.lua`, C++ defaults in `SkillModManager::setDefaults`):

| Family | Groups | Clamp |
|--------|--------|-------|
| Permanent (`0x100`) | `TEMPLATE` (Lua templates), `SKILLBOX` (learned skills) | none |
| Bonus (`0x1000`) | `WEARABLE` (SEA/tapes), `STRUCTURE`, `CITY`, `DROID` | ±25 / ±125 |
| Temporary (`0x10000`) | `BUFF`, `ABILITYBONUS` | ±125 |

Groups are clamped individually, then summed across groups by `SkillModList::getSkillMod()`. `BUFF`-group values are excluded from the client-visible CREO4 skill-mod entry (`SkillModList::getVisibleSkillMod()` aggregates only PERMANENT/BONUS families).

**Consumption patterns** (critical when granting mods from new skills, trees, or items):

- **Generic readers** work from any source: primary defenses, `dodge_attack` (added *above* the defense hard cap), state defenses, accuracy/damage mods, toughness, `mitigate_damage`, and utility/profession mods (`burst_run`, `slope_move`, `mask_scent`, `camouflage`, `surveying`, `trapping`, `creature_harvesting`, `tame_bonus`, healing-effectiveness mods).
- **Buff-only readers** see nothing from learned-skill (SKILLBOX) grants because they scan active buffs via `getSkillModFromBuffs()` or a specific buff CRC: `xp_increase`, `craft_bonus`, `experiment_bonus`, `heal_recovery`, `incap_recovery`, `reduce_spice_downtime`.
- **Weapon-mediated reads** only count while the equipped weapon's template lists the mod: primary/secondary defenses and toughness via `defenderDefenseModifiers` / `defenderSecondaryDefenseModifiers` / `defenderToughnessModifiers`, flat damage bonuses (e.g., `unarmed_damage`) via `damageModifiers`.

### Scripting (Lua)

| File | Purpose |
|------|---------|
| `engine/lua/Lua.h` | C++ Lua binding wrapper |
| `scripts/screenplays/screenplays.lua` | Main screenplay loader |
| `scripts/commands/*.lua` | Slash command implementations |

**C++ → Lua bridge**: `Lua::runFile()`, `Lua::getGlobalObject()`, `LuaObject::getIntField()`, etc.

**Lua → C++**: Screenplay callbacks registered via `SuiManager`

### SUI (Server UI)

| File | Purpose |
|------|---------|
| `SuiManager.h/.cpp` | SUI page registry, callback dispatch |
| `SuiListBox.h` | List box SUI (used by `/customskills`) |
| `SuiCallback.h` | Base callback class |

**Flow for Custom Skills**: C++ `CustomSkillsCommand` → C++ `CustomSkillsMenu` creates `SuiListBox` directly → client → `SuiManager` → C++ `CustomSkillsSuiCallback` handles events. Lua is used only for config loading (`CustomSkillsConfig.cpp`) and command registration — **not for menu creation or callbacks**.

### Packets (Baselines/Deltas)

| File | Purpose |
|------|---------|
| `BaseLineMessage.h` | Baseline message builder |
| `DeltaMessage.h` | Delta message builder |
| `CreatureObjectMessage4.h` | Creature baseline (run speed, skill mods, etc.) |
| `CreatureObjectDeltaMessage4.h` | Creature deltas (speed, skill mod updates) |

**Common updates**: `insertFloat()`, `insertInt()`, `insertLong()`, `insertAscii()`

---

## Custom Skills Integration Points

### Hook Pattern

```cpp
// Core3 file
#include "server/zone/managers/customskills/.../CustomSkillsService.h"

// In Core3 method:
return CustomSkillsService::hookMethod(this, args...);
```

**Pattern**: Minimal delegation → module-owned service does all logic

### Module Structure (Reference)

```
MMOCoreORB/src/server/zone/managers/customskills/
├── CustomSkillsConfig.h/.cpp       # Config singleton (loads config.lua)
├── CustomSkillsModifiers.h/.cpp    # Central modifier authority
├── CustomSkillsModifierType.h      # Enum of 17 modifier types
├── CustomSkillsCommand.h/.cpp      # /customskills command
├── CustomSkillsMenu.h/.cpp         # C++ SUI menu
├── CustomSkillsSuiCallback.h/.cpp  # SUI event handling
├── combat/CustomSkillsCombat.h/.cpp
├── buffs/CustomSkillsBuffs.h/.cpp
├── crafting/CustomSkillsCrafting.h/.cpp
├── durability/CustomSkillsDurability.h/.cpp
├── gathering/CustomSkillsGathering.h/.cpp
├── movement/CustomSkillsMovement.h/.cpp
├── progression/CustomSkillsProgression.h/.cpp
└── skillmods/CustomSkillsSkillMods.h/.cpp
```

---

## Useful Code Navigation Tips

### Finding Hook Locations

Search for `CustomSkills` in Core3:
```bash
grep -r "CustomSkills" MMOCoreORB/src/ --include="*.cpp" --include="*.h"
```

### Finding Badge Usage

```bash
grep -r "hasBadge\|getBadge\|BadgeList" MMOCoreORB/src/server/zone/managers/player/ --include="*.cpp"
```

### Finding Skill Mod Usage

```bash
grep -r "getSkillMod\|addSkillMod\|WEARABLE" MMOCoreORB/src/server/zone/objects/creature/ --include="*.cpp"
```

### Finding Combat Damage Flow

```bash
grep -r "applyDamage\|getArmorReduction\|getArmorPiercing" MMOCoreORB/src/server/zone/managers/combat/ --include="*.cpp"
```

---

## Common Gotchas

**No limit — this section grows indefinitely.** Every discovered gotcha should be added here. The table below is a starting set; append new rows as you find them.

| Issue | Solution | Source/Context |
|-------|----------|----------------|
| Segfault on object access | Forgot `Locker` or `ManagedReference` | General C++ |
| Deadlock | Inconsistent lock order; use `Locker(a, b)` for multi-lock | General C++ |
| Config not reloading | Config is cached; restart server after `config.lua` change | CustomSkillsConfig |
| Badge not found | Use stable key (`BadgeList::get(key)`), not index | Badge system |
| Skill mod not visible | Client needs `CreatureObjectMessage4` baseline update | SkillModList / packets |
| Factory timer wrong | Snapshot activator at start; persists across restart | FactoryObjectImplementation |
| Buff duration not applying | Check eligibility classifier (CRC allowlist) | CustomSkillsBuffs |
| Movement desync | Client (H13A) and server (H13B) must use identical calculation | CustomSkillsMovement |
| Buff-only mod granted via skill tree/learned skill does nothing | Readers like `xp_increase`/`craft_bonus`/`heal_recovery` scan active buffs only — use a generic-reader mod or extend the reader | SkillMod consumption patterns |
| New toughness or secondary-defense mod "never procs" | Equipped weapon's template must list the mod (`defenderToughnessModifiers` / `defenderSecondaryDefenseModifiers`) | Weapon-mediated reads |
| Extreme XP multipliers delete XP instead of raising it | Float→int cast overflow yields negative award; keep multiplied awards below 2^31 | Progression system |
| Mod looks "engine-read" but only works for one species/command | Check the consuming command, not just `getSkillMod()` — e.g., `enhanced_regen` is read *only* by Trandoshan innate `/regen` (`RegenerationCommand.h:51`): converts to CON on the 5-min innate buff (1 pt = +1.75 CON, cap 100; CON regen = CON × 13/2100 HP/s). Useless for other species and never passive | Species/command-gated mods |
| HAM regeneration has no skill-mod hooks | The passive tick (`activateHAMRegeneration`, CreatureObjectImplementation.cpp ~3202) reads only CONSTITUTION/STAMINA/WILLPOWER: `stat × 13/2100 HP/s`, kneeling ×1.25, sitting ×1.75, min 1/tick | Regeneration system |

---

## Common Gotchas (Extended)

*Add new gotchas here as discovered, using the same format:*

| Issue | Solution | Source/Context |
|-------|----------|----------------|
| *(new gotcha 1)* | *(solution)* | *(file/system)* |
| *(new gotcha 2)* | *(solution)* | *(file/system)* |

---

## Updating This Guide

### ⚠️ For Factual Errors / API Corrections — Use the Errata Process

**Do not directly edit this guide for factual corrections.** Instead:

1. **File a finding** in [`CODE_GUIDE_ERRATA.md`](CODE_GUIDE_ERRATA.md) using the entry template
2. **Wait for independent verification** by a different contributor (enforced: reporter ≠ verifier)
3. **Only after verification** → apply the minimal fix and update the finding status to `APPLIED`

This two-LLM verification requirement prevents hallucinated APIs/constants from entering the guide.

### For Additions (Patterns, Gotchas, Search Tips)

These may be added directly if they are:
- Observed patterns (not claimed facts about specific APIs)
- Gotchas with reproduced evidence
- Verified search tips

**When you discover something useful, add it here:**

1. **New pattern** → Add to "Key Patterns & Conventions"
2. **New system reference** → Add to "Core Systems Reference"
3. **New gotcha** → Add to "Common Gotchas"
4. **New search tip** → Add to "Useful Code Navigation Tips"
5. **Custom Skills integration detail** → Update "Custom Skills Integration Points"

**Format**: Keep entries concise, include file paths, explain *why* it matters.

---

## Related Documentation

- **Custom Skills Architecture**: `ARCHITECTURE.md`
- **Custom Skills Hooks**: `IMPLEMENTATION_GUIDE.md`
- **Custom Skills Modifiers**: `MODIFIER_REFERENCE.md`
- **Badge Catalog**: `BADGE_REFERENCE.md`
- **Errata log & correction procedure for this guide**: `CODE_GUIDE_ERRATA.md`
