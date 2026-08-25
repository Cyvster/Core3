# BRIEF-027 Findings -- Routing hidden swgemu options through the mod (prefilled mod config)

- Status: COMPLETE (research only; no code changes)
- Date: 08252026
- Sources: src/conf/ConfigManager.{h,cpp}, src/server/ServerCore.cpp,
  src/server/zone/managers/customskills/CustomSkillsConfig.{h,cpp},
  docs/customskills/CONFIG_OPTIONS.md (BRIEF-023 inventory).

## Verdict: FEASIBLE-WITH-CAVEATS

The mechanism works today for every [dyn]/[dyn-cache] option **if** the mod's
override file is executed inside the ConfigManager Lua state after
config-local.lua. That requires ONE three-line addition to
`ConfigManager::loadConfigData()` (an implementation brief, out of scope here)
-- there is currently no second hook a mod can reach. Pure-Lua injection from
the mod's own config loader is NOT possible because CustomSkillsConfig runs its
own private Lua state and DirectorManager exposes config *read* mirrors only
(no setter registration).

---

## RQ1 -- Config load order and merge semantics

`ConfigManager::loadConfigData()` (src/conf/ConfigManager.cpp:23-93):

1. Line 33: hard-required `conf/config.lua`.
2. Lines 38-47: optional `conf/config-local.lua`, loaded ONLY if present --
   this is the stock secondary-file mechanism (grep confirms no other
   config-* source anywhere in src/).
3. Both files run in the SAME lua_State (`lua.runFile`), then lines 55-77:
   `clearConfigData()` wipes the in-memory map and re-parses the global
   `Core3` table (`parseConfigData("Core3")`, line 60) followed by legacy
   top-level globals (line 72). Keys are flattened to dotted strings.

Merge rule = **last assignment wins**: `runFile` chunks share one Lua state, so
a later file's `Core3.Foo = x` overwrites the earlier file's value before the
single parse pass. At the item level `updateItem()` (ConfigManager.cpp:611-642)
drops-and-replaces any existing key, and each replace increments `configVersion`
(line 640), which auto-refreshes every cached getter (ConfigManager.h:278-355
pattern).

Hot-reload: `loadConfigData()` is called again by `ServerCore::processConfig()`
(ServerCore.cpp:1128-1131, invoked from initialize() at :659); console commands
can trigger reprocessing. Because the whole map is cleared and rebuilt
(clearConfigData, ConfigManager.cpp:95-106), a reload re-runs BOTH files in the
same order -- an overlay file would be re-applied on every hot-reload too.

**No config/mod/include third file exists today**; supporting one is exactly
the missing hook.

## RQ2 -- Lua exposure: what the mod can and cannot do

- CustomSkillsConfig::load() (customskills/CustomSkillsConfig.cpp:110-127)
  creates its OWN local `Lua` instance and runs
  `scripts/customskills/config.lua`. Nothing written there reaches
  ConfigManager's state; the state dies when load() returns.
- DirectorManager registers only config READ mirrors for screenplays:
  `isJtlEnabled()` (DirectorManager.cpp:5107-5108, registered :554) and
  `isCovertOvertSystem()` (:5016). There is NO registered
  `getConfBool/setConfBool` style binding -- grep for getConf* in
  DirectorManager.cpp returns nothing. Server-side Lua cannot set
  ConfigManager keys.
- Therefore the "prefilled file" must be consumed BY ConfigManager itself.
  Two viable routes:
  - **Route A (recommended)**: add `lua.runFile("conf/mod-overrides.lua")`
    guarded by File::setReadOnly() exactly like the config-local block
    (mirror ConfigManager.cpp:38-47). ~5 lines, zero behavior change when the
    file is absent. Works at boot AND on every hot-reload automatically.
  - Route B (no C++ change): ship instructions to append
    `dofile("conf/mod-overrides.lua")` (or copy the block) into the operator's
    config-local.lua. Fragile: operators overwrite config-local on upgrades;
    precedence becomes uncontrolled. Rejected as primary mechanism.

