# ConfigManager Option Inventory (all server-configurable options)

- Status: REFERENCE (BRIEF-023 delivered; verified against source 08252026)
- Compiled: 08252026 by ox-alpha (opencode)
- Sources swept: `src/conf/ConfigManager.h` getter definitions (note: actual path is
  `src/conf/`, not `src/server/conf/`), every `ConfigManager::instance()->get*` /
  legacy-getter / local-pointer (`configManager->get*`, `config->get*`) callsite under
  `src/` (docs excluded), cross-checked against shipped `bin/conf/config.lua`.
- Line numbers are valid for the working tree as of 08252026.

## Classification legend

- **(a)** Key already present in shipped `bin/conf/config.lua` (documented by presence;
  comments exist for some sections).
- **(b)** Functional but undocumented -- readable in config-local.lua, absent from the
  shipped template. This is BRIEF-023's target set.
- **(c)** Dead read: getter/callsite exists but no live caller reaches it (or it is
  compile-time gated out of normal builds).

Read-behavior tags: **[startup]** read once during boot; **[dyn]** re-evaluated per call
(ConfigManager hot-reloads on configVersion bump); **[dyn-cache]** dynamic but cached,
auto-refreshes on configVersion bump; **[lua]** exposed to scripting.

---

## 0. Owner's six seed options -- VERIFIED IN CODE

| Option | Type | Default | Verified consumer(s) |
|---|---|---|---|
| Core3.PlayerManager.DisableGroupVisibility | bool | false | src/server/zone/managers/visibility/VisibilityManager.cpp:34 |
| Core3.PlayerManager.WipeFillingOnClone | bool | false | src/server/zone/managers/player/PlayerManagerImplementation.cpp:1883 |
| Core3.PlayerManager.GalaxyWideGrouping | bool | false | src/server/zone/managers/group/GroupManager.cpp:45; src/server/zone/objects/group/GroupObjectImplementation.cpp:371; commands InviteCommand.h:41, MakeLeaderCommand.h:32, DismissGroupMemberCommand.h:38 |
| Core3.PlayerManager.AdvancedWaypoints | bool | false | src/server/zone/objects/creature/commands/WaypointCommand.h:18 |
| Core3.PlayerCreationManager.MaxCharactersPerGalaxy | int | 10 | src/server/zone/managers/player/creation/PlayerCreationManager.cpp:321; src/server/zone/packets/zone/ClientIdMessageCallback.h:171 |
| Core3.JTL.JTLEnabled | bool | false | src/conf/ConfigManager.h:841 (isJtlEnabled, cached); surfaced to Lua as `isJtlEnabled()` via src/server/zone/managers/director/DirectorManager.cpp:5107-5108, registered at :554 |

All six exist and are live reads. All are class **(b)** (absent from shipped
bin/conf/config.lua). None are dead.

---

## 1. Core3 top-level (server infrastructure)

### 1a. Class (a): documented in shipped config.lua

