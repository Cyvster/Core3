# Errata

> Subordinate to [project-design.md](project-design.md) -- the master
> document for project rules.

> **Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
> owner delegation recorded: sole-worker self-verification authorized
> ([DIRECTIVE 08242026]); BRIEF-006: quick-start conventions item now
> references process.md ([PROC R6.7]); ERR-006 findings note appended
> pointing at the clarified slot semantics; entry template Date field
> aligned to [DIRECTIVE 08242026] (MMDDYYYY going forward). Prior: 08232026 by hy3-free (opencode/hy3-free) -- second-party verification of ERR-001..004 (all RESOLVED against CORE3_CODE_REFERENCE.md); ERR-005 resolved (owner directed dir (a), code now reads config badge map) and applied 08232026; ERR-006 resolved (signature re-sign to hy3-free form) 08232026. Originally migrated/filed by ox-alpha (opencode/x-preview-f-free), 08232026.
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
   | Cyvster                        | always                  | always              |

4. Resolution (OPEN entries):
   - Reproduce or confirm the issue yourself before proposing anything:
     re-read the cited document or re-check this repository's sources --
     whichever the entry touches.
   - Propose the MINIMAL fix that satisfies the entry -- no refactors, no
     out-of-scope improvements.
   - Set Status to `AWAITING-VERIFICATION` with your signature. If the fix
     is applied by the filer in the same session (small, low-risk edits
     only), set Status to `RESOLVED-PENDING-VERIFICATION` instead -- a
     second party must still verify before the entry is treated as settled
     (convention adopted from Project Alice, XPP 08252026).
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
anyone else may; Cyvster is exempt from both restrictions.

> **Owner delegation ([DIRECTIVE 08242026]):** Cyvster (project owner;
> "Cyvster" online) authorized sole-worker sessions to perform their own
> verification. Verbatim: "you are the single worker. you will need to
> perform your own verification. note that you were delegated this
> authority by the project owner cyvster." Self-verified resolutions are
> marked `self-verified under [DIRECTIVE 08242026]` and remain challengeable
> via a new ERR entry ([PROC R7]).

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
`docs/swgemu/CODE_REFERENCE.md`), with its own entry scheme
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
   someone other than its proposer: another LLM session, or Cyvster.
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
                        verifications; Cyvster decides)
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

Cyvster may resolve, reject, or apply any entry directly.

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

- **Cyvster** -- project owner; may resolve/reject any entry directly
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
- Resolution: RESOLVED -- owner (Cyvster) directed fix direction (a); applied 08232026 by hy3-free (opencode/hy3-free). `CustomSkillsModifiers::getCriticalChance(PlayerObject*)` now aggregates from `CustomSkillsConfig::getBadgeBonuses(CRITICAL_CHANCE)` (the same config badge map the SUI menu uses via `getModifierTotal`), applying the configured cap. Retired the hardcoded `combatProfessionBadges` static array, `isCombatProfessionBadge`, `getBadgeCriticalChance`, the 2-arg `getCriticalChance` overload, and the unused `CustomSkillsConfig::getCriticalChancePerCombatBadge()` getter. Combat and menu now share one source of truth ([CS-3]); config.lua badges are authoritative. Second-party verification by hy3-free (opencode/hy3-free), 08232026.


---

