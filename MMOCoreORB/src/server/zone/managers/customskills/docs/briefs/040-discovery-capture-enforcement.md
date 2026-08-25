# Brief 040 -- Discovery-capture enforcement review: why R6.9 leaks, and plugging every gap

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Trigger: owner observed swgemu/CODE_REFERENCE.md went ~5 hours without
  updates during a heavy code-dig session. R6.9 exists but leaks.

## Problem statement

R6.9 (discovery capture) is documented in process.md and mirrored in brief
authoring rules -- yet today's session produced discoveries that never got
captured anywhere: the QueueCommand result-code constants and where they
live; the IDL global-vs-namespace forward-decl ambiguity trap (cost a build
failure); CombatSpam color-byte semantics (only partially captured via 034's
section); SUI prompt-text vs list-row rendering behavior (caused a "blank
page" defect). Each was learned the hard way and lives only in git commit
messages, if anywhere.

Root causes to investigate (verify each in the actual texts):

1. **Rule lives only at brief-authoring time.** Workers executing briefs
   see the brief's own capture line, but nothing forces capture at DELIVERY
   time. A worker that discovers something mid-task but whose brief didn't
   anticipate it has no trigger.
2. **No discovery sink for non-brief work.** Hotfixes (like today's two
   compile fixes) have no brief, no checklist item, no reminder -- facts
   die with the session.
3. **Placement ambiguity**: workers weren't sure which doc owns which fact
   (customskills vs swgemu CODE_REFERENCE; CONFIG_OPTIONS; MIGRATION_LEDGER).
4. **No verification hook**: delivery checklists ask "did you capture?" but
   nobody re-checks "is anything uncaptured?" at reconciliation.

## Research tasks

1. Audit every place R6.9 is mentioned (process.md, LLM_GUIDE, briefs
   README, Alice counterparts) and map WHERE in the workflow each mention
   fires. Identify dead spots (steps with no R6.9 presence).
2. Review today's session as a case study: list discoveries made vs
   captured (the four above are known misses; find others by reading git
   log + hotfix diffs).
3. Design enforcement points. Candidates (evaluate all, adopt what fits):
   - **Delivery checklist addition**: "list discoveries made this task;
     each has either a doc cite or 'none'" -- mandatory line in every
     delivery report.
   - **Hotfix rule**: any fix requiring a second attempt or root-cause
     diagnosis MUST append its lesson to a reference doc in the same
     commit.
   - **Coordinator duty**: before marking a brief DELIVERED, coordinator
     greps the diff for new knowledge (new packet fields, new constants,
     new gotchas) and checks they landed in docs.
   - **Discovery sink**: a lightweight docs/discoveries.md inbox for
     facts that don't yet have an obvious home; swept into proper docs at
     reconciliation.
4. Update the owning documents with the adopted mechanisms.

## Deliverables

- [ ] Findings: workflow map of where R6.9 fires/doesn't + case-study list
      of uncaptured discoveries from 08252026.
- [ ] Adopted enforcement changes implemented in process.md, LLM_GUIDE,
      briefs README (and Alice mirror per XPP if policy-level).
- [ ] Backfill capture: the four known missed facts -> correct reference
      docs (dense format, file:line cites).
- [ ] Commit `[BRIEF-040]`, push.
