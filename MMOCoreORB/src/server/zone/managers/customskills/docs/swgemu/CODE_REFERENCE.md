# Core3 Code Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

---
## Purpose

Reference for navigating and understanding the SWGEmu Core3 codebase: layout,
patterns, key systems, and module integration points. Part II of this document covers building, running, and testing.

**Factual corrections** to this document go through
[../rules/errata.md](../rules/errata.md) (two-party verification). The
historical correction log for its predecessor lives at
[../archive/CODE_GUIDE_ERRATA.md](../archive/CODE_GUIDE_ERRATA.md).

---

---
## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- initial creation (as SWGEMU_CODE_GUIDE.md)
- ox-alpha (opencode/x-preview-f-free), 08232026 -- split into code
  reference + user guide pair; errata routing updated

---

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

---
## Related Documentation

- Module code reference (hooks, services, menu): [../../customskills/CODE_REFERENCE.md](../../customskills/CODE_REFERENCE.md)
- Module modifier/badge specifications: [../../customskills/CODE_REFERENCE.md](../../customskills/CODE_REFERENCE.md) Appendices A-B
- Player-facing emulator guide: [USER_GUIDE.md](USER_GUIDE.md)


---
## Access-Restricted In-game Commands (Admin/GM)

How privileged slash commands work in Core3, verified against source.

### Registration

Every slash command is defined as a Lua file under
`MMOCoreORB/bin/scripts/commands/` (806+ files) and wired into the live
command table by `CommandConfigManager.cpp`. Privileged commands are
ordinary entries flagged as requiring admin (examples on disk:
`createNPC.lua`, `addBannedPlayer.lua`, `cityban.lua`, `editBank.lua`).

### Gating chain (ObjectControllerImplementation.cpp)

When a queued command is flagged `requiresAdmin()`, execution requires
BOTH conditions, checked against the player's PlayerObject ("ghost"):

1. **God mode**: `ghost->hasGodMode()` must be true. God mode is an
   account-level privilege -- accounts carry an admin level (see
   `SWGRealmsAPI.cpp`, account `admin_level`) which the server maps to
   the god-mode flag.
2. **Ability**: `ghost->hasAbility(<command name>)` must ALSO be true --
   god mode alone is not sufficient; the character must hold the command
   ability like any other skill-granted ability.

