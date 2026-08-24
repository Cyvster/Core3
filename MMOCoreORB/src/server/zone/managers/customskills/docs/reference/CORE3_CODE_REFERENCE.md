# Core3 Code Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Purpose

Reference for navigating and understanding the SWGEmu Core3 codebase: layout,
patterns, key systems, and module integration points. For building, running,
and testing Core3, see [CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md).

**Factual corrections** to this document go through
[../rules/errata.md](../rules/errata.md) (two-party verification). The
historical correction log for its predecessor lives at
[../archive/CODE_GUIDE_ERRATA.md](../archive/CODE_GUIDE_ERRATA.md).

---

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- initial creation (as SWGEMU_CODE_GUIDE.md)
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- split into code
  reference + user guide pair; errata routing updated

---

## Codebase Layout

```
Core3/
|---- MMOCoreORB/                    # Main server code
|   |---- src/
|   |   |---- server/
|   |   |   |---- zone/              # Zone server (gameplay logic)
|   |   |   |   |---- managers/      # System managers (combat, crafting, player, etc.)
|   |   |   |   |---- objects/       # Game objects (creature, player, installation, etc.)
|   |   |   |   `---- packets/       # Network packets (baselines, deltas, messages)
|   |   |   |---- db/                # MySQL/Mantis database layers
|   |   |   `---- ...
|   |   |---- tests/                 # GoogleTest sources (compiled INTO the core3 binary)
|   |   `---- main.cpp               # Entry point + CLI mode flags
|   |---- bin/
|   |   |---- conf/config.lua        # Runtime configuration (DB, server toggles)
|   |   `---- scripts/               # Lua: commands, config, screenplays, managers
|   |---- sql/                       # Schema dumps (swgemu.sql, mantis.sql, datatables.sql)
|   |---- utils/
|   |   |---- engine3/               # Core engine (git submodule: swgemu/engine3)
|   |   `---- googletest-release-1.13.0/  # Vendored GoogleTest/GoogleMock
|   `---- CMakeLists.txt             # Build definition (see CORE3_USER_GUIDE)
|---- docker/                        # Container build/run scripts
|---- linux/                         # Native bootstrap script
`---- wsl2/                          # WSL2 setup notes
```

**Note**: the engine is NOT a top-level `engine/` directory; it lives at
`MMOCoreORB/utils/engine3/` as a git submodule (see `.gitmodules`). The
submodule directory is empty until initialized; include paths like
`engine/lua/Lua.h` resolve only when it is checked out.

---

## Key Patterns & Conventions

### 1. Singleton Managers

Most zone systems use `Singleton<T>`:

```cpp
class CombatManager : public Singleton<CombatManager>, public Logger {
public:
    static CombatManager* instance() { return Singleton<CombatManager>::instance(); }
};
```

Usage: `CombatManager::instance()->method()`

### 2. Reference Counting (ManagedReference)

All game objects use `ManagedReference<T*>` for automatic reference counting:

```cpp
ManagedReference<CreatureObject*> creature = player->getTarget();
```

Rules:
- Never store raw pointers to game objects long-term
- `ManagedReference` handles addRef/release automatically
- Use `_this.getReferenceUnsafeStaticCast()` in object methods for self-reference

### 3. Locking Pattern (Locker)

Thread safety via `Locker` RAII:

```cpp
void CreatureObjectImplementation::someMethod() {
    Locker locker(this);  // Locks the object's mutex
    // ... critical section ...
}  // Auto-unlocks on scope exit

// Cross-object locking (deadlock-safe consistent order):
Locker clocker(creature, target);
```

Always lock parent before child; use `Locker(a, b)` for multi-lock.

### 4. Logger Integration

Managers inherit `Logger`:

```cpp
info("Loading Custom Skills config");
warning("Config missing, using defaults");
error("Failed to load config");
```

### 5. String Handling

Custom `String` class:

```cpp
String name = "test";
name == "test";          // operator==
name.length();
name.toLowerCase();
String::valueOf(123);    // int to String
```

Use `toCharArray()` (not `c_str()`) when interfacing with Core3 APIs.

### 6. Vector/Map Containers

Engine containers: `Vector<T>`, `VectorMap<K,V>`, `SortedVector<T>`:

```cpp
Vector<int> list;
list.add(1); list.size(); list.get(0);

