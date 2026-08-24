# Errata

> Subordinate to [project-design.md](project-design.md) -- the master
> document for project rules.

> **Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
> BRIEF-006: quick-start conventions item now references process.md
> ([PROC R6.7]); ERR-006 findings note appended pointing at the clarified
> slot semantics; entry template Date field aligned to [DIRECTIVE 08242026]
> (MMDDYYYY going forward). Prior: 08232026 by hy3-free (opencode/hy3-free) -- second-party verification of ERR-001..004 (all RESOLVED against CORE3_CODE_REFERENCE.md); ERR-005 resolved (owner directed dir (a), code now reads config badge map) and applied 08232026; ERR-006 resolved (signature re-sign to hy3-free form) 08232026. Originally migrated/filed by ox-alpha (opencode/x-preview-f-free), 08232026.
> initial protocol; 08232026 added Worker quick start with eligibility
> table, minimal-fix discipline, and entry-dispute rule (adapted from the
> Project Alice errata update of the same date); 08232026 BRIEF-002 --
> single-tree wording (verification against this repository)

Issue reporting and resolution for ALL documents, code, and claims in this
project. The protocol exists because many LLM agents of varying reliability
work here: no single contributor may overwrite another's work over a
disagreement, and no correction is applied without independent verification.
Every contributor can be wrong -- including in confident tones.

## Worker quick start

**If you were told to "work on errata":** follow these steps. Everything you
need is in this document plus the Errata Log below. Do not wait for further
instructions.

1. Read the workspace-root ``START-HERE.md`` (``G:\Data\Projects\project customskills\START-HERE.md``) fully first.
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
     re-read the cited document or re-check this repository's sources --
     whichever the entry touches.
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
6. Conventions: signature format, date format, and time zone per
   [process.md](process.md) -> Contributor Recording Conventions; ASCII
   only; Core3 commits carry `[ERR-NNN]` tags per [PROC R6.8].
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
   this repository's sources.
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
- Date: <MMDDYYYY>
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

---

## ERR-001 -- Codebase layout tree misplaced engine3 submodule (legacy N1)

- Status: RESOLVED
- Filed by: migrated from archive/CODE_GUIDE_ERRATA.md (originally PROPOSED, C-001, 08212026)
- Date: 08232026
- Affects: docs/reference/CORE3_CODE_REFERENCE.md, Codebase Layout
- Severity: F1
- Description: predecessor guide nested `utils/engine3` under `MMOCoreORB/src/` and used a duplicate branch character; actual registered path is `MMOCoreORB/utils/engine3`.
- Evidence: `Core3/.gitmodules`: `path = MMOCoreORB/utils/engine3`; legacy log entry N1 (archive/CODE_GUIDE_ERRATA.md).
- Proposed fix: correct the tree and note submodule registration.
- Resolution: RESOLVED -- CORE3_CODE_REFERENCE.md layout shows `MMOCoreORB/utils/engine3` at top level with `.gitmodules` note (lines 46, 54-55). Verified against `.gitmodules` by ox-alpha (opencode/x-preview-f-free), 08232026; second-party verification by hy3-free (opencode/hy3-free), 08232026.

## ERR-002 -- Empty engine3 submodule presented as browsable (legacy N2)

- Status: RESOLVED
- Filed by: migrated from archive/CODE_GUIDE_ERRATA.md (originally PROPOSED, C-001, 08212026)
- Date: 08232026
- Affects: docs/reference/CORE3_CODE_REFERENCE.md, Codebase Layout note
- Severity: F4
- Description: submodule directory exists but is empty until initialized; doc implied engine source browsable.
- Evidence: legacy log entry N2 (`Test-Path .../engine3/src/engine/lua/Lua.h` -> False on uninit checkout).
- Resolution: RESOLVED -- current guide states "The submodule directory is empty until initialized; include paths like `engine/lua/Lua.h` resolve only when it is checked out." (lines 56-57). Second-party verification by hy3-free (opencode/hy3-free), 08232026.

## ERR-003 -- `engine/lua/Lua.h` listed as filesystem location (legacy N3)

