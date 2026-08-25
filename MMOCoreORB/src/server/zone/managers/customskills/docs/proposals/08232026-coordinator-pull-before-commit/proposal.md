# Sync before commit; attribute others' work

**Proposed by:** ox-alpha (opencode/x-preview-f-free), 08232026
**Last updated:** 08232026 by ox-alpha (opencode/x-preview-f-free)
**Status:** ADOPTED -- Cyvster (directive), 08232026
**Trigger:** Shared-tree race conditions tonight (errata file clobber;
duplicate BRIEF-004 delivery). Framework source: Project Alice proposal
`08232026-coordinator-pull-before-commit` (ADOPTED there).

---

## 1. Governing conventions

- [../../rules/process.md](../../rules/process.md): R5, R6.5, R6.7, R6.8.
- Disputes: [../rules/errata.md](../rules/errata.md).
- Companions: clean-status-before-add (staging half), per-worker-topology
  (structural successor).

## 2. Why this exists

Anyone committing on the shared tree without syncing first can publish over
another session's just-landed work or sweep its uncommitted state. Push
discipline makes your OWN work visible; it cannot protect you from others'
landed-but-unpulled commits. The committer needs mechanical ordering.

## 3. Scope

| Covers | Excludes |
|--------|----------|
| Ordered sync before any commit on the shared tree | Per-worker topology |
| Explicit-path staging (restates S1 reference) | CI tooling |
| Original-author attribution when committing others' work | Rewriting history |

## 4. The proposal

C1. Before ANY commit on this tree, in ORDER:
    1. `git status --porcelain` -- entries beyond the intended change:
       STOP and reconcile per the staging-discipline proposal.
    2. `git fetch origin`, then integrate: prefer `git pull --ff-only`
       when strictly behind with no divergence; `git rebase origin/<base>`
       only when divergence requires it. Never commit on a tree that is
       behind origin.
    3. Stage explicit paths; commit ([PROC R6.8] tag required).
C2. When a commit integrates work authored by another session/contributor,
    the commit body carries:
    `Original-author: <name/model> (<origin>), <MMDDYYYY>`
C3. These duties apply to whoever commits -- there is no permanent
    coordinator caste on this repository; the role attaches to the act.

## 5. Integration plan

Phase 1 (this adoption): process.md gains a "Commit & sync discipline"
section; commit tagged
`[PROPOSAL-08232026-coordinator-pull-before-commit]`.

## 6. Relationship matrix

| Interacts with | Nature |
|----------------|--------|
| Remote-push proposal | Two halves of one visibility loop |
| Staging discipline | C1 step 1 references it |
| Per-worker topology | Structural successor; C-rules relax at E-sunset |

## 7. Implementation notes

Executed at adoption.

## 8. Revision log

| Date | Change | Source |
|------|--------|--------|
| 08232026 | Initial draft, adopted same day | Owner directive |