VectorMap<String, int> map;
map.put("key", 123); map.contains("key"); map.get("key"); map.remove("key");
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

**Combat flow (applyDamage entry point)**:

1. **Hit/defense calculation** -- `getDefenderDefenseModifier()` computes
   defender defense skill (includes weapon-mediated mods from
   `defenderDefenseModifiers` / `defenderSecondaryDefenseModifiers`).
2. **Armor reduction** -- per-pool inside damage application:
   `getArmorReduction()` called for health/action/mind separately. Includes
   toughness (pre-armor), armor/PSG/Force armor, food mitigation
   (`mitigate_damage`), state defenses.
3. **Damage application** -- final damage applied to pools.

**Mitigation layers** (application order; all percentage-based):

1. **Toughness** -- `<weapon>_toughness` variants + `jedi_toughness`:
   multiplicative pre-armor reduction, silent. Weapon-mediated; force damage
   mitigated only by `jedi_toughness`; lightsaber-type bypasses it; DoT ticks
   skip toughness.
2. **Armor / PSG / Force armor & shield** -- `getArmorReduction()` (per pool).
3. **Food mitigation** -- `mitigate_damage`: post-armor % per pool, hard cap
   100. Only mitigation that emits chat spam.
4. **State defenses** -- `knockdown_defense`, `dizzy_defense`, `stun_defense`,
   `blind_defense`, `intimidate_defense`, `posture_change_down/up_defense` --
   not hardcoded in C++; command scripts declare defender modifier lists and
   resist rolls sum `getSkillMod()` values generically.

### Player System

| File | Purpose |
|------|---------|
| `PlayerManagerImplementation.cpp` | XP awards, badge granting, speed validation |
| `PlayerObject.h/.cpp` | Player-specific data (badges, skills, UI state) |
| `BadgeList.h` | Global badge registry (loads from badge_map.iff) |

Badge check: `ghost->hasBadge(badgeIndex)` where `ghost = player->getPlayerObject()`.

### Experience (XP) System

| File | Purpose |
|------|---------|
| `PlayerManagerImplementation.cpp` | `awardExperience()` -- central positive-XP multiplier chain (~line 2605) |
| `PlayerObjectImplementation.cpp` | `addExperience()` -- storage + cap clamp (~line 727); `hasCappedExperience()` is exact-equality |
| `SkillManager.cpp` | `updateXpLimits()` (~line 788) -- rebuilds per-player cap map on login/skill change |

Multiplier chain (positive awards, `applyModifiers == true`):
`amount x speciesModifier x buffMultiplier x localMultiplier x
globalExpMultiplier x customSkills (bp / 10000)` -> `(int)` cast ->
`addExperience()`.

Cap enforcement:
- Clamp-on-award with partial grant; reaching a cap never blocks later awards.
- Cap value = highest `xpCap` among owned skill boxes (`skills.iff` column 14),
  else the type's `xp_limits.iff` default, else literal `2000`.
  `prestige_*` types forced uncapped.
- Lowering a cap pulls existing XP down on next login/skill change; raising
  one never grants retroactive XP.

Overflow safety (XP is signed int32 end-to-end): a float product above ~2^31
makes the `(int)` cast UB (typically yields `INT_MIN`, i.e. a *negative*
award). Keep multiplied awards well below 2^31. Fixed behaviors: jedi clone
loss = 5% of jedi cap; `jedi_general` negative floor -10,000,000.

### Crafting System

| File | Purpose |
|------|---------|
| `CraftingManagerImplementation.cpp` | Experimentation, assembly success calculation |
| `SharedLabratory.cpp` | Assembly success (shared by all labs) |
| `CraftingSessionImplementation.cpp` | Session state, prototype creation, practice mode |
| `FactoryObjectImplementation.cpp` | Factory production, timer management |

Amazing success: `AMAZINGSUCCESS` constant = **0**
(`CraftingManager.idl:37`). Native chance unverified -- "~5%" is a placeholder
pending measurement.

### Buff System

| File | Purpose |
|------|---------|
| `BuffImplementation.cpp` | Buff activation, renewal, duration, modifiers |
| `Buff.idl` / `Buff.h` | Buff data (CRC, duration, attribute modifiers) |
| `CreatureObjectImplementation.cpp` | `renewBuff()`, `addBuff()`, skill mod integration |

