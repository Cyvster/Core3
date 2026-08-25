# Brief 038 -- Mission terminal options: port direction/difficulty features from cyvster2

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Depends on: BRIEF-037 (cyvster2 inventory) -- read its ledger entries for
  the missions/area first; this brief implements what they document.

## Goal

Port the mission-terminal enhancements the owner built directly into
cyvster2 -- notably player-facing options for mission DIRECTION and
DIFFICULTY -- into the Custom Skills mod as proper mod architecture.

## CRITICAL CONTEXT: original implementation quality

The owner reports the cyvster2 mission options caused a noticeable STUTTER
when opening the options UI. The original implementation is a SPECIFICATION
of intended behavior only -- NOT a quality reference. The port must use best
practices and treat performance as a first-class requirement.

## Research phase (do this first, deliver findings before code)

1. Locate cyvster2's mission modifications: diff fork-base..cyvster2 for
   managers/mission/, MissionManager, mission terminal SUI/packet handling,
   and any mission-related lua. Identify exactly what "direction option" and
   "difficulty option" did (owner description: players could influence the
   direction and difficulty of missions from the terminal).
2. Determine how the mod can hook equivalent behavior WITHOUT forking
   MissionManager directly (our pattern: delegation/friend hooks like
   CustomSkillsCombat). Check how missions generate targets/distance/reward.
3. Design the mod surface:
   - Config knobs (server owner): enable per-option, difficulty range caps,
     reward multipliers if part of it.
   - Player-facing UI: how the option is presented (terminal menu rows?
     a mod command? extend existing SUI?). Follow our existing SUI patterns.
4. PERFORMANCE REQUIREMENTS (owner directive): the port must eliminate the
   original stutter. Requirements for the design:
   - No synchronous heavy work on terminal-open / SUI-build path.
   - Precompute or cache what can be cached (mission templates, option
     lists); invalidate properly rather than recompute per open.
   - Move anything slow off the zone executor (use task callbacks).
   - Budget: opening the options UI should cost no more than our menu pages
     (~60-150us server-side, per BRIEF-033 methodology).
   - Document in _038_design.md what the original did that caused the
     stutter (file:line on cyvster2) and how the port avoids it.
5. Write `_038_design.md` with the mapping table (cyvster2 edit -> mod
   mechanism), the performance analysis, and get owner sign-off BEFORE
   implementation if any design choice is ambiguous.

## Implementation phase (after research; may be split to its own brief)

5. Implement hooks + config + UI per approved design, self-documenting
   config per house standard.
6. Docs: CODE_REFERENCE (mechanism + keys), USER_GUIDE (player-facing).
7. Update MIGRATION_LEDGER.md status column (BRIEF-037's doc): missions
   items -> IN-MOD.
8. Brace balance zero; commit `[BRIEF-038]`, push.

## Constraints

- No direct edits outside mod-owned files unless a hook genuinely requires
  touching Core3 code (document any such touch in delivery).
- Owner values these features ("pretty cool things in there") -- preserve
  their spirit even if implementation differs mechanically.
