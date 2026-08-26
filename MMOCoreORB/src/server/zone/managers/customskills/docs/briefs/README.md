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
8. **[R6.9] discovery line (mandatory in every delivery report)**: list each
   discovery made during the task with the reference-doc cite where it was
   captured, or state "no discoveries". A delivery without this line is
   incomplete and is returned by the coordinator ([BRIEF-040]).
9. Problems or disputes go through [../rules/errata.md](../rules/errata.md)
   -- never silent edits outside your scope.
9. If no brief is UNCLAIMED, stop and report back -- do not invent work.

## Brief authoring rules

Every new brief MUST include:

1. A **discovery-capture line** ([R6.9]) in its Task/Method or Deliverables:
   which owning reference doc discovered facts land in (e.g.
   "R6.9: SUI layout facts -> customskills/CODE_REFERENCE.md"), or an
   explicit "no discoveries expected" note if the task is purely editorial.
2. Discovery capture as a **Deliverables/Acceptance checkbox** whenever the
   task touches code or server behavior -- not just a side note.

A brief without these is incomplete; the coordinator rejects or patches it
before claim ([R6.9], BRIEF-019).
3. **XPP check**: if the brief implements a policy/procedure, file the
   mirror-review brief on Project Alice in the same session
   ([rules/process.md](../rules/process.md) -> Cross-Project Propagation)
   -- or note "not a policy" if purely technical.

## How delegation works