Buff families are identified by CRC. Skill-modifier API on Buff
(`Buff.idl`): `getSkillModifierValue(name)`, `setSkillModifier(name, value)`,
`getSkillModifiers()` -- **no `getSkillMod()` method exists on Buff**.

### Movement System

| File | Purpose |
|------|---------|
| `CreatureObjectImplementation.cpp` | `getRunSpeed()`, `updateRunSpeed()` |
| `PlayerManagerImplementation.cpp` | `checkPlayerSpeedTest()` (server validation) |
| `CreatureObjectMessage4.h` | Baseline packet (run speed at variable 0x07) |
| `CreatureObjectDeltaMessage4.h` | Delta packet (run speed updates) |

Client/server sync: both sides must compute the same effective speed
(H13A/H13B pattern).

### Skill Mods

| File | Purpose |
|------|---------|
| `SkillModManager.h/.cpp` | Global caps, defaults, registration |
| `SkillModList.h/.cpp` | Per-creature skill mod aggregation |
| `CreatureObjectImplementation.cpp` | `getSkillMod()`, `addSkillMod()`, visible mods |

Source groups (`SkillModManager.h:43-58`; clamps in
`bin/scripts/managers/skill_mod_manager.lua` and `setDefaults`):

| Family | Groups | Clamp |
|--------|--------|-------|
| Permanent (`0x100`) | `TEMPLATE` (Lua templates), `SKILLBOX` (learned skills) | none |
| Bonus (`0x1000`) | `WEARABLE` (SEA/tapes), `STRUCTURE`, `CITY`, `DROID` | +/-25 / +/-125 |
| Temporary (`0x10000`) | `BUFF`, `ABILITYBONUS` | +/-125 |

Groups are clamped individually, then summed across groups by
`SkillModList::getSkillMod()`. `BUFF`-group values are excluded from the
client-visible CREO4 entry (`getVisibleSkillMod()` aggregates only
PERMANENT/BONUS families).

**Consumption patterns** (critical when granting mods from skills/trees/items):

- **Generic readers** work from any source: primary defenses, `dodge_attack`
  (adds *above* the defense hard cap), state defenses, accuracy/damage mods,
  toughness, `mitigate_damage`, utility/profession mods (`burst_run`,
  `slope_move`, `mask_scent`, `camouflage`, `surveying`, `trapping`,
  `creature_harvesting`, `tame_bonus`, healing-effectiveness mods).
- **Buff-only readers** see nothing from SKILLBOX grants (they scan active
  buffs): `xp_increase`, `craft_bonus`, `experiment_bonus`, `heal_recovery`,
  `incap_recovery`, `reduce_spice_downtime`.
- **Weapon-mediated reads** only count while the equipped weapon's template
  lists the mod: primary/secondary defenses and toughness via
  `defenderDefenseModifiers` / `defenderSecondaryDefenseModifiers` /
  `defenderToughnessModifiers`; flat damage bonuses via `damageModifiers`.

### Scripting (Lua)

| File | Purpose |
|------|---------|
| `engine/lua/Lua.h` | C++ Lua binding wrapper (include path; requires engine3 submodule) |
| `scripts/screenplays/screenplays.lua` | Main screenplay loader |
| `scripts/commands/*.lua` | Slash command implementations |

C++ -> Lua bridge: `Lua::runFile()`, `Lua::getGlobalObject()`,
`LuaObject::getIntField()`, etc. Lua -> C++: screenplay callbacks registered
via `SuiManager`.

### SUI (Server UI)

| File | Purpose |
|------|---------|
| `SuiManager.h/.cpp` | SUI page registry, callback dispatch |
| `SuiListBox.h` | List box SUI (used by `/customskills`) |
| `SuiCallback.h` | Base callback class |

Custom Skills flow: C++ `CustomSkillsCommand` -> `CustomSkillsMenu` creates
`SuiListBox` directly -> client -> `SuiManager` -> `CustomSkillsSuiCallback`.
Lua is used only for config loading and command registration -- **not** for
menu creation or callbacks.

### Packets (Baselines/Deltas)

