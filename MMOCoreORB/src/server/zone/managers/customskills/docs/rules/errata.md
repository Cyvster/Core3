# Errata

> Subordinate to [project-design.md](project-design.md) -- the master
> document for project rules.

> **Last reconciled:** 2026-08-23 by ox-alpha (opencode/x-preview-f-free) --
> initial protocol; 2026-08-23 added Worker quick start with eligibility
> table, minimal-fix discipline, and entry-dispute rule (adapted from the
> Project Alice errata update of the same date)

Issue reporting and resolution for ALL documents, code, and claims in this
project. The protocol exists because many LLM agents of varying reliability
work here: no single contributor may overwrite another's work over a
disagreement, and no correction is applied without independent verification.
Every contributor can be wrong -- including in confident tones.

## Worker quick start

**If you were told to "work on errata":** follow these steps. Everything you
need is in this document plus the Errata Log below. Do not wait for further
instructions.

1. Read the workspace-root ``START-HERE.md`` (``G:\Data\swgemu customskills\START-HERE.md``) fully first.
2. Pick the first eligible entry by scanning the Errata Log, oldest number
   first:
   - An **OPEN** entry -> your job is RESOLUTION (research + proposed fix).
   - An **AWAITING-VERIFICATION** entry -> your job is VERIFICATION --
     but only if you are eligible (table below).
3. Eligibility (two-party rule):

   | Your relationship to the entry | May propose resolution? | May verify + apply? |
   |--------------------------------|-------------------------|---------------------|
   | You filed it                   | yes                     | NO                  |
   | Neither of the above           | yes                     | yes                 |
   | Daniel                         | always                  | always              |

4. Resolution (OPEN entries):
   - Reproduce or confirm the issue yourself before proposing anything:
     check the Core3 dev tree (`../Core3`), the package mirror, or the
     cited document -- whichever the entry touches.
   - Propose the MINIMAL fix that satisfies the entry -- no refactors, no
     out-of-scope improvements.
   - Set Status to `AWAITING-VERIFICATION` with your signature.
5. Verification (AWAITING entries):
   - Re-check the evidence; confirm the proposed fix addresses the reported
     issue.
   - Apply the edit, set Status to `RESOLVED`, append your verifier
     signature under Resolution.
   - If the fix is wrong or incomplete: do NOT edit it silently. Return the
     entry to OPEN with a signed note explaining what fails so the proposer
     can revise it.
6. Conventions: ASCII only; ISO dates (`YYYY-MM-DD`); Central Time; long
   signing format on every signature; Core3 commits carry `[ERR-NNN]` tags
   per [PROC R6.8].
7. Disputes about an existing entry's content or approach go through a NEW
   errata entry referencing its number -- never inline edits to someone
   else's entry. (Consistent with [PROC R7]: duplicate filings of the SAME
   issue remain prohibited.)
8. If nothing is eligible for you (every OPEN entry is yours, everything
   else verified), stop and report back -- do not invent work.

Verification eligibility summary: the FILER never verifies their own entry;
anyone else may; Daniel is exempt from both restrictions.

## Scope

This document is the **errata** authority. It contains ONLY correction
content:

- **IS errata content:** numbered correction entries `[ERR-NNN]`,
  design-boundary rulings pending ratification, verification records, and
  status transitions.
- **IS NOT errata content** (lives elsewhere): design rules
  (`docs/rules/project-design.md`), collaboration process
  (`docs/rules/process.md`).
- On conflict, the master document wins.

## Relationship to the legacy code-guide log

`docs/archive/CODE_GUIDE_ERRATA.md` is the historical errata log scoped to
the retired `SWGEMU_CODE_GUIDE.md` (superseded by
`docs/reference/CORE3_CODE_REFERENCE.md`), with its own entry scheme
(E1-E9, M1-M3, N1-N3, S1)
and procedure. That log remains authoritative for its existing entries; it
is **not extended**. New findings against any document -- including the code
guide -- are filed HERE as `ERR-NNN`.

## Principles

1. **No silent corrections.** If you believe existing content is wrong,
   file an entry here. Do not edit the target while the issue is OPEN.
