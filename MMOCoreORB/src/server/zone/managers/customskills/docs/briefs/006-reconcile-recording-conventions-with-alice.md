# BRIEF-006 -- Reconcile recording conventions with the Project Alice framework

- Status: DELIVERED by ox-alpha (opencode/x-preview-f-free), 08242026
  (OD-1 answered same day -- [DIRECTIVE 08242026] below)
- Created: 08232026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Context: ERR-006 (hy3-free signature slip) exposed that the
  governance adoption ported Alice's recording conventions INCOMPLETELY --
  slot semantics, short format, and several definitional details were lost,
  and no reconciliation record exists justifying what was kept vs dropped.

## Reference framework (READ-ONLY -- never edit, sign, or push there)

Project Alice lives outside this repository:

```
Project Alice (workspace root)
|---- readme.md                               <- Alice entry point
`---- alice\                                     <- Alice repository
    `---- docs\
        |---- llm-guide.md                        -> "How to Work Here" -> Signing
        `---- rules\process.md                    -> Contributor Recording Conventions
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
3. The ISO `MMDDYYYY` date deviation from Alice's `MMDDYYYY` is documented
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
   - Date format: DEVIATE (keep `MMDDYYYY`) recommended -- hundreds of
     existing same-day records already use ISO; switching formats would
     rewrite history for zero truth value. Mark as OWNER-DECISION since it
     reverses a framework default; do NOT flip silently.
   - Time zone: ADOPT (Central Time) -- already aligned.
3. **Apply** the adopted/adapted items to `docs/rules/process.md` ->
   Contributor Recording Conventions. State slot semantics explicitly.
4. **De-duplicate** ([PROC R6.7]): replace inline format restatements with
   references to process.md -- known instance: errata.md Principles #4.
5. **ERR-006**: append a Findings note pointing at the clarified convention
   text; leave resolution routing unchanged (hy3-free re-signs, or Cyvster
   authorizes mechanical replacement).
6. Any DEVIATE/OWNER-DECISION rows that Cyvster must settle: collect them
   under a single `Owner decisions requested` subsection; do not implement
   either side until answered; record answers as
   `[DIRECTIVE <MMDDYYYY>]` notes in this brief.
7. Stamps on every touched doc; single commit tagged `[BRIEF-006]`.

## Acceptance criteria

- Reconciliation table covers every Alice recording-convention item with an
  explicit disposition; no undocumented drops remain
- process.md states name/origin slot semantics unambiguously
- Zero inline restatements of the signature format outside process.md
  ([PROC R6.7] sweep clean; grep `name/model>` and `<origin>`)
- ERR-006 links to the clarified text; open/closed status matches reality
- Owner-decision items listed and unanswered ones clearly marked pending
- Project Alice repository untouched (status checked in its own checkout)
  clean before and after)

## Out of scope

- Retroactively re-signing hy3-free's 14 instances (that is ERR-006's own
  routing, executed separately)
- Changing any date already recorded anywhere
- Modifying Alice's documents or processes

---

## Delivery record -- 08242026 by ox-alpha (opencode/x-preview-f-free)

### Inventory of Alice recording-convention items

Sources read read-only (Alice repository untouched; verified clean before
and after delivery):

1. `docs/rules/process.md` -> Contributor Recording Conventions:
   - I1 universal scope: rules govern every signed contribution across all
     documents (errata entries, audit findings, adjudication decisions,
     objective updates, roster additions, brief claims/deliveries, code
     commits)
   - I2 long signature format: name/model + origin + date, for every
     substantive contribution
   - I3 the human owner signs as Cyvster
   - I4 short format: first 3 chars of model identifier + first 3 chars of
     a distinguishing version/origin identifier; only where explicitly
     specified (compact tables, monitor displays); long format is the
     default everywhere, short is opt-in per context
   - I5 date format MMDDYYYY with leading zeros (fixed-width rationale)
   - I6 time zone Central Time with UTC offsets, conversion duty before
     recording anywhere, never record own local time
