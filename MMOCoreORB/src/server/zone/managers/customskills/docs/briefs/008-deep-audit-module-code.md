# BRIEF-008 -- Deep code audit of the Custom Skills module; produce remediation brief(s)

- Status: DELIVERED by ox-alpha (opencode/x-preview-f-free), 08242026
  (remediation scheduled via BRIEF-011; ERR-008 filed OPEN)
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

---

## Audit report -- 08242026 by ox-alpha (opencode/x-preview-f-free)

### Coverage checklist (all MANIFEST-listed module files reviewed)

CustomSkillsCommand.h, CustomSkillsMenu.h/.cpp, CustomSkillsSuiCallback.h/.cpp,
CustomSkillsConfig.h/.cpp, CustomSkillsModifiers.h/.cpp,
CustomSkillsModifierType.h, combat/, buffs/, crafting/, durability/,
gathering/, movement/, progression/, skillmods/ (.h+.cpp each),
config.lua, customSkills.lua -- ALL READ IN FULL this session.
Clean-per-dimension notes where no defect found are folded into the
findings' "reviewed-clean" annotations below.

### Findings

**AUDIT-1 [F1 behavioral / correctness / consistency]** Menu badge
assignments are a SECOND copy of config data. The anonymous namespace of
`CustomSkillsMenu.cpp` hardcodes 24 group arrays (`badgesCritChance`,
`badgesTriple`, ...) that mirror `config.lua` assignments. Drift is
already live: after BRIEF-007 expanded Critical Chance to 60 badges, the
menu's BONUS_CRIT_CHANCE page still lists only the old 24-key subset and
its category totals compute from that subset (24%) while actual gameplay
totals 60% ([CS-3] violated; menu contradicts `/customskills` prompt
summary which reads the config map). Root cause: presentation grouping
has no source-of-truth linkage to config assignments. Fix direction:
derive menu lists from the config map at runtime, or add boot-time
drift validation that warns loudly. Filed as **ERR-008 (OPEN)**.

**AUDIT-2 [F1 behavioral]** Combat spam labels emit as SEPARATE messages
(`sendCustomCombatSpam(criticalMessage, 11)` in
`combat/CustomSkillsCombat.cpp::applyDamage`) instead of appending to
the hit line -- root cause of the owner-reported newline drop. Already
scheduled for fix/removal-fallback via **BRIEF-010**; not double-filed.

**AUDIT-3 [F2 robustness]** `getTitle()` indexes a titles[] array by
`(int)page` with no bounds check or static_assert; `getParent()` uses
enum range comparisons; `getModifierName()` mirrors the modifier enum in
a char* array. Any future enum insertion silently breaks titles/names
(OOB read risk on titles). Fix: static_assert(array size == COUNT) at
each site + bounds-guarded title lookup.

**AUDIT-4 [F3 consistency]** Config loader special-cases criticalChance
(duplicated logic vs `loadModifier`), carries a magic per-modifier C++
default (`gatheringQuantity` defaultBadgeBonus = 20000 vs 0 everywhere
else), applies different negative-cap validation per path, and allows
badgeOverrides entries to introduce keys absent from badges[]
(undocumented). Consolidation candidate.

**AUDIT-5 [F5]** `divideDuration` maps durations <= 1s (including 0) to
1s; `shouldDegradeWeapon` mixes random(100)/random(9999) granularity
between native and modified paths; `formatModifierBonus` pads with
trailing spaces for SUI column alignment (fragile). Cosmetic/edge.

**AUDIT-6 [note, no action]** Concurrency posture: config singleton is
populated once at construction and read-only afterward; safe given no
runtime reload exists (restart-to-apply is documented behavior).
Reviewed-clean dimensions elsewhere: buff eligibility CRC handling,
gathering/movement/progression arithmetic (int64 intermediate math
correct), practice-XP ordering, SEA-cap wearable adjustment math,
SUI callback lifecycle (ManagedReference pattern), command state-mask
checks.

### Disposition

- AUDIT-1 -> ERR-008 (OPEN) + remediation via BRIEF-011 Phase A
- AUDIT-2 -> BRIEF-010 (existing)
- AUDIT-3..5 -> BRIEF-011 Phases B/C
- AUDIT-6 -> recorded, no action
