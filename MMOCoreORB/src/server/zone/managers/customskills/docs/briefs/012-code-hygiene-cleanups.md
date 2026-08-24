# BRIEF-012 -- Code hygiene: enum/table safety, loader consistency, edge-case cleanups

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), produced by
  the BRIEF-008 deep audit (AUDIT-3/4/5); split from the original
  consolidated remediation brief. All items are behavior-preserving or
  edge-only; no balance changes.

## Phase B -- robustness (AUDIT-3)

1. `CustomSkillsMenu.cpp getTitle()`: titles[] is indexed by
   `(int)page` with no guard. Add a static_assert equating array length
   to the Page sentinel count and bounds-guard returning "Custom Skills"
   on overflow.
2. `CustomSkillsModifiers.cpp getModifierName()`: same static_assert
   against `CustomSkillsModifierType::COUNT`.
3. `getParent()` relies on enum ordering via range comparisons; replace
   with an explicit parent-map initializer OR add a compile-time check +
   comment justifying the range reliance.

## Phase C -- consistency/craft (AUDIT-4/5)

4. Config loader consolidation: unify criticalChance's special-cased
   loading with `loadModifier` helper semantics where practical;
   reconcile divergent negative-cap handling (generic path clamps to 0,
   critical path silently ignores).
5. Replace the magic `20000` defaultBadgeBonus for gatheringQuantity
   (CustomSkillsConfig.cpp load call) with a named constant -- or move
   the default into config.lua entirely and pass 0.
6. Document in one comment line that badgeOverrides entries may
   introduce keys absent from badges[] (loader behavior, by design).
7. Edge cleanups: `divideDuration` treats 0-second input as 1s output --
   decide and document intended behavior; `shouldDegradeWeapon`
   mixes random(100)/random(9999) granularity between native and
   modified paths -- unify to random(9999).

## Acceptance criteria

- static_asserts present at both array sites.
- No behavioral change from items 1-3 and 6; item 7's duration-edge
  decision documented either way; degrade-roll unification is
  statistically neutral at equal percentages (state the equivalence).
- Loader consolidation (item 4-5) preserves all current config.lua
  semantics exactly -- verified by re-reading against the shipped file.
- Compile verified or deferral documented (engine3 submodule caveat).
- Single commit tagged `[BRIEF-012]`, pushed.
