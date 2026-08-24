# BRIEF-014 -- Remove the combat spam label feature from code and documentation

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Supersedes BRIEF-010/013 enhancement plans: same-line labels
  are not achievable without breaking player client settings (BRIEF-010
  Phase 1 evidence), and the owner has elected full removal.

## Remove from module C++

1. `combat/CustomSkillsCombat.cpp::applyDamage`: delete the entire label
   emission block (`isCombatSpamLabelsEnabled` check,
   `getCriticalCombatSpamLabel`, `sendCustomCombatSpam` call). The crit
   damage multiplier logic above it stays untouched.
2. `CustomSkillsConfig.h/.cpp`: remove
   - `combatSpamLabelsEnabled` member, getter, default, and load line
   - `criticalCombatSpamLabel` member + getter and its load line
   - the `modifierCombatSpamLabels[]` array, its per-modifier load in
     `loadModifier`, `setDefaults` initialization, and
     `getModifierCombatSpamLabel()`
3. **SWGEmu patch files**: remove `sendCustomCombatSpam` from
   `CreatureObject.idl` + `CreatureObjectImplementation.cpp` (module-
   added via hooks; no other callers). Regenerate
   `integration/core3-hooks.patch`.

## Remove from config.lua

- Root field `combatSpamLabelsEnabled`
- Every `combatSpamLabel = "(...)"` entry (criticalChance +
  doubleAttackChance/tripleAttackChance/quadAttackChance)

## Remove from documentation

1. `docs/customskills/CODE_REFERENCE.md` Appendix A: drop the combat-spam
   property row from every modifier section; note in the Critical Chance
   history paragraph that the label feature was removed (owner decision
   after BRIEF-010).
2. `docs/customskills/USER_GUIDE.md`: remove all four "Combat spam"
   bullet lines.
3. `docs/installation/INSTALLATION.md`: remove
   `combatSpamLabelsEnabled` from the root-settings example and the
   separate-message clause in the verification step.

## Acceptance criteria

- Repo-wide grep clean: `combatSpamLabel`, `CombatSpamLabel`,
  `sendCustomCombatSpam`, `combatSpamLabelsEnabled` return zero hits in
  module code/config/docs (errata/archive historical records excepted).
- Crit damage math unchanged -- only the label emission disappears.
- Patch regenerated; MANIFEST integration list updated if file set
  changed.
- Single commit tagged `[BRIEF-014]`, pushed. Compile deferred to Docker
  build env (documented).

## Out of scope

- ERR-009 (Double/Triple/Quad implementation-vs-removal) -- separate
  owner decision in progress; this brief does not touch their config
  entries or menu rows beyond the spam-label lines named above.