| Option | Type | Default (code) | Consumer file:line |
|---|---|---|---|
| Core3.MakeLogin | bool | true | ConfigManager.h:254 <- ServerCore.cpp:693 [startup] |
| Core3.MakeZone | bool | true | ConfigManager.h:258 <- ServerCore.cpp:704 [startup] |
| Core3.MakePing | bool | true | ConfigManager.h:262 <- ServerCore.cpp:711 [startup] |
| Core3.MakeStatus | bool | true | ConfigManager.h:266 <- ServerCore.cpp:715 [startup] |
| Core3.ORB | string | "" | ConfigManager.h:322 <- ServerCore.cpp:677 [startup] |
| Core3.ORBPort | int | 44419 | ConfigManager.h:326 <- ServerCore.cpp:679 [startup] |
| Core3.DBHost | string | 127.0.0.1 | ConfigManager.h:330 <- db/ServerDatabase.cpp:15 [startup] |
| Core3.DBPort | int | 3306 | ConfigManager.h:362 <- ServerDatabase.cpp:19 [startup] |
| Core3.DBName | string | swgemu | ConfigManager.h:366 <- ServerDatabase.cpp:18 [startup] |
| Core3.DBUser | string | root | ConfigManager.h:370 <- ServerDatabase.cpp:16 [startup] |
| Core3.DBPass | string | Gemeni1 | ConfigManager.h:374 <- ServerDatabase.cpp:17 [startup] |
| Core3.DBSecret | string | swgemusecret | ConfigManager.h:378 <- login/LoginServerImplementation.cpp:70 [startup] |
| Core3.DBInstances | int | 1 (lua ships 2) | ServerDatabase.cpp:25 [startup] |
| Core3.MantisHost | string | 127.0.0.1 | ConfigManager.h:382 <- db/MantisDatabase.cpp:18 [startup] |
| Core3.MantisPort | int | 3306 | ConfigManager.h:386 <- MantisDatabase.cpp:23 [startup] |
| Core3.MantisName | string | swgemu | ConfigManager.h:398 (MantisDatabase ctor) [startup] |
| Core3.MantisUser | string | root | ConfigManager.h:402 (MantisDatabase ctor) [startup] |
| Core3.MantisPass | string | Gemeni1 | ConfigManager.h:406 (MantisDatabase ctor) [startup] |
| Core3.MantisPrfx | string | "" | ConfigManager.h:410 (MantisDatabase ctor) [startup] |
| Core3.MetricsHost | string | 127.0.0.1 | ConfigManager.h:422 <- ServerCore.cpp:684,689 [startup] |
| Core3.MetricsPort | int | 8125 | ConfigManager.h:430 <- ServerCore.cpp:684,690 [startup] |
| Core3.MetricsPrefix | string | "" | ConfigManager.h:426 <- ServerCore.cpp:687 [startup] |
| Core3.UseMetrics | bool | false | ConfigManager.h:294 [dyn-cache] <- Metrics.h:21,25,29,33; ServerCore.cpp:683-686 |
| Core3.PingPort | int | 44462 | ConfigManager.h:446 <- ServerCore.cpp:806 [startup] |
| Core3.PingAllowedConnections | int | 3000 | ConfigManager.h:466 <- ServerCore.cpp:808 [startup] |
| Core3.StatusPort | int | 44455 | ConfigManager.h:442 <- ServerCore.cpp:798 [startup] |
| Core3.StatusAllowedConnections | int | 100 | ConfigManager.h:462 <- ServerCore.cpp:800 [startup] |
| Core3.StatusInterval | int | 60 | ConfigManager.h:470 <- status/StatusServer.cpp:16 [startup] |
| Core3.ZoneAllowedConnections | int | 300 | ConfigManager.h:482 <- ServerCore.cpp:738 [startup] |
| Core3.ZoneGalaxyID | int | 2 | ConfigManager.h:486 <- ServerCore.cpp:748,832; zone/ZoneServerImplementation.cpp:58; login/SWGRealmsAPI.cpp:130 [startup] |
| Core3.ZoneServerPort | int | 0 (auto) | ConfigManager.h:490 <- ServerCore.cpp:735 [startup] |
| Core3.ZonesEnabled | sorted string[] | (empty) | ConfigManager.h:494 <- ZoneServerImplementation.cpp:241 [startup] |
| Core3.SpaceZonesEnabled | sorted string[] | (empty) | ConfigManager.h:498 <- ZoneServerImplementation.cpp:294 [startup] |
| Core3.TrePath | string | tre | ConfigManager.h:434 <- templates/manager/TemplateManager.cpp:428; managers/player/BadgeList.cpp:30 [startup] |
| Core3.TreFiles | string[] | (empty) | ConfigManager.h:394 <- TemplateManager.cpp:435; BadgeList.cpp:37 [startup] |
| Core3.LogFile | string | log/core3.log | ConfigManager.h:514 <- ServerCore.cpp:661 [startup] |
| Core3.LogFileLevel | int | 4 (INFO) | ConfigManager.h:518 <- ServerCore.cpp:664 [startup] |
| Core3.LogJSON | bool | false | ConfigManager.h:542 <- ServerCore.cpp:662 [startup] |
| Core3.LogSync | bool | false | ConfigManager.h:546 <- ServerCore.cpp:663 [startup] |
| Core3.LuaLogJSON | bool | false | ConfigManager.h:550 <- DirectorManager.cpp:422 [startup] |
| Core3.PathfinderLogJSON | bool | false | ConfigManager.h:554 <- collision/PathFinderManager.cpp:36 [startup] |
| Core3.PlayerLogLevel | int | 4 (INFO) | ConfigManager.h:578 <- objects/player/PlayerObjectImplementation.cpp:92 [startup] |
| Core3.MaxLogLines | int | 1000000 | ConfigManager.h:582 <- PlayerManagerImplementation.cpp:132,499 [startup] |
| Core3.RESTServerPort | int | 0 (disabled) | ConfigManager.h:562 <- web/RESTServer.cpp:342 [startup] |
| Core3.InactiveAccountTitle | string | "Account Disabled" | ConfigManager.h:566 <- login/account/AccountManager.cpp:278 [dyn] |
| Core3.InactiveAccountText | string | (disabled msg) | ConfigManager.h:570 <- AccountManager.cpp:279 [dyn] |
| Core3.CleanupMailCount | int | 25000 | ConfigManager.h:558 <- chat/ChatManagerImplementation.cpp:103 [startup] |
| Core3.MaxNavMeshJobs | int | 6 | ConfigManager.h:506 <- ServerCore.cpp:732 [startup] |
| Core3.MaxAuctionSearchJobs | int | 1 | ConfigManager.h:510 <- AuctionManagerImplementation.cpp:54 [startup] |
| Core3.DumpObjFiles | bool | true | ConfigManager.h:270 <- pathfinding/RecastNavMeshBuilder.cpp:487 [dyn] |
| Core3.ProgressMonitors | bool | false | ConfigManager.h:340 [dyn-cache] <- ChatManagerImpl.cpp:119; DirectorManager.cpp:176; MissionManagerImpl.cpp:150; StructureManager.cpp:163; CreatureTemplateManager.cpp:233; ShipAgentTemplateManager.cpp:208; TemplateManager.cpp:1078 |
| Core3.UnloadContainers | bool | true | ConfigManager.h:280 [dyn-cache] <- objects/scene/variables/ContainerObjectsMap.cpp:119 |
| Core3.CharacterBuilderEnabled | bool | false | ConfigManager.h:574 <- SuiManager.cpp:205; PlanetManagerImpl.cpp:280; CharacterBuilderTerminalImplementation.cpp:32,46; commands/ObjectCommand.h:220 (NOTE: ObjectCommand passes its own default `true`, diverging from the getter default) |
| Core3.AutoReg | bool | true | ConfigManager.h:474 <- LoginServerImplementation.cpp:68 [startup] |
| Core3.TermsOfService | string | "" | ConfigManager.h:534 <- PlayerObjectImplementation.cpp:3707 [dyn] |
| Core3.TermsOfServiceVersion | int | 0 | ConfigManager.h:538 <- PlayerObjectImplementation.cpp:3704,3726 [dyn] |
| Core3.PvpMode | bool | false | ConfigManager.h:310 [dyn-cache]; setPvpMode :318 <- ServerCore.cpp:469,490-495 (console `setpvp`); CreatureObjectImplementation.cpp:3474,3725 |

