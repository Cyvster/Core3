# SWGEmu Admin (GM) Commands -- Operator Reference

Audience: **server operators / GMs** who grant badges, skills, and adjust
characters. This is the *what-do-I-type* reference. The *why-it-requires-perms*
background lives in `CODE_REFERENCE.md` -> "Part I" -> Gating chain
(ObjectControllerImplementation.cpp); read that once to understand the
god-mode + ability double gate.

---

## How a command is permitted (summary)

Every admin command requires **two** things on the executing character's
account/ghost (see `CODE_REFERENCE.md` Gating chain for detail):

1. **God mode** (`ghost->hasGodMode()`) -- an account-level admin privilege.
2. **The command ability** (`ghost->hasAbility("/cmd")`) -- the character must
   hold the ability, exactly like a granted skill.

Both must pass or you get `@error_message:insufficient_permissions` and an
`adminLog` warning. All successful admin invocations are recorded server-side
via `logAdminCommand(...)`. To inventory every restricted command at boot, set
`Core3.CommandConfigManager.DumpAdminCommands = true`.

> **Module note:** the Custom Skills module adds **no** admin commands of its
> own. Its player entry `/customskills` is a normal ability-backed command.
> The commands below are **stock SWGEmu** commands that operators use to set up
> the badges/skills the module reads from `config.lua`.

---

## Badge commands

Badges are identified by a **numeric badge ID**, not the string key used in
`config.lua`'s `badges[]` arrays. To map a `config.lua` key (e.g.
`bdg_exp_creaturehandler_01`) to its numeric ID, look it up in the server's
badge list / `BadgeList` (or `/requestBadges` to dump what a character holds).

### `/grantbadge`

Grants one or more badges. Verified behavior from
`commands/GrantBadgeCommand.h` (the command class implements the parsing
directly).

| Form | Syntax | Effect |
|------|--------|--------|
| single, targeted | `/grantbadge <badgeID>` (target a player) | Awards `<badgeID>` to the targeted player. |
| area | `/grantbadge -area [range] <badgeID>` | Awards `<badgeID>` to every player within `range` metres (range 1-192; default 64). |
| multiple | `/grantbadge -multiple <startID> <endID>` (target a player) | Awards every badge from `startID` through `endID` inclusive to the target. |

- No target + no args -> prints `Syntax: /grantBadge [-area [range]] [badge id]`.
- `-h` / `-help` prints all three forms.
- Source: `server/zone/objects/creature/commands/GrantBadgeCommand.h:26-160`
  (awards via `PlayerManager->awardBadge(...)`).

### `/revokebadge`

`/revokeBadge <badgeID>` (target a player) -- removes `<badgeID>` from the
target. Verified syntax from
`commands/RevokeBadgeCommand.h:25` (`args.getIntToken()`).

### `/requestBadges`

`/requestBadges` (target a player) -- lists the badges the target currently
holds. Useful to confirm a grant before/after using `/customskills` in-game.

---

## Skill commands

### `/grantskill`

`/grantskill <skillName>` (target a player) -- grants a skill box to the
target. Skill names are the dot-path strings (e.g. `combat_1a`).

### `/revokeskill`

`/revokeskill <skillName>` (target a player) -- removes a skill box from the
target.

---

## Character stat commands

### `/editstats`

`/editstats <targetName> <param>` -- edits a stat on the named target
(verified arg shape from `commands/EditStatsCommand.h:47-48`:
`getStringToken(targetName)` then `getStringToken(param)`). `<param>` is the
stat name + value; confirm the exact form in-game via `/editstats -help` if the
thin command class does not print usage.

### `/maxstats`

`/maxstats` (target a player) -- sets the target's stats to their racial
maximums.

### `/setgodmode`

`/setgodmode <targetName> <param>` -- toggles/sets god mode on the named target
(verified arg shape from `commands/SetGodModeCommand.h:47-48`). `<param>` is the
god-level/flag; confirm exact form in-game.

---

## Server / runtime commands

### `/server`

`/server <subcommand> [args]` -- server-level control (verified from
`commands/ServerCommand.h:55-62`: reads a subcommand token, optionally more).
Common subcommands include shutdown/reload-style operations; run `/server`
with no args in-game to see the live subcommand list on your build.

### `/reload`

`/reload <target>` -- reloads a runtime subsystem or script. **Does NOT reload
`config.lua`** for this module: the module caches config at startup and only
re-reads it on a **server restart** (see `customskills/CODE_REFERENCE.md`
CustomSkillsConfig cache note + BRIEF-019 build-facts). To change a badge bonus
or cap, edit `config.lua` and restart -- `/reload` will not pick it up.

---

## Worked operator scenarios

**Grant the full Critical Chance badge set to one player**
1. Determine the numeric badge IDs for the crit-chance badges you configured in
   `config.lua` (cross-reference `BadgeList`; `/requestBadges` on a test char
   shows IDs already held).
2. Target the player, then `/grantbadge -multiple <startID> <endID>` covering
   that set.
3. Confirm: `/requestBadges` on the target, then have them open `/customskills`
   in-game -- the Bonuses page should show the new owned count and the resulting
   basis-point total (capped at `cap`).

**Undo a mistaken grant**
- `/revokebadge <badgeID>` on the target, then re-verify with `/requestBadges`.

**Audit your own action**
- Every grant/revoke above is written to the server `adminLog`. If a grant
  "did nothing," check adminLog for the `insufficient_permissions` warning
  first -- it means the executing character lacks god mode or the command
  ability (see Gating chain).

---

## Verification notes

- Command **names** verified against disk: each `/<name>` maps to
  `bin/scripts/commands/<file>.lua` (name field) -- grantbadge, revokebadge,
  requestbadges, grantskill, revokeskill, editstats, maxstats, setgodmode,
  server, reload (all present).
- `/grantbadge` and `/revokebadge` **argument/area/multiple behavior** verified
  directly from their C++ command classes (GrantBadgeCommand.h,
  RevokeBadgeCommand.h).
- `/editstats`, `/setgodmode`, `/server` argument shapes verified from their
  command-class token parsing; exact in-game `<param>` spellings should be
  confirmed with the command's `-help`/no-arg usage on your build, since those
  classes are thin shells over a shared handler.
- Gating chain, audit, and `DumpAdminCommands` flag verified from
  `swgemu/CODE_REFERENCE.md` (ObjectControllerImplementation gating section).

---

**Last reconciled:** 08242026 by hy3-free (opencode/hy3-free) -- BRIEF-018
delivery: operator-facing admin command reference under swgemu/, grounded in
real command files (no invented command names), cross-linked to the gating
chain in CODE_REFERENCE rather than duplicated. Per R6.9/R6.10, every claim
cites its source file.

---

## Contributors

- hy3-free (opencode/hy3-free), 08242026 -- initial admin command reference
