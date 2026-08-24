# BRIEF-008 -- Deep code audit of the Custom Skills module; produce remediation brief(s)

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Context: the module was authored substantially by novice
  contributors; the owner wants a rigorous quality review of ALL
  module-owned code and a concrete, evidence-based correction plan.

## Scope

IN SCOPE -- every module-owned source file listed in MANIFEST.md:

- All `.h`/`.cpp` under
  `MMOCoreORB/src/server/zone/managers/customskills/` including service
  subdirs (`combat/`, `buffs/`, `crafting/`, `durability/`,
  `gathering/`, `movement/`, `progression/`, `skillmods/`)
- `MMOCoreORB/bin/scripts/customskills/config.lua`
- `MMOCoreORB/bin/scripts/commands/customSkills.lua`
- `integration/core3-hooks.patch` content (module-owned description of
  delegation points) may be reviewed for correctness of what it claims.

OUT OF SCOPE -- everything else in the Core3 tree (SWGEmu code). You may
READ Core3 headers/sources to verify how module calls behave (cite what
you read), but Core3 files are neither audited deliverables nor
modifiable.

## Required reading before auditing

1. Workspace-root `START-HERE.md`, then
   [../customskills/LLM_GUIDE.md](../customskills/LLM_GUIDE.md)
   (session protocol).
2. [../rules/project-design.md](../rules/project-design.md) -- CRITICAL:
   several constructs that look like bugs are intentional design
   ([CS-N]: basis points, native fallback, client/server parity,
   menu-as-runtime). Do not report intentional decisions as defects.
3. [../rules/process.md](../rules/process.md),
   [../rules/errata.md](../rules/errata.md) -- filing rules, severity
   classes F1-F5.
4. [../../swgemu/CODE_REFERENCE.md](../../swgemu/CODE_REFERENCE.md)
   Part I -- Core3 patterns baseline for "best practice" judgments.
5. The existing errata log -- do not re-report RESOLVED findings
   (ERR-001..006); verify claimed resolutions only if suspicion arises.

## Audit dimensions

Review every file against each dimension; a single defect may hit
several:

1. **Correctness**: logic errors, wrong math (bp conversions, cap
   application order), off-by-one, inverted conditions, unhandled
   null/empty cases, stale assumptions after refactors (e.g. ERR-005
   changed the crit path).
2. **Concurrency**: Core3 runs heavily multithreaded. Flag unsynchronized
   shared state, singleton access during zone threads, non-thread-safe
   lazily-built data, TOCTOU on config values, missing locks where other
   managers use them.
3. **Resource & lifetime**: SUI callback captures (dangling
   CreatureObject/PlayerObject references), pointer ownership vs Core3
   reference-counting conventions, leaked transactions/objects on early
   return.
4. **API misuse**: incorrect or fragile Core3 API usage versus patterns
   in `../../swgemu/CODE_REFERENCE.md`; string/key handling; VectorMap
   semantics (duplicate-key put behavior!).
5. **Robustness of config handling**: malformed/missing fields, out-of-
   range values, badge keys that no longer resolve (`BadgeList::get`),
   interplay of `badgeBonus = 0` + full-override coverage (BRIEF-007
   will change this area -- coordinate with its delivered state).
6. **Performance**: per-tick/per-hit hot paths (H01 combat hook!), map
   rebuilds, string churn, O(n^2) scans over badges/modifiers.
7. **Lua quality**: `config.lua` / `customSkills.lua` structure,
   validation gaps, error visibility.
8. **Consistency**: naming, dead code, duplicated logic that R6.7-style
   consolidation should catch, doc-vs-code drift beyond what Appendix A
   already tracks.

## Deliverables

1. **Audit report**: append to THIS brief as a signed section --
   `## Audit report -- <MMDDYYYY> by <signature>`. One numbered finding
   per issue, each with: dimension(s), severity (reuse F1-F5), location
   anchors (`file :: function`, quoted snippet, line numbers stamped
   with the date they were valid), why it is a problem, and a proposed
   minimal fix sketch.
2. **Remediation brief(s)**: create `009-<topic>.md` (take the next free
   number at creation time per [PROC R3]; if findings cluster into
   unrelated work, create multiple briefs but prefer ONE consolidated
   brief with phased deliverables). Each remediation brief must be
   self-contained: exact files, exact changes or investigation steps,
   acceptance criteria including compile + test expectations, tagged for
   `[BRIEF-009]` execution.
3. **Errata filings**: genuine behavioral BUGS (F1/F2 affecting live
   behavior) are filed as ERR-NNN entries (OPEN) rather than silently
   absorbed into remediation briefs; the remediation brief then cites
   them. Style/craft issues stay in the brief only.
4. **No code changes.** This brief is read-only except: additions to
   this file, new brief file(s), new errata entries, tracker stamps.

## Constraints

- Evidence rule ([ERR protocol]): "it seems wrong" is not a finding.
  Every claim cites quoted code you actually read.
- Intentional-design trap check: before filing, confirm the construct
  is not covered by project-design principles or existing errata.
- Findings must reflect the repository state AT YOUR REVIEW TIME; stamp
  line numbers with date.
- Do not modify SWGEmu files, caps, config values, or gameplay balance.

## Acceptance criteria

- Coverage checklist in the report showing every MANIFEST-listed module
  file reviewed (file -> reviewed-by line).
- >= 1 documented finding OR an explicit signed statement that a file
  is clean per dimension (spot-checks must still be listed).
- Remediation brief(s) exist, UNCLAIMED, self-contained, with severity-
  ordered phases (correctness/concurrency first, style last).
- Behavioral bugs filed as OPEN ERR entries citing evidence.
- Single commit tagged `[BRIEF-008]`, pushed ([PROC R6.5]); index row +
  stamp updated; this brief set DELIVERED with notes.

## Out of scope

- Applying any fix (that is the remediation briefs' job).
- Auditing SWGEmu/Core3 engine code, SQL schema, or TRE/datatable data.
- Balance/value judgments about modifier numbers (owner domain).