2. **Evidence required.** Entries must cite stable anchors (file path plus
   function/class name; quote the actual text or code). Line numbers may be
   included but must be stamped with the date they were valid. "It seems
   wrong" is not evidence.
3. **Two-party rule.** A correction is applied only after verification by
   someone other than its proposer: another LLM session, or Daniel.
4. **Sign everything.** Per [PROC R6.8 conventions], every entry, finding,
   verification, and resolution carries the contributor signature.
5. **Errata duty.** Any contributor who opens this document attempts at
   least the oldest OPEN entry before other work.
6. **Unresolved != failed.** If research does not settle an issue, append
   what you found under **Findings** and leave it OPEN.

## Entry Lifecycle

```
OPEN --> AWAITING-VERIFICATION --> RESOLVED
  ^              |                        |
  |              v                        v
  +-------- back to OPEN          REJECTED (with reason)
                 |
                 v
          HUMAN-REVIEW (structural/destructive edits, or conflicting
                        verifications; Daniel decides)
```

- **OPEN** -- filed with evidence; unverified.
- **AWAITING-VERIFICATION** -- proposer has written a specific proposed fix;
  needs a second party.
- **RESOLVED** -- verified by a second party; fix applied; resolution notes
  record who verified.
- **REJECTED** -- evidence insufficient or claim incorrect; reason recorded;
  entry kept permanently.
- **HUMAN-REVIEW** -- required when a fix would delete sections, restructure
  a document, or when verifications conflict.

Anyone may move an entry from AWAITING-VERIFICATION back to OPEN by
appending signed findings that undermine the proposal, including a note
explaining what fails so the proposer can revise.

## Severity classes

| Class | Meaning | Typical action |
|-------|---------|----------------|
| F1 Factual error | Contradicts codebase or source of record | Fix after verification |
| F2 Stale | Was correct; code moved on | Fix + re-stamp date |
| F3 Ambiguous/misleading | Not false, but leads readers wrong | Reword after verification |
| F4 Unverifiable | Claimed without proof | Mark unverified in target text, or verify then promote |
| F5 Style/formatting | No truth value | May be fixed directly |

## How to file

1. Pick the next `ERR-NNN` number per [PROC R3].
2. Append the completed template to the bottom of this log.
3. Sign it. Note the filing in your delivery/commit record:
   `File ERR-NNN: <short title>`.

## How to attempt resolution

1. Research the claim against project documents and, where relevant, the
   Core3 tree (`../Core3`) or package mirror.
2. If confident: write the exact proposed change (document, section, old
   text, new text) under **Proposed fix**, set AWAITING-VERIFICATION, sign.
   Propose the MINIMAL fix that satisfies the entry -- no refactors, no
   out-of-scope improvements.
3. If not confident: append findings under **Findings**, signed and dated;
   leave status unchanged.
4. Never resolve your own proposal.

## How to verify

1. Confirm the evidence supports the claim and the fix matches it.
   Eligibility: the filer of the entry may not verify it (see Worker quick
   start table).
2. Apply the edit yourself (or decline).
3. Set RESOLVED / REJECTED / back to OPEN; add verifier signature. If
   returning to OPEN, include a signed note explaining what fails.
4. Note application in your delivery/commit record:
   `Apply ERR-NNN: <short title>` per [PROC R6.8].

Daniel may resolve, reject, or apply any entry directly.

## Template

```markdown
## ERR-NNN -- <short title>

- Status: OPEN
- Filed by: <signature>
- Date: <YYYY-MM-DD>
- Affects: <document>, <section/anchor>
- Severity: F1-F5
- Description: <what is wrong>
- Evidence: <quotes, references, grep/read output>
- Proposed fix: <exact change; filled only when proposing>
- Findings: <appended research notes, signed and dated individually>
- Resolution: <filled on RESOLVED/REJECTED, incl. verifier signature>
```

---

## Errata Log

*(entries appended below; none exist yet)*

---

## Contributors

- **Daniel** -- project owner; may resolve/reject any entry directly
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- initial protocol,
  generalizing archive/CODE_GUIDE_ERRATA.md Part 2 with the two-party
  lifecycle
