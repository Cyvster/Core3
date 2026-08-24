# Explicit-path staging only; clean intended tree before add

**Proposed by:** ox-alpha (opencode/x-preview-f-free), 08232026
**Last updated:** 08232026 by ox-alpha (opencode/x-preview-f-free)
**Status:** ADOPTED -- Daniel (directive), 08232026
**Trigger:** Same-night collisions on the shared tree; framework source:
Project Alice proposal `08232026-clean-status-before-add` (ADOPTED there),
whose trigger incidents included a null-byte-corrupted rewrite and foreign
scratch files swept into commits by broad `git add -A`.

---

## 1. Governing conventions

- [../../rules/process.md](../../rules/process.md): R6.5, R6.7, R6.8.
- Disputes: [../rules/errata.md](../rules/errata.md).

## 2. Why this exists

Broad staging (`git add -A` / `git add .`) on a shared tree sweeps other
sessions' uncommitted state into your commit. On this repository tonight a
shared-tree race produced exactly that class of risk. Staging must be
deliberate.

## 3. Scope

| Covers | Excludes |
|--------|----------|
| All workers: explicit `git add <paths>` only | Coordinator pull discipline (companion proposal) |
| Pre-stage `git status --porcelain` check | CI enforcement tooling |
| Protecting others' uncommitted work | Per-worker topology |

## 4. The proposal

S1. Stage with explicit file paths (`git add <path> ...`). `git add -A`
    and `git add .` are prohibited on this shared tree.
S2. Before staging, run `git status --porcelain`. Entries must match the
    intended change only. Unexpected entries: STOP, identify, never delete
    or stash another session's files; report persistent strays via errata,
    naming the files and best-known owner.
S3. Applies to every worker and to any coordinator role alike.
S4. Sunset: relaxes to style preference when the per-worker topology
    proposal completes provisioning (cross-worker pollution becomes
    structurally impossible).

## 5. Integration plan

Phase 1 (this adoption): process.md gains a "Staging discipline" section;
commit tagged `[PROPOSAL-08232026-clean-status-before-add]`.

## 6. Relationship matrix

| Interacts with | Nature |
|----------------|--------|
| Coordinator-discipline proposal | Companion (pull half) |
| Per-worker topology | Sunset dependency |
| [PROC R4] | Failure route when unexpected entries appear |

## 7. Implementation notes

Note: several of this repository's own historical commits used `git add -A`
during single-worker sessions; going forward S1 applies without exception.

## 8. Revision log

| Date | Change | Source |
|------|--------|--------|
| 08232026 | Initial draft, adopted same day | Owner directive |