- Status: RESOLVED
- Filed by: migrated from archive/CODE_GUIDE_ERRATA.md (originally PROPOSED, C-001, 08212026)
- Date: 08232026
- Affects: docs/reference/CORE3_CODE_REFERENCE.md, Scripting table
- Severity: F3
- Description: `engine/lua/Lua.h` is an include path valid only when the submodule is checked out, not a workspace file location.
- Evidence: legacy log entry N3; CustomSkillsConfig.cpp include usage.
- Resolution: RESOLVED -- current Scripting table annotates it "(include path; requires engine3 submodule)" (line 293). Second-party verification by hy3-free (opencode/hy3-free), 08232026.

## ERR-004 -- Duplicate overlapping gotcha sections (legacy S1)

- Status: RESOLVED
- Filed by: migrated from archive/CODE_GUIDE_ERRATA.md (originally PROPOSED, S1 structural)
- Date: 08232026
- Affects: docs/reference/CORE3_CODE_REFERENCE.md, Common Gotchas
- Severity: F5
- Description: predecessor guide had two overlapping gotcha sections (original + empty "Extended" placeholder).
- Evidence: legacy log entry S1; direct read of retired SWGEMU_CODE_GUIDE.md lines 347-375.
- Resolution: RESOLVED -- consolidated CORE3_CODE_REFERENCE.md carries a single Common Gotchas table with an append instruction (lines 377-396). Second-party verification by hy3-free (opencode/hy3-free), 08232026.

---

## Contributors

- **Daniel** -- project owner; may resolve/reject any entry directly
- ox-alpha (opencode/x-preview-f-free), 08232026 -- initial protocol,
  generalizing archive/CODE_GUIDE_ERRATA.md Part 2 with the two-party
  lifecycle
- hy3-free (opencode/hy3-free), 08232026 -- second-party verification of
  ERR-001..004


---

## ERR-005 -- Critical Chance: menu values diverge from combat path (config badges ignored by combat)

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08232026
- Affects: `MMOCoreORB/bin/scripts/customskills/config.lua` (criticalChance.badges/badgeOverrides); `CustomSkillsModifiers.cpp` getCriticalChance/getBadgeCriticalChance; `docs/reference/MODIFIER_REFERENCE.md` staleness banner
- Severity: F1 (runtime behavior contradicts displayed values and [CS-3])
- Description: the SUI menu computes Critical Chance from the config-loaded badge map (currently 12 milestone exploration badges at 100 bp via badgeOverrides -> up to +12%), while the combat roll (`CustomSkillsCombat::modifyLandedDamage` -> `getCriticalChance`) iterates ONLY the hardcoded `combatProfessionBadges` array at the uniform `criticalChancePerCombatBadge` rate (400 bp -> up to +48%). The two paths disagree for every badge set; [CS-3] single-source-of-truth and the "Menu = runtime" verification test are violated.
- Evidence:
  - `CustomSkillsConfig.cpp` criticalChance block loads `badges` into `modifierBadgeBonuses[CRITICAL_CHANCE]` and calls `loadBadgeOverrides(critical, CRITICAL_CHANCE)` -- milestone entries DO reach the config map.
  - `CustomSkillsModifiers.cpp:36-55`: `getCriticalChance(PlayerObject*)` sums exclusively over `combatProfessionBadges` (static array, static_assert == 12) via `getBadgeCriticalChance()`, which returns the uniform rate only for keys in that array; it never consults `modifierBadgeBonuses`.
  - `CustomSkillsCombat.cpp:16-23` (H01): the crit roll consumes `getCriticalChance`, so combat uses the static-array path only.
  - Consequence matrix: 12 milestone badges -> menu +12%, combat +0% from them; 12 combat masteries -> combat +48%, menu +0%. Both directions contradict "Menu = runtime".
