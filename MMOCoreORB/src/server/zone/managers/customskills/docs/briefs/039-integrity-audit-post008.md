# Brief 039 -- Integrity audit: all post-BRIEF-008 deliveries

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Trigger: adopted integrity-audit cadence (XPP from Alice) -- a batch of
  ~15 briefs delivered since the last deep audit (BRIEF-008), much of it
  written by free-tier delegation workers; one compile-breaking defect
  (global-ns forward decls) already escaped to a build. Audit is due.

## Scope

Everything delivered after BRIEF-008's audit, code AND docs:

- Code: consolidated strike + tiered FCT (034), config overlay hook
  (027-impl), repeat-craft (036), SWGEMU options viewer (031), menu
  changes (025), shortened modifier names, command registrations.
- Docs: CONFIG_OPTIONS.md, MIGRATION_LEDGER.md, CODE_REFERENCE/USER_GUIDE
  updates, errata entries, ADMIN/PLAYER_COMMANDS cites.

## Audit checks

1. **Spec conformance**: re-read each brief; verify delivered code matches
   the spec (not just "compiles"). Flag drift in either direction.
2. **Cross-feature interactions** (the gap spot-checks miss):
   - consolidatedStrike knobs vs each other (fctEnabled off but chat tag on?)
   - repeat-craft snapshot vs config hot-reload (does overlay hook's
     configVersion bump corrupt an in-progress snapshot? should not --
     verify snapshot path never touches ConfigManager state)
   - repeat-craft vs Crafting Speed / Amazing Success modifiers (order of
     operations sane?)
   - menu viewer live reads vs overlay-forced values (display matches
     effective server behavior?)
   - new command registration vs existing command table entries
3. **Copy-paste/artifact scan**: worker-pressure artifacts -- wrong names,
   stale comments referencing superseded designs, leftover scratch keys
   (cs36.* format sanity), inconsistent error messages.
4. **Quality pass**: same lens as BRIEF-037's cyvster2 quality column --
   hot-path smells, lock usage, missing null guards on optional features.
5. **Docs-vs-code**: every documented key/command/cite exists and behaves
   as documented; MIGRATION_LEDGER statuses accurate.

## Rules

- READ-ONLY audit: findings are filed, not self-fixed. Each finding gets
  an ERR entry (OPEN) or a remediation brief reference, per severity:
  CRITICAL (breaks feature/server) / DEFECT (wrong behavior) / SMELL /
  DOC-MISMATCH.
- Verify claims against actual code with file:line cites; no trust in
  delivery summaries (second-party rule).

## Deliverables

- [ ] `docs/audits/integrity/08252026/checklist.md` + `findings.md`
      (Alice-style structure), findings table w/ severity + disposition.
- [ ] ERR entries filed for anything CRITICAL/DEFECT.
- [ ] Remediation briefs proposed for clusters (don't fix inline).
- [ ] Commit `[BRIEF-039]`, push.

## Out of scope

- Performance re-measurement (BRIEF-033 numbers still stand unless audit
  finds code that contradicts them).
- cyvster2 ledger content (audited at birth this session).