### 1b. Class (c): dead reads (getter defined, zero callers)

| Option | Type | Default | Definition | Note |
|---|---|---|---|---|
| Core3.TreManager.LatestTre | string | default_patch.tre | ConfigManager.h:389-391 | getLatestTre() never called anywhere in src/ |
| Core3.MOTD | string | "Welcome to SWGEmu!" | ConfigManager.h:413-415 | getMessageOfTheDay() never called; live MOTD uses bin/conf/motd.txt instead |
| Core3.JTL.LaunchFromDevice | bool | false | ConfigManager.h:848-859 | launchFromDevice() never called |
| Core3.ZoneProcessingThreads | int | 10 | ConfigManager.h:477-479 | getZoneProcessingThreads() has NO callers (zone queues are sized by Core3.Zone.ThreadsDefault/<Zone> instead) |
| Core3.LoginProcessingThreads | int | 1 | ConfigManager.h:453-455 | getLoginProcessingThreads() has NO callers (ServerCore.cpp:813-818 starts login with only port + allowed connections) |

Gotcha worth surfacing in CODE_REFERENCE.md: the last two are *documented* in shipped
config.lua yet ignored by current code -- classic trap for operators tuning them.

### Stale config.lua entries never read by code (inverse problem)

`MakeWeb`, `WebPorts`, `WebAccessLog`, `WebErrorLog`, `WebSessionTimeout` -- legacy web
server, no reader remains. `DeleteCharacters = 10` -- the code actually reads
**Core3.PurgeDeletedCharacters** (ConfigManager.h:502 <- ObjectManager.cpp:1103), so the
shipped `DeleteCharacters` key silently does nothing. That makes
Core3.PurgeDeletedCharacters (int, minutes, default 10) class **(b)** despite looking
documented.

---

## 2. Logging / session stats extras (all class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.RotateLogSizeMB | int | 100 | ConfigManager.h:522 <- ServerCore.cpp:661; NavMeshManager.cpp:21; PathFinderManager.cpp:37; SWGRealmsAPI.cpp:113,2484; RESTServer.cpp:38; AuctionManagerImpl.cpp:48; TransactionLog.cpp:410,768 (as fallback default) [startup] |
| Core3.RotateLogAtStart | bool | false | ConfigManager.h:526 <- ServerCore.cpp:661; ZoneClientSessionImplementation.cpp:70; RESTServer.cpp:36; SWGRealmsAPI.cpp:111,2482; AuctionManagerImpl.cpp:46; DirectorManager.cpp:331,421; TransactionLog.cpp:407,765 [startup] |
| Core3.SessionStatsSeconds | int | 1800 (clamped 300..3600 unless WITH_DEV_MODE) | ConfigManager.h:591 [dyn-cache] <- PlayerObjectImplementation.cpp:2477 |
| Core3.OnlineLogSeconds | int | 300 | ConfigManager.h:606 <- PlayerManagerImplementation.cpp:184 [startup] |
| Core3.OnlineLogSize | int | 100000000 | ConfigManager.h:615 [dyn-cache] <- PlayerManagerImplementation.cpp:7387 |
| Core3.LogOnlineCount | int | 3 | managers/player/OnlineZoneClientMap.h:49 [dyn] |
| Core3.LogOnlineOnSessionChange | bool | true | PlayerManagerImplementation.cpp:226 [startup] |
| Core3.Revision | string | "" | ConfigManager.h:418 <- ServerCore.cpp:298; RESTServer.cpp:84; ZoneServerImplementation.cpp:964; TransactionLog.cpp:888; commands/RevisionInfoCommand.h:24 [dyn] |