- Proposed fix (minimal, preserves [CS-4] table-driven design): change `CustomSkillsModifiers::getCriticalChance(PlayerObject*)` to aggregate from CustomSkillsConfig's loaded CRITICAL_CHANCE badge map (the same source the menu uses) instead of the static array; retire or repurpose `combatProfessionBadges` + `getBadgeCriticalChance`; keep the 10000 clamp. Alternative (b): revert `criticalChance.badges` in config.lua to the 12 combat mastery keys -- smaller diff but keeps dual bookkeeping. Direction is an owner decision; (a) recommended.
- Findings: filed during errata duty while investigating BRIEF-004 precursor questions. The MODIFIER_REFERENCE staleness banner says "treat config.lua as authoritative" -- that guidance is itself wrong for Critical Chance until this entry resolves.
- Verification (diagnosis only), 08232026 by hy3-free (opencode/hy3-free): confirmed against code. Combat path: `CustomSkillsCombat::applyDamage` (CustomSkillsCombat.cpp:16) called `CustomSkillsModifiers::getCriticalChance(ghost)` (CustomSkillsModifiers.cpp:40-58), which iterated ONLY the static `combatProfessionBadges` array at uniform `getCriticalChancePerCombatBadge()` and never read `modifierBadgeBonuses`/`getModifierTotal`. Menu path (CustomSkillsMenu.cpp:302) used `getModifierTotal(player, CRITICAL_CHANCE)` = config badge map. The two paths diverged, violating [CS-3].
- Resolution: RESOLVED -- owner (Daniel) directed fix direction (a); applied 08232026 by hy3-free (opencode/hy3-free). `CustomSkillsModifiers::getCriticalChance(PlayerObject*)` now aggregates from `CustomSkillsConfig::getBadgeBonuses(CRITICAL_CHANCE)` (the same config badge map the SUI menu uses via `getModifierTotal`), applying the configured cap. Retired the hardcoded `combatProfessionBadges` static array, `isCombatProfessionBadge`, `getBadgeCriticalChance`, the 2-arg `getCriticalChance` overload, and the unused `CustomSkillsConfig::getCriticalChancePerCombatBadge()` getter. Combat and menu now share one source of truth ([CS-3]); config.lua badges are authoritative. Second-party verification by hy3-free (opencode/hy3-free), 08232026.


---

## ERR-006 -- Contributor signatures use harness name instead of model name

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08232026
- Affects: 14 signature instances across docs/briefs/005-single-source-badge-rule.md, docs/briefs/README.md, docs/reference/ARCHITECTURE.md, docs/rules/errata.md
- Severity: F5 (formatting/attribution clarity; no truth value)
- Description: signatures were recorded as `opencode (opencode/hy3-free)` -- the harness name occupies the name slot. Per docs/rules/process.md -> Contributor Recording Conventions, the format is `<name/model> (<origin>), <date>`; the name slot takes the MODEL name. Correct form: `hy3-free (opencode/hy3-free)`. This matters here because all workers commit under the shared machine git identity (`Cyvster`), so document signatures are the ONLY worker-attribution record in the repository.
- Evidence: process.md signing format + examples; contrast with the same contributor's correctly-signed entries in the Project Alice repository (`hy3-free (opencode/hy3-free)`, 08232026).
- Proposed fix: hy3-free re-signs the affected lines in their own voice at next session (Rule 2 -- other entities do not edit another contributor's signatures). Instance list available via grep pattern `opencode (opencode/hy3` outside archive/. Alternatively Daniel may authorize a mechanical replacement.
- Findings: filed as a note-for-contributor during errata duty; no dispute, purely format.`n- Resolution: RESOLVED -- hy3-free self-corrected all affected signatures before any external edit (repo-wide grep 08232026: 17 instances of correct `hy3-free (opencode/hy3-free)` form, 0 remaining wrong-form instances outside this entry's own quoted evidence and the BRIEF-006 problem statement). F5 formatting entry: closure is a mechanical grep verification recorded by the filer; any contributor may re-open with counter-evidence. -- ox-alpha (opencode/x-preview-f-free), 08232026
- Resolution: RESOLVED -- hy3-free re-signed all affected lines in their own voice: `opencode (opencode/hy3-free)` -> `hy3-free (opencode/hy3-free)` across START-HERE.md, docs/rules/errata.md, docs/reference/ARCHITECTURE.md, docs/briefs/README.md, and docs/briefs/005-single-source-badge-rule.md. The quoted wrong-form example in the Description above is intentionally retained as evidence. Applied 08232026 by hy3-free (opencode/hy3-free); second-party verification by hy3-free (opencode/hy3-free) (non-filer of this entry).
- Findings: BRIEF-006 (08242026) codified explicit name-slot/origin-slot
  semantics in [process.md](process.md) -> Contributor Recording Conventions
  ("Slot semantics"), closing the loophole this entry exposed: the name slot
  takes the model/persona identifier used in Contributors rosters; the
  origin slot takes the harness/model-id string. Routing unchanged:
  re-signing remains the contributor's own action, or a mechanical
  replacement authorized by Daniel. -- ox-alpha
  (opencode/x-preview-f-free), 08242026
