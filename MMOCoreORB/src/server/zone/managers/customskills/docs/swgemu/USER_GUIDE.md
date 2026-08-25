# SWGEmu User Guide (Players)

Audience: people playing on an SWGEmu server. This guide covers the
emulator and client generally -- connection mechanics, first steps, and
in-game commands. Server-specific features (custom systems, rates,
events) are documented by whoever runs that server; the Custom Skills
module has its own guides under `docs/customskills/`.

> Maintained inside the Custom Skills repository because that project
> develops against Core3; content here is deliberately generic to SWGEmu.

---
## What SWGEmu Is

SWGEmu is an open-source recreation of the Star Wars Galaxies (SWG) game
servers. The original servers shut down long ago; SWGEmu rebuilds the
server software (Core3) so anyone can play again -- either on a public
server run by someone else, or on their own private server.

Two pieces are involved:

| Piece | What it is | Where it comes from |
|-------|-----------|---------------------|
| Game client | The original SWG game software | Your own legally obtained installation |
| Server | Core3 emulator server | Downloaded and run by yourself, or hosted by a community |

---
## Getting Connected

### Information you need

To connect to ANY SWGEmu server you need exactly two values from that
server's operators:

1. **Login server address** -- a hostname or IP address (for example
   `swgemu.com` or `127.0.0.1` for a server on your own machine)
2. **Login server port** -- a number (commonly `44453` in examples and
   local setups)

If you RUN your own server, the address is your own machine (`127.0.0.1`)
or its LAN/public IP, and the port is whatever the login server is
configured to listen on.

### Entering the values

Option A -- Launcher (recommended): open the launcher's Settings, find
**Login Servers**, add an entry with the address and port above, save,
then launch the game through the launcher.

Option B -- Config file: in the SWG client folder, edit
`swgemu_login.cfg` and set the login server address entries there
(`loginServerAddress0=<address>` style lines), then start the client.

Accounts are created per-server: public servers have their own
registration pages or in-game account creation; a self-hosted server
creates accounts from its own database/tools.

### The official SWGEmu team servers

The SWGEmu team itself runs public servers reachable through their
launcher without manual address entry:

- **Finalizer** -- the team's current live server
- **TC-Prime** -- the team's testing server

Note: the team has signaled Finalizer will eventually close when their
next-generation realm launches; check swgemu.com for current status.

[NEEDS INPUT: exact Finalizer login hostname/port if this guide should
carry literal values rather than relying on the launcher.]

---
## First Steps In Game

1. Create your character -- species and profession choice matter less than
   you think; skills can be surrendered and retrained.
2. Learn the controls: movement, camera, radial menu (click on things!),
   and the command system below.
3. Train skills from trainers in cities; earn experience by doing.
4. Find the server's custom features early if it has any -- ask in chat
   or read the server's own documentation.

---
## In-game Commands

Commands are typed into chat with a leading `/`. Every player's available
command set grows as they train skills -- abilities are granted by skill
boxes. The full registry lives in the server software under
`MMOCoreORB/bin/scripts/commands/` (806+ definitions); what follows is a
practical orientation, not an exhaustive list.

### Communication

- Nearby speech, emotes, and spatial text happen automatically through
  normal chat input.
- Group, guild, and channel chat have their own tabs/channels in the
  chat window UI.
- `/tell <name> <message>` style private messaging, friends lists, and
  ignore lists are managed through the community window.

### Movement and posture

- `/stand`, `/kneel`, `/prone`, `/sitServer` -- posture affects combat
  accuracy and defense.
- Climbing, swimming, and mounts come from the world itself or tamed
  creatures.

### Combat basics

- Target something, then use weapon attacks from your toolbar or typing
  their names (`melee1hSpinAttack1`, `flameSingle2` style special moves
  are trained abilities).
- `/duel <target>` starts consensual PvP; `/yield` ends it.
- `/dragIncapacitatedPlayer` helps downed groupmates.

### Crafting and resources

- Sample resources with survey devices, place extractors/harvesters
  (`harvesterActivate`, `harvesterHarvest`,
  `harvesterSelectResource`), and craft from drafting UIs
  (`requestDraftSlots` family backs the crafting window).
- `/forage` scrounges items from the environment where allowed.

### Corpses and loot (relevant to harvesting mods)

- `harvestCorpse` collects meat/hide/bone from defeated creatures --
  servers that raise harvesting yields (like Challenge Tier servers)
  multiply exactly this yield path.

### Discovery

- `/find` and waypoint tools navigate you around planets; the planetary
  map marks key locations.

---
## Where Server Customs Fit

Anything unusual on your server -- custom menus like `/customskills`,
bonus XP events, raised harvest yields -- is server-side customization.
Operators document theirs separately; if your server runs the Custom
Skills module, see `../customskills/USER_GUIDE.md` in this repository.

---

**Audience note for maintainers:** keep this file generic-emulator
content only ([PROC R6.7] separation); module behavior belongs in the
customskills guides. Signed contributions follow
[../rules/process.md](../rules/process.md).
