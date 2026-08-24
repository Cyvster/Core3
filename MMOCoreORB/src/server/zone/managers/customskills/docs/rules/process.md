# Collaboration & Recording Process

> Subordinate to [project-design.md](project-design.md) -- the master
> document for project rules.

> **Last reconciled:** 2026-08-23 by ox-alpha (opencode/x-preview-f-free) --
> initial draft; 2026-08-23 added Repository & Release Conventions
> (salvaged from archived quickstart/overview docs during compression);
> 2026-08-23 BRIEF-002: single-tree rewrite (R6.5/R6.6, containment rule,
> release conventions) after the package mirror was dissolved

The written process for any work on this repository, whether by Daniel, a
single LLM session, or multiple coordinated entities. Adopted 2026-08-23,
adapted from the Project Alice process framework. Originally written for a
two-tree layout (Core3 dev tree + package mirror); rewritten 2026-08-23 for
the single-tree layout after BRIEF-001 dissolved the mirror.

## Scope

This document is the **process** authority. It contains ONLY process content:

- **IS process content:** how contributors claim, deliver, record, sync, and
  correct work; signing and date conventions; traceability tags; formatting
  rules for documents.
- **IS NOT process content** (lives elsewhere): design decisions and
  principles (`docs/rules/project-design.md`); correction records
  (`docs/rules/errata.md`); current task state (`docs/tracking/objectives.md`,
  `docs/briefs/`).
- On conflict with the master document, the master document wins.

---

## Rule 1 -- Instructions before workers

If an activity involves multiple workers (LLM sessions, batch reviews), the
organizer writes the instructions FIRST: canonical paths, deliverable
format, completion criteria, failure route, coordinator. Prefer encoding the
work as a brief (`docs/briefs/`) so it is self-contained.

## Rule 2 -- One entity, one voice

An entity edits only within its claimed scope. No entity signs, completes,
or revises content on behalf of another. If another contributor's work looks
wrong or stalled, report it via [ERR] -- do not touch it.

## Rule 3 -- Numbering discipline

Before filing anything numbered (ERR-NNN, BRIEF-NNN), read the current log
and take the next free number; re-read immediately before writing. Numbers
are never reused, including for withdrawn items.

## Rule 4 -- Failure route

If a tool fails mid-edit: STOP retrying, verify file integrity, and report
the state in your delivery note. Malformed partial edits are worse than no
edit. Scratch/recovery notes go under `docs/temp/` per the Document Creation
Rules in `docs/README.md`.

## Rule 5 -- Claims are commitments

A claim without progress is a stall. The owner may void any claim whose
worker is gone or silent; voided claims are marked with worker + reason and
return to the queue with partial work preserved and credited.

## Rule 6 -- Trackers carry reconciliation stamps

Every living tracker ends its status section with:

```
**Last reconciled:** <YYYY-MM-DD> by <name/model (<origin>)>.
```

Living trackers in this project: `MANIFEST.md`, `docs/tracking/objectives.md`,
`docs/briefs/README.md`, `docs/rules/errata.md`, and any active design doc
under `docs/feature-planning/`. Update the stamp whenever the tracker
materially changes, so staleness is detectable.

## Rule 6.5 -- Record work where it lands

Everything -- module code, scripts, docs, and the integration patch -- lives
inside this Core3 repository (module dir:
`MMOCoreORB/src/server/zone/managers/customskills/`). Therefore:

1. All changes (code AND documentation) are committed in this repository
   (branch `cyvster3`) under your own identity with a traceability tag per
   R6.8. Uncommitted work is an incomplete delivery.
2. Documentation-only changes carry their own reconciliation stamp and,
   where applicable, a Contributors entry.
3. The standalone mirror folder (`customskills-mod/`, with its `package/`
   copy) was dissolved 2026-08-23; references to it are historical only.

## Rule 6.6 -- Change completeness

A change that alters module behavior, files, or integration points MUST, in
the same working session and commit set, also update everything that
describes it:

