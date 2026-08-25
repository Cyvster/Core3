# BRIEF-018 -- Document SWGEmu admin (GM) commands in a dedicated section

- Status: CLAIMED
- Created: 08242026 by hy3-free (opencode/hy3-free), owner directive.
  Owner is currently compiling/running the engine and wants a written admin
  command reference so server operators (not just developers) know how to
  grant/revoke the badges and skills that drive the Custom Skills module,
  how the admin permission chain works, and how to audit admin actions.

## Context

The module's player entry point is `/customskills` (a normal ability-backed
command, see `bin/scripts/commands/customSkills.lua`). The *modifiers* it
exposes are driven by SWGEmu **badges** and **skills** the player holds --
those are granted/revoked through stock Core3 GM commands, not module code.

Today there is **no operator-facing admin command reference** in the docs.
`swgemu/CODE_REFERENCE.md` describes the admin *permission chain* in the
abstract (god mode + ability grant, `logAdminCommand` auditing, the
`DumpAdminCommands` startup flag) but never enumerates the actual commands an
operator types. This brief closes that gap with a real, copy-pasteable section.

Audience separation (owner directive 08242026, enforced in `docs/README.md`):
emulator/operator content belongs under `swgemu/`. The new content is an
emulator operator guide, so it lands there -- NOT in `customskills/`.

## Deliverables

1. Create `docs/swgemu/ADMIN_COMMANDS.md` -- an operator guide covering:

   ### A. The admin permission chain (operator must understand this first)
   - Two independent checks gate every `requiresAdmin()` command
     (`CommandQueue.cpp` logic, summarized from `swgemu/CODE_REFERENCE.md`
     lines ~430-459):
     1. **God mode**: `PlayerObject->hasGodMode()` true. God mode is an
        account-level privilege -- the account's `admin_level`
        (`SWGRealmsAPI.cpp`) maps to the god-mode flag.
     2. **Ability grant**: `ghost->hasAbility("/<command>")` must ALSO be
        true. God mode alone is not enough; the character must hold the
        ability like any skill-granted ability.
   - On failure: `adminLog` warning "<name> attempted to use '/<cmd>'
     without permissions", `@error_message:insufficient_permissions`, queued
     action cleared.
   - Every successful invocation passes through `logAdminCommand(...)`
     (server-side audit trail).
   - To inventory what is restricted at startup: set
     `Core3.CommandConfigManager.DumpAdminCommands = true` (dumps the full
     admin command list).

   ### B. Command reference table
   For each command below give: exact slash-name, what it does, required
   target (self / selected creature / no target), arguments, and a worked
   example. Commands confirmed present in `bin/scripts/commands/`:

   | Command | Purpose | Notes for this module |
   |---------|---------|------------------------|
   | `/grantbadge <badgeKey>` | Grants a badge by key to the target (or self) | The ONLY way to grant the badges that feed every Custom Skills modifier. Badge keys are the strings in `config.lua` badges lists (e.g. `combat_bountyhunter_master`, `poi_rabidbeast`, `bdg_corvette_imp_destroy`). Keys are case-sensitive and must match `BadgeList`. |
   | `/revokebadge <badgeKey>` | Removes a granted badge | Use to undo a mis-grant; modifier bonuses recompute from the remaining badge set on next bonus refresh. |
   | `/requestbadges` | Lists badges the character currently holds | Operator sanity-check that a player actually has the key feeding a modifier. |
   | `/grantskill <skillName>` | Grants a skill | Needed for the *profession-mastery* badges only if your server awards those badges via skill templates; otherwise use `/grantbadge`. |
   | `/revokeskill <skillName>` | Revokes a skill | |
   | `/editstats <stat> <value>` | Sets a character stat | Useful to test defense-cap / armor-penetration edge cases live. |
   | `/maxstats` | Maxes all stats on the target | Quick way to stage a maxed test character. |
   | `/setgodmode` | Toggles the caller's god-mode flag | Required before any admin command above will run for that character. |
   | `/server` | Server control / info command | Consult in-game `/server help` for sub-actions. |
   | `/reload` | Hot-reloads script/lua without a full restart | After editing `config.lua`'s `badgeBonus`/`cap`/`badges` values you still must RESTART -- Lua config is read once at boot; `/reload` does not re-read module config (cite this caveat explicitly so operators do not expect live config updates). |

   ### C. Worked operator scenarios (copy-paste)
   1. **Grant a player the full crit-chance badge set**: for each of the 60
      keys listed under `criticalChance.badges` in `config.lua`, run
      `/grantbadge <key>` on the target. (State that a bulk grant is normally
      done via a script or the account's badge grant tool, not 60 manual
      commands -- point at the pattern, do not invent a non-existent command.)
   2. **Verify**: target runs `/customskills` in game; the Crit Chance line
      should read the expected total (e.g. 60.00% at full set).
   3. **Undo a mistake**: `/revokebadge <key>`.
   4. **Audit**: check the server `adminLog` for the grant/revoke entries.

   ### D. Module-specific caveat block
   - The Custom Skills module adds **no** `requiresAdmin()` commands of its
     own (`swgemu/CODE_REFERENCE.md` lines 453-459). All privileged operation
     flows through the stock Core3 commands in table B.
   - Changing `config.lua` (e.g. `badgeBonus`, badge lists) requires a
     **server restart**, not `/reload` -- restate this prominently.
   - After granting badges, player bonuses recompute automatically; no
     separate refresh command is needed.

2. Add a one-line link to `swgemu/ADMIN_COMMANDS.md` from
   `docs/swgemu/USER_GUIDE.md` (operator-facing pointer) and from
   `docs/README.md` directory table if a row for it is warranted (the
   `swgemu/` cell already says "player user guide + merged Core3 developer
   code reference"; extend that cell text to mention the new operator guide).
   Keep audience separation: do NOT put admin command content under
   `customskills/`.

3. Sign the new document per `rules/process.md` Contributor Recording
   Conventions (hy3-free (opencode/hy3-free), 08242026). Add yourself to the
   Contributors list if present.

## Acceptance

1. `docs/swgemu/ADMIN_COMMANDS.md` exists with sections A-D above and the
   worked scenarios.
2. Every command in table B is a real Core3 command (verify the file exists
   under `bin/scripts/commands/`); no invented command names.
3. The permission chain section matches `swgemu/CODE_REFERENCE.md`
   lines ~430-459 (god mode + ability grant; `logAdminCommand` audit).
4. The restart-not-reload caveat for `config.lua` is stated explicitly.
5. Link added to `swgemu/USER_GUIDE.md`; `docs/README.md` `swgemu/` cell
   updated to mention the operator guide.
6. Commit tagged `[BRIEF-018]`, pushed ([PROC R6.5]).

## Notes

- This is a documentation-only brief; it changes no code or config.
- `config.lua` badge keys referenced above are the live keys from the
  shipped config (verified 08242026). If badges lists change later, update
  the scenario example to match.
- If during writing you find a Core3 admin command relevant to the module
  that is NOT in table B, add it with the same fields -- but do not add
  module-specific commands that do not exist.