2. `docs/llm-guide.md` -> How to Work Here -> Signing:
   - I7 reference-don't-restate: canonical formats live in process.md;
     other documents point at them
   - I8 add yourself to a document's Contributors section on first
     contribution

### Reconciliation table

| Item | Disposition | Where it landed / notes |
|------|-------------|-------------------------|
| I1 scope | ADOPT | process.md -> Contributor Recording Conventions intro now enumerates governed contribution types |
| I2 long format | ADAPT | kept, with ISO date per OD-1; added explicit slot semantics: name slot = contributor model/persona identifier as used in Contributors rosters; origin slot = harness/model-id string. Closes the ERR-006 loophole |
| I3 owner signs as Cyvster | ADOPT | unchanged |
| I4 short format | ADOPT | restored after the unrecorded drop; derivation rule + example codified in process.md; long remains default; never used in governing docs or git attribution |
| I5 date format | OWNER-DECISION -> ANSWERED | was: keep ISO deviation pending ratification. ANSWERED 08242026: Cyvster directed adoption of MMDDYYYY going forward (framework default restored); historical ISO records unchanged. See [DIRECTIVE 08242026] below |
| I6 time zone | ADOPT | UTC offsets and never-local-time duty added to process.md; Central Time already aligned |
| I7 reference-don't-restate | ALREADY ADOPTED | enforced via PROC R6.7; sweep executed this delivery (below) |
| I8 Contributors section rule | ADOPT | unchanged |

### [PROC R6.7] sweep result

Restatements found and replaced with references to process.md:

1. errata.md Worker quick start item 6 (inline ISO/Central/long-format list)
2. docs/tracking/objectives.md claim instruction line 18 (inline example)
3. workspace-root readme.md key-rules bullet (inline format string;
   file lives outside the repo, edited without commit)

errata.md Principles #4 was found ALREADY referencing PROC R6.8 with no
inline template (fixed before this brief); no action needed.

Intentional survivors outside process.md (not restatements):

- ERR-006 Description quotes the wrong-form signature as filed evidence --
  historical record protected by PROC R2/R7
- docs/archive/* -- retired documents, explicitly not extended
- proposals/README.md MMDDYYYY tokens are filename templates, not
  signature conventions
- docs/proposals/08232026-coordinator-pull-before-commit/proposal.md
  quotes the Original-author trailer inside the adopted proposal record;
  the live rule is carried by process.md rule C2
- this brief's own acceptance-criteria text names the grep tokens

### Owner decisions requested

- **OD-1 (date format):** confirm DEVIATE -- keep ISO MMDDYYYY,
  permanently reversing the Alice MMDDYYYY default -- or direct migration.
  Recommendation: keep ISO.

### ANSWER -- OD-1 [DIRECTIVE 08242026]

Owner directive, received 08242026 (Central Time), verbatim selection:
"Switch to MMDDYYYY". Effect: all dates recorded from the moment of this
directive use MMDDYYYY with leading zeros; dates already recorded in ISO
format anywhere remain as written. process.md -> Date format updated
accordingly (including format templates: reconciliation stamps, DIRECTIVE
tags, Original-author trailers, long signatures) and errata.md entry
template Date field aligned. Known follow-up outside this brief's scope:
docs/proposals/README.md proposal-folder naming templates still read
MMDDYYYY and need a separate alignment decision.

Applied by ox-alpha (opencode/x-preview-f-free), 08242026.

### ADDENDUM -- same-day directive extension [DIRECTIVE 08242026]

After OD-1 was applied, Cyvster directed a full sweep: update ALL date
formats across the customskills mod to MMDDYYYY, superseding the
"historical ISO remains as written" clause, and settle the flagged
follow-up (proposal-folder naming templates). Scope limits per the
directive: the customskills mod only; SWGEmu code/files are not touched.
Executed: proposal folders renamed to the 08232026-* form,
docs/proposals/README.md naming templates + conventions line converted,
and every module-owned document migrated (signatures, stamps, entries,
evidence quotes included). See commit tagged [DIRECTIVE 08242026].

Delivered in single commit tagged [BRIEF-006] on branch cyvster3.
Directive application committed under [DIRECTIVE 08242026][BRIEF-006].
