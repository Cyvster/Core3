# SWGEmu Admin (GM) Commands — Operator Reference
*Audience: server operators / GMs.* What-to-type reference for **stock SWGEmu admin commands**, rebuilt from *Commands V2 - Posted* (owner-provided authoritative reference). Permission background: `CODE_REFERENCE.md` → Gating chain (god-mode + ability double gate).

## Permission model
Every admin command requires BOTH on the executing character: (1) **God mode** (`ghost->hasGodMode()`) — account-level admin privilege; (2) **Command ability** (`ghost->hasAbility("/cmd")`) — character must hold the ability like any skill. Either fails → `@error_message:insufficient_permissions` + `adminLog` warning. All successful calls logged via `logAdminCommand(...)`. Boot inventory of restricted commands: set `Core3.CommandConfigManager.DumpAdminCommands = true`. Syntax: `<required>`, `[optional]`.

## Account Control
`/setGodMode <name> <admin level | on | off>` — sets the character's admin level (staff levels).  
`/getAccountInfo <-a> <accountName> | <-c> <characterName>` — looks up account info for current target or named account; shows last login date/time.  
`/kick <firstName> <reason>` — kicks player + all clients on same account, banning for duration (default 10 min); `-1` = indefinite; see `/removeBannedPlayer`; players only.  
`/setFirstName <firstName>` — sets target's first name; must pass name filters; players only.  
`/setLastName <lastName>` — sets target's last name; must pass name filters.  
`/freezePlayer <firstname> <reason>` — freezes + mutes targeted/named player until unfrozen.  
`/unfreezePlayer [reason]` — unfreezes/unmutes targeted player.  
`/getPlayerId <player>` — returns player's objectId via system message; requires name, not target.  
`/getStationName <player>` — returns station name for targeted player or named account.  
`/addBannedPlayer <playerName> <reason>` — bans a player's account without disconnecting them.  
`/removeBannedPlayer <accountName> <reason>` — removes an account ban; takes ACCOUNT name.

## Server Control
`/broadcast [-help] [-event | -imperial | -rebel] <message>` — calls broadcastGalaxy.  
`/broadcastArea [-help] [range] [-event | -rebel | -imperial] <message>` — broadcasts within range (5–5000).  
`/broadcastGalaxy [-help] [-event | -imperial | -rebel] <message>` — broadcast to all logged-in players server-wide.  
`/broadcastPlanet [-event | -imperial | -rebel] <message>` — broadcast to all players on current planet.  
`/cityinfo [cityFilter] | [planetname <rank>]` — city status report without city terminal; optionally search by filter.  
`/database <characters | deleted_characters | playerstructures | cityregions | sceneobjects | clientobjects> <objectid>` — debug DB query interface.  
`/listGuilds [guildFilter]` — lists guilds matching name/tag filter.  
`/resendLoginMessageToAll` — re-broadcasts the login message to everyone.  
`/server <command> [args]` — in-game access to server command line. Subcommands: `debug <on|off>` toggle; `info` server info; `market <enable/disable/info>`; `playerinfo <target/player name>`; `playermanager <setxpmodifier> <value>`; `playermanager <listjedi>`; `playermanager <list_frsjedi>`; `playermanager <listadmins>`; `revision`; `statistics [reset]`.

## Player Information
`/findplayer <string filter>` — reports location/direction/info about named player.  
`/snoop [args]` — inspect a targeted object's inventory and nested containers. Targets: `bank`, `buffs`, `credits`, `datapad`, `equipment`, `faction`, `ham`, `lots`, `vendors`, `veteranrewards`, or `screenplaystate [stateName] <state>`.

## Control (self)
`/invulnerable` — self invulnerability (can still attack back); `invisible` arg makes you invisible.  
`/setSpeed <speed> [duration]` — self movement speed for duration (default 30 min).

## Teleport
`/goto <waypointSearchToken> | <?> for all` — warp to datapad waypoints or pick from listbox; self.  
`/teleport <x> <y> [planet] [<z> <parentID>]` — teleport self to coordinates.  
`/teleportTarget <player>` — teleport named player to you.  
`/teleportTarget <player> <x> <y> [planet] [<z> <parentID>]` — teleport named/targeted player to location.  
`/teleportTo <player>` — teleport self to player's location.

## Stats
`/adjustLotCount [player] [+/-]<lots>` — adjust lot count (may be negative — careful).  
`/credits <player> <add/subtract> <amount> <bank/cash>` — edit cash/bank credits (may be negative — careful).  
`/gmrevive [buff] [[<player>] | [area [<range>] [imperial | rebel | neutral]]]` — revive/heal/buff self, target, or area (restores HAM, wounds, force power, battle fatigue; default range 32 m).  
`/killPlayer [playerName] [-area [range]] [-wounds [<health> [action] [mind]]] [reason]` — kill targeted/named player; `-w` variant applies wounds to all HAM stats instead.  
`/setPlayerState <player> <state>` — set a player state.

