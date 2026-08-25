# BRIEF-021 -- Deep-dive admin command reference from code inspection

- Status: CLAIMED
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
