# BRIEF-021 -- Cross-verify PDF-derived admin commands against Core3 command classes

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner-approved follow-up
  to the BRIEF-018 redo (ERR-012).
- Priority: ERRATA > BRIEFS > anything else. No live errata blocking.

## Problem

swgemu/ADMIN_COMMANDS.md (commit ddd2cf6de9) now documents ~63 admin commands.
Five are disk-verified with file:line cites; the remaining ~58 derive from the
owner-supplied Commands V2 reference and are flagged as reference-derived.
R6.10 requires claims to cite verifiable sources — these entries currently
cite an external PDF, not this codebase.

## Goal

For every PDF-derived command, verify against THIS codebase:

1. Does a command class exist? (`MMOCoreORB/src/server/zone/objects/creature/commands/<Name>Command.h`
   and/or `bin/scripts/commands/<name>.lua`)
2. Do its argument shapes match what ADMIN_COMMANDS.md documents?
3. Record file:line evidence for each verified command.

## Method

1. List all `*Command.h` under `src/server/zone/objects/creature/commands/`
   and all `.lua` under `bin/scripts/commands/`.
2. For each of the ~58 PDF-derived commands, check presence + parse args from
   the C++ class (`getToken`/`getIntToken`/`getFloatToken` calls) or lua wrapper.
3. Update each ADMIN_COMMANDS.md line: replace "reference-derived" caveat with
   file:line cite where verified; annotate divergences (arg shape differences,
   missing commands) explicitly rather than silently deleting them.
4. Commands NOT found on this codebase get a visible marker, e.g.
   `[not on this build]`, so operators know before typing them. Do not delete
   them -- the doc serves operators on multiple server versions.

## Acceptance criteria

- [ ] Every command line in ADMIN_COMMANDS.md carries either a file:line cite
      (verified) or an explicit `[not on this build]` / divergence annotation.
- [ ] Divergences between PDF and codebase listed in the delivery report.
- [ ] No silent removals.
- [ ] Committed + pushed; brief marked DELIVERED with report.

## Notes

- The Assembla wiki Admin Command Reference (2008) may help adjudicate
  historical naming (e.g. grantBadge vs grantbadge case) but the CODEBASE is
  authoritative for what works on THIS build.
- Discovery capture per R6.9: any new facts about the command-registration
  path (CommandConfigManager, command list lua) go to swgemu/CODE_REFERENCE.md
  same-session.
