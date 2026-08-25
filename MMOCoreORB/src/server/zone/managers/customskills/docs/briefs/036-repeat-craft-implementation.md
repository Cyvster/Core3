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
4. **Practice-mode handling -- plain operator option (owner directive)**:
   config knob `repeatAllowPractice` bool DEFAULT TRUE (matches vanilla,
   where players can already practice-craft repeatedly). No anti-farming
   intent or messaging.
5. `repeatEnabled` bool default false (whole feature off until operator opts in).
6. Experiment/customization: auto-apply snapshot allocations; if schematic
   changed or slots mismatch, discard snapshot with a notice.
6. WINDOW FLOW (owner requirement): the player STAYS IN THE SAME crafting
   window. Repeat must not bounce through tool/inventory windows: after the
   prototype is created, /repeatcraft (or a UI button if wireable) starts
   the next session pre-filled in-place -- craft, hit repeat, wait, hit
   repeat. No window hopping.
7. No rate cap (owner decision): dropped repeatMaxPerSession -- its only
   purpose was anti-farming pacing. Vanilla handles interruption cleanly
   (cancelSession at CraftingSessionImplementation.cpp:186 on every exit
   path; ingredients returned) and the snapshot lives outside session
   state, so closing the tool mid-craft never corrupts it. Stale snapshots
   (schematic/slot mismatch) are discarded with a notice.
8. Anti-abuse stance (owner directive): NO farming countermeasures. AFK
   crafting via macros already exists; repeat does not create a new class
   of abuse, just removes tedium.

## Deliverables

- [ ] Snapshot storage + /repeatcraft command + pre-fill flow implemented.
- [ ] Config knobs w/ self-documenting comments (house standard).
- [ ] CODE_REFERENCE.md updated (command, keys, lifecycle notes ref 035).
- [ ] USER_GUIDE.md player-facing blurb.
- [ ] Brace balance zero; no real names/local paths; commit `[BRIEF-036]`, push.