---

## 3. Core3.Login.* and Core3.Login.API.*

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.Login.EnableSessionId | bool | false | ConfigManager.h:729 [dyn-cache] <- AccountManager.cpp:195; ServerCore.cpp:698 |
| Core3.Login.SessionDuration | string | "00:15" | AccountManager.cpp:160 [dyn] |
| Core3.RegistrationMessage | string | "Automatic registration is currently disabled..." | AccountManager.cpp:234 [dyn] |
| Core3.Login.API.BaseURL | string | "" | SWGRealmsAPI.cpp:134; ServerCore.cpp:725,830 (non-empty enables the SWGRealms API path) [startup] |
| Core3.Login.API.APIToken | string | "" | SWGRealmsAPI.cpp:144 [startup] |
| Core3.Login.API.WorkerThreads | int | 4 | SWGRealmsAPI.cpp:122 [startup] |
| Core3.Login.API.DebugLevel | int | 0 | SWGRealmsAPI.cpp:126 [startup] |
| Core3.Login.API.DryRun | bool | false | SWGRealmsAPI.cpp:132 [startup] |
| Core3.Login.API.FailOpen | bool | false | SWGRealmsAPI.cpp:152 [startup] |
| Core3.Login.API.Timeout | int (sec) | 30 | SWGRealmsAPI.cpp:154 [startup] |
| Core3.Login.API.RotateLogSizeMB | int | falls back to Core3.RotateLogSizeMB | SWGRealmsAPI.cpp:113 [startup] |
| Core3.Login.API.MetricsInterval | int (sec) | 600 | SWGRealmsAPI.cpp:2445 [startup] |
| Core3.Login.API.StreamURL | string | code-default ws URL | SWGRealmsAPI.cpp:2510 [startup] |

All class (b).

---

## 4. Core3.RESTServer.*

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.RESTServer.LogLevel | int | 4 (INFO) | web/RESTServer.cpp:330 [startup] |
| Core3.RESTServer.WorkerThreads | int | 4 | RESTServer.cpp:331 [startup] |
| Core3.RESTServer.APIToken | string | "" (auth off) | RESTServer.cpp:350 [startup] |
| Core3.RESTServer.SSLKeyFile | string | "" | RESTServer.cpp:367 [startup] |
| Core3.RESTServer.SSLCertFile | string | "" | RESTServer.cpp:374 [startup] |
| Core3.RESTServer.RotateLogSizeMB | int | falls back to Core3.RotateLogSizeMB | RESTServer.cpp:38 [startup] |
| Core3.RESTServer.exportDir | string | log/exports/api/%Y-%m-%d/%H/ | web/APIProxyObjectManager.cpp:328 [dyn] |

All class (b).

---

## 5. Zone threading (dynamic per-zone keys)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.Zone.ThreadsDefault | int | 1 | zone/GroundZoneImplementation.cpp:36 [startup] |
| Core3.Zone.Threads<ZoneCapitalized> e.g. ThreadsCorellia | int | ThreadsDefault value | GroundZoneImplementation.cpp:37 [startup] |
| Core3.SpaceZone.ThreadsDefault | int | 1 | zone/SpaceZoneImplementation.cpp:28 [startup] |
| Core3.SpaceZone.Threads<ZoneCapitalized> | int | ThreadsDefault value | SpaceZoneImplementation.cpp:29 [startup] |
| Core3.ZonePortsBalancer | int | 1 (round-robin; other = random) | login/objects/Galaxy.h:125 -- NOTE: only compiled with USE_RANDOM_EXTRA_PORTS defined |
| Core3.ZoneServer.ClientLogLevel | int (per-account overridable) | -1 (off) | zone/ZoneClientSessionImplementation.cpp:49 -- passes accountID, so Core3.AccountFlags.<accountID>.Core3.ZoneServer.ClientLogLevel overrides per account |

All class (b). The `Core3.AccountFlags.<id>.<key>` overlay exists for any getter call
that passes an accountID (ConfigManager.h:198-207, findItem ConfigManager.cpp:451-455);
ClientLogLevel is currently the only in-tree consumer that uses it.

---

## 6. Core3.PlayerManager.* (class b -- the hidden set)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.PlayerManager.DisableGroupVisibility | bool | false | VisibilityManager.cpp:34 [startup-ish, cached in manager] |
| Core3.PlayerManager.WipeFillingOnClone | bool | false | PlayerManagerImplementation.cpp:1883 [dyn] |
| Core3.PlayerManager.GalaxyWideGrouping | bool | false | GroupManager.cpp:45; GroupObjectImplementation.cpp:371; InviteCommand.h:41; MakeLeaderCommand.h:32; DismissGroupMemberCommand.h:38 [dyn] |
| Core3.PlayerManager.AdvancedWaypoints | bool | false | WaypointCommand.h:18 [dyn] |
| Core3.PlayerManager.ValidClientVersion | string | 20050408-18:00 | ClientIdMessageCallback.h:98 [dyn] |