| File | Purpose |
|------|---------|
| `BaseLineMessage.h` | Baseline message builder |
| `DeltaMessage.h` | Delta message builder |
| `CreatureObjectMessage4.h` | Creature baseline (run speed, skill mods) |
| `CreatureObjectDeltaMessage4.h` | Creature deltas (speed, skill mod updates) |

Common inserts: `insertFloat()`, `insertInt()`, `insertLong()`,
`insertAscii()`.

---

## Custom Skills Integration Points

### Hook Pattern

```cpp
// Core3 file
#include "server/zone/managers/customskills/.../CustomSkillsService.h"

// In Core3 method:
return CustomSkillsService::hookMethod(this, args...);
```

Minimal delegation -> module-owned service does all logic. Hook inventory:
[ARCHITECTURE.md](ARCHITECTURE.md).

### Module Structure

```
MMOCoreORB/src/server/zone/managers/customskills/
|---- CustomSkillsConfig.h/.cpp       # Config singleton (loads config.lua)
|---- CustomSkillsModifiers.h/.cpp    # Central modifier authority
|---- CustomSkillsModifierType.h      # Modifier type enum
|---- CustomSkillsCommand.h/.cpp      # /customskills command
|---- CustomSkillsMenu.h/.cpp         # C++ SUI menu
|---- CustomSkillsSuiCallback.h/.cpp  # SUI event handling
|---- combat/  buffs/  crafting/  durability/
|---- gathering/  movement/  progression/  skillmods/
```

---

## Code Navigation Tips

```bash
# Find hook locations
grep -r "CustomSkills" MMOCoreORB/src/ --include="*.cpp" --include="*.h"

# Find badge usage
grep -r "hasBadge\|getBadge\|BadgeList" MMOCoreORB/src/server/zone/managers/player/

# Find skill mod usage
grep -r "getSkillMod\|addSkillMod\|WEARABLE" MMOCoreORB/src/server/zone/objects/creature/

# Find combat damage flow
grep -r "applyDamage\|getArmorReduction\|getArmorPiercing" MMOCoreORB/src/server/zone/managers/combat/
```

---

## Common Gotchas

| Issue | Solution | Source/Context |
|-------|----------|----------------|
| Segfault on object access | Forgot `Locker` or `ManagedReference` | General C++ |
| Deadlock | Inconsistent lock order; use `Locker(a, b)` for multi-lock | General C++ |
| Config not reloading | Config cached; restart server after `config.lua` change | CustomSkillsConfig |
| Badge not found | Use stable key (`BadgeList::get(key)`), not index | Badge system |
| Skill mod not visible | Client needs CREO4 baseline/delta update | SkillModList / packets |
| Factory timer wrong | Snapshot activator at start; persists across restart | FactoryObjectImplementation |
| Buff duration not applying | Check eligibility classifier (CRC allowlist) | CustomSkillsBuffs |
| Movement desync | Client (H13A) and server (H13B) must use identical calculation | CustomSkillsMovement |
| Buff-only mod granted via learned skill does nothing | Readers like `xp_increase` scan active buffs only -- use generic-reader mod or extend reader | Skill mod consumption patterns |
| New toughness/secondary-defense mod never procs | Equipped weapon template must list the mod | Weapon-mediated reads |
| Extreme XP multipliers delete XP instead of raising it | Float->int cast overflow yields negative award; keep awards below 2^31 | Progression system |
| Mod looks engine-readable but works for one species/command | Check the consuming command, not just `getSkillMod()` -- e.g. `enhanced_regen` read only by Trandoshan innate `/regen` | Species/command-gated mods |
| HAM regeneration has no skill-mod hooks | Passive tick reads only CON/STAMINA/WILLPOWER (`stat x 13/2100 HP/s`; kneel x1.25, sit x1.75) | Regeneration system |

Append new gotchas with reproduced evidence; factual corrections go through
[../rules/errata.md](../rules/errata.md).

---

## Related Documentation

- Build/run/test operations: [CORE3_USER_GUIDE.md](CORE3_USER_GUIDE.md)
- Architecture & design: [ARCHITECTURE.md](ARCHITECTURE.md)
- Hooks: [ARCHITECTURE.md](ARCHITECTURE.md)
- Modifiers: [MODIFIER_REFERENCE.md](MODIFIER_REFERENCE.md)
- Badges: [BADGE_REFERENCE.md](BADGE_REFERENCE.md)
