# BRIEF-038 Design -- Mission Terminal Options Port (direction + difficulty)

- Status: DELIVERED (design; awaiting owner sign-off before implementation)
- Date: 082526 -- ox-alpha (opencode/x-preview-f-free)
- Spec source: cyvster2 @ origin/cyvster2 (behavior spec ONLY, not quality ref)
- Ledger basis: [../cyvster2/MIGRATION_LEDGER.md](../cyvster2/MIGRATION_LEDGER.md) M01-M05

## 1. Findings

### 1.1 What cyvster2 implemented (behavior spec)

**Difficulty option (ledger M01):**
- `MMOCoreORB/bin/scripts/screenplays/tools/mission_level_choice.lua` (84
  lines): SuiListBox titled "Mission level selection" offering Reset/Easiest(1)/
  Mid1(2)/Mid2(12)/Mid3(25)/High1(35)/High2(45)/High3(60)/Hard(135)/Hardest(200).
  Persisted via `writeScreenPlayData(pPlayer, "mission_level_choice",
  "levelChoice", N)`; system message confirms.
- Consumption site A -- cyvster2 `MissionManagerImplementation.cpp`
  `randomizeGenericDestroyMission` (~:800-817): reads
  `targetGhost->getScreenPlayData("mission_level_choice", "levelChoice")`,
  `Integer::valueOf(level)`, and if >0 does `diffDisplay += levelChoice`
  (skipping group-level calc). Upstream equivalent on cyvster3: diffDisplay
  block sits in `randomizeGenericDestroyMission` around :825-855.
- Consumption site B -- `getRandomLairSpawn` (~:1880-1905): if levelChoice >0,
  `playerLevel = levelChoice`, replacing player/group level in the lair
  difficulty window filter (`minLevel = min(playerLevel - 5, minLevelCeiling)`,
  acceptance test `minDiff <= playerLevel+5 && maxDiff >= minLevel`).

**Direction option (ledger M02):**
- `bin/scripts/screenplays/tools/mission_direction_choice.lua` (94 lines):
  SuiListBox with compass rose comment block; options Reset(-1),
  N=0, NE=315, E=270, SE=225, S=180, SW=135, W=90, NW=45. Persisted as
  `writeScreenPlayData(..., "mission_direction_choice", "directionChoice", deg)`.
- Consumption site C -- new free function `getMissionPosition(player, distance,
  angle)` (cyvster2 MissionManagerImplementation.cpp ~:768-777): converts
  compass degrees to world XZ via `newAngle = angle*(PI/180) + PI/2`.
- Consumption site D -- inside `randomizeGenericDestroyMission`'s position
  search loop (~:857-900): replaces upstream's
  `player->getWorldCoordinate(distance, System::random(360))`; when dirChoice
  >= 0, heading = chosen +/- `System::random(5)` degrees (sign flip when
  `System::random(100) > 40`, so ~60/40 minus/plus), wrapped at 0/360.

**Terminal wiring:** cyvster2 `MissionTerminalImplementation.cpp`
`handleObjectMenuSelect` radial IDs **112** ("Choose Mission Level") and
**113** (direction): each builds a Lua function via
`DirectorManager::instance()->getLuaInstance()` ->
`createFunction("mission_level_choice", "openWindow")` -> `callFunction()`
synchronously (:87-107). Radial entries added in `fillObjectMenuResponse`.

### 1.2 Correctness defects found in cyvster2 code

1. **Unguarded empty-string parse**: `Integer::valueOf(level)` /
   `Float::valueOf(dir)` run on ScreenPlayData that returns "" before the
   player's first choice. Core3 `Integer::valueOf("")` throws
   IllegalArgumentException; at minimum it parses to garbage. Every mission
   generation for a player who never opened the option menus hits this path.
   (Consumption sites A, B, D.)
2. **Reset-vs-unset ambiguity**: levelChoice stores 0 for Reset;
   `levelChoice > 0` treats unset and Reset identically -- acceptable, but the
   direction side stores -1 for Reset and tests `dirChoice >= 0`, so an unset
   "" parsed as -1 accidentally behaves like Reset only by luck of Float
   parsing. Fragile sentinel design; the port uses explicit presence checks.
3. **Angle convention fragility**: hand-rolled `angle + PI/2` trig was wrong
   twice in history (collaborator fix commits 608116821c, db6b0b6c56,
   off-by-90). The port should reuse the engine's proven
   `CreatureObject::getWorldCoordinate(dist, angleDeg)` which already handles
   the convention, applying only the heading override -- not re-derived trig.
4. **Deviation bias**: sign chosen by `random(100) > 40` gives 59% negative --
   harmless but sloppy; port uses symmetric `random(11) - 5`.

### 1.3 Stutter root-cause analysis

The owner remembers a stutter "when opening the options menu." The option
menus themselves are NOT expensive: one Lua function call building one small
SuiListBox (10 or 9 rows) costs on the order of our own C++ menu pages, plus
sub-100us Lua dispatch overhead. The real cost chain, all synchronous on the
zone executor:

1. **The follow-on action is the heavy one.** The menus instruct the player to
   "use the mission terminal" after choosing -- i.e., the stutter lands on the
   next `handleMissionListRequest`. That call regenerates the ENTIRE mission
   bag synchronously (cyvster3 `MissionManagerImplementation.cpp:195-228`):
   up to 12 general missions (24 on cyvster2 per M03) through
   `populateMissionList` (:554) -> `randomizeGeneralTerminalMissions` ->
   `randomizeGenericDestroyMission` per destroy slot.
2. **Per-mission cost is large and multiplies.** Each destroy mission runs
   `calculatePlayerLevel(player)` (:825), `getRandomLairSpawn`'s rejection
   sampling loop (up to list-size iterations, then two full linear fallback
   scans; lair-template lookups per candidate), template lookup +
   `isSharedTangibleObjectTemplate` validation, then a position-search while
   loop of up to **20 attempts**, each doing distance rolls, `getHeight`,
   `getWaterHeight`, `zone->isWithinBoundaries`, AND
   `getInRangeActiveAreas` + an active-area iteration for city exclusion
   (cyvster3 :869-905 region). With a 24-slot bag that is hundreds of terrain
   queries and area queries in one executor tick.
3. **Direction constraint makes it WORSE, not better**: constraining headings
   to a 10-degree wedge means boundary failures and city/water rejections
   cluster -- the 20-attempt loop exhausts far more often toward map edges
   (e.g. choosing North standing at the north edge), so each mission burns its
   full retry budget. This matches "stutter appeared after I added these
   options."
4. **Ambient cyvster2 contributors** (ledger R6.9 notes): CLOSEOBJECTRANGE
   192->512 (Q10) inflated every `getInRangeActiveAreas`-style query the
   mission generator itself performs, and account-shared lots (Q02) added
   unrelated fan-out stutters. Both amplify perception of any terminal hitch.
5. **Minor**: the two ScreenPlayData string reads + parses execute per mission
   generated (not per terminal open) -- measurable only in aggregate.