## Skills
`/grantBadge <badge id>` — grant badge by **numeric ID** to targeted player (not the `config.lua` string key). Area form: `/grantBadge [-area [range]] <badge id>` (0–192 m, default 64).  
`/setExperience <firstName | target> <experienceType> <amount>` — set XP type to amount.  
`/grantSkill <skillBox>` — award skill box incl. prerequisites (dot-path names, e.g. `combat_1a`).  
`/setfaction <name> <imperial | rebel | neutral> <onleave | covert | overt>` — set object's faction + state.  
`/setFactionStanding <player> <factionName> <value>` — set faction points for any known faction (Janta, Kunga, Mook...).  
`/gmJediState <player> <0 | 1 | 2 | 4 | 8>` — controls which Jedi boxes show under All Professions tab (0=non-FS, 1=FS, 2=Initiate, 4=light FRS, 8=dark FRS).  
`/resetJedi <player>` — reset all Force skills on target.  
`/revokeBadge <badge id>` — remove a badge from targeted player (disk-verified: `RevokeBadgeCommand.h:25`).  
`/requestBadges` — list badges the target holds; use to verify grants (disk-verified: `bin/scripts/commands/`).  
`/editStats <targetName> <param>` — edit a stat (`EditStatsCommand.h:47-48`; confirm exact `<param>` via `-help`).  
`/maxStats` — set targeted player's stats to racial max.  
`/reload <target>` — reload runtime subsystem/script; **does NOT reload `config.lua`** (module caches config at startup; edit + **restart**).

## Quests
`/listActiveQuests <player>` — list active quests in window.  
`/listCompletedQuests <player>` — list completed quests in window.  
`/completeQuest quest <quest>` — mark a quest completed.  
`/completeQuest screenplaystate <screenPlayStateName> <state>` — set/clear screenplay states.  
`/clearCompleteQuest screenplaystate <screenPlayStateName> <state>` — clear completed-quest screenplay state.  
`/getVeteranRewardTime <player>` — time to next veteran reward.  
`/clearVeteranReward [player] <milestone>` — reset used veteran reward for re-selection (path to reward script).

## Village (FS)
`/gmFsVillage getCurrentPhase` — currently active phase.  
`/gmFsVillage getCurrentPhaseID` — active phase id.  
`/gmFsVillage getCurrentPhaseDuration` — time left in phase.  
`/gmFsVillage changeCurrentPhase` — trigger phase change (testing only).
`/setPlayerAppearance <player> <shared_mobile template | blank to reset>` — set appearance to any shared_mobile template (e.g. `object/mobile/shared_darth_vader.iff`); blank resets.

## Objects
`/dumpt` — dump debug info about targeted object (requires target), emailed to you.  
`/dumpz [target | zoneId]` — dump debug info about current zone, emailed to you.  
`/findobject [-p] <string filter> <range>` — find objects by name within range on planet; options to store waypoint or teleport.  
`/getobjvars [objectID]` — dump all object variables + values on target or objectID.  
`/generateCraftedItem <script path> [quantity] [template number]` — generate crafted items from draft schematic scripts.  
`/object createitem <objectTemplatePath> [<quantity>]` — spawn tangible items to inventory.  
`/object createloot <lootGroup> [creature combat level]` — spawn loot group to inventory.  
`/object createresource <resourceName> [<quantity>]` — spawn resource to inventory.  
`/object createarealoot <loottemplate> [<range>] [<level>]` — spawn area loot.  
`/createSpawningElement spawn <IffObjectPath> [X] [Z] [Y] [planet] [cellID]` — spawn items into world.  
`/createSpawningElement lair <lairTemplate> [level]` — spawn lairs into world.  
`/createSpawningElement delete <oid>` — delete spawned item (oid given at spawn).  
`/setName <name>` — set name of targeted object.  
`/createCreature <template> [object template | ai template | baby | event [level]] [X] [Z] [Y] [planet] [cellID]` — spawn creatures/babies into world.

## Module note
Custom Skills adds **no** admin commands. `/customskills` is a normal ability-backed command. The badge/skill commands above (`/grantBadge`, `/grantSkill`, ...) are what operators use to set up what the module reads from `config.lua`. Badge IDs are **numeric**, not `config.lua` keys — map via `BadgeList` or `/requestBadges`.

## Worked scenarios
**Grant a full badge set:** find numeric IDs for your configured badges → target player → `/grantBadge -area 64 <badge id>` or repeated single grants → verify with `/requestBadges` → open `/customskills` in-game (Bonuses page shows owned count + basis-point total, capped at `cap`).  
**Undo a grant:** revoke via badge ID, re-verify with `/requestBadges`.  
**Audit:** every call is in `adminLog`; "did nothing" ⇒ check adminLog for `insufficient_permissions` (missing god mode or ability).

## Verification (R6.10)
Command list + descriptions extracted from owner-supplied **Commands V2 - Posted.pdf** (18-line extract at `_pdf_commands_raw.txt`, repo root, uncommitted scratch). Cross-checked against prior disk verification (`bin/scripts/commands/<file>.lua`: grantbadge, revokebadge, requestbadges, grantskill, revokeskill, editstats, maxstats, setgodmode, server, reload). Commands present in the PDF but not yet disk-verified against this codebase's command classes are marked as reference-derived; per R6.10, exact argument shapes for those should be confirmed against their C++ command classes before relying on edge behavior. Gating chain + audit facts from `swgemu/CODE_REFERENCE.md`.
*Reconciled 08252026 by hy3-free (opencode/hy3-free) — BRIEF-018 redo. Contributors: hy3-free, 08242026–08252026.*
