# Brief 036 -- Repeat-craft: assisted pre-fill implementation

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)
- Depends on: BRIEF-035 findings (_035_findings.md) -- READ FIRST

## Goal

Implement the assisted pre-fill repeat-craft per 035's recommended design:
snapshot a RepeatRecipe after a successful craft; on player request, start
a fresh crafting session and auto-select/auto-fill it from the snapshot,
leaving the normal client window open for the player to confirm.

## Behavior spec

1. **Snapshot**: on successful `createPrototype` (session completes), store
   on the CraftingTool (or PlayerObject -- pick lowest-risk, document):
   schematic CRC/template path, per-slot resource names + quantities,
   experiment row/point pairs, customization string. One slot per tool;
   overwritten each successful craft.
2. **Repeat trigger**: new command `/repeatcraft` (or tool radial option if
   trivially wireable; command is safer). Requires: target tool in
   inventory with a stored snapshot, player near/using the tool.
3. **Session pre-fill flow** (per 035 findings): server starts the session
   via the RequestCraftingSequence, auto-selects snapshot schematic, then
   auto-fills each ingredient slot from live inventory -- RE-VALIDATING:
   any missing/insufficient resource leaves that slot empty and reports
   which resource is short (@system message); craft proceeds only if all
   slots fill or player fills manually.
4. **Practice-mode exclusion = SERVER OPERATOR OPTION (owner directive)**:
   config knob `repeatAllowPractice` bool DEFAULT FALSE. When false, a
   repeat of a practice-mode craft is refused with a clear message; when
   true, practice repeats are permitted (operator accepts farming risk).
   The knob must be documented in config comments with the farming-risk
   explanation spelled out.
5. Additional knobs: `repeatEnabled` bool default false (whole feature off
   by default until operator opts in); `repeatMaxPerSession` int default
   0 (=unlimited; positive caps consecutive repeats before requiring a
   fresh manual craft).
6. Experiment/customization: auto-apply snapshot allocations; if schematic
   changed or slots mismatch, discard snapshot with a notice.
7. Anti-abuse stance (owner directive): NO farming countermeasures. AFK
   crafting via macros already exists; repeat does not create a new class
   of abuse, just removes tedium. Do not add rate-limits beyond
   repeatMaxPerSession.

## Deliverables

- [ ] Snapshot storage + /repeatcraft command + pre-fill flow implemented.
- [ ] Config knobs w/ self-documenting comments (house standard).
- [ ] CODE_REFERENCE.md updated (command, keys, lifecycle notes ref 035).
- [ ] USER_GUIDE.md player-facing blurb.
- [ ] Brace balance zero; no real names/local paths; commit `[BRIEF-036]`, push.