| If you changed... | You must also update... |
|-------------------|------------------------|
| Hook placement or behavior | `docs/reference/ARCHITECTURE.md` hook inventory + `integration/core3-hooks.patch` |
| Modifier units, defaults, caps, badges | `docs/reference/MODIFIER_REFERENCE.md` + `config.lua` defaults |
| Files added/removed/renamed | `MANIFEST.md` |
| Menu structure | `docs/reference/ARCHITECTURE.md` SUI section + package `MENU_DESIGN.md` |
| Config schema | `docs/installation/INSTALLATION.md` config reference |

Reviewers must reject deliveries that modify behavior without updating the
corresponding documentation.

## Rule 6.7 -- Reference, don't duplicate

Documents must not copy or restate rules, formats, or procedures defined in
another document. Reference the source document and section instead. Rules
are defined ONCE; when a rule changes, it changes in one place.

Example: "Per [PROC R6.6], update the hook inventory when hook behavior
changes."

## Rule 6.8 -- Every change names its authority

Every commit that adds, changes, or removes tracked files
carries a traceability tag (first line or body):

- `[BRIEF-NNN]` -- work claimed and delivered under a brief
- `[ERR-NNN]` -- resolution of a filed erratum
- `[M#]` -- milestone-directed work without a brief (e.g. `[M5]`)
- `[DIRECTIVE <YYYY-MM-DD>]` -- owner instruction given outside the repo;
  quote the directive verbatim in the commit body where feasible

Documentation-only commits follow the same rule. A tag without a real
referent violates R5.

## Rule 7 -- Disputes graduate, never escalate inline

Disagreements about content are recorded as errata entries and verified by a
second party. Nobody rewrites another contributor's content over a
disagreement, and nobody files the same dispute under multiple numbers.

---

## Repository & Release Conventions

- Development happens in this repository on branch `cyvster3`.
- **Containment rule**: module-owned files stay inside
  `MMOCoreORB/src/server/zone/managers/customskills/` except where Core3's
  runtime layout requires otherwise. The sanctioned exceptions are:
  `MMOCoreORB/bin/scripts/customskills/config.lua` (runtime config) and
  `MMOCoreORB/bin/scripts/commands/customSkills.lua` (command registration),
  plus delegation hooks inside existing Core3 sources (documented via the
  patch + MANIFEST).
- Before deleting a working branch: tag the completed module, install on a
  clean integration branch, and verify there before removing anything.
- Distribution: the module is part of the repo; to install onto another
  server, copy the module directory, the two script files, and apply
  `integration/core3-hooks.patch`. See `docs/installation/INSTALLATION.md`.
- `MANIFEST.md` and the installation guide are updated with every material
  change ([PROC R6.6]).

## Contributor Recording Conventions

### Signing format

Every substantive contribution -- document edits, errata entries,
verifications, brief claims/deliveries -- is signed:

```
<name/model> (<origin>), <YYYY-MM-DD>
```

Example: `ox-alpha (opencode/x-preview-f-free), 2026-08-23`
The human owner signs as `Daniel`.

Add yourself to a document's **Contributors** section the first time you
contribute to it. Historical attributions (e.g. earlier model sessions) are
never rewritten -- new entries are appended.

### Date format

All dates use ISO format `YYYY-MM-DD`. This project standard intentionally
differs from other internal projects and matches all existing customskills
records; do not convert historical dates.

### Time zone

Dates are recorded in Central Time (US & Canada), Daniel's local time zone.
Contributors in other time zones convert before recording.

---

## Formatting requirements for governing documents

- Heading levels: `##` major sections, `###` subsections.
- ASCII only: em dashes become `--`; arrows become `->`; no smart quotes.
- Keep lines under ~100 characters.
- Tables for registries and checklists.
- Citation style: `[CS-N]` principles, `[PROC R#.#]` process rules,
  `[ERR-NNN]` errata, `[BRIEF-NNN]` briefs, hook IDs `H01`-`H16`.
- Every edit updates the document's `Last reconciled` stamp (R6).

## Contributors

- **Daniel** -- project owner
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- initial draft,
  adapted from Project Alice docs/rules/process.md framework
