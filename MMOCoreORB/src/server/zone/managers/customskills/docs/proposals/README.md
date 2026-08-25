# Proposals -- Guide & Index

How proposals are written, reviewed, decided, and archived here. Adapted
from the Project Alice proposals framework (read-only reference:
`G:\Data\Projects\project alice\github\alice\docs\proposals\README.md`). Any
contributor may author a proposal; only Cyvster adopts or rejects one.

**If you were told to work on proposal `<MMDDYYYY>-<short-name>`**: open
that folder, read `proposal.md`, then follow the Reviewer quick start.

**If you were told to draft a proposal**: follow the Author quick start.

## Author quick start

1. Read the workspace-root `START-HERE.md` fully first.
2. Check the index below -- if an OPEN proposal covers the same change,
   work on that one instead; never duplicate.
3. Create the folder: `docs/proposals/<MMDDYYYY>-<short-name>/`
   (short-name = 1-4 words, lowercase, dash-separated).
4. Write `proposal.md` inside it (always that exact filename): metadata
   block, governing-conventions citations ([PROC R6.7] -- reference,
   never restate), motivation with evidence, scope table, numbered body
   for citation, integration plan, relationship matrix, implementation
   notes, revision log.
5. Set Status: DRAFT while writing; UNDER REVIEW when ready for feedback.
6. Conventions: ASCII only (`--`, `->`); MMDDYYYY dates; Central Time;
   lines under ~100 characters; mechanical lists over prose.
7. Commit AND push immediately under your own identity, message
   `Propose <short-name>` ([PROC R6.5], [PROC R8]).
8. Add the index row at the bottom of this file; update it as status
   changes.
9. Disputes go through [../rules/errata.md](../rules/errata.md) -- never
   silent workarounds.

## Reviewer quick start

1. Read the workspace-root `START-HERE.md` fully first.
2. Read the proposal's `proposal.md` fully, plus earlier feedback files --
   endorse or counter by reference instead of restating ([PROC R6.7]
   spirit).
3. Write ONE feedback file: `feedback/<MMDDYYYY>_<short-id>.md`
   (short-id = your display name lowercased, `[a-z0-9]`, max 12 chars):
   metadata, overall assessment + recommendation, numbered issues
   (issue -> evidence -> recommendation) tagged HIGH/MEDIUM/LOW, open
   questions, summary table, signed.
4. Feedback is advisory except Cyvster's. Report findings -- do not edit
   `proposal.md`; the author incorporates feedback.
5. Commit and push (`Feedback on <short-name>`).
6. Disputes go through errata.

After review settles: author revises (version bump + revision-log rows
citing feedback sources), then Cyvster records the outcome in
`decision.md`.

## Scope

A proposal asks the project to CHANGE a rule/process, design decision,
governance mechanism, or structure. NOT proposals: bug reports (errata),
task delegation (briefs), research notes (temp/). Broken -> errata. Known
work -> brief. New or changed-how-we-work -> proposal.

## Folder layout

```
docs/proposals/<MMDDYYYY>-<short-name>/
|-- proposal.md                      <- living document (always this name)
|-- feedback/
|   |-- <MMDDYYYY>_<short-id>.md   <- one file per reviewer per round
|-- decision.md                      <- created when Cyvster decides
```

## Lifecycle

```
DRAFT -> UNDER REVIEW -> REVISED -> (loop) ->
ADOPTED | REJECTED | WITHDRAWN | SUPERSEDED
```

Feedback files are append-only once submitted. Every ignored HIGH item
gets a rebuttal line in the revision log. Errata remains the channel for
content disputes during review.

## Index

One row per proposal folder, newest last.

| Folder | Title | Status |
|--------|-------|--------|
| [`08232026-remote-push-required/`](08232026-remote-push-required/proposal.md) | Push after every logical commit; claims are invisible until pushed | ADOPTED 08232026 |
| [`08232026-clean-status-before-add/`](08232026-clean-status-before-add/proposal.md) | Explicit-path staging only; never `git add -A` on this tree | ADOPTED 08232026 |
| [`08232026-coordinator-pull-before-commit/`](08232026-coordinator-pull-before-commit/proposal.md) | Sync before commit; attribute others' work | ADOPTED 08232026 |
| [`08232026-per-worker-topology/`](08232026-per-worker-topology/proposal.md) | Per-worker clone/worktree policy (provisioning pending) | POLICY ADOPTED 08232026 |

---

**Last reconciled:** 08232026 by ox-alpha (opencode/x-preview-f-free) --
subsystem created, four coordination proposals adopted via owner directive
(see each decision.md).

## Contributors

- **Cyvster** -- project owner; adopts/rejects proposals
- ox-alpha (opencode/x-preview-f-free), 08232026 -- subsystem creation,
  coordination proposal set
