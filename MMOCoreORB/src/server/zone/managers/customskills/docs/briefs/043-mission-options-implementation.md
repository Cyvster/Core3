# Brief 043 -- Mission terminal options: implement direction/difficulty per _038_design

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)
- Depends on: `_038_design.md` (research) + OWNER DECISIONS recorded there
  (read both first). R6.11 applies: delegation only, NO shadowing
  MissionManager (2,396 lines, upstream-active).

## Implementation spec (from approved design)

1. **Core3 delegation points** (minimal, document each):
   - MissionManager.h: `friend class CustomSkillsMissions;`
   - Three hook sites in MissionManagerImplementation.cpp:
     a. diffDisplay override (~design doc cites the cyvster2 equivalents;
        find current-line equivalents in cyvster3 file)
     b. lair-spawn level override in getRandomLairSpawn path
     c. destroy-mission placement heading wedge -- call
        `CustomSkillsMissions::getMissionPosition(...)` which returns vanilla
        position when no direction chosen, else ±5 deg wedge around it.
   - Mission-list size cap: replace hard-coded `missionCount >= 3` with
     config-driven value (vanilla default 3).
   - Descriptive title: at mission creation, if enabled, set
     "CL<diffDisplay> Destroy the <mobileName>" (cyvster2 :971 pattern).
   Each touch is 1-5 lines delegating to mod code; no logic moves into Core3.

2. **Mod files**: new `customskills/missions/CustomSkillsMissions.{h,cpp}`:
   - `getDifficultyOverride(player)`, `getLevelOverride(player)`,
     `getMissionPosition(...)`, `shouldUseDescriptiveTitles()`,
     `getMissionListSize()` etc.
   - Choice persistence: ScreenPlayData with cyvster2's exact keys
     (`mission_level_choice/levelChoice`, `mission_direction_choice/
     directionChoice`) so returning players keep settings.
3. **UI**: two SuiListBoxes opened from terminal radials (two flat rows on
   mission terminals, general + faction): "Mission Direction" and "Mission
   Difficulty". Pure C++ SUI construction -- NO Lua screenplays on this path
   (cyvster2's Lua layer + per-open ScreenPlayData reads were part of its
   jank). Cache tier list statically; choices read once per populate, not
   per mission.
4. **Config** (`bin/scripts/customskills/config.lua`, `missions` table,
   self-documenting house style):
   - `missionOptionsEnabled` bool default true (master switch)
   - `directionOptionEnabled` bool default true
   - `difficultyOptionEnabled` bool default true
   - `missionListSize` int default 3 (vanilla); owner sets higher
   - `descriptiveTitles` bool default true
   Ten-tier list verbatim from cyvster2/design doc; difficulty max tier =
   highest defined tier (200 / Hardest).
5. **Faction terminals**: same radials/options as general (owner decision).
6. **Performance requirements** (BRIEF-038 stutter analysis): options UI open
   <= ~60-150us server CPU; no per-mission ScreenPlayData reads during
   generation (read cached choice once per populate); no unbounded retry
   loops added.

## Docs + cleanup

- CODE_REFERENCE.md: missions section (hooks, keys, tiers, config).
- USER_GUIDE.md: player-facing how-to.
- MIGRATION_LEDGER.md: mark mission items IN-MOD.
- Errata: none expected unless implementation reveals defects.

## Verify + deliver

Brace balance zero all touched files; no real names/local paths; every Core3
touch listed in delivery note; commit `[BRIEF-043]`; push origin cyvster3.
Final response: files touched (mod vs Core3), config keys, UI flow summary,
any design deviation with reason.
