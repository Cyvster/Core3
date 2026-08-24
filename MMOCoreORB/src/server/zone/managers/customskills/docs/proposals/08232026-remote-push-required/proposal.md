# Push after every logical commit; claims are invisible until pushed

**Proposed by:** ox-alpha (opencode/x-preview-f-free), 08232026
**Last updated:** 08232026 by ox-alpha (opencode/x-preview-f-free)
**Status:** ADOPTED -- Daniel (directive), 08232026
**Trigger:** Two same-night collisions on this repository's shared tree:
(1) an errata-duty pass was written against a stale snapshot and raced a
concurrent session's commit to `docs/rules/errata.md`; (2) BRIEF-004 was
claimed in a brief FILE but never committed, so a parallel session could
not see the claim, delivered the same brief independently, and the
duplicate work had to be discarded (`git checkout`). Framework source:
Project Alice proposal `08222026-remote-push-required`
(`G:\Data\Projects\project alice\github\alice\docs\proposals\`, ADOPTED there).

---

## 1. Governing conventions

- Process authority: [../../rules/process.md](../../rules/process.md) --
  R5 (claims are commitments), R6.5 (record work where it lands),
  R6.8 (traceability tags). Reference, never restate.
- Correction channel: [../rules/errata.md](../rules/errata.md).

## 2. Why this exists

An uncommitted or unpushed claim is invisible to every other worker. Both
trigger incidents trace to work-or-claims that existed only in one
session's local state. Alice adopted the identical rule for the identical
reason ("a claim that is not pushed is invisible and may be re-claimed").

## 3. Scope

| Covers | Excludes |
|--------|----------|
| `git push` required after EVERY logical commit | Per-worker topology (separate proposal) |
| Claim commits pushed immediately at claim time | CI visibility guard tooling |
| Briefs pickup steps updated to include push | Re-attributing past local-only commits |

## 4. The proposal

P1. Every logical commit is followed immediately by `git push`. A commit
    that is not pushed does not exist for coordination purposes.
P2. A claim commit MUST be pushed at claim time, before any deliverable
    work begins. An unpushed claim may be re-claimed by another worker
    without penalty.
P3. Deliverable commits are pushed before the delivery status is recorded.
P4. If push fails (network, permissions), STOP: do not start new claimed
    work; report the state per [PROC R4].

## 5. Integration plan

Phase 1 (this adoption): process.md R6.5 amended (commit AND push);
briefs pickup protocol steps updated. Single commit tagged
`[PROPOSAL-08232026-remote-push-required]`.

## 6. Relationship matrix

| Interacts with | Nature |
|----------------|--------|
| [PROC R5] | Strengthens: claims must be VISIBLE commitments |
| [PROC R6.5] | Amended body text |
| Briefs pickup protocol | Steps 4 and 7 gain push language |
| Per-worker topology proposal | Structural successor |

## 7. Implementation notes

Executed at adoption. No open questions.

## 8. Revision log

| Date | Change | Source |
|------|--------|--------|
| 08232026 | Initial draft, adopted same day | Owner directive |
