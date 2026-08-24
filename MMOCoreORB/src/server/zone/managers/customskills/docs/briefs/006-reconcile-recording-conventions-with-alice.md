# BRIEF-006 -- Reconcile recording conventions with the Project Alice framework

- Status: UNCLAIMED
- Created: 2026-08-23 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Context: ERR-006 (hy3-free signature slip) exposed that the
  governance adoption ported Alice's recording conventions INCOMPLETELY --
  slot semantics, short format, and several definitional details were lost,
  and no reconciliation record exists justifying what was kept vs dropped.

## Reference framework (READ-ONLY -- never edit, sign, or push there)

Project Alice lives outside this repository:

```
G:\Data\project alice\                          <- workspace root
├── START-HERE.md                               <- Alice entry point
└── github\alice\                               <- Alice repository
    └── docs\
        ├── llm-guide.md                        -> "How to Work Here" -> Signing
        └── rules\process.md                    -> Contributor Recording Conventions
                                                    (THE canonical definitions: long/short
                                                    signature formats, date format, time zone)
```

Treat those documents as the source framework. This project's adaptations
must trace back to them item by item.

## Problem

Our `docs/rules/process.md` -> Contributor Recording Conventions was written
as a loose paraphrase. Consequences:

1. Name-slot vs origin-slot semantics were never defined, so hy3-free signed
   as `opencode (opencode/hy3-free)` (ERR-006).
2. Alice's SHORT signature format was dropped without a recorded decision.
3. The ISO `YYYY-MM-DD` date deviation from Alice's `MMDDYYYY` is documented
   as intentional, but exists only as an inline remark -- not as part of a
   tracked adoption/reconciliation.
4. Other documents restate signature format instead of referencing process.md
   (violates [PROC R6.7]) -- e.g. `docs/rules/errata.md` Principles #4
   includes an inline format template.

## Deliverables

1. **Inventory**: extract EVERY recording-convention item from the two Alice
   documents listed above (long format, short format + when it applies, date
   format, time zone, where signatures are required, roster rules).
2. **Reconciliation table** (deliver as a section appended to this brief
   file, signed): one row per Alice item ->
   `ADOPT | ADAPT (how) | DEVIATE (why) | OWNER-DECISION`. Recommended
   starting positions:
   - Slot semantics: ADOPT with explicit definition -- name slot = the
     contributor's model/persona identifier as used in Contributors rosters;
     origin slot = harness/model-id. Closes the ERR-006 loophole.
   - Short format: ADOPT (opt-in per context, Alice's rules) or OWNER-DECISION.
   - Date format: DEVIATE (keep `YYYY-MM-DD`) recommended -- hundreds of
     existing same-day records already use ISO; switching formats would
     rewrite history for zero truth value. Mark as OWNER-DECISION since it
     reverses a framework default; do NOT flip silently.
   - Time zone: ADOPT (Central Time) -- already aligned.
3. **Apply** the adopted/adapted items to `docs/rules/process.md` ->
   Contributor Recording Conventions. State slot semantics explicitly.
4. **De-duplicate** ([PROC R6.7]): replace inline format restatements with
   references to process.md -- known instance: errata.md Principles #4.
5. **ERR-006**: append a Findings note pointing at the clarified convention
   text; leave resolution routing unchanged (hy3-free re-signs, or Daniel
   authorizes mechanical replacement).
6. Any DEVIATE/OWNER-DECISION rows that Daniel must settle: collect them
   under a single `Owner decisions requested` subsection; do not implement
   either side until answered; record answers as
   `[DIRECTIVE <YYYY-MM-DD>]` notes in this brief.
7. Stamps on every touched doc; single commit tagged `[BRIEF-006]`.

## Acceptance criteria

- Reconciliation table covers every Alice recording-convention item with an
  explicit disposition; no undocumented drops remain
- process.md states name/origin slot semantics unambiguously
- Zero inline restatements of the signature format outside process.md
  ([PROC R6.7] sweep clean; grep `name/model>` and `<origin>`)
- ERR-006 links to the clarified text; open/closed status matches reality
- Owner-decision items listed and unanswered ones clearly marked pending
- Project Alice repository untouched (`git -C "G:\Data\project alice\github\alice" status`
  clean before and after)

## Out of scope

- Retroactively re-signing hy3-free's 14 instances (that is ERR-006's own
  routing, executed separately)
- Changing any date already recorded anywhere
- Modifying Alice's documents or processes
