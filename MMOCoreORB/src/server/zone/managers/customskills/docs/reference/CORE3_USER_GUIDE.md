# Core3 User Guide (Build, Run, Test)

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Purpose

Operational guide for this Core3 repository: building,
configuring, running, and testing. For code structure and patterns, see
[CORE3_CODE_REFERENCE.md](CORE3_CODE_REFERENCE.md).

All paths below are relative to the Core3 checkout root unless noted.
Factual corrections go through [../rules/errata.md](../rules/errata.md).

---

## Contributors

- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- initial guide;
  testing-infrastructure findings verified against source 2026-08-23
  (citations inline)

---

## Repository Layout (operational view)

```
Core3/
├── MMOCoreORB/          # Server sources, scripts, sql schemas, CMake
├── docker/              # Dockerfile + build.sh/run.sh (containerized dev)
├── linux/bootstrap.sh   # Native Debian dependency bootstrap
└── wsl2/                # WSL2 setup notes
```

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

## Related Documentation

- Code structure & patterns: [CORE3_CODE_REFERENCE.md](CORE3_CODE_REFERENCE.md)
- Module hooks & verification matrix: [ARCHITECTURE.md](ARCHITECTURE.md)
- Module install/remove: [../installation/INSTALLATION.md](../installation/INSTALLATION.md)