**For Cyvster:** hand an LLM this one instruction:

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
| [`009-condense-doc-formatting.md`](009-condense-doc-formatting.md) | Condense all guides; enforce heading hierarchy, section delimiters, feature-section template, TOCs | DELIVERED |
| [`010-combat-spam-labels-investigation-and-fix.md`](010-combat-spam-labels-investigation-and-fix.md) | Combat spam labels: same-line append fix, enable/disable switch, menu listing; removal fallback needs owner sign-off | DELIVERED |
| [`011-menu-config-single-source.md`](011-menu-config-single-source.md) | Menu/config single-source: eliminate duplicated badge assignments; resolve ERR-008 | DELIVERED |
| [`012-code-hygiene-cleanups.md`](012-code-hygiene-cleanups.md) | Enum/table safety asserts, loader consolidation, edge-case cleanups (behavior-preserving) | DELIVERED |
| [`013-delegated-broadcast-combat-spam.md`](013-delegated-broadcast-combat-spam.md) | Delegate broadcastCombatSpam to module code; same-line labels via raw-text full lines | WITHDRAWN |
| [`014-remove-combat-spam-labels.md`](014-remove-combat-spam-labels.md) | Remove combat spam label feature entirely from code, config, and docs (owner decision) | DELIVERED |
| [`015-implement-attack-repeat-tiers.md`](015-implement-attack-repeat-tiers.md) | Implement Double/Triple/Quad attack repeat tiers in the applyDamage delegation; resolves ERR-009 | DELIVERED |
| [`016-defaultize-badge-overrides.md`](016-defaultize-badge-overrides.md) | Uniform badgeBonus defaults everywhere; badgeOverrides become dormant placeholders; caps re-verified | DELIVERED |
| [`017-verify-badge-reconfiguration.md`](017-verify-badge-reconfiguration.md) | Independent verification + completion of the badge reconfiguration; repair config corruption + doc sweep | DELIVERED |
| [`018-document-swgemu-admin-commands.md`](018-document-swgemu-admin-commands.md) | Document SWGEmu admin (GM) commands in a dedicated `swgemu/` section: permission chain, command table, worked operator scenarios | DELIVERED |
| [`019-enforce-discovery-capture.md`](019-enforce-discovery-capture.md) | Make R6.9 discovery capture a standing, unmissable procedure (checklist + gotcha convention) | DELIVERED |
| [`020-apply-dense-format-reference-docs.md`](020-apply-dense-format-reference-docs.md) | Apply approved dense doc format (tight blocks, stacked no-bullet lines) to reference/lookup docs only | DELIVERED |
| [`021-verify-admin-commands-vs-codebase.md`](021-verify-admin-commands-vs-codebase.md) | Cross-verify PDF-derived admin commands against Core3 command classes; cite file:line or mark not-on-build | DELIVERED |
| [`022-player-commands-reference.md`](022-player-commands-reference.md) | PLAYER_COMMANDS.md: 283-command grouped inventory + 66 verified deep-dives (39.6KB) | DELIVERED |
| [`023-hidden-config-options.md`](023-hidden-config-options.md) | CONFIG_OPTIONS.md: full verified inventory of ConfigManager-read options; integrated into CODE_REFERENCE | DELIVERED |
| [`024-privacy-scrub.md`](024-privacy-scrub.md) | Remove owner real name + local drive paths from all tracked files; add standing privacy rule | DELIVERED |
| [`025-menu-modifier-display-and-formatting.md`](025-menu-modifier-display-and-formatting.md) | Menu: show all modifiers per badge, short names (Cri/Crit Multi/Double/Triple/Quad), fix bottom-pane gap, retry column alignment | DELIVERED |
| [`026-swemu-options-viewer.md`](026-swemu-options-viewer.md) | Populate SWGEMU Options menu category: curated live server-config viewer (no secrets) -- implemented as BRIEF-031 | DELIVERED |
| [`027-hidden-options-through-mod-research.md`](027-hidden-options-through-mod-research.md) | Research: route hidden options through mod overlay (verdict: FEASIBLE-WITH-CAVEATS); impl delivered b273b193ae (overlay hook + 55-line commented example) | DELIVERED |
| [`028-document-lifecycle-archives-adoption.md`](028-document-lifecycle-archives-adoption.md) | Adopt Alice-style bounded ledgers + permanent archives (tombstones, verbatim moves, reconciliation-time trigger) | DELIVERED (rules L1-L6 in process.md; errata convention conformed) |
| [`029-review-verification-gate-from-alice.md`](029-review-verification-gate-from-alice.md) | XPP mirror-review: verification gate evaluation -- folded into BRIEF-030 gap 1 (ADOPTED) | DELIVERED (folded into 030) |
| [`030-align-process-to-alice.md`](030-align-process-to-alice.md) | XPP alignment pass 1: adopt Alice's governance gaps (verification gate, audits, errata discipline, milestones, roster); creates Alice mirror brief | DELIVERED |
| [`031-swemu-options-viewer-implementation.md`](031-swemu-options-viewer-implementation.md) | BRIEF-026 impl: static curated registry (~20 options, secrets excluded), live ConfigManager reads, restart flags | DELIVERED |
| [`032-floating-combat-text-investigation.md`](032-floating-combat-text-investigation.md) | FCT hook investigation: scale YES / color YES / content PARTIAL / position+effects NO (client-mod territory) | DELIVERED (research) |
| [`033-menu-performance-research.md`](033-menu-performance-research.md) | Menu performance: ~60-150us/open, 1000-burst ~100-150ms NEGLIGIBLE; client SUI cliff at ~1000-1500 rows/page | DELIVERED (research) |
| [`034-consolidated-strike-fct.md`](034-consolidated-strike-fct.md) | Double/Triple/Quad become damage multipliers (single strike) + tiered FCT: scale+color escalate per tier/crit, config-driven | DELIVERED |
| [`035-crafting-tool-repeat-research.md`](035-crafting-tool-repeat-research.md) | Viability: repeat-craft option for inventory crafting tool (session lifecycle, snapshot reuse, client constraints); request predated any filing (no agent connected); re-filed from owner restatement | DELIVERED (PARTIAL: assisted pre-fill viable; server-side loop not) |
| [`036-repeat-craft-implementation.md`](036-repeat-craft-implementation.md) | Repeat-craft: /repeatcraft + tool-snapshot pre-fill (luaStringData cs36.*, zero IDL changes); repeatEnabled default OFF | DELIVERED |
| [`037-cyvster2-modification-inventory.md`](037-cyvster2-modification-inventory.md) | Archaeology: 610 commits -> ~48 net mods (12 FEATURE/28 TWEAK/3 FIX/5 DEAD); MIGRATION_LEDGER.md live: 4 IN-MOD, 30 TODO, 14+ WONT-PORT | DELIVERED |
| [`038-mission-terminal-options-port.md`](038-mission-terminal-options-port.md) | Port mission terminal direction/difficulty options from cyvster2 into mod; research -> design -> implement; updates ledger | RESEARCH DELIVERED (design in [`_038_design.md`](_038_design.md); implementation pending owner sign-off) |
| [`039-integrity-audit-post008.md`](039-integrity-audit-post008.md) | Integrity audit: 0 CRITICAL / 4 DEFECT / 3 SMELL / 2 DOC-MISMATCH; ERR-016..019 filed (flytext broadcast scope, backpack-blind prefill, exp snapshot dead-write) | DELIVERED |
| [`040-discovery-capture-enforcement.md`](040-discovery-capture-enforcement.md) | R6.9 enforcement: mandatory delivery discovery line (pickup step 8), hotfix same-commit rule, coordinator grep duty; backfilled 5 leaked facts | DELIVERED |
| [`041-repeatcraft-subcommand.md`](041-repeatcraft-subcommand.md) | Convert /repeatcraft to /customskills repeatcraft subcommand (TRE policy: no new top-level commands); add tool radial-menu repeat option; research alternate autocraft objects | CLAIMED |
| [`042-remediation-fct-repeatcraft.md`](042-remediation-fct-repeatcraft.md) | Remediation: FCT visibility (suppress vanilla on escalation, broadcast), lair/TANO tier support, repeat-craft hardening (ERR-017..019), subcommand+radial | DELIVERED |

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

**Last reconciled:** 08252026 by ox-alpha (opencode/x-preview-f-free) --
index brought current: 023/026/027/032/033/035 DELIVERED (research batch),
034 consolidated strike DELIVERED, 029 folded into 030, missing 031 row
added, numeric order restored. In flight: 027 impl (config overlay hook),
036 repeat-craft implementation DELIVERED.
Prior: 08242026 -- BRIEF-008 reconciliation (see git history for earlier entries).