## RQ3 -- Safety classification of the ~65 class-(b) hidden options

Tags from CONFIG_OPTIONS.md legend ([startup] vs [dyn]/[dyn-cache]).

### SAFE-DYNAMIC (~51 options) -- routable via overlay, live-tunable

All six owner seeds except DisableGroupVisibility:
WipeFillingOnClone, GalaxyWideGrouping, AdvancedWaypoints (PlayerManagerImpl
cpp:1883; GroupManager.cpp:45 et al.), ValidClientVersion,
PlayerCreationManager.MaxCharactersPerGalaxy / EnableTutorial,
JTL.JTLEnabled (dyn-cache), plus:
- Mission/bounty: ListRequestCooldown, IncludeFactionPets,
  AnonymousBountyTerminals, MaxBountiesPerJedi, PrivateStructureJediMissions,
  BountyExpirationTime, PlayerBountyCooldown(+Time) (8 keys)
- Combat/PvP: GCWManager.useCovertOvertSystem, ChatManager.PvpBroadcastChannel,
  CombatManager.AllowSameAccountLinkDeadBeneficialActions (3)
- Regions/spawning: all 6 (minimumLair/SpaceSpawnInterval, DisableWorld/SpaceSpawns,
  spawnCheckRange, spaceSpawnCheckRange) -- all dyn-cache
- Structures/travel: StructureMaintenanceTask.AllowBankPayments,
  EnhancedFurnitureRotate, StructureObject.MaintenanceBootDelay,
  Tweaks.StructureObject.DestoryOrphans (4)
- Player objects: AlwaysSafeLogout, LinkDeadDelay, TangibleObject NoTrade/
  ForceNoTrade/ForceNoTradeADK messages (5)
- TransactionLog: Enabled, AsyncExport, Verbose, PruneCreatureObjects,
  PruneCraftedComponents, SceneObject.exportDir (6)
- Misc: FrsManager.ImmediateMaintXpDeduction, SameAccountTipsAreFree,
  AuctionItem.ExportOnDestroy, LootManager.DebugAttributes (4)
- AI: AiAgent.LogLevel, AiAgent.Verbose, AiAgent.ConsoleThrottle,
  ShipAiAgent.LogLevel (4)
- Accounts/session: AccountManager.CreatedDateFirstConnect,
  HolocronTicketsEnabled, Login.EnableSessionId, Login.SessionDuration,
  RegistrationMessage (5)
- Logging: SessionStatsSeconds, LogOnlineCount (2)
- RESTServer.exportDir, ZoneServer.ClientLogLevel (+ per-account overlay) (2)

Caveat: toggling gameplay-visible switches mid-session (GalaxyWideGrouping,
useCovertOvertSystem) changes behavior immediately for online players -- safe
crash-wise, but document as player-visible.

### RESTART-REQUIRED (~14) -- expose but flag "(restart required)"

DisableGroupVisibility (cached in VisibilityManager at construction,
VisibilityManager.cpp:34), RotateLogSizeMB, RotateLogAtStart,
OnlineLogSeconds, LogOnlineOnSessionChange, NavMeshManager.LogLevel,
NameManager.FilterTable, TreManager.ReloadStrings,
CommandConfigManager.DumpAdminCommands, CommandCooldown.<cmd>,
AuctionManager.LogLevel / Startup.ExpireInvalid,
ShuttleZoneComponent.BootDelay, StructureManager.CreateNavMesh.<path>,
AiAgent.<Template>.LogLevel, all Zone/SpaceZone thread counts and
Login.API.* / RESTServer worker settings.

### NEVER-EXPOSE (~10)

- Secrets per ERR-014/BRIEF-026 policy: DBPass, DBSecret, MantisPass,
  Login.API.APIToken, RESTServer.APIToken, SSLKeyFile, SSLCertFile
  (also auto-masked by isSensitiveKey, ConfigManager.cpp:559-563).