**Conclusion for the port**: the option UI must be trivially cheap (it can
be), and the design must NOT add work to the regeneration path beyond one
cached read. Any optional improvement to regeneration batching is out of
scope here but noted in Open Questions (#5).

## 2. Proposed Architecture

### 2.1 Hook strategy (no fork of MissionManager internals)

`MissionManager.idl` exposes NO virtual seams -- `populateMissionList`,
`randomizeGenericDestroyMission`, and `getRandomLairSpawn` are all `private
native`. Delegation therefore needs the established friend-hook pattern
(same as `CombatManager.h:24 friend class CustomSkillsCombat;`):

- `MissionManager.h` (generated from idl): add
  `friend class CustomSkillsMissions;` -- one-line touch, documented per house
  rule.
- `CustomSkillsMissions` (new mod unit, `customskills/missions/`) provides
  three static hooks called at minimal insertion points in
  `MissionManagerImplementation.cpp`:
  - `CustomSkillsMissions::overrideDifficulty(player, diffDisplay)` at the
    diffDisplay block (replaces consumption site A);
  - `CustomSkillsMissions::overridePlayerLevel(player, playerLevel)` in
    `getRandomLairSpawn` (site B);
  - `CustomSkillsMissions::overrideHeading(player, headingDegrees)` at the
    position-roll (site D) -- returns -1 for "no preference"; the engine's own
    `getWorldCoordinate(distance, heading)` is kept, avoiding re-derived
    trig entirely.
  Each hook early-returns in <50ns when the feature is disabled or the
  player has no stored choice.
- Terminal radials: extend `MissionTerminalImplementation::
  fillObjectMenuResponse` / `handleObjectMenuSelect` with the two options
  gated on config, calling `CustomSkillsMissions::showDirectionMenu(player)`
  etc. directly -- pure C++ SuiListBox construction, NO Lua on this path.
  Reuse cyvster2's radial IDs 112/113 (unassigned upstream) for spec parity.

### 2.2 Per-request caching (the performance fix)

ScreenPlayData reads happen today per mission generated. Instead:
- `populateMissionList` calls
  `CustomSkillsMissions::cacheChoices(player)` ONCE at entry: reads both
  ScreenPlayData keys, parses with empty-guards, and stores the result in a
  per-player transient holder (member map on the mod singleton keyed by
  player objectID, cleared at end of populateMissionList via RAII guard).
- The three hooks read the cache (hash-map lookup) instead of touching
  ScreenPlayData/OODB per mission.
- Correctness: choices only change via the option menus; a stale cache within
  a single populateMissionList call is impossible because menu selection
  cannot interleave (same player, same executor tick semantics guarded by the
  existing Locker crossLocker(missionTerminal, player)).

### 2.3 Config knobs (`bin/scripts/customskills/config.lua`, self-documenting)

```
missionsEnabled            -- master gate, default false (opt-in)
missionDirectionEnabled    -- show direction radial+SUI, default true when master on
missionDifficultyEnabled   -- show difficulty radial+SUI, default true when master on
missionDifficultyMaxTier   -- highest selectable tier index owners allow (cap Hard/Hardest)
missionDirectionDeviationDeg -- +/- degrees around chosen heading, default 5 (unit spelled out)
missionDefaultLevel        -- pre-selected tier shown in SUI, default Reset
-- commented REAL examples using actual keys per house standard
```

### 2.4 Player-facing UI (follows our SUI patterns)

- Terminal radial gains a "Mission Options" parent (or the two flat rows
  112/113, matching cyvster2 -- owner call, see Open Questions #1).
- Difficulty SUI: SuiListBox, ROWS not prompt-text (house rule), same ten
  tiers as cyvster2 but filtered/clamped by `missionDifficultyMaxTier`;
  current active choice pre-marked in its row text ("* Mid 2 (current)").
- Direction SUI: SuiListBox, 8 compass rows + Reset, current choice marked.
- Selection writes ScreenPlayData + confirmation system message (same copy as
  cyvster2, which is good UX copy).
- Budget: SUI construction identical to `/customskills` menu pages --
  measured 60-150us server CPU per BRIEF-033 methodology. Zero Lua dispatch,
  zero regeneration work, zero template/terrain queries on this path.

### 2.5 Persistence recommendation

| Option | Verdict |
|---|---|
| ScreenPlayData (cyvster2) | **RECOMMENDED.** Already persistent, keyed string get/set on PlayerObject (PlayerObject.idl:791-804), zero IDL/schema change, survives wipes of mod-specific storage, exactly what the spec used. Cost mitigated by 2.2 caching. |
| New PlayerObject persistent field | Rejected: IDL change + baseline shift for two small strings. |
| Mod-side tool storage | Rejected: reinvents persistence for no gain. |

Keys kept identical to cyvster2 (`mission_level_choice/levelChoice`,
`mission_direction_choice/directionChoice`) so returning players' existing
choices survive the migration transparently.

## 3. Mapping table (cyvster2 edit -> mod mechanism)

| cyvster2 | Mod mechanism | Touch |
|---|---|---|
| mission_level_choice.lua / mission_direction_choice.lua | C++ `CustomSkillsMissions` SuiListBoxes (no Lua runtime) | new mod files |
| MissionTerminalImplementation.cpp :87-107 radial handlers | Same radial IDs, direct C++ dispatch, config-gated | 2 small insertions |
| MissionManagerImplementation.cpp ~:807-817 diffDisplay += levelChoice | `friend class CustomSkillsMissions` + `overrideDifficulty()` hook | 1-line friend + 1-line call |
| ~:1896-1905 playerLevel = levelChoice | `overridePlayerLevel()` hook | 1-line call |
| ~:768-777 getMissionPosition() + :857-883 deviation logic | `overrideHeading()` feeding vanilla `getWorldCoordinate` | 1-line call; delete nothing upstream |
| ScreenPlayData per-mission reads | Once-per-populate cached read (2.2) | inside hook unit |

## 4. Performance statement

- Menu open path: radial select -> C++ SuiListBox build -> sendTo. Comparable
  to existing mod menus: **~60-150us** server CPU. No DirectorManager Lua
  instance, no screenplay lookup, no DB write on open (write happens on
  selection confirm only, one tiny ScreenPlayData set).
- Regeneration path delta vs vanilla: +1 hash lookup per mission (cached) and
  2 ScreenPlayData reads per terminal request total (vs cyvster2's 2 per
  MISSION). Feature-off path: three early-return branch checks per destroy
  mission, unmeasurable.
- Known residual: vanilla regeneration cost itself (Section 1.3 item 2) is
  untouched by this brief; flagged as follow-up candidate, not silently mixed
  in.

## 5. Open questions for owner sign-off

1. **Radial shape**: two flat rows (112/113, cyvster2 parity) or one
   "Mission Options" submenu containing both?
2. **Scope**: direction+difficulty apply to GENERAL terminals only (as
   cyvster2 effectively did via generic destroy missions), or also
   faction-terminal destroy missions?
3. **Tier list**: keep cyvster2's ten tiers verbatim, and what should
   `missionDifficultyMaxTier` default to (full list incl. Hardest=200)?
4. **Bag expansion (M03) and bounty x100 (M04)**: in-scope for the
   implementation brief or deferred?
5. **Regeneration cost follow-up**: want a separate micro-brief to batch/
   trim the destroy-position retry loop (bounded area-query caching), or
   accept vanilla behavior since the stutter diagnosis shows it predates the
   option menus?
6. Confirm ScreenPlayData key reuse (old cyvster2 characters keep their
   settings) is desired rather than a clean-slate namespace.