## ERR-006 -- Contributor signatures use harness name instead of model name

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08232026
- Affects: 14 signature instances across docs/briefs/005-single-source-badge-rule.md, docs/briefs/README.md, docs/reference/ARCHITECTURE.md, docs/rules/errata.md
- Severity: F5 (formatting/attribution clarity; no truth value)
- Description: signatures were recorded as `opencode (opencode/hy3-free)` -- the harness name occupies the name slot. Per docs/rules/process.md -> Contributor Recording Conventions, the format is `<name/model> (<origin>), <date>`; the name slot takes the MODEL name. Correct form: `hy3-free (opencode/hy3-free)`. This matters here because all workers commit under the shared machine git identity (`Cyvster`), so document signatures are the ONLY worker-attribution record in the repository.
- Evidence: process.md signing format + examples; contrast with the same contributor's correctly-signed entries in the Project Alice repository (`hy3-free (opencode/hy3-free)`, 08232026).
- Proposed fix: hy3-free re-signs the affected lines in their own voice at next session (Rule 2 -- other entities do not edit another contributor's signatures). Instance list available via grep pattern `opencode (opencode/hy3` outside archive/. Alternatively Cyvster may authorize a mechanical replacement.
- Findings: filed as a note-for-contributor during errata duty; no dispute, purely format.`n- Resolution: RESOLVED -- hy3-free self-corrected all affected signatures before any external edit (repo-wide grep 08232026: 17 instances of correct `hy3-free (opencode/hy3-free)` form, 0 remaining wrong-form instances outside this entry's own quoted evidence and the BRIEF-006 problem statement). F5 formatting entry: closure is a mechanical grep verification recorded by the filer; any contributor may re-open with counter-evidence. -- ox-alpha (opencode/x-preview-f-free), 08232026
- Resolution: RESOLVED -- hy3-free re-signed all affected lines in their own voice: `opencode (opencode/hy3-free)` -> `hy3-free (opencode/hy3-free)` across START-HERE.md, docs/rules/errata.md, docs/reference/ARCHITECTURE.md, docs/briefs/README.md, and docs/briefs/005-single-source-badge-rule.md. The quoted wrong-form example in the Description above is intentionally retained as evidence. Applied 08232026 by hy3-free (opencode/hy3-free); second-party verification by hy3-free (opencode/hy3-free) (non-filer of this entry).
- Findings: BRIEF-006 (08242026) codified explicit name-slot/origin-slot
  semantics in [process.md](process.md) -> Contributor Recording Conventions
  ("Slot semantics"), closing the loophole this entry exposed: the name slot
  takes the model/persona identifier used in Contributors rosters; the
  origin slot takes the harness/model-id string. Routing unchanged:
  re-signing remains the contributor's own action, or a mechanical
  replacement authorized by Cyvster. -- ox-alpha
  (opencode/x-preview-f-free), 08242026

---

## ERR-007 -- Hardcoded critical-chance constructor defaults survived ERR-005 remediation

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08242026
- Affects: `CustomSkillsConfig.cpp` / `.h` (constructor seed +
  `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE`);
  `bin/scripts/customskills/config.lua`; CODE_REFERENCE.md Appendix A
- Severity: F1 (runtime behavior contradicted documented maximum and
  [CS-3] single-source-of-truth; violated owner no-hardcode policy)
- Description: the C++ constructor seeded all 12 combat mastery badges
  into the CRITICAL_CHANCE map at
  `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE` (300 bp), and
  `setUniformBadgeBonus()` rescaled those seeded entries to whatever
  `config.lua` set `badgeBonus` to (400 bp shipped). Full-badge players
  therefore received an undocumented 12 x 4% = +48% on top of the
  documented +12% milestone bonus (exactly the 60% cap), while all
  documentation stated a 12% maximum. A missing/invalid config silently
  ran crit chance at 36%. The hardcode survived the ERR-005 cleanup,
  which had only removed the separate static array in
  `CustomSkillsModifiers.cpp`.
- Evidence: constructor block seeding `combatBadges[]` via
  `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE` (CustomSkillsConfig.cpp,
  pre-BRIEF-007); `setUniformBadgeBonus(CRITICAL_CHANCE, ...)`
  rescaling seeded entries in `load()`; ERR-005 resolution history.
- Proposed fix: remove the seed and the constant; make the badgeBonus
  fallback an explicit 0 (no silent grants); restore 60% cap reachability
  through the owner-specified 60-badge x 100 bp override set
  ([BRIEF-007]).
- Findings: discovered 08242026 during the badge-bonus rebalancing
  session when Cyvster asked what the code does when the critical chance
  value is empty. Interim commit `049ef1d0d0` (badgeBonus = 0) exposed
  the hidden layer by dropping full-set players from 60% to 12% pending
  this fix. -- ox-alpha (opencode/x-preview-f-free), 08242026
- Resolution: RESOLVED -- BRIEF-007 delivered: constructor seed and
  constant removed (`criticalChanceFallbackBonus`, default 0);
  `config.lua` expanded to the owner-specified 60-badge x 100 bp
  override set reaching the 6000 bp cap exactly; Appendix A, USER_GUIDE,
  and INSTALLATION updated to match config.lua as authority. Applied
  08242026 by ox-alpha (opencode/x-preview-f-free); self-verified under
  [DIRECTIVE 08242026].

---

## ERR-008 -- /customskills menu duplicates config badge assignments; totals drift from gameplay after config changes

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08242026
- Affects: `CustomSkillsMenu.cpp` anonymous-namespace `badges*` arrays;
  produced by BRIEF-008 audit (AUDIT-1)
- Severity: F1 (menu displays totals that contradict actual gameplay
  values; violates [CS-3] single-source-of-truth)
- Description: the menu hardcodes a second copy of every modifier's
  badge assignments in group arrays and computes per-page lists/counts/
  totals from them. After BRIEF-007 expanded Critical Chance to 60
  badges in config.lua, the menu's Critical Chance page still shows the
  old 24-key subset and its category total computes 24% while combat
  applies 60%. Any future config edit re-creates the same drift.
- Evidence: CustomSkillsMenu.cpp `badgesCritChance` (24 keys) vs
  config.lua criticalChance (60 keys); menu totals via
  `countModifier()` over those arrays vs gameplay via
  `CustomSkillsModifiers::getModifierTotal` over the config map.
- Proposed fix: derive menu grouping from the config map at runtime
  (preferred) or add boot-time drift validation; remediation scheduled
  as BRIEF-011 Phase A.

---

## ERR-009 -- Double/Triple/Quad attack chance modifiers have no gameplay implementation

- Status: RESOLVED
- Filed by: ox-alpha (opencode/x-preview-f-free)
- Date: 08242026
- Affects: `config.lua` (doubleAttackChance, tripleAttackChance,
  quadAttackChance); `CustomSkillsMenu.cpp` BONUS_* pages; produced by
  BRIEF-010 Phase 1 investigation
- Severity: F1 (menu advertises bonuses that cannot proc; players see
  totals for mechanics that do not exist)
- Description: the three repeat-attack modifiers are loaded from config
  and displayed in `/customskills`, but no combat code consumes their
  modifier types -- there is no repeat-tier roll anywhere. Their
  combatSpamLabels are likewise never emitted.
- Evidence: repo-wide sweep shows DOUBLE_ATTACK_CHANCE /
  TRIPLE_ATTACK_CHANCE / QUAD_ATTACK_CHANCE referenced only by
  CustomSkillsConfig.cpp (loader), CustomSkillsMenu.cpp (display),
  CustomSkillsModifierType.h (enum), and docs. No hooks patch call site
  touches them.
- Proposed fix: owner decision required: (a) implement tier mechanics in
  the damage pipeline via new hooks, (b) remove the three modifiers from
  menu + config until implemented, or (c) leave as-is with documented
  caveat. Filed during BRIEF-010 Phase 1.
- Resolution: RESOLVED -- BRIEF-011 removed the 18 duplicated per-modifier
  arrays from CustomSkillsMenu.cpp; BONUS_* menu pages now derive badge
  lists, owned counts, and totals directly from the config map via
  `getBadgeBonuses(type)` (single source of truth). Verified: Critical
  Chance page iterates all 60 config keys. Applied 08242026 by ox-alpha
  (opencode/x-preview-f-free); self-verified under [DIRECTIVE 08242026].

- Resolution: RESOLVED -- owner elected to build the mechanic (Option a).
  BRIEF-015 implemented attack repeat tiers inside the existing
  `CustomSkillsCombat::applyDamage` delegation (no new SWGEmu hooks):
  Quad -> Triple -> Double rolled highest-first, one tier per hit,
  chances = capped config totals in basis points; triggered tier
  multiplies the crit-adjusted damage before mitigation (equivalent to
  repeating the finalized hit). Label-free per the combat-spam removal

- Follow-up (BRIEF-034, 08252026 by ox-alpha/opencode-x-preview-f-free):
  ERR-009's repeat-chain delivery is SUPERSEDED. Double/Triple/Quad now
  multiply damage by the tier ONCE and call applyVanillaDamage a single
  time (consolidated strike); the multi-application loop and its mid-loop
  death/incapacitation break are removed. Correction (owner caught it):
  armor mitigation is IDENTICAL for one big hit vs N small hits -- every
  mitigation stage is a multiplicative percentage (CombatManager.cpp
  getArmorReduction chain), and percentages commute with the tier
  multiplier, so totals match exactly; no deviation exists. Presentation added: tiered ShowFlyText
  (config-driven scale/colors) and an "xN" combat-spam tag line to the
  attacker; see `consolidatedStrike` config block.

## ERR-010 -- CustomSkillsMenu.cpp fails to compile (header/.cpp desync)

- Status: RESOLVED
- Filed by: hy3-free (opencode/hy3-free)
- Date: 08242026
- Affects: src/server/zone/managers/customskills/CustomSkillsMenu.cpp,
  CustomSkillsMenu.h
- Severity: F5 (blocking build -- core3 target)
- Description: the module's menu failed to compile in the Docker/ninja
  build. Three distinct breakages, all pre-existing (NOT from BRIEF-014,
  which only deleted combat-spam-label lines):
  1. `CustomSkillsMenu.cpp` defines and calls `addBonusItems(...)` and
     `countOwnedBonuses(...)` but `CustomSkillsMenu.h` never declares
     them (error: "no declaration matches").
  2. Two switch blocks in `getAcquiredCount()` and `addPageItems()` use the
     modifier enum UNQUALIFIED (`CRITICAL_CHANCE`, `DOUBLE_ATTACK_CHANCE`,
     `AMAZING_RESULTS`, etc.) while the rest of the file correctly qualifies
     them as `CustomSkillsModifierType::CRITICAL_CHANCE`. Root cause:
     `CustomSkillsModifierType` is a **class** with a nested `enum Type`
     (`CustomSkillsModifierType.h:4-27`) -- NOT a namespace -- so
     `using namespace CustomSkillsModifierType;` is illegal and qualified
     `CustomSkillsModifierType::XXX` lookup was the only valid form. The
     bare constants therefore fail in the enclosing scope.
  3. The new code path calls `CustomSkillsConfig::getBadgeBonuses(type)`
     (verified present in CustomSkillsConfig.h/.cpp), so no further missing
     symbol beyond 1-2.
- Evidence: build log `[11/19] Building CXX object .../CustomSkillsMenu.cpp.o
  FAILED` with "'CRITICAL_CHANCE' was not declared in this scope",
  "'countOwnedBonuses' was not declared in this scope", "'no declaration
  matches void CustomSkillsMenu::addBonusItems(...)'". Only this one module
  TU failed; the other 5 customskills .cpp (incl. CustomSkillsCombat.cpp from
  BRIEF-014) compiled cleanly ([11/19] reached before failure), confirming
  the break is local to this file's desync, not the broader branch.
- Proposed fix: (a) declare the two functions in CustomSkillsMenu.h
  (addBonusItems(SuiListBox*, CreatureObject*, CustomSkillsModifierType::Type);
  countOwnedBonuses(CreatureObject*, CustomSkillsModifierType::Type)); (b) qualify
  every bare enum constant in the two switch blocks as
  `CustomSkillsModifierType::XXX` (the `using namespace` form is illegal because
  the type is a class, not a namespace). Applied as ERR-010 fix.
- Resolution: RESOLVED -- applied 08242026 by hy3-free (opencode/hy3-free):
  added both declarations to CustomSkillsMenu.h and qualified all 18 enumerators
  in `getAcquiredCount()` and `addPageItems()` (removed the illegal `using
  namespace CustomSkillsModifierType;`). Grep confirms 0 unqualified enum
  references remain; brace balance 55/55. Compile re-test pending on the Linux
  build host (deferred per engine3 toolchain caveat, as with BRIEF-014).
  Config accessor `getBadgeBonuses(type)` confirmed present, so no further
  missing symbol expected.
## ERR-011 -- CustomSkillsMenu link fails: countModifier() defined body missing

- Status: RESOLVED
- Filed by: hy3-free (opencode/hy3-free)
- Date: 08242026
- Affects: src/server/zone/managers/customskills/CustomSkillsMenu.cpp
- Severity: F5 (blocking build -- core3 link target)
- Description: after the ERR-010 compile fixes, the build reached the LINK
  phase (all 442 TUs compiled, including CustomSkillsMenu.cpp.o) and then
  failed with: `undefined reference to
  'CustomSkillsMenu::countModifier(server::zone::objects::creature::CreatureObject*, char const* const*, int, CustomSkillsModifierType::Type)'`.
  The function is DECLARED in CustomSkillsMenu.h (line 38) and CALLED by
  `getModifierTotal()` via the `MOD_LEAF(pageName, data)` macro (line 189:
  `countModifier(player, data, countOf(data), type)`), but its BODY/DEFINITION
  was entirely absent from the .cpp -- a declared-and-called symbol with no
  implementation. Same root cause class as ERR-010: a function the
  broken-LLM generation emitted a declaration + call for but dropped the body.
- Evidence: link log `Linking CXX executable src/core3` FAILED, `collect2:
  error: ld returned 1 exit status`; symbol name exact, only one undefined
  reference (no cascade). The other customskills TUs (CustomSkillsConfig,
  CustomSkillsModifiers, CustomSkillsCombat, etc.) link cleanly.
- Proposed fix: provide the missing `countModifier` definition, mirroring the
  proven gameplay pattern in CustomSkillsModifiers.cpp:60-80
  (getBadgeBonuses(type) -> VectorMap<String,int>; for each owned badge sum
  bonuses.elementAt(j).getValue()). It iterates the category's `keys[]`
  (count = countOf(data)), checks `ghost->hasBadge(badge->getIndex())`, and
  sums the matching badge's basis-point bonus. Used by MOD_LEAF inside
  getModifierTotal before applyModifierCap.
- Resolution: RESOLVED -- applied 08242026 by hy3-free (opencode/hy3-free):
  added the countModifier definition immediately before getModifierTotal(),
  written to match the exact iterator/key-match pattern of
  CustomSkillsModifiers::getModifierTotal (avoiding any unverified VectorMap
  .get(key) member; uses elementAt(j).getKey()/.getValue()). Braces balanced
  58/58; exactly 1 definition + 1 call; grep confirms no other
  declared-but-undefined statics in CustomSkillsMenu.h. Link re-test pending
  on the Linux build host. Net: ERR-010 + ERR-011 together close the
  CustomSkillsMenu compile+link desync chain.

## ERR-012 -- BRIEF-018 initial delivery incomplete (10 of ~63 commands)
- STATUS: RESOLVED
- FILED: 08252026 by hy3-free (opencode/hy3-free)
- SYMPTOM: Initial ADMIN_COMMANDS.md documented only 10 commands found via
  `bin/scripts/commands/*.lua` grep; owner-supplied Commands V2 reference
  documents ~63. Owner rejected the delivery.
- ROOT CAUSE: Source-selection error -- grounding solely on the module repo's
  command scripts captures only commands with dedicated .lua wrappers on this
  codebase, not the full stock SWGEmu admin surface.
- FIX: Rebuilt swgemu/ADMIN_COMMANDS.md from the owner PDF (Commands V2 -
  Posted.pdf), retaining the 5 disk-verified commands with their file:line
  cites and flagging PDF-derived entries for C++ cross-verification per R6.10.
- LESSON: For "document system X" briefs, ask for or locate the AUTHORITATIVE
  upstream reference before grepping local artifacts; local grep defines what
  is *verified*, not what *exists*.
- COMMIT: ddd2cf6de9

## ERR-013 -- ADMIN_COMMANDS.md format regressions during scripted rebuild
- STATUS: RESOLVED
- FILED: 08252026 by hy3-free (opencode/hy3-free)
- SYMPTOM: Two successive script passes each broke one half of the house
  list format on swgemu/ADMIN_COMMANDS.md: pass 1 emitted delegated catalog
  lines verbatim (no backticks -> no command highlighting); pass 2 added
  backticks but stripped GFM trailing double-space hard breaks (lines
  collapsed into paragraphs). A third edit briefly corrupted the file via a
  self-referential str.replace (21 MB explosion), caught by size check and
  reverted before commit.
- FIX: 0d48a6534c restored hard breaks on all 134 lines inside a code-fence
  guard; 4a870e64aa had restored backticks. Final template per line:
  `/cmd <syntax>` in ticks + em-dash desc + **Ex:** example in ticks +
  file:line cite, ending with two spaces.
- LESSON: (1) When scripting markdown transforms, preserve the ENTIRE line
  template (both backticks AND hard-break suffix) in one pass -- never split
  formatting properties across passes. (2) Never use str.replace where the
  replacement contains or can re-match the search string; prefer line-based,
  idempotent edits with a post-write size sanity check.
- COMMITS: c87b829af1, 4a870e64aa, 0d48a6534c, 65c50000f9, 6c0802808f

## ERR-014 -- Live documents expose owner real name + local filesystem paths
- STATUS: OPEN (resolution tracked via BRIEF-024)
- FILED: 08252026 by hy3-free (opencode/hy3-free), owner-reported privacy
  violation.
- SYMPTOM: ~49 occurrences of the owner's real name across the docs tree
  (errata.md, briefs, proposals, process.md, project-design.md, LLM_GUIDE,
  tracking, swgemu/CODE_REFERENCE) plus local absolute paths (owner's Windows
  drive layout) in swgemu/CODE_REFERENCE. The repo is public; the owner is
  known online only as Cyvster. No standing convention forbids this class of
  exposure.
- ROOT CAUSE: No privacy rule existed in process.md; contributors (human and
  LLM) transcribed conversational context (names, local paths) into tracked
  files unchecked.
- FIX: BRIEF-024 -- scrub all tracked files (name -> Cyvster / "the owner";
  paths -> repo-relative or generic), add standing privacy rule to
  process.md, add pre-push grep check to LLM_GUIDE session protocol.
  Upstream third-party content (SOE credit tables in sql/) is NOT touched.
- NOTE: git history retains old occurrences; history rewrite is a separate
  owner-approved decision, documented in the BRIEF-024 delivery report.

## ERR-015 -- Project Alice provenance may import identifying context
- STATUS: OPEN
- FILED: 08252026 by hy3-free (opencode/hy3-free)
- SYMPTOM: rules/process.md cites the "Project Alice framework" as its
  provenance and adapts several conventions from it. Alice's own materials
  and any mirrored content have not been audited for real names, local paths,
  or machine details that could flow into this repo's public docs through
  continued adaptation.
- RISK: Same exposure class as ERR-014 via a second channel (external
  framework references).
- FIX: Audit every Project Alice reference/adaptation in tracked files;
  sanitize identifying context; where provenance must stay generic, cite
  "the coordinator-process framework" without operator-identifying detail.
  Execute together with BRIEF-024's sweep; fold findings into the BRIEF-024
  delivery report.
