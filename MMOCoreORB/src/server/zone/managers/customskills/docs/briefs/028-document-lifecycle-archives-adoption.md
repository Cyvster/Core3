# BRIEF-028 -- Adopt Alice-style document lifecycle archives (work queues)

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive:
  review Project Alice's archival implementation for adoption here.
- Priority: HIGH -- same failure mode Alice hit is active here: our
  errata.md and briefs index grow unbounded and already cost significant
  context per session load.

## Reference implementation

Project Alice brief 041 (document-lifecycle-archives, DELIVERED) + its landing in
Alice's process.md (policy-procedure). Key design:

- **Living ledgers stay small**; closed material moves verbatim to a
  permanent sibling archive. Nothing deleted, storage location changes only.
- **L1 Tombstones**: active file keeps a one-line pointer
  (`ERR-014 -- ... RESOLVED -> see errata-archive.md`); citation numbers
  remain valid everywhere.
- **L2 Verbatim moves**, append-only archive under dated headers.
- **L3 Archive self-index** (id/title/date table at top of archive).
- **L4 Trigger discipline**: archival happens only at reconciliation time,
  in the same commit as the stamp update; never mid-task.
- **L5 Worker guidance line** in quick starts.
- **L6 Verification blocks travel with their entry.**
- Soft-cap registry: errata 60 KB, index files 40 KB, ledger 30 KB.

## Our exposure (current state, verify at execution time)

| Living file | Today | Growth |
|---|---|---|
| `docs/rules/errata.md` | ~14 entries but long-form (ERR-030 style blocks are huge) | every session |
| `docs/briefs/README.md` | 27 index rows + protocol | every brief |
| `docs/proposals/README.md` | growing | every proposal |
| `docs/tracking/objectives.md` | moderate | periodic |

We are smaller than Alice was at adoption, which is the argument for
adopting NOW while the split is cheap (Alice adopted at ~90 KB errata).

## Task

1. **Review Alice's landed rules in its process.md** (not just the brief)
   for anything learned post-delivery; note deltas worth carrying.
2. **Adapt the registry to our files**: map each living doc to an archive
   target + soft cap. Proposed start:
   - `docs/rules/errata.md` -> `docs/rules/errata-archive.md`, cap 60 KB
   - `docs/briefs/README.md` -> `docs/briefs/archive.md`, cap 40 KB
     (index rows move; brief FILES stay put -- they are already per-file)
   - `docs/proposals/README.md` -> `docs/proposals/archive.md`, cap 40 KB
   - `docs/tracking/objectives.md`: evaluate (accrual vs maintained)
3. **Write rules L1-L6 into `docs/rules/process.md`** adapted to our
   numbering (new [PROC R#.#]) with our citation conventions.
4. **Add the worker guidance line** to LLM_GUIDE session protocol and
   errata worker quick start.
5. **Do NOT perform a retroactive split yet** unless a file already exceeds
   its cap; otherwise the rule activates at next reconciliation.
6. R6.9: record adoption decision reference in the docs README.

## Deliverables

- [ ] Registry + L1-L6 rules in process.md (our numbering).
- [ ] Guidance lines added (LLM_GUIDE step or note; errata quick start).
- [ ] Delivery report: current sizes vs caps, any immediate splits performed,
      deltas taken from Alice's post-delivery learnings.
- [ ] Commit tagged `[BRIEF-028]`; pushed.
