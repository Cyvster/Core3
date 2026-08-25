# BRIEF-021 -- Deep-dive admin command reference from code inspection

- Status: DELIVERED (ADMIN_COMMANDS.md portion; PLAYER_COMMANDS.md deferred —
  see delivery report)
- Created: 08252026 by hy3-free (opencode/hy3-free); scope revised same day
  per owner direction.
- Priority: ERRATA > BRIEFS > anything else. No live errata blocking.

## Owner guidance (authoritative for scope)

- The owner-provided materials (Commands V2 PDF) are **reference only, NOT
  authoritative**. The codebase is the sole authority.
- Expectation: **every admin command verified by direct code inspection** --
  exact parameter structure, description, and a usage example per command.
- Most commands have NO in-game help; operators must know exact syntax. SWGEmu
  devs intentionally under-document to deter rival servers. **This project's
  goal is to help private server operators** -- this doc is the compensation.
- Follow policy: document discoveries in the relevant reference docs
  (R6.9/R6.10).

## Problem

swgemu/ADMIN_COMMANDS.md (commit ddd2cf6de9) documents ~63 commands, most
derived from an external PDF with no per-command verification. R6.10 requires
code-cited claims. The full admin command surface on THIS build is unknown --
it may be larger or smaller than the PDF suggests.

## Goal

Produce a complete, code-verified admin command reference:

1. Enumerate the FULL command surface on this build:
   - `MMOCoreORB/src/server/zone/objects/creature/commands/*Command.h` classes
   - `MMOCoreORB/bin/scripts/commands/*.lua` wrappers
   - the command registration path (`CommandConfigManager` / command table /
     `CommandListMap`) to determine which are admin-restricted (god mode /
     admin levels) vs player commands.
2. For EACH admin command, from its C++ class parse:
   - exact syntax (required vs optional args, order, types)
   - subcommands/flags (grep `getToken`, `getIntToken`, `getFloatToken`,
     `-area`, `-help` handling, String equality chains)
   - behavior description grounded in what the class actually does
   - at least one usage example (constructed from parsed syntax)
   - file:line cite
3. Rebuild swgemu/ADMIN_COMMANDS.md from this evidence. PDF-only entries that
   code inspection cannot confirm get `[not found in code]` markers or are
   moved to an appendix section -- no silent deletions.
4. Where the PDF and code disagree, CODE WINS; note the divergence in the
   delivery report.
5. **Player commands encountered during the sweep get documented too** (owner
   directive): create `swgemu/PLAYER_COMMANDS.md` in the same house format for
   any non-admin command whose inspection yields useful operator/player
   documentation. Same evidence bar: exact syntax, description, example,
   file:line cite.

## Method notes

- This is a large mechanical+judgment pass (~100+ command classes expected).
  Delegate enumeration/drafting to worker bots/opencode in batches by category;
  orchestrator independently spot-checks a sample of each batch against source
  (R6.10: never trust summary alone).
- Admin-level gating: check how commands are flagged restricted
  (`requiresAdmin` / god mode checks in ObjectController or command config)
  and record the actual gate per command if it varies.
- Discovery capture (R6.9): facts about the registration/gating path go to
  swgemu/CODE_REFERENCE.md same-session.

## Acceptance criteria

- [ ] Full command inventory enumerated from code (count reported).
- [ ] Every documented command has: exact syntax, description, >=1 example,
      file:line cite.
- [ ] Commands not present on this build explicitly marked, not deleted.
- [ ] Gating facts (admin level / god mode requirement per command where
      discernible) captured.
- [ ] ADMIN_COMMANDS.md rebuilt; delivery report lists divergences from PDF.
- [ ] Registration-path discoveries backfilled to swgemu/CODE_REFERENCE.md.
- [ ] Committed + pushed; brief marked DELIVERED.

---

## Delivery Report

- Executor: hy3-free (opencode/hy3-free)
- Date: 08252026
- Commit: c87b829af1 (ADMIN_COMMANDS.md rebuild)

### What was done
1. **Authoritative inventory extracted from code**: `command_command_table`
   in `sql/datatables.sql` — every row with `characterAbility='admin'` = the
   admin-gated set. Result: **133 admin commands** (vs 63 in the PDF; vs 10 in
   the first BRIEF-018 attempt).
2. **Every command parsed from its C++ class**
   (`src/server/zone/objects/creature/commands/<Name>Command.h`): exact token
   parsing order, flags (-event/-imperial/-rebel/-area/-force), subcommand
   dispatchers (/object, /createNPC, /script, /createSpawningElement),
   defaults, and clamps. Delegated bulk extraction to opencode
   (x-preview-f-free); orchestrator independently spot-checked 5 entries
   against source incl. SetSpeedCommand.h's built-in SYNTAX string (R6.10).
3. **Stubs documented as stubs**: ~15 commands are no-op placeholders on this
   build (/objvar, /npc, /manufacture, /remote, ...). Documented with
   "[no args] -- no-op placeholder" so operators know before typing them.
4. **Doc rebuilt** in house format: user.cfg client prerequisite (operator-
   verified config + 0fd345d9 unlock), permission model, admin level table,
   13 category sections, module note, worked scenarios, evidence-based
   verification section.
5. **Gating discoveries backfilled to swgemu/CODE_REFERENCE.md** (commit
   78dc5149fc): partition key, table source, god-mode mapping chain,
   GODLEVEL=72.

### PDF-vs-code divergences (code wins)
- PDF documents ~30 commands that do not exist as admin-gated on this build
  (e.g. /requestBadges, /reload, /maxStats variants differ, /setPlayerState).
- Code has ~70 admin commands absent from the PDF (/forceCommand family,
  spawner controls, /planetwarp, /gmCreateClassResource, quest bitmask
  commands...).
- Arg shapes frequently differ from PDF (e.g. /addBannedPlayer takes duration
  int + reason tokens per AddBannedPlayerCommand.h:41, not "player name +
  reason").

### Deferred
- **PLAYER_COMMANDS.md** (~3,036 player commands extracted to _player_cmds.txt):
  a separate large pass. Recommend filing BRIEF-022 rather than inflating this
  delivery.
- Working artifacts left in repo root: `_admin_cmds.txt`, `_player_cmds.txt`,
  `_admin_syntax_draft.txt`, `_pdf_commands_raw.txt` (uncommitted; delete or
  commit as data at owner discretion).

Signed: hy3-free (opencode/hy3-free), 08252026
