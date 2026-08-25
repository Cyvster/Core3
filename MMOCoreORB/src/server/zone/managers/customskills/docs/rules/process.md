# Collaboration & Recording Process

> Subordinate to [project-design.md](project-design.md) -- the master
> document for project rules.

> **Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
> R6.10 added (code-verify documented claims; owner directive); R6.9
> added (discovery capture: session learnings land in owning reference
> docs same-session; owner directive); prior: 08242026 -- BRIEF-006:
> recording conventions reconciled with the Project Alice framework
> (explicit slot semantics, short format adopted, deviation and scope
> tracking); [DIRECTIVE 08242026]: date format switched to MMDDYYYY
> going forward (OD-1 answered; format templates updated; historical ISO
> records unchanged); prior: 08232026 by ox-alpha
> (opencode/x-preview-f-free) -- R8 added (shared-tree coordination, four
> adopted proposals); prior: 
> initial draft; 08232026 added Repository & Release Conventions
> (salvaged from archived quickstart/overview docs during compression);
> 08232026 BRIEF-002: single-tree rewrite (R6.5/R6.6, containment rule,
> release conventions) after the package mirror was dissolved

The written process for any work on this repository, whether by Cyvster, a
single LLM session, or multiple coordinated entities. Adopted 08232026,
adapted from the Project Alice process framework. Originally written for a
two-tree layout (Core3 dev tree + package mirror); rewritten 08232026 for
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
**Last reconciled:** <MMDDYYYY> by <name/model (<origin>)>.
```

Living trackers in this project: `MANIFEST.md`, `docs/tracking/objectives.md`,
`docs/briefs/README.md`, `docs/rules/errata.md`, and any active design doc
under `docs/feature-planning/`. Update the stamp whenever the tracker
materially changes, so staleness is detectable.

## Rule 6.5 -- Record work where it lands; make it visible

Everything -- module code, scripts, docs, and the integration patch -- lives
inside this Core3 repository (module dir:
`MMOCoreORB/src/server/zone/managers/customskills/`). Therefore:

1. All changes (code AND documentation) are committed in this repository
   (branch `cyvster3`) under your own identity with a traceability tag per
   R6.8, then **pushed immediately** (R8/P1). A commit that is not pushed
   does not exist for coordination purposes.
2. Claim commits are pushed AT CLAIM TIME, before deliverable work begins;
   an unpushed claim may be re-claimed without penalty (R8/P2).
3. If push fails, STOP: do not start new claimed work; report per R4.
4. Documentation-only changes carry their own reconciliation stamp and,
   where applicable, a Contributors entry.
5. The standalone mirror folder (`customskills-mod/`, with its `package/`
   copy) was dissolved 08232026; references to it are historical only.

## Rule 6.6 -- Change completeness

A change that alters module behavior, files, or integration points MUST, in
the same working session and commit set, also update everything that
describes it:

| If you changed... | You must also update... |
|-------------------|------------------------|
| Hook placement or behavior | `docs/customskills/CODE_REFERENCE.md` hook inventory + `integration/core3-hooks.patch` |
| Modifier units, defaults, caps, badges | `docs/customskills/CODE_REFERENCE.md` Appendix A + `config.lua` defaults |
| Files added/removed/renamed | `MANIFEST.md` |
| Menu structure | `docs/customskills/CODE_REFERENCE.md` SUI section + package `MENU_DESIGN.md` |
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
- `[DIRECTIVE <MMDDYYYY>]` -- owner instruction given outside the repo;
  quote the directive verbatim in the commit body where feasible

Documentation-only commits follow the same rule. A tag without a real
referent violates R5.

## Rule 6.9 -- Discovery capture

Facts learned during a work session about the toolchain, environment, or
deployment mechanics -- how builds, data archives, stringfiles, client
assets, or server tooling actually behave -- are recorded in the OWNING
reference document ([PROC R6.6] table plus docs/README.md index) in the
SAME session they are learned, not left in conversation history, scratch
notes, or planning documents. Division of labor: planning documents record
decisions; reference documents record mechanisms. If no reference document
owns the topic, file an erratum proposing a home instead of inventing a
new document unilaterally.

## Rule 6.10 -- Code-verify documented claims

Factual or technical claims added to any document -- formulas, ordering
rules, values, defaults, API behavior -- MUST be verified against the
actual code or config at the time of writing, citing the verifying
source (file + function; quote the snippet where it is short). A
document must never substantiate a claim by pointing at another
document that merely restates it: [PROC R6.7] governs where a rule
lives; THIS rule governs how its content is proven. Worked examples and
sample calculations are claims and follow the same standard. When code
verification is genuinely impossible (toolchain/environment limits),
mark the claim `unverified -- pending code check` instead of presenting
it as established fact; later sessions resolve such marks via
[PROC R6.9] capture.

## Rule 7 -- Disputes graduate, never escalate inline

Disagreements about content are recorded as errata entries and verified by a
second party. Nobody rewrites another contributor's content over a
disagreement, and nobody files the same dispute under multiple numbers.

---

## Rule 8 -- Shared-tree coordination (adopted proposals 08232026)

Adopted via the proposals subsystem (docs/proposals/, framework: Project
Alice). Four mechanisms, layered; the last relaxes the first three.

### Staging discipline (clean-status-before-add, S1-S4)

S1. Stage with explicit file paths (`git add <path> ...`). `git add -A`
    and `git add .` are prohibited on this tree.
S2. Before staging, `git status --porcelain` must list ONLY the intended
    change. Unexpected entries: STOP; never delete or stash another
    session's files; report persistent strays via errata with owner.
S3. Applies to every worker equally.
S4. Sunset per Repository topology E4.

### Commit & sync discipline (coordinator-pull-before-commit, C1-C3)

C1. Before ANY commit, in order: (1) porcelain check per S2;
    (2) `git fetch origin`, then ff-only pull when strictly behind,
    rebase only on divergence -- never commit behind origin;
    (3) stage explicit paths; commit.
C2. Commits integrating another session's work carry in the body:
    `Original-author: <name/model> (<origin>), <MMDDYYYY>`
C3. These duties attach to whoever commits; no permanent coordinator role.

### Repository topology (per-worker-topology, E1-E5) -- POLICY

E1. Once provisioned, each session works in an isolated clone or dedicated
    git worktree -- never directly in another session's checkout.
E2. Work publishes to a personal branch (`work/<short-id>/<topic>`).
E3. Integration to the shared base branch goes through an explicit
    ff-only/rebase gate with Original-author trailers preserved.
E4. The staging/sync guards above relax to style preferences when:
    two workers provisioned AND gate used for one integration AND no
    direct shared-tree commits for 7 consecutive days.
E5. INTERIM (until provisioning): concurrent sessions STAGGER SCOPES --
    check briefs index + recent commits before starting; stay inside your
    declared scope while another session is active.
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

Adopted from the Project Alice framework (`docs/rules/process.md` ->
Contributor Recording Conventions and `docs/llm-guide.md` -> "How to Work
Here" -> Signing). Item-by-item dispositions are tracked in the BRIEF-006
reconciliation table (docs/briefs/006-reconcile-recording-conventions-with-alice.md).
These rules apply universally to every signed contribution across all
documents: errata entries, audit findings, objective updates, contributor
roster additions, brief claims/deliveries, and code commits.

### Signing format (long format -- the default)

Every substantive contribution -- document edits, errata entries,
verifications, brief claims/deliveries -- is signed in the long format:

```
<name/model> (<origin>), <MMDDYYYY>
```

Slot semantics: the name slot holds the contributor's model/persona
identifier -- the same identifier used in Contributors rosters (for example
`ox-alpha`, `hy3-free`). The origin slot holds the harness/model-id string
(for example `opencode/x-preview-f-free`). A harness or product name alone
is never a valid name-slot value: `opencode (opencode/hy3-free)` is wrong;
`hy3-free (opencode/hy3-free)` is correct [ERR-006].

Example: `ox-alpha (opencode/x-preview-f-free), 08232026`
The human owner signs as `Cyvster`.

Add yourself to a document's **Contributors** section the first time you
contribute to it. Historical attributions (e.g. earlier model sessions) are
never rewritten -- new entries are appended.

### Signing format (short format -- opt-in)

Use only where explicitly specified (compact tables, monitor-style
displays): the first 3 characters of the model/persona identifier followed
by the first 3 characters of a distinguishing version/origin identifier.
Example: `ox-alpha (opencode/x-preview-f-free)` becomes `ox-x-p`. The long
format remains the default everywhere; short format never appears in
governing documents or git history attribution.

### Date format

All dates use MMDDYYYY with leading zeros -- exactly eight characters,
enabling consistent sorting, comparison, and filename embedding.
Adopted 08242026 by owner directive [DIRECTIVE 08242026] (BRIEF-006
OD-1), restoring the Alice framework default. Examples: January 3, 2026 ->
01032026; August 24, 2026 -> 08242026. A same-day directive extension
ordered conversion of ALL existing records rather than leaving historical
ISO strings in place: the module's entire documentation set was migrated,
including proposal-folder names ([DIRECTIVE 08242026] follow-through).
ISO MMDDYYYY no longer appears in module documents. This convention
governs module records only; SWGEmu core files outside the module
directory are never reformatted by this project's policy.

### Time zone

Dates are recorded in Central Time (US & Canada) -- Cyvster's local time
zone: UTC-6 during standard time (CST), UTC-5 during daylight saving time
(CDT). Contributors in other time zones convert their local time to
Central Time before recording anything in this project (documents, commit
messages, telemetry, monitor output); never record your own local time.

---

## Privacy (owner directive 08252026, ERR-014)

The repository is public. In every tracked file:

- The owner is referenced as **Cyvster** (or "the owner") -- never by real
  name.
- No local filesystem paths (`X:\...`, `C:\Users\...`, home directories)
  or machine-specific details; use repo-relative paths or generic
  descriptions ("the TRE builder workspace outside this repository").
- No account names, emails, or credentials beyond what git already records.

Applies to docs, briefs, proposals, code comments, and config examples.
Upstream third-party content (e.g. SOE credit tables in `sql/`) is exempt.
Pre-push check: grep the diff for the owner's real name and `[A-Z]:[\\/]`
path patterns; a hit blocks the push until scrubbed ([ERR-014]).

---

## Cross-Project Propagation (XPP) -- adopted 08252026

Proposal 08252026-cross-project-policy-propagation (ADOPTED by owner
directive same day). Custom Skills and Project Alice share governance;
a policy implemented on one project MUST file a mirror-review brief on the
other in the SAME session as its implementation.

- Mirror brief: "Review + adopt: <policy> (from <origin project>)", linking
  the origin commit/file; outcome ADOPT / ADAPT / DECLINE recorded in the
  delivery report. Tailoring to the receiving project is expected; DECLINE
  must state why.
- Filed UNCLAIMED unless the coordinator executes it immediately.
- Propagation ledger (name, origin, outcome, date) lives in docs/README.md.
- Scope: process rules, documentation standards, quality gates,
  privacy/security practices, workflow procedures -- not project-specific
  technical work.

### XPP ledger

| Policy | Origin | Outcome | Date |
|--------|--------|---------|------|
| Discovery capture mandate (R6.9) | customskills BRIEF-019 | ADOPTED on Alice (briefs README authoring rules) | 08252026 |
| Bounded ledgers + archives (L1-L6) | Alice brief 041 | ADOPTED here (BRIEF-028) | 08252026 |
| Privacy rule (handle-only, no local paths, pre-push grep) | customskills ERR-014 | ADAPTED on Alice (scrub done 08252026; llm-guide grep check pending) | 08252026 |
| Second-party verification gate | Alice | REVIEW PENDING -> see briefs index mirror-review | 08252026 |

## Formatting requirements for governing documents

- Heading levels: `##` major sections, `###` subsections.
- ASCII only: em dashes become `--`; arrows become `->`; no smart quotes.
- Keep lines under ~100 characters.
- Tables for registries and checklists.
- Citation style: `[CS-N]` principles, `[PROC R#.#]` process rules,
  `[ERR-NNN]` errata, `[BRIEF-NNN]` briefs, hook IDs `H01`-`H16`.
- Every edit updates the document's `Last reconciled` stamp (R6).

## Contributors

- **Cyvster** -- project owner
- ox-alpha (opencode/x-preview-f-free), 08232026 -- initial draft,
  adapted from Project Alice docs/rules/process.md framework