GOTCHA: PlayerManagerImplementation.cpp:6914 reads **`PlayerManager.accountVictimList`**
(bool, false) -- *without* the `Core3.` prefix. Operators must declare it at the TOP
LEVEL of the Core3 table as a sibling table named `PlayerManager`... which collides with
`Core3.PlayerManager.*`. In practice the correct spelling for that one key is a
top-level `PlayerManager.accountVictimList`; flag this naming inconsistency in docs.

## 7. Core3.PlayerCreationManager.* (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.PlayerCreationManager.MaxCharactersPerGalaxy | int | 10 | PlayerCreationManager.cpp:321; ClientIdMessageCallback.h:171 [dyn] |
| Core3.PlayerCreationManager.EnableTutorial | bool | false | PlayerCreationManager.cpp:379 [dyn] |

## 8. Core3.JTL.* 

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.JTL.JTLEnabled | bool | false | ConfigManager.h:841 [dyn-cache] -> DirectorManager.cpp:5108 (Lua `isJtlEnabled()`) -- class (b), master switch gating all JTL/space-travel lua |
| Core3.JTL.LaunchFromDevice | bool | false | ConfigManager.h:854 -- class **(c)**, no consumers |

---

## 9. Mission / bounty (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.MissionManager.ListRequestCooldown | int ms | 1400 | MissionManagerImplementation.cpp:200 [dyn] |
| Core3.MissionManager.IncludeFactionPets | bool | true | ConfigManager.h:354 [dyn-cache] <- MissionManagerImpl.cpp:837,1881 |
| Core3.MissionManager.AnonymousBountyTerminals | bool | false | MissionManagerImpl.cpp:1104; MissionObjectiveImplementation.cpp:324; events/FindTargetTask.h:136 [dyn] |
| Core3.MissionManager.MaxBountiesPerJedi | int | 5 | MissionManagerImpl.cpp:2096 [dyn] |
| Core3.MissionManager.PrivateStructureJediMissions | bool | true | MissionManagerImpl.cpp:2139 [dyn] |
| Core3.MissionManager.BountyExpirationTime | int ms | MISSIONDURATION constant | MissionObjectiveImplementation.cpp:59 [dyn] |
| Core3.MissionManager.PlayerBountyCooldown | bool | INCONSISTENT: true at :2068,:2159,:2234 but false at :2180 | MissionManagerImplementation.cpp:2068,2159,2180,2234 [dyn] |
| Core3.MissionManager.PlayerBountyCooldownTime | int ms | 86400000 (24h) | MissionManagerImpl.cpp:2160 [dyn] |

Flag: the PlayerBountyCooldown default disagreement across four callsites in the same
file looks like a bug magnet; worth a behavior note.

## 10. Combat / PvP / GCW (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.GCWManager.useCovertOvertSystem | bool | false | ConfigManager.h:716 [dyn-cache] <- ~20 sites incl. CombatManager.cpp:382,3526; QueueCommand.cpp:320; QueueCommand-family headers (CombatQueueCommand.h:184, DeclareOvertCommand.h:23, TransferForceCommand.h:99); CreatureObjectImpl.cpp:3483,3598,3672,3874; TangibleObjectImpl.cpp:193,351; GCWManagerImpl.cpp:758,1442,2682; BuildingObjectImpl.cpp:1487; InstallationObjectImpl.cpp:790,891; PetControlDeviceImpl.cpp:164; ScannerZoneComponent.cpp:82; ContrabandScanSessionImpl.cpp:576; PvpTefRemovalTask.h:52; PlayerManagerImpl.cpp:1831; AiAgentImpl.cpp:4223; DirectorManager.cpp:5016 (Lua `isCovertOvertSystem`) |
| Core3.ChatManager.PvpBroadcastChannel | bool | false | ConfigManager.h:703 [dyn-cache] <- ChatManagerImpl.cpp:324; PlayerManagerImpl.cpp:1441; PlayerObjectImpl.cpp:1834; TangibleObjectImpl.cpp:227 |
| Core3.CombatManager.AllowSameAccountLinkDeadBeneficialActions | bool | true | CreatureObjectImplementation.cpp:3847 [dyn] |

## 11. Account manager (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.AccountManager.CreatedDateFirstConnect | bool | false | AccountManager.cpp:412,483 [dyn] |
| Core3.AccountManager.HolocronTicketsEnabled | bool | false | packets/ui/CreateTicketMessageCallback.h:26,38 [dyn] |