- Class (c) dead reads (would silently do nothing):
  TreManager.LatestTre, MOTD, JTL.LaunchFromDevice, ZoneProcessingThreads,
  LoginProcessingThreads, AiAgent.AiAgentLoadTesting (DEBUG_AI),
  PlanetManager shuttle timings (DEBUG_TRAVEL).
- Naming traps unless specially handled: `PlayerManager.accountVictimList`
  (no Core3. prefix; collides with the Core3.PlayerManager table),
  `DestoryOrphans` typo-key, stale `DeleteCharacters`.

## RQ4 -- Precedence design

Load order in the recommended Route A:
`config.lua` -> `config-local.lua` -> `mod-overrides.lua` ->
single parse pass -> last-write-wins. Consequences:

1. As written, the mod file would OVERRIDE operator values in config-local for
   any duplicated key. Unacceptable for operator sovereignty.
2. Fix inside the mod file itself (no C++ logic needed): the shipped
   `mod-overrides.lua` should assign ONLY keys the admin explicitly enables.
   Recommended format: every option present but commented out by default, e.g.

   ```lua
   -- Custom Skills mod :: SWGEMU option overrides
   -- Uncomment a line to force that value. Anything left commented
   -- leaves the server default (or your config-local.lua value) in effect.
   -- Core3.PlayerManager.GalaxyWideGrouping = true,
   ```

   With nothing uncommented the file is a no-op, so operator-set values always
   win whenever the operator has expressed a preference.
3. Operator escape hatch / precedence contract (document in USER_GUIDE):
   - Operator comment in config.lua/local wins over mod defaults-by-commentary
     (mod file only sets what it explicitly sets).
   - To defeat a mod-forced value, the operator deletes/comments the line in
     mod-overrides.lua OR (stronger) we adopt the inverse order in the future
     implementation brief: load mod-overrides BEFORE config-local so
     config-local is authoritative. Recommend deciding at implementation time;
     default recommendation is **mod file last, sparse by default**, because
     the file ships fully commented out.
4. Hot-reload re-runs the full sequence (clearConfigData + both/three runFiles,
   ConfigManager.cpp:33-55), so precedence holds across reloads.

## RQ5 -- Minimal implementation sketch (for the future brief)

- Hook point: ConfigManager.cpp loadConfigData(), insert after line 47:

  ```
  File modFile("conf/mod-overrides.lua");
  if (modFile.setReadOnly()) {
      if (!lua.runFile("conf/mod-overrides.lua")) {
          error("ConfigManager failed to parse conf/mod-overrides.lua");
          return false;
      }
  }
  ```

- File: bin/conf/mod-overrides.lua, grouped per CONFIG_OPTIONS.md sections,
  every line commented, each entry carrying: real key, type, code default,
  one-line effect, restart-vs-dynamic tag. Mirrors the BRIEF-026 viewer rows so
  the menu can render "edit this file" hints beside live values.
- Synergy with BRIEF-026: viewer registry already stores {label, configKey,
  restart}; add optional `overlayGroup` field pointing at the section header in
  mod-overrides.lua.
- Runtime experiment still owed: confirm on Linux host that a third runFile in
  the shared state merges (static analysis says yes -- single state, single
  parse pass -- but BRIEF-027 method asked for empirical confirmation).

## Seed list summary

| Bucket | Count | Examples |
|---|---|---|
| SAFE-DYNAMIC | ~51 | GalaxyWideGrouping, WipeFillingOnClone, AdvancedWaypoints, JTLEnabled, MaxCharactersPerGalaxy, all Mission/*, Regions/*, TransactionLog toggles |
| RESTART-REQUIRED | ~14 | DisableGroupVisibility, zone thread counts, NameManager.FilterTable, CommandCooldown.*, Login.API.* workers |
| NEVER-EXPOSE | ~10 | DB*/Mantis*/APIToken secrets, 7 class-(c) dead reads, naming-trap keys |

Exact per-option tags: see docs/customskills/CONFIG_OPTIONS.md sections 2-18
(read-behavior column verified 08252026).
