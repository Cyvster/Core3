# SWGEmu Admin (GM) Commands — Operator Reference
*Audience: server operators / GMs.* What-to-type reference. Permission background: `CODE_REFERENCE.md` → Gating chain (god-mode + ability double gate).

## Permission model
Every admin command requires BOTH on the executing character: (1) **God mode** (`ghost->hasGodMode()`) — account-level admin privilege; (2) **Command ability** (`ghost->hasAbility("/cmd")`) — character must hold the ability like any skill. Either fails → `@error_message:insufficient_permissions` + `adminLog` warning. All successful calls logged via `logAdminCommand(...)`. Boot inventory: `Core3.CommandConfigManager.DumpAdminCommands = true`.
> **Module note:** Custom Skills adds **no** admin commands. `/customskills` is a normal ability-backed command. Below are **stock SWGEmu** commands used to set up the badges/skills the module reads from `config.lua`.

## Commands
`/grantbadge <badgeID>` (target) — grant one badge by **numeric ID** (not the `config.lua` string key). Forms: `-area [range]` (1–192m, def 64), `-multiple <start> <end>`. Src `GrantBadgeCommand.h:26-160`.  
`/revokebadge <badgeID>` (target) — remove one badge. Src `RevokeBadgeCommand.h:25`.  
`/requestBadges` (target) — list badges the target holds; use to verify grants.  
`/grantskill <skillName>` (target) — grant a skill box (dot-path, e.g. `combat_1a`).  
`/revokeskill <skillName>` (target) — remove a skill box.  
`/editstats <targetName> <param>` — edit a stat. Arg shape `EditStatsCommand.h:47-48` (`getStringToken`×2); confirm exact `<param>` via `/editstats -help`.  
`/maxstats` (target) — set stats to racial max.  
`/setgodmode <targetName> <param>` — toggle/set god mode. Arg shape `SetGodModeCommand.h:47-48`; confirm `<param>` in-game.  
`/server <subcommand> [args]` — server control. Src `ServerCommand.h:55-62`; run with no args to list subcommands.  
`/reload <target>` — reload a subsystem/script. **Does NOT reload `config.lua`** — module caches config at startup; edit `config.lua` + **restart** to change a badge bonus/cap.

## Worked scenarios
- **Grant a full badge set:** find numeric IDs for the crit-chance badges in `config.lua` → target player → `/grantbadge -multiple <startID> <endID>` → verify with `/requestBadges`, then open `/customskills` in-game (Bonuses page shows owned count + basis-point total, capped at `cap`).
- **Undo a grant:** `/revokebadge <badgeID>` → re-verify `/requestBadges`.
- **Audit:** every grant/revoke is in `adminLog`. A "did nothing" grant ⇒ check adminLog for `insufficient_permissions` (missing god mode or ability; see Gating chain).

## Verification (R6.10)
Names verified on disk (`bin/scripts/commands/<file>.lua`): grantbadge, revokebadge, requestbadges, grantskill, revokeskill, editstats, maxstats, setgodmode, server, reload. `/grantbadge`+`/revokebadge` args from C++ classes; `/editstats`,`/setgodmode`,`/server` arg shapes from token parsing (exact `<param>` via in-game `-help`). Gating/audit/`DumpAdminCommands` from `swgemu/CODE_REFERENCE.md`.
*Reconciled 08242026 by hy3-free (opencode/hy3-free) — BRIEF-018. Contributors: hy3-free, 08242026.*
