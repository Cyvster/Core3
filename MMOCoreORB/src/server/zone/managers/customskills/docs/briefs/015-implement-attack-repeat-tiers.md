# BRIEF-015 -- Implement Double/Triple/Quad attack repeat tiers (resolves ERR-009)

- Status: DELIVERED by ox-alpha (opencode/x-preview-f-free), 08242026 (compile deferred -- engine3 caveat)
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive ("build it") resolving ERR-009 Option A.

## Problem

ERR-009: Double/Triple/Quad Attack Chance are loaded from config and
displayed in `/customskills`, but no combat code consumes them -- players
see bonuses that cannot proc.

## Design (agreed with owner)

- Implemented inside the EXISTING `CombatManager::applyDamage`
  delegation (`CustomSkillsCombat::applyDamage`) -- NO new SWGEmu hooks.
- After ALL calculations (crit included), run the sequential upgrade
  chain off the landed hit: roll Double -> on success roll Triple ->
  on success roll Quad. First failure ends the chain; each success
  adds one extra application of the same calculated hit.
- Triggered tier RESENDS the already-calculated hit through the retained
  `applyVanillaDamage` -- real applications, not a damage multiplier
  (owner design, revised from the original multiplication draft). Each
  application runs the full vanilla path (armor, native spam line,
  observers); sequence stops if the defender dies or is incapacitated;
  function returns the primary hit result.
- Effective chance per modifier = its capped config total (basis points
  of 10000), same convention as Critical Chance.
- Label-free: delivered after the owner's combat-spam-label removal
  decision ([BRIEF-014]); procs are visible as larger damage numbers
  only. Indicators are a possible future feature, not in scope.

## Deliverables

1. `combat/CustomSkillsCombat.cpp`: repeat-tier logic in `applyDamage`
   (int64-safe multiplication, applied after crit adjustment, before
   `applyVanillaDamage`). Uses `getModifierTotal` +
   `applyModifierCap` -- no new config fields.
2. Docs ([PROC R6.6]): CODE_REFERENCE.md Appendix A Offense entries gain
   an "Implemented" confirmation line each; ERR-009 resolved; errata log
   stamp updated.

## Acceptance criteria

- Tier selection verified by inspection against documented order
  (Quad -> Triple -> Double, single trigger).
- Config-driven chances respected (cap-clamped totals).
- No label/spam emissions introduced.
- Single commit tagged `[BRIEF-015]`, pushed. Compile deferred
  (engine3 caveat).