## 12. AI / ship AI (class b unless noted)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.AiAgent.LogLevel | int | WARNING at AiAgentImpl.cpp:104; second read site :3132 and ShipAiAgentImpl.cpp:2440 pass -1 (off) -- two different defaults for the same key | AiAgentImplementation.cpp:104,3132; ShipAiAgentImplementation.cpp:356,2440 [dyn] |
| Core3.AiAgent.<TemplateName>.LogLevel | int | -1 (off) | AiAgentImplementation.cpp:267-269 -- per-NPC-template override, key derived from template name [startup per agent load] |
| Core3.AiAgent.Verbose | bool | false | ai/bt/Behavior.cpp:27 (version-cached) [dyn-cache] |
| Core3.AiAgent.ConsoleThrottle | int | 100 (1 under DEBUG_AI) | ConfigManager.h:661-680 [dyn-cache] <- AiAgentImplementation.cpp:4850; ShipAiAgentImplementation.cpp:2411 |
| Core3.AiAgent.AiAgentLoadTesting | bool | false | ConfigManager.h:683-695; callsites AiAgentImplementation.cpp:3057,3851; bt/decorator/LookForTarget.h:29-33; btspace/decoratorspace/LookForTargetSpace.h:31-33 -- ENTIRE feature is `#ifdef DEBUG_AI`; class **(c)** in normal builds |
| Core3.ShipAiAgent.LogLevel | int | WARNING | ShipAiAgentImplementation.cpp:356 [dyn] |

## 13. Spawning / regions (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.Regions.minimumLairSpawnInterval | int ms | 5000 | ConfigManager.h:742 [dyn-cache] <- SpawnAreaImplementation.cpp:40 |
| Core3.Regions.minimumSpaceSpawnInterval | int ms | 5000 | ConfigManager.h:755 [dyn-cache] <- SpaceSpawnAreaImplementation.cpp:49 |
| Core3.Regions.DisableWorldSpawns | bool | false | ConfigManager.h:768 [dyn-cache] <- SpawnAreaImplementation.cpp:44 |
| Core3.Regions.DisableSpaceSpawns | bool | false | ConfigManager.h:781 [dyn-cache] <- SpaceSpawnAreaImpl.cpp:53 |
| Core3.Regions.spawnCheckRange | float | 64.0 | ConfigManager.h:794 [dyn-cache] <- SpawnAreaImpl.cpp:167,262 |
| Core3.Regions.spaceSpawnCheckRange | float | 1024.0 | ConfigManager.h:807 [dyn-cache] <- SpaceSpawnAreaImpl.cpp:243 |

## 14. Loot / FRS / misc managers (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.LootManager.DebugAttributes | bool | false | ConfigManager.h:820 [dyn-cache] <- LootManagerImpl.cpp:315 |
| Core3.FrsManager.ImmediateMaintXpDeduction | bool | false | FrsManagerImplementation.cpp:390,1009 [dyn] |
| Core3.NameManager.FilterTable | string | oldFilterWords | NameManager.cpp:192 [startup] |
| Core3.TreManager.ReloadStrings | bool | false | StringIdManager.cpp:90 [startup] |
| Core3.SameAccountTipsAreFree | bool | false | TipCommand.h:104; sui TipCommandSuiCallback.h:41 [dyn] |
| Core3.NavMeshManager.LogLevel | int | INFO | NavMeshManager.cpp:22 [startup] |
| Core3.CommandConfigManager.DumpAdminCommands | bool | false | objectcontroller/command/CommandConfigManager.cpp:113 [startup] |
| Core3.CommandCooldown.<commandName> | int ms | per-command coded cooldown | QueueCommand.h:360 (setCooldown) [startup, command load] |
| Core3.AuctionManager.LogLevel | int | -1 | AuctionManagerImpl.cpp:42 [startup] |
| Core3.AuctionManager.RotateLogSizeMB | int | falls back to Core3.RotateLogSizeMB | AuctionManagerImpl.cpp:48 [startup] |
| Core3.AuctionManager.Startup.ExpireInvalid | bool | false | AuctionManagerImpl.cpp:440,511 [startup] |
| Core3.AuctionItem.ExportOnDestroy | bool | false | AuctionItemImplementation.cpp:81 [dyn] |

## 15. Structures / travel (class b unless noted)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.StructureMaintenanceTask.AllowBankPayments | bool | true | StructureManager.cpp:1376 [dyn] |
| Core3.StructureManager.EnhancedFurnitureRotate | bool | false | commands/RotateFurnitureCommand.h:30 [dyn] |
| Core3.StructureManager.CreateNavMesh.<objectFullPath> | bool | false | StructureObjectImplementation.cpp:218-220 (client-object navmesh opt-in; key contains full template path) [startup] |
| Core3.StructureObject.MaintenanceBootDelay | int sec | 600 (+random 3600) | StructureObjectImpl.cpp:422 [dyn] |
| Core3.Tweaks.StructureObject.DestoryOrphans | bool | false | StructureObjectImpl.cpp:228 (typo "Destory" is the real key) [dyn] |
| Core3.ShuttleZoneComponent.BootDelay | int ms | 300000 | ShuttleZoneComponent.cpp:48; ShuttleInstallationZoneComponent.cpp:22 [startup] |
| Core3.PlanetManager.ShuttleportAwayTime / ShuttleportLandedTime / ShuttleportLandingTime / StarportAwayTime / StarportLandedTime / StarportLandingTime | int min | values parsed from serverdata travel lua | PlanetManagerImplementation.cpp:231-236 -- entire block is `#if DEBUG_TRAVEL`; class **(c)** in normal builds |

