# BRIEF-040 Delivery — Discovery-Capture Enforcement

## Workflow map: where R6.9 fires vs leaks

| Workflow point | R6.9 presence before 040 | After 040 |
|---|---|---|
| Brief authoring | Yes (authoring rules) | Unchanged |
| Worker execution | Only if brief's own capture line anticipated it | Unchanged (inherent limit) |
| Delivery report | No trigger | MANDATORY discovery line w/ doc cites or "no discoveries" |
| Hotfixes (no brief) | Nothing | Same-commit lesson capture required |
| Coordinator signoff | No check | Grep diff for undocumented new knowledge |
| Reconciliation | Ad hoc | Integrity-audit cadence covers |

## Case study: discoveries leaked on 08252026 (all since backfilled)

1. QueueCommand result codes are class members, not globals
   (commands/QueueCommand.h:60-61) -- cost a build failure.
2. IDL classes must never be forward-declared globally in mod headers;
   include the real header instead -- cost a build failure across every TU
   including CombatManager.h.
3. Vanilla hit-location flytext stacks at the same anchor as any mod flytext;
   escalated text must suppress vanilla or render alone.
4. CombatManager has two applyDamage overloads; player-vs-TANO took an
   unhooked path (fixed via applyTanoTargetDamage).
5. SUI prompt-text-only pages read as blank in-game; use listbox rows.

## Enforcement changes implemented

- process.md -> Discovery-capture enforcement section: hotfix rule +
  coordinator duty.
- briefs/README.md pickup protocol step 8: mandatory [R6.9] delivery line.
- Backfill commits: d13d168bd5 (4 discoveries), 6fca01895d (CombatManager
  integration surface).

## Remaining

- LLM_GUIDE pointer to the enforcement rules (added this commit).
- Alice mirror brief per XPP (policy-level): filed separately.
