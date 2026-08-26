# Brief -- Declutter command guides: move nonfunctional commands to a bottom section

- Status: OPEN
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)

## Problem

`docs/swgemu/ADMIN_COMMANDS.md` (and the player-commands reference when
BRIEF-022 delivers) mix working and nonfunctional commands in one flow.
Operators scanning for usable syntax must mentally filter out broken/vestigial
entries. Nonfunctional commands are still worth documenting (they exist in
the command table and confuse admins who try them), but they do not belong
interleaved with functional ones.

## Task

1. Review ADMIN_COMMANDS.md entries against the codebase: classify each as
   FUNCTIONAL or NONFUNCTIONAL (nonfunctional = registered but no-op, broken,
   GM-only stubs that crash, etc. -- use code inspection, not assumptions).
2. Restructure the doc: functional commands stay in the main body organized
   by category; ALL nonfunctional commands move to a single clearly-labeled
   section near the bottom ("Nonfunctional Commands" or similar), each with
   its reason (no-op, disabled upstream, requires missing system, ...).
3. Same treatment applies to BRIEF-022 player commands output when it lands.
4. House style throughout; ASCII only; keep every existing file:line cite.

## Deliverables

- [ ] ADMIN_COMMANDS.md restructured, classification per entry.
- [ ] Note added to BRIEF-022 authoring guidance to follow the same split.
- [ ] Commit `[BRIEF-045]`, push.
