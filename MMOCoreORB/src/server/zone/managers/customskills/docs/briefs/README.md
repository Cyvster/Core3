# Task Briefs

Self-contained, delegable task specifications. Any LLM can execute one by
reading only what the brief lists -- no conversation history required.

> Subordinate to [../rules/project-design.md](../rules/project-design.md).
> Follow the pickup protocol and [../rules/process.md](../rules/process.md).

## Pickup protocol (for executing LLMs)

1. Read the workspace-root ``START-HERE.md`` (``G:\Data\Projects\project customskills\START-HERE.md``) fully first.
2. In the index below, find the **first** brief whose Status is UNCLAIMED.
3. Before claiming, re-read the index -- if another contributor claimed
   while you were reading, move to the next UNCLAIMED brief instead.
4. Claim it: set Status to CLAIMED with your signature and date in the
   brief FILE (the file carries the signature -- see Index maintenance),
   then commit `Claim <brief-id>` in this repository AND PUSH immediately -- an unpushed claim may be re-claimed ([PROC R6.5], R8/P2).
5. Read the brief plus every context file it lists, then do exactly the
   deliverables. Touch nothing outside the brief's scope.
6. Honor [PROC R6.6] change completeness: behavior changes carry their doc
   + package-mirror updates.
7. Set Status DELIVERED with notes on where the work landed; push; update
   [../tracking/objectives.md](../tracking/objectives.md) if the brief maps
   to an objective.
8. Problems or disputes go through [../rules/errata.md](../rules/errata.md)
   -- never silent edits outside your scope.
9. If no brief is UNCLAIMED, stop and report back -- do not invent work.

## How delegation works

**For Daniel:** hand an LLM this one instruction:

> Read the workspace-root START-HERE.md fully, then follow the pickup protocol
> in docs/briefs/README.md: claim the next UNCLAIMED brief and complete it.

**Index maintenance:**

- Append new rows at the bottom of the SINGLE table, Status UNCLAIMED.
  Keep rows in numeric order. Numbers are never reused ([PROC R3]).
- Index cells carry the state word only (`DELIVERED`, `UNCLAIMED`,
  `CLAIMED`, `WITHDRAWN`) -- never a name or date. Attribution lives in
  the brief file's signed Status line and in git history ([PROC R6.7]).
- The brief FILE's Status line is authoritative and IS signed. If file and
  index disagree, fix both; the file wins.

## Brief index

| File | Title | Status |
|------|-------|--------|
| [`001-migrate-mod-into-core3.md`](001-migrate-mod-into-core3.md) | Migrate customskills-mod into the Core3 repository; dissolve dual tree | DELIVERED |
| [`002-post-migration-doc-repair.md`](002-post-migration-doc-repair.md) | Post-migration documentation & governance repair (links, R6.5/R6.6, START-HERE) | DELIVERED |
| [`003-reconcile-hooks-patch-modifier-count.md`](003-reconcile-hooks-patch-modifier-count.md) | Regenerate patch incl. H16; reconcile modifier count + menu docs | DELIVERED |
| [`004-modifier-reference-refresh.md`](004-modifier-reference-refresh.md) | Refresh MODIFIER_REFERENCE against current config.lua (badgeOverrides, rarityNaming) | DELIVERED |
| [`005-single-source-badge-rule.md`](005-single-source-badge-rule.md) | Document single-source-of-truth rule for badge-backed modifiers | DELIVERED |
| [`006-reconcile-recording-conventions-with-alice.md`](006-reconcile-recording-conventions-with-alice.md) | Reconcile recording conventions with Project Alice framework (slots, short format, deviations) | DELIVERED |
| [`007-remove-hardcoded-critical-chance-defaults.md`](007-remove-hardcoded-critical-chance-defaults.md) | Remove residual C++ critical-chance seed/defaults; restore 60% cap via 60 explicit badge overrides | DELIVERED |
| [`008-deep-audit-module-code.md`](008-deep-audit-module-code.md) | Deep quality audit of all module-owned code; file ERR entries and produce remediation brief(s) | DELIVERED |
| [`009-condense-doc-formatting.md`](009-condense-doc-formatting.md) | Condense all guides; enforce heading hierarchy, section delimiters, feature-section template, TOCs | UNCLAIMED |
| [`010-combat-spam-labels-investigation-and-fix.md`](010-combat-spam-labels-investigation-and-fix.md) | Combat spam labels: same-line append fix, enable/disable switch, menu listing; removal fallback needs owner sign-off | UNCLAIMED |
| [`011-menu-config-single-source.md`](011-menu-config-single-source.md) | Menu/config single-source: eliminate duplicated badge assignments; resolve ERR-008 | DELIVERED |
| [`012-code-hygiene-cleanups.md`](012-code-hygiene-cleanups.md) | Enum/table safety asserts, loader consolidation, edge-case cleanups (behavior-preserving) | UNCLAIMED |

## Notes

- Briefs 001-003 were executed in sequence on 08232026 (001 physically
  relocated the module into this repo; 002 repaired references/governance;
  003 reconciled hooks/patch/count). Each lists its prerequisite.
- Brief 004 was opened from 003 delivery findings: config.lua drifted far
  past MODIFIER_REFERENCE (all modifiers enabled, badgeOverrides feature,
  rarityNaming section, criticalChance restructure). Execute it before
  trusting reference default tables.
- Brief file paths are stable across the migration: since 001 delivered, this
  queue lives at
  `Core3/MMOCoreORB/src/server/zone/managers/customskills/docs/briefs/`.
- The original 001 ("sync package with core3") was superseded by owner
  directive 08232026 when the dual-tree model was abolished; its remaining
  content-accuracy work was carried into 003.

---

**Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
BRIEF-008 DELIVERED (full module audit; AUDIT-1 filed as ERR-008 OPEN,
AUDIT-2 routed to BRIEF-010, remainder split into BRIEF-011 (menu fix) + BRIEF-012 (hygiene), both UNCLAIMED).
Prior: 08242026 -- BRIEF-009 added (doc condensation + formatting standard: single H1,
rule-delimited H2 sections, feature template, TOCs, >=40% line-count
reduction target) and BRIEF-010 added (combat spam labels:
investigate same-line append defect, add enable/disable switch +
Server Config listing; removal only on owner sign-off). Prior:
08242026 -- BRIEF-007 DELIVERED (C++ seed/constant removed; 60-badge x 100bp
override set reaches the 60% cap exactly; ERR-007 filed RESOLVED
self-verified under [DIRECTIVE 08242026]; compile check deferred to
Docker build env). Prior:
08242026 -- BRIEF-006 DELIVERED (recording conventions reconciled with Project Alice;
slot semantics + short format codified in process.md; OD-1 answered same
day via [DIRECTIVE 08242026]: dates switch to MMDDYYYY going forward).
Prior: 08232026 by hy3-free (opencode/hy3-free) --
BRIEF-004 DELIVERED (MODIFIER_REFERENCE refreshed against config.lua;
rarityNaming + badgeOverrides documented in INSTALLATION; ARCHITECTURE SUI
menu categories corrected). Prior: hy3-free (opencode/hy3-free), 08232026 --
BRIEF-005 added and DELIVERED (single-source rule for badge-backed modifiers,
spawned from ERR-005); ox-alpha (opencode/x-preview-f-free), 08232026 --
BRIEF-002 index rewrite; BRIEF-003 statuses 001-003 DELIVERED, added brief 004.

## Contributors

- **Daniel** -- project owner
- ox-alpha (opencode/x-preview-f-free), 08232026 -- initial queue
- hy3-free (opencode/hy3-free), 08232026 -- BRIEF-005
