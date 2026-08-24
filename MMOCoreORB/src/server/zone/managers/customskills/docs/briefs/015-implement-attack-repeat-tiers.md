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
- After the critical-hit roll, roll repeat tiers highest-first:
  Quad -> Triple -> Double. Only ONE tier triggers per hit.
- Triggered tier multiplies the incoming damage by 2 / 3 / 4. Because
  armor mitigation is proportional, multiplying pre-mitigation damage is
  equivalent to repeating the finalized hit; matches documented
  "repeats finalized damage" semantics and composes with crit
  ("repeats apply to critical-adjusted damage").
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