Failure of either check logs an `adminLog` warning ("<name> attempted to
use the '/<cmd>' command without permissions"), sends
`@error_message:insufficient_permissions`, and clears the queued action.

### Auditing

- Every successful admin command invocation passes through
  `logAdminCommand(...)` -- admin actions are recorded server-side.
- Setting config option `Core3.CommandConfigManager.DumpAdminCommands`
  to true makes CommandConfigManager dump the full admin command list at
  startup (useful for inventorying what is restricted).

### For module developers

The Custom Skills module adds NO restricted commands; its player entry
point `/customskills` is a normal ability-backed command registered via
`bin/scripts/commands/customSkills.lua`. Any future privileged module
command must follow the chain above (requiresAdmin flag + ability grant)
and will be admin-audited automatically.

> **Operator quick reference:** the *what-do-I-type* guide for these commands
> (badge/skill/stat/server) is `ADMIN_COMMANDS.md` in this directory. It lists
> real command syntax and worked grant/revoke/audit scenarios.

---

# Part II -- Building, Running, Testing

---
## Repository Layout (operational view)

```
Core3/
|---- MMOCoreORB/          # Server sources, scripts, sql schemas, CMake
|---- docker/              # Dockerfile + build.sh/run.sh (containerized dev)
|---- linux/bootstrap.sh   # Native Debian dependency bootstrap
`---- wsl2/                # WSL2 setup notes
```

---

---
## System Requirements

Per root `README.md` (Debian 12 baseline) and `MMOCoreORB/CMakeLists.txt:31-49`:

- C++14 compiler: Clang >= 16 / GCC >= 5.4 / MSVC >= 2017, 64-bit only
- CMake >= 3.18; Ninja recommended (docker path uses Ninja + ccache)
- **Java runtime** -- required by the IDL compiler (`idlc`) at configure/build time
- **MariaDB/MySQL** -- client library to link AND a reachable server to boot anything
- BerkeleyDB 5.3 (engine3 object persistence), OpenSSL, zlib, pthreads,
  Lua **5.3 exact**, Boost(thread); optional jemalloc, cpprestsdk

The docker image (`docker/Dockerfile`, Debian bookworm) installs all of the
above including mariadb-server inside the container, plus ccache, valgrind,
gcovr. Prefer the docker path on Windows (see `docker/build.sh`,
`docker/run.sh`; WSL2 notes in `wsl2/`).

---

---
## Building

```bash
# Configure (recursive glob picks up new .cpp files -- reconfigure after adding files)
cmake -B build

# Build the server target
cmake --build build --target core3 -j$(nproc)
```

Key CMake options (`MMOCoreORB/CMakeLists.txt:52-74`):

| Option | Default | Notes |
|--------|---------|-------|
| `COMPILE_TESTS` | **ON** | Compiles GoogleTest suite into the `core3` binary and registers it with CTest |
| `BUILD_IDL` | ON | Runs Java-based idlc to generate IDL stubs |
| `ENABLE_ODB` | off | Builds the `odb3` object database tool |
| `ENABLE_BUILD_CLIENT` | ON | Builds `core3client` |
| `ENABLE_ASAN` / `ENABLE_TSAN` / `ENABLE_UBSAN` | off | Sanitizer builds |
| `ENABLE_REST_SERVER` | off | cpprestsdk REST interface |

Targets (`src/CMakeLists.txt` end): `core3` (always), `odb3`, `core3client`,
plus utilities `tags`, `compdb`, `update-engine3`, `idl`, `rebuild-idl`.

There is **no standalone test binary or test target**: test sources are
compiled directly into `core3` itself (see Testing below). A legacy orphaned
`src/tests/CMakeLists.txt` defines an unused `core3tests` library; ignore it.

Build heft: ~744 `.cpp` + ~2,500 `.h` under `MMOCoreORB/src`, plus IDL
codegen and vendored googletest. Expect a long, load-heavy compile; the
in-tree docker build script gates on system load average for this reason.

---

---
## Database Setup (required)

**Every execution mode that boots anything requires a reachable MariaDB**
with imported schema. There is no documented way around this:
`ServerCore.cpp:672-675` constructs `ServerDatabase` and `MantisDatabase`
unconditionally before any component toggles are evaluated, and even the
unit-test harness initializes the MySQL client library
(`src/tests/TestCore.h:26`).

1. Install/start MariaDB; create database and user.
2. Import schemas from `MMOCoreORB/sql/`: `swgemu.sql` (required),
   `datatables.sql`, and optionally `mantis.sql`.
3. Configure connection in `MMOCoreORB/bin/conf/config.lua`
   (`DBHost`, `DBPort=3306`, `DBName`, `DBUser`, `DBPass`, `DBSecret`;
   Mantis settings nearby).
4. Startup queries the `galaxy` table; ensure it is populated.

---

---
## Running

Entry point flags (`MMOCoreORB/src/main.cpp`):

| Invocation | Behavior |
|------------|----------|
| `./core3` | Full server boot |
| `./core3 testScreenPlays` | Loads ConfigManager + instantiates the DirectorManager Lua VM and loads all screenplays. No network ports opened. Fastest full-Lua smoke test (~lines 47-56) |
| `./core3 runUnitTests` | Runs the GoogleTest suite (requires `COMPILE_TESTS=ON`) (~lines 66-79) |
| `./core3 cleanDumpNav...` / `deleteNavMeshes` / `playercleanup` | Maintenance modes (ServerCore.cpp:838-842) |

Component toggles in `bin/conf/config.lua:47-51` (`MakeLogin`, `MakeZone`,
`MakePing`, `MakeStatus`, `MakeWeb`) let you boot a minimal process without
some sub-servers -- but the database requirement above still applies.

In-game console command `test <function>` runs functions from the
`Tests` screenplay (`bin/scripts/screenplays/tests/tests.lua`) --
e.g. AI movement/aggro tests (ServerCore.cpp:375-379).

Working directory matters: run from `MMOCoreORB/bin/` so relative script
paths resolve (CTest sets this as its working directory).

---

---
## Testing Capabilities

### What exists

**Unit tests: GoogleTest 1.13, vendored and compiled into the server binary.**

- Framework: `utils/googletest-release-1.13.0/` (with GoogleMock)
- Wiring: `COMPILE_TESTS=ON` -> `add_subdirectory(utils/googletest-release-1.13.0)`
  + `enable_testing()` (`CMakeLists.txt:220-234`); test sources globbed from
  `src/tests/*.cpp|h` into the `core3` executable (`src/CMakeLists.txt:69-86`);
  registered as `add_test(core3 ... runUnitTests)` with working dir
  `bin/` (`src/CMakeLists.txt:193-197`)
- Run via `ctest -R core3` (from build dir) or directly `./core3 runUnitTests`
- Scale: ~264 TEST/TEST_F cases across 16 suites. Highlights:
  `StringTest.cpp` (178 cases), `LuaMobileTest.cpp` (parses real mobiles/loot
  through live managers), `ConfigManagerTest.cpp`, `CreatureObjectTest.cpp`,
  `ZoneTest.cpp`, `AreaShapeTests.cpp`, deadlock-detection base classes, and
  mock infrastructure (`MockBehavior.h`) for AI behavior trees

**Lua validation:** no lint tooling (no luacheck config anywhere). The
available checks are the C++-driven ones: `testScreenPlays` mode,
`LuaMobileTest`, `CommandLuaTest`.

**Sanitizers:** ASan/TSan/UBSan build options exist for memory/race hunting.

**CI:** none. No `.github/workflows`, `.gitlab-ci.yml`, or Jenkinsfile exists
in the tree (the only CI files belong to vendored googletest). All testing is
local and manual-triggered.

### Constraints (be honest about these)

1. **No DB-less mode.** Unit tests link and initialize MySQL; several suites
   construct real managers/databases. A MariaDB endpoint is effectively
   required for meaningful runs.
2. **Compile cost.** Any test change recompiles part of the `core3` binary --
   minutes, not seconds.
3. **Engine-coupled tests.** Most gameplay logic takes `CreatureObject*` /
   `PlayerObject*` and engine singletons; unit-testing it requires the
   running-object infrastructure, not plain instantiation.

### Comparison with Project Alice-style testing

Alice is a single Python file: `python -m unittest discover -s tests` runs
per-commit in seconds with zero deployment. That exact loop does NOT port
here. What does port:

| Practice | Portability to Core3 |
|----------|----------------------|
| Per-commit automated tests | Partial -- `ctest`/`runUnitTests` works but needs built binary + MariaDB; realistic cadence is per-feature, not per-commit |
| Smoke test of script/config load | Yes, directly analogous: `core3 testScreenPlays` after any `scripts/` change |
| Static/convention enforcement | Yes, via grep checks in review (see CODE_REFERENCE navigation tips) |
| Manual verification matrix | Already our standard: see ARCHITECTURE.md "Testing & Verification Matrix" |

### Recommended cadence for this module

1. **Every session:** clean configure + build of `core3` (compile gate).
2. **After any Lua/script change:** `./core3 testScreenPlays`.
3. **Before delivery, when environment allows:** `ctest -R core3` with
   MariaDB up; treat failures as ERR-worthy regressions.
4. **Behavioral claims:** verify per the focused-test matrix in
   [ARCHITECTURE.md](ARCHITECTURE.md) against a live server.
5. **Future option:** add GoogleTest files under `src/tests/` for
   engine-independent module logic (pure bp math, formatting, classifier
   tables) following existing suite patterns; register nothing extra -- the
   glob picks them up.

---

---
## Deploying Module Changes

The module is part of this repository -- no patch application or mirroring
is needed for local development:

```bash
# From repo root:
cmake -B build && cmake --build build --target core3 -j$(nproc)
# Deploy: core3 binary + updated files under MMOCoreORB/bin/scripts/
```

`<module>/integration/core3-hooks.patch` exists for installing the module
onto a DIFFERENT (vanilla) Core3 checkout; see
[../installation/INSTALLATION.md](../installation/INSTALLATION.md). Reverse
it there with `git apply --reverse`; if check fails, port hunks manually per
`<module>/MANIFEST.md`; never force.

Remember ([PROC R6.5/R6.6] in `../rules/process.md`): commit code AND its
doc updates together under your traceability tag.

### Client data archives (TRE builder)

Some module artifacts live in the server's data archives, maintained in
Daniel's TRE builder workspace (`G:\Data\swgemu server\TRE builder\`),
outside this repository:

- **Datatable rows** (e.g. Challenge Tier skill trees) are appended to
  `datatables\skill\skills.csv`. Its NAME column holds internal keys only;
  display strings never go there.
- **Display strings** ship as compiled STF sources under `string\en\`.
  Precedent: `exp_n.stf` carries the `challenge_tier` XP label. Skill-box
  display names belong in a `skill_n.stf` source (to be added when the
  Challenge Tier trees deploy; decision record:
  [../feature-planning/CHALLENGE_TIER_SKILLS.md](../feature-planning/CHALLENGE_TIER_SKILLS.md)).

After editing either side, rebuild the affected archive(s) with the TRE
builder and redeploy them together with the server binary.

---

---
## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| Patch `--check` fails | Target Core3 revision differs | Port hunks manually per MANIFEST; do not force |
| CMake can't find Lua | Wrong version present | Install Lua 5.3 exactly (5.4 breaks bindings) |
| idlc errors at configure | Missing Java runtime | Install JDK/JRE; `rebuild-idl` target available |
| Boot fails on `galaxy` query | Schema not imported / wrong DB | Import `sql/swgemu.sql`; check `config.lua` credentials |
| New .cpp not compiled | CMake glob cached | Re-run `cmake -B build` (reconfigure), then rebuild |
| Tests crash on startup | MariaDB unreachable | Bring DB up; unit tests initialize MySQL client regardless of scope |
| Scripts not loading at boot | Wrong working directory | Run from `MMOCoreORB/bin/` |
| Engine includes unresolved | engine3 submodule not initialized | `git submodule update --init` |

---

---
## Related Documentation

- Module code reference & hooks: [../../customskills/CODE_REFERENCE.md](../../customskills/CODE_REFERENCE.md)
- Module install/remove: [../installation/INSTALLATION.md](../installation/INSTALLATION.md)
