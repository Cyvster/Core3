# SWGEmu Admin (GM) Commands — Operator Reference

*Audience: server operators / GMs.* What-to-type reference. Permission background: `CODE_REFERENCE.md` → Gating chain (god-mode + ability double gate).

## Permission model
Every admin command requires BOTH on the executing character:
1. **God mode** (`ghost->hasGodMode()`) — account-level admin privilege.
2. **Command ability** (`ghost->hasAbility("/cmd")`) — character must hold the ability like any skill.

Either check fails → `@error_message:insufficient_permissions` + `adminLog` warning. All successful calls logged via `logAdminCommand(...)`. Boot-time inventory: set `Core3.CommandConfigManager.DumpAdminCommands = true`.

> **Module note:** Custom Skills adds **no** admin commands. `/customskills` is a normal ability-backed command. Below are **stock SWGEmu** commands used to set up the badges/skills the module reads from `config.lua`.

## Commands

| Command | Syntax | Effect / notes |
|---------|--------|----------------|
| `/grantbadge` | `/grantbadge <badgeID>` (target) | Grant one badge (numeric ID). Forms: `-area [range]` (1–192m, def 64), `-multiple <start> <end>`. Src: `GrantBadgeCommand.h:26-160`. |
| `/revokebadge` | `/revokebadge <badgeID>` (target) | Remove one badge. Src: `RevokeBadgeCommand.h:25`. |
| `/requestBadges` | `/requestBadges` (target) | List badges the target holds. Use to verify grants. |
| `/grantskill` | `/grantskill <skillName>` (target) | Grant a skill box (dot-path, e.g. `combat_1a`). |
| `/revokeskill` | `/revokeskill <skillName>` (target) | Remove a skill box. |
| `/editstats` | `/editstats <targetName> <param>` | Edit a stat. Arg shape `EditStatsCommand.h:47-48` (`getStringToken`×2). Confirm exact `<param>` via `/editstats -help`. |
| `/maxstats` | `/maxstats` (target) | Set stats to racial max. |
| `/setgodmode` | `/setgodmode <targetName> <param>` | Toggle/set god mode. Arg shape `SetGodModeCommand.h:47-48`. Confirm `<param>` in-game. |
| `/server` | `/server <subcommand> [args]` | Server control. Src: `ServerCommand.h:55-62`. Run with no args to list subcommands. |
| `/reload` | `/reload <target>` | Reload a subsystem/script. **Does NOT reload `config.lua`** — module caches config at startup; edit `config.lua` + **restart** to change a badge bonus/cap. |

**Badge IDs are numeric**, not the `config.lua` string key (e.g. `bdg_exp_creaturehandler_01`). Map a key→ID via `BadgeList` or `/requestBadges`.

## Worked scenarios
- **Grant a full badge set:** find numeric IDs for the crit-chance badges in `config.lua` → target player → `/grantbadge -multiple <startID> <endID>` → verify with `/requestBadges`, then open `/customskills` in-game (Bonuses page shows owned count + basis-point total, capped at `cap`).
- **Undo a grant:** `/revokebadge <badgeID>` → re-verify `/requestBadges`.
- **Audit:** every grant/revoke is in `adminLog`. A "did nothing" grant ⇒ check adminLog for `insufficient_permissions` (missing god mode or ability; see Gating chain).

## Verification (R6.10)
Names verified on disk (`bin/scripts/commands/<file>.lua`): grantbadge, revokebadge, requestbadges, grantskill, revokeskill, editstats, maxstats, setgodmode, server, reload. `/grantbadge` + `/revokebadge` args verified from C++ classes; `/editstats`, `/setgodmode`, `/server` arg shapes from token parsing (exact `<param>` confirmed via in-game `-help`). Gating/audit/`DumpAdminCommands` from `swgemu/CODE_REFERENCE.md`.

*Reconciled 08242026 by hy3-free (opencode/hy3-free) — BRIEF-018. Contributors: hy3-free, 08242026.*