## 16. Player objects / logout (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.PlayerObject.AlwaysSafeLogout | bool | false | PlayerObjectImplementation.cpp:2337 [dyn] |
| Core3.PlayerObject.LinkDeadDelay | int sec | 180 | PlayerObjectImpl.cpp:2556 [dyn] |
| Core3.TangibleObject.NoTradeMessage | string | "" | ConfigManager.h:628 [dyn-cache] <- TangibleObjectImpl.cpp:835; ArmorObjectImpl.cpp:85; JediRobeAttributeListComponent.h (via ForceNoTrade trio context) |
| Core3.TangibleObject.ForceNoTradeMessage | string | "" | ConfigManager.h:641 [dyn-cache] <- TangibleObjectImpl.cpp:831; ArmorObjectImpl.cpp:81; components/JediRobeAttributeListComponent.h:33 |
| Core3.TangibleObject.ForceNoTradeADKMessage | string | "" | ConfigManager.h:654 [dyn-cache] <- TangibleObjectImpl.cpp:833; ArmorObjectImpl.cpp:83 |

## 17. Exports / transaction log (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.SceneObject.exportDir | string | log/exports/%Y-%m-%d/%H/ | SceneObjectImplementation.cpp:2547 [dyn] |
| Core3.TransactionLog.Enabled | bool | false | TransactionLog.cpp:915 (macro CONFIG_CACHED_BOOL_GETTER :901-913) [dyn-cache] |
| Core3.TransactionLog.AsyncExport | bool | false | TransactionLog.cpp:917 [dyn-cache] |
| Core3.TransactionLog.Verbose | bool | false | TransactionLog.cpp:919 [dyn-cache] |
| Core3.TransactionLog.PruneCreatureObjects | bool | true | TransactionLog.cpp:921 [dyn-cache] |
| Core3.TransactionLog.PruneCraftedComponents | bool | true | TransactionLog.cpp:923 [dyn-cache] |
| Core3.TransactionLog.RotateLogSizeMB | int | falls back to Core3.RotateLogSizeMB | TransactionLog.cpp:410,768 [startup] |
| Core3.TransactionLog.LogLevel | int | DEBUG | TransactionLog.cpp:411,769 [startup] |
| Core3.TransactionLog.WorkerThreads | int | 4 | TransactionLog.cpp:421 [startup] |

## 18. Lua engine (class b)

| Option | Type | Default | Consumer file:line |
|---|---|---|---|
| Core3.LuaEngine.LogLevel | int | ERROR | DirectorManager.cpp:417 [startup] |
| Core3.LuaEngine.LuaEventLogLevel | int | INFO | DirectorManager.cpp:334 [startup] |
| Core3.DirectorManager.SlowLoadMs | int ms | 1000 | DirectorManager.cpp:356 [startup] |

Plus Lua-exposed config mirrors (read via ConfigManager, called from scripts):
`isJtlEnabled()` (DirectorManager.cpp:5107-5108) and `isCovertOvertSystem()`
(DirectorManager.cpp:5016).

---

## 19. Summary counts

- Total distinct option keys enumerated: ~135 static keys + 6 families of dynamic keys
  (Zone/SpaceZone Threads<Zone>, CommandCooldown.<cmd>, AiAgent.<Template>.LogLevel,
  StructureManager.CreateNavMesh.<path>, AccountFlags.<acct>.<key> overlay).
- Class (a) documented in shipped config.lua: ~60 (mostly server infra/logging).
- Class (b) functional-but-hidden: ~65 -- including all six seed options.
- Class (c) dead reads: Core3.TreManager.LatestTre, Core3.MOTD, Core3.JTL.LaunchFromDevice,
  Core3.ZoneProcessingThreads, Core3.LoginProcessingThreads, plus build-gated
  Core3.AiAgent.AiAgentLoadTesting (DEBUG_AI) and Core3.PlanetManager.* shuttle timing
  overrides (DEBUG_TRAVEL).
- Silent traps found: `DeleteCharacters` (stale name; real key is
  Core3.PurgeDeletedCharacters), `PlayerManager.accountVictimList` (missing `Core3.`
  prefix), `DestoryOrphans` typo-as-key, PlayerBountyCooldown default inconsistency,
  ObjectCommand.h:220 CharacterBuilderEnabled local default `true` vs getter `false`,
  Core3.AiAgent.LogLevel double-default (WARNING vs -1).

## 20. Method / caveats

- Extraction was mechanical grep + manual verification of every ambiguous callsite
  (dynamic key construction inspected: GroundZoneImpl.cpp:33-37, SpaceZoneImpl.cpp,
  AiAgentImpl.cpp:265-273, StructureObjectImpl.cpp:218, QueueCommand.h:360,
  TransactionLog.cpp:901-923, Galaxy.h:123-133).
- Callsites inside commented-out blocks were ignored (e.g. PlanetManagerImpl.cpp:567).
- ConfigManager supports runtime hot-reload (`loadConfigData`, configVersion);
  [dyn]/[dyn-cache] options respond to `setConfigFile`/REST config changes without
  restart; [startup] options require restart. Cached getters refresh automatically on
  configVersion bump.
- Per-account overlay: any getter invoked with an accountID resolves
  `Core3.AccountFlags.<accountID>.<name>` first (ConfigManager.h:198-207).

## 21. Mod option: customSkillsConfig.training (P07, BRIEF-049/049b)
| Option | Type | Default | Consumer |
|---|---|---|---|
| training.trainersTeachAll | bool | true | skillTrainer.lua getTrainerSkillTable -- trainer teaches own tree + all elite/master trees above its line (cyvster2 hierarchical semantics; brawler covers TKA/swordsman/pikeman/fencer, NOT smuggler) |
| training.trainersTeachEverything | bool | false | same file -- universal union of all 36 trees from ANY trainer (overrides trainersTeachAll when true) |

Read once per trainer interaction from `scripts/customskills/config.lua`
(`customSkillsConfig.training` table). When true, every skill trainer offers
the union of all 36 profession trees (774 skills, elite/master included);
per-skill prerequisites, XP, and skill-point costs remain engine-enforced at
learn time. Set false for vanilla one-tree-per-trainer behavior. Requires
server restart (screenplay Lua is not hot-reloaded).

## 22. Mod option: customSkillsConfig.structures (BRIEF-050)

| Option | Type | Default | Consumer |
|---|---|---|---|
| structures.accountSharedLots | bool | true (owner approved) | CustomSkillsStructureLots (src/server/zone/managers/customskills/structures/) |

Behavior when enabled:
- Lot pool = `maximumLots x (characters on the account, this galaxy)`, shared
  across ALL account characters. Per-character cap still comes from the
  querying ghost's `maximumLots`, so /adjustLotCount grants scale the pool.
- Placement grants ADMIN on the new structure to every OTHER character OID on
  the placer's account (placer keeps vanilla owner/ADMIN).
- Cache: per-account `AccountLotEntry` (totalLotsUsed + perCharLots map) in
  CustomSkillsStructureLots.cpp; mutex-guarded; built LAZY on first query per
  account after boot (one scan of live characters' ownedStructures), then
  updated incrementally at each ownedStructures mutation point. Never rescanned
  per query -- this is the anti-cyvster2-stutter design (MIGRATION_LEDGER Q02).
- Incremental maintenance points (all tagged `// BRIEF-050 (mod hook)`):
  placeStructure + camp placement (StructureManager.cpp:546, :631),
  DestroyStructureTask.h:118, TransferstructureCommand.h:207/:216,
  camp adoption CampSiteActiveAreaImplementation.cpp:340/:370.
- Query hook: PlayerObjectImplementation::getLotsRemaining
  (PlayerObjectImplementation.cpp:3142) falls back to vanilla math if the
  account cannot be resolved or has no characters.

## 23. Mod option: customSkillsConfig.loot (BRIEF-046/047, E02/E05)

| Option | Type | Default | Consumer |
|---|---|---|---|
| loot.creditsToTopDamager | bool | true (owner approved) | CreatureManagerImplementation.cpp death-credit block (~:687) |
| loot.nonHumanoidCredits | bool | true (cyvster2 parity) | same gate, widened to all NPC creatures |
| loot.creditMultiplier | float | 5.0 (cyvster2 shipped 5x; vanilla 1.0) | multiplies calculateLootCredits() result |
| loot.attachmentAutoName | bool | true (owner approved) | LootManagerImplementation.cpp setSkillMods() |

Behavior when enabled:
- creditsToTopDamager: NPC credit drops skip the corpse and are paid directly
  to the top-damage player (`ThreatMap::getHighestDamagePlayer`, null-checked)
  with a system message confirming the amount. Event mobs always excluded.
  false = vanilla corpse credits.
- attachmentAutoName: newly looted Armor/Clothing Attachments are renamed
  `[AA] <statname>: <value>` / `<[CA]> <statname>: <value>` from their highest
  skill mod. Name is set exactly once after the mod scan (cyvster2 renamed
  inside the loop). false = vanilla generic attachment names.

## 24. Mod option: customSkillsConfig.surveying (BRIEF-048, C06)

| Option | Type | Default | Consumer |
|---|---|---|---|
| surveying.maxRange | int (meters) | 2624 (owner approved; vanilla was 384) | SurveyToolImplementation.cpp + SurveyToolSetRangeSuiCallback.h + ResourceSpawner.cpp |

Behavior: survey tool range tiers scale dynamically from 64m up to maxRange
across six skill-gated tiers (gates at surveying 20/35/55/75/100/120,
vanilla-preserved), in 64m-granular steps; sample grid is 6x6 points above
1024m (3x3/4x4/5x5 below as vanilla). Setting maxRange = 384 restores the
exact vanilla ladder. Accepted range 64-8192; requires server restart.
Shared tier helpers live in `SurveyRangeLadder` (SurveyToolSetRangeSuiCallback.h).
