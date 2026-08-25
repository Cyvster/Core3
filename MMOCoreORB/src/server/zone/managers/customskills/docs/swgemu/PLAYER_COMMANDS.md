# SWGEmu Player Commands — Full Reference
*Audience: players & staff.* Every player-facing command on this build, grouped by activity, parsed from the C++ command classes with file:line cites. Admin/GM-only commands live in `ADMIN_COMMANDS.md` and are excluded here (dual-use commands noted). Code is authoritative. Syntax: `<required>`, `[optional]`.

## Method
Command list derived from `CommandConfigManager.cpp` registrations cross-checked against `src/server/zone/objects/creature/commands/*` classes; groupings curated; descriptions from command-class semantics. Deep-dive section covers argumented/notable commands.

# SWGEmu Player Commands — Full Reference
*Audience: players & staff.* Every player-facing command on this build, grouped by activity, parsed from the C++ command classes with file:line cites. Admin/GM-only commands live in `ADMIN_COMMANDS.md` and are excluded here. Code is authoritative.

## Social & Entertainer (29)
`/anon` — toggle anonymous player profile. `AnonCommand.h:18`  
`/bandFlourish` — band flourish control for grouped musicians/dancers. `BandFlourishCommand.h:162`  
`/changeBandMusic` — band leader changes the whole band's song. `ChangeBandMusicCommand.h:25`  
`/changeDance` — switch current dance (no arg lists dances). `ChangeDanceCommand.h:18`  
`/changeMusic` — switch current song (no arg lists songs). `ChangeMusicCommand.h:18`  
`/colorlights` — toggle color-light frame on instrument performance. `ColorlightsCommand.h:18`  
`/dazzle` — entertainer light effect. `DazzleCommand.h:18`  
`/flourish` — perform flourish 1-8 while dancing/playing. `FlourishCommand.h:18`  
`/holoEmote` — play owned holo-emote generator animation. `HoloEmoteCommand.h:16`  
`/lfg` — toggle looking-for-group flag. `LfgCommand.h:18`  
`/listen` — listen to a performing musician for buff/XP. `ListenCommand.h:16`  
`/rolePlay` — toggle role-play flag. `RolePlayCommand.h:18`  
`/setMoodInternal` — set mood internal dispatch. `SetMoodInternalCommand.h:16`  
`/setPerformanceBuffTarget` — set the target who receives your performance buffs. `SetPerformanceBuffTargetCommand.h:14`  
`/smokebomb` — smokebomb visual effect. `SmokebombCommand.h:18`  
`/socialInternal` — internal social/emote dispatch. `SocialInternalCommand.h:22`  
`/spatialChatInternal` — internal spatial chat dispatch. `SpatialChatInternalCommand.h:16`  
`/spotlight` — spotlight visual effect. `SpotlightCommand.h:18`  
`/startBand` — start band-synced music with your group. `StartBandCommand.h:21`  
`/startDance` — begin dancing (arg = dance name). `StartDanceCommand.h:33`  
`/startMusic` — begin playing music (needs equipped instrument). `StartMusicCommand.h:36`  
`/stopBand` — stop the entire band's performance. `StopBandCommand.h:18`  
`/stopDance` — stop dancing. `StopDanceCommand.h:18`  
`/stopMusic` — stop playing music. `StopMusicCommand.h:19`  
`/stoplistening` — stop listening to a musician. `StoplisteningCommand.h:18`  
`/stopwatching` — stop watching a dancer. `StopwatchingCommand.h:18`  
`/toggleAwayFromKeyBoard` — toggle AFK flag. `ToggleAwayFromKeyBoardCommand.h:18`  
`/ventriloquism` — entertainer ventriloquism effect. `VentriloquismCommand.h:18`  
`/watch` — watch a performing dancer for buff/XP. `WatchCommand.h:16`  

## Medical (non-Force) (31)
`/activateClone` — activate Clone. `ActivateCloneCommand.h:16`  
`/applyDisease` — apply Disease. `ApplyDiseaseCommand.h:17`  
`/applyPoison` — apply Poison. `ApplyPoisonCommand.h:17`  
`/centerOfBeing` — center Of Being. `CenterOfBeingCommand.h:16`  
`/channelForce` — channel Force. `ChannelForceCommand.h:17`  
`/cureDisease` — cure Disease. `CureDiseaseCommand.h:21`  
`/curePoison` — cure Poison. `CurePoisonCommand.h:18`  
`/deathBlow` — death Blow. `DeathBlowCommand.h:21`  
`/diagnose` — diagnose. `DiagnoseCommand.h:21`  
`/equilibrium` — equilibrium. `EquilibriumCommand.h:16`  
`/extinguishFire` — extinguish Fire. `ExtinguishFireCommand.h:18`  
`/feignDeath` — feign Death. `FeignDeathCommand.h:15`  
`/firstAid` — first Aid. `FirstAidCommand.h:84`  
`/healDamage` — heal Damage. `HealDamageCommand.h:374`  
`/healEnhance` — heal Enhance. `HealEnhanceCommand.h:250`  
`/healMind` — heal Mind. `HealMindCommand.h:67`  
`/healState` — heal State. `HealStateCommand.h:178`  
`/healWound` — heal Wound. `HealWoundCommand.h:224`  
`/innate` — innate. `InnateCommand.h:16`  
`/meditate` — meditate. `MeditateCommand.h:19`  
`/powerBoost` — power Boost. `PowerBoostCommand.h:21`  
`/quickHeal` — quick Heal. `QuickHealCommand.h:82`  
`/regeneration` — regeneration. `RegenerationCommand.h:16`  
`/requestSetStatMigrationData` — request Set Stat Migration Data. `RequestSetStatMigrationDataCommand.h:29`  
`/requestStatMigrationData` — request Stat Migration Data. `RequestStatMigrationDataCommand.h:19`  
`/requestStimpack` — request Stimpack. `RequestStimpackCommand.h:23`  
`/rescue` — rescue. `RescueCommand.h:19`  
`/revivePlayer` — revive Player. `RevivePlayerCommand.h:148`  
`/tendDamage` — tend Damage. `TendDamageCommand.h:29`  
`/tendWound` — tend Wound. `TendWoundCommand.h:30`  
`/vitalize` — vitalize. `VitalizeCommand.h:16`  

## Trader & Crafting (29)
`/addPower` — add Power. `AddPowerCommand.h:20`  
`/applyPowerup` — apply Powerup. `ApplyPowerupCommand.h:20`  
`/cancelCraftingSession` — cancel Crafting Session. `CancelCraftingSessionCommand.h:18`  
`/closeContainer` — close Container. `CloseContainerCommand.h:16`  
`/createManfSchematic` — create Manf Schematic. `CreateManfSchematicCommand.h:19`  
`/createPrototype` — create Prototype. `CreatePrototypeCommand.h:19`  
`/extractObject` — extract Object. `ExtractObjectCommand.h:19`  
`/factoryCrateSplit` — factory Crate Split. `FactoryCrateSplitCommand.h:18`  
`/findMyTrainer` — find My Trainer. `FindMyTrainerCommand.h:13`  
`/getAttributesBatch` — get Attributes Batch. `GetAttributesBatchCommand.h:20`  
`/giveItem` — give Item. `GiveItemCommand.h:25`  
`/imagedesign` — imagedesign. `ImagedesignCommand.h:19`  
`/insure` — insure. `InsureCommand.h:19`  
`/nextCraftingStage` — next Crafting Stage. `NextCraftingStageCommand.h:19`  
`/openContainer` — open Container. `OpenContainerCommand.h:17`  
`/recruitSkillTrainer` — recruit Skill Trainer. `RecruitSkillTrainerCommand.h:19`  
`/requestCraftingSession` — request Crafting Session. `RequestCraftingSessionCommand.h:20`  
`/requestDraftSlotsBatch` — request Draft Slots Batch. `RequestDraftSlotsBatchCommand.h:18`  
`/requestResourceWeightsBatch` — request Resource Weights Batch. `RequestResourceWeightsBatchCommand.h:16`  
`/resourceContainerSplit` — resource Container Split. `ResourceContainerSplitCommand.h:19`  
`/resourceContainerTransfer` — resource Container Transfer. `ResourceContainerTransferCommand.h:19`  
`/selectDraftSchematic` — select Draft Schematic. `SelectDraftSchematicCommand.h:16`  
`/setCurrentSkillTitle` — set Current Skill Title. `SetCurrentSkillTitleCommand.h:16`  
`/surrenderSkill` — surrender Skill. `SurrenderSkillCommand.h:15`  
`/teach` — teach. `TeachCommand.h:18`  
`/tip` — tip. `TipCommand.h:130`  
`/transferItemArmor` — transfer Item Armor. `TransferItemArmorCommand.h:22`  
`/transferItemMisc` — transfer Item Misc. `TransferItemMiscCommand.h:24`  
`/transferItemWeapon` — transfer Item Weapon. `TransferItemWeaponCommand.h:22`  

## Scout & Resources (16)
`/areatrack` — areatrack. `AreatrackCommand.h:18`  
`/burstRun` — burst Run. `BurstRunCommand.h:16`  
`/conceal` — conceal. `ConcealCommand.h:99`  
`/distract` — distract. `DistractCommand.h:18`  
`/firejet` — firejet. `FirejetCommand.h:18`  
`/fish` — fish. `FishCommand.h:16`  
`/forage` — forage. `ForageCommand.h:18`  
`/harvestCorpse` — harvest Corpse. `HarvestCorpseCommand.h:21`  
`/loot` — loot. `LootCommand.h:28`  
`/maskscent` — maskscent. `MaskscentCommand.h:13`  
`/medicalForage` — medical Forage. `MedicalForageCommand.h:18`  
`/requestCoreSample` — request Core Sample. `RequestCoreSampleCommand.h:24`  
`/requestSurvey` — request Survey. `RequestSurveyCommand.h:26`  
`/sample` — sample. `SampleCommand.h:16`  
`/sampleDNA` — sample D N A. `SampleDNACommand.h:19`  
`/tame` — tame. `TameCommand.h:19`  

## Pets & Droids (8)
`/associateDroidControlDeviceWithShip` — associate Droid Control Device With Ship. `AssociateDroidControlDeviceWithShipCommand.h:17`  
`/detonateDroid` — detonate Droid. `DetonateDroidCommand.h:17`  
`/emboldenpets` — emboldenpets. `EmboldenpetsCommand.h:21`  
`/enragepets` — enragepets. `EnragepetsCommand.h:21`  
`/healDroidDamage` — heal Droid Damage. `HealDroidDamageCommand.h:125`  
`/healDroidWound` — heal Droid Wound. `HealDroidWoundCommand.h:157`  
`/healPet` — heal Pet. `HealPetCommand.h:130`  
`/tellpet` — tellpet. `TellpetCommand.h:20`  

## Jedi & Force (54)
`/avoidIncapacitation` — avoid Incapacitation. `AvoidIncapacitationCommand.h:19`  
`/berserk1` — berserk 1. `Berserk1Command.h:18`  
`/berserk2` — berserk 2. `Berserk2Command.h:16`  
`/forceAbsorb1` — force Absorb 1. `ForceAbsorb1Command.h:20`  
`/forceAbsorb2` — force Absorb 2. `ForceAbsorb2Command.h:18`  
`/forceArmor1` — force Armor 1. `ForceArmor1Command.h:20`  
`/forceArmor2` — force Armor 2. `ForceArmor2Command.h:20`  
`/forceCureDisease` — force Cure Disease. `ForceCureDiseaseCommand.h:17`  
`/forceCurePoison` — force Cure Poison. `ForceCurePoisonCommand.h:18`  
`/forceFeedback1` — force Feedback 1. `ForceFeedback1Command.h:19`  
`/forceFeedback2` — force Feedback 2. `ForceFeedback2Command.h:18`  
`/forceResistBleeding` — force Resist Bleeding. `ForceResistBleedingCommand.h:21`  
`/forceResistDisease` — force Resist Disease. `ForceResistDiseaseCommand.h:21`  
`/forceResistPoison` — force Resist Poison. `ForceResistPoisonCommand.h:21`  
`/forceResistStates` — force Resist States. `ForceResistStatesCommand.h:20`  
`/forceRun1` — force Run 1. `ForceRun1Command.h:28`  
`/forceRun2` — force Run 2. `ForceRun2Command.h:25`  
`/forceRun3` — force Run 3. `ForceRun3Command.h:26`  
`/forceShield1` — force Shield 1. `ForceShield1Command.h:20`  
`/forceShield2` — force Shield 2. `ForceShield2Command.h:20`  
`/forceSpeed1` — force Speed 1. `ForceSpeed1Command.h:21`  
`/forceSpeed2` — force Speed 2. `ForceSpeed2Command.h:21`  
`/healActionSelf1` — heal Action Self 1. `HealActionSelf1Command.h:14`  
`/healActionSelf2` — heal Action Self 2. `HealActionSelf2Command.h:14`  
`/healActionWoundOther1` — heal Action Wound Other 1. `HealActionWoundOther1Command.h:15`  
`/healActionWoundOther2` — heal Action Wound Other 2. `HealActionWoundOther2Command.h:15`  
`/healActionWoundSelf1` — heal Action Wound Self 1. `HealActionWoundSelf1Command.h:17`  
`/healActionWoundSelf2` — heal Action Wound Self 2. `HealActionWoundSelf2Command.h:17`  
`/healAllOther1` — heal All Other 1. `HealAllOther1Command.h:17`  
`/healAllOther2` — heal All Other 2. `HealAllOther2Command.h:17`  
`/healAllSelf1` — heal All Self 1. `HealAllSelf1Command.h:17`  
`/healAllSelf2` — heal All Self 2. `HealAllSelf2Command.h:16`  
`/healBattleFatigueOther1` — heal Battle Fatigue Other 1. `HealBattleFatigueOther1Command.h:16`  
`/healBattleFatigueOther2` — heal Battle Fatigue Other 2. `HealBattleFatigueOther2Command.h:17`  
`/healBattleFatigueSelf1` — heal Battle Fatigue Self 1. `HealBattleFatigueSelf1Command.h:19`  
`/healBattleFatigueSelf2` — heal Battle Fatigue Self 2. `HealBattleFatigueSelf2Command.h:17`  
`/healHealthSelf1` — heal Health Self 1. `HealHealthSelf1Command.h:15`  
`/healHealthSelf2` — heal Health Self 2. `HealHealthSelf2Command.h:14`  
`/healHealthWoundOther1` — heal Health Wound Other 1. `HealHealthWoundOther1Command.h:16`  
`/healHealthWoundOther2` — heal Health Wound Other 2. `HealHealthWoundOther2Command.h:15`  
`/healHealthWoundSelf1` — heal Health Wound Self 1. `HealHealthWoundSelf1Command.h:17`  
`/healHealthWoundSelf2` — heal Health Wound Self 2. `HealHealthWoundSelf2Command.h:16`  
`/healMindSelf1` — heal Mind Self 1. `HealMindSelf1Command.h:17`  
`/healMindSelf2` — heal Mind Self 2. `HealMindSelf2Command.h:17`  
`/healMindWoundOther1` — heal Mind Wound Other 1. `HealMindWoundOther1Command.h:16`  
`/healMindWoundOther2` — heal Mind Wound Other 2. `HealMindWoundOther2Command.h:15`  
`/healMindWoundSelf1` — heal Mind Wound Self 1. `HealMindWoundSelf1Command.h:17`  
`/healMindWoundSelf2` — heal Mind Wound Self 2. `HealMindWoundSelf2Command.h:17`  
`/healStatesOther` — heal States Other. `HealStatesOtherCommand.h:17`  
`/healStatesSelf` — heal States Self. `HealStatesSelfCommand.h:17`  
`/regainConsciousness` — regain Consciousness. `RegainConsciousnessCommand.h:24`  
`/stopBleeding` — stop Bleeding. `StopBleedingCommand.h:17`  
`/totalHealOther` — total Heal Other. `TotalHealOtherCommand.h:13`  
`/totalHealSelf` — total Heal Self. `TotalHealSelfCommand.h:13`  

## Group, Guild, Duel & PvP (25)
`/assist` — assist. `AssistCommand.h:15`  
`/consent` — consent. `ConsentCommand.h:34`  
`/declareOvert` — declare Overt. `DeclareOvertCommand.h:16`  
`/decline` — decline. `DeclineCommand.h:21`  
`/delegateFactionPoints` — delegate Faction Points. `DelegateFactionPointsCommand.h:71`  
`/denyService` — deny Service. `DenyServiceCommand.h:16`  
`/disband` — disband. `DisbandCommand.h:19`  
`/dismissGroupMember` — dismiss Group Member. `DismissGroupMemberCommand.h:21`  
`/dragIncapacitatedPlayer` — drag Incapacitated Player. `DragIncapacitatedPlayerCommand.h:168`  
`/duel` — duel. `DuelCommand.h:16`  
`/endDuel` — end Duel. `EndDuelCommand.h:15`  
`/gcwStatus` — gcw Status. `GcwStatusCommand.h:19`  
`/guildremove` — guildremove. `GuildremoveCommand.h:20`  
`/guildstatus` — guildstatus. `GuildstatusCommand.h:20`  
`/haveconsent` — haveconsent. `HaveconsentCommand.h:19`  
`/invite` — invite. `InviteCommand.h:20`  
`/join` — join. `JoinCommand.h:15`  
`/leaveGroup` — leave Group. `LeaveGroupCommand.h:19`  
`/makeLeader` — make Leader. `MakeLeaderCommand.h:20`  
`/serverSysGroup` — server Sys Group. `ServerSysGroupCommand.h:16`  
`/showCouncilRank` — show Council Rank. `ShowCouncilRankCommand.h:19`  
`/showPvPRating` — show Pv P Rating. `ShowPvPRatingCommand.h:16`  
`/toggleDisplayingFactionRank` — toggle Displaying Faction Rank. `ToggleDisplayingFactionRankCommand.h:18`  
`/unconsent` — unconsent. `UnconsentCommand.h:37`  
`/uninvite` — uninvite. `UninviteCommand.h:22`  

## Squad Leader (10)
`/boostmorale` — boostmorale. `BoostmoraleCommand.h:19`  
`/formup` — formup. `FormupCommand.h:17`  
`/rally` — rally. `RallyCommand.h:18`  
`/retreat` — retreat. `RetreatCommand.h:52`  
`/setBoostmorale` — set Boostmorale. `SetBoostmoraleCommand.h:17`  
`/setFormup` — set Formup. `SetFormupCommand.h:17`  
`/setRetreat` — set Retreat. `SetRetreatCommand.h:17`  
`/setSteadyaim` — set Steadyaim. `SetSteadyaimCommand.h:17`  
`/steadyaim` — steadyaim. `SteadyaimCommand.h:17`  
`/volleyFire` — volley Fire. `VolleyFireCommand.h:18`  

## Travel & Movement (13)
`/boardShuttle` — board Shuttle. `BoardShuttleCommand.h:36`  
`/dismount` — dismount. `DismountCommand.h:30`  
`/dismountandstore` — dismountandstore. `DismountandstoreCommand.h:16`  
`/flashSpeeder` — flash Speeder. `FlashSpeederCommand.h:21`  
`/gallop` — gallop. `GallopCommand.h:20`  
`/gallopStop` — gallop Stop. `GallopStopCommand.h:18`  
`/kneel` — kneel. `KneelCommand.h:19`  
`/mount` — mount. `MountCommand.h:27`  
`/newbieSelectStartingLocation` — newbie Select Starting Location. `NewbieSelectStartingLocationCommand.h:17`  
`/prone` — prone. `ProneCommand.h:19`  
`/purchaseTicket` — purchase Ticket. `PurchaseTicketCommand.h:20`  
`/stand` — stand. `StandCommand.h:17`  
`/takeCover` — take Cover. `TakeCoverCommand.h:18`  

## Housing & Structures (16)
`/declareresidence` — declareresidence. `DeclareresidenceCommand.h:21`  
`/defuseMinefield` — defuse Minefield. `DefuseMinefieldCommand.h:15`  
`/destroystructure` — destroystructure. `DestroystructureCommand.h:23`  
`/installMissionTerminal` — install Mission Terminal. `InstallMissionTerminalCommand.h:19`  
`/moveFurniture` — move Furniture. `MoveFurnitureCommand.h:25`  
`/nameStructure` — name Structure. `NameStructureCommand.h:18`  
`/paymaintenance` — paymaintenance. `PaymaintenanceCommand.h:18`  
`/permissionListModify` — permission List Modify. `PermissionListModifyCommand.h:22`  
`/placeStructure` — place Structure. `PlaceStructureCommand.h:22`  
`/placeStructureMode` — place Structure Mode. `PlaceStructureModeCommand.h:19`  
`/rotateFurniture` — rotate Furniture. `RotateFurnitureCommand.h:18`  
`/serverDestroyObject` — server Destroy Object. `ServerDestroyObjectCommand.h:20`  
`/setpermission` — setpermission. `SetpermissionCommand.h:15`  
`/setprivacy` — setprivacy. `SetprivacyCommand.h:18`  
`/structurestatus` — structurestatus. `StructurestatusCommand.h:21`  
`/transferstructure` — transferstructure. `TransferstructureCommand.h:23`  

## Harvesters (5)
`/harvesterActivate` — harvester Activate. `HarvesterActivateCommand.h:15`  
`/harvesterDeactivate` — harvester Deactivate. `HarvesterDeactivateCommand.h:15`  
`/harvesterDiscardHopper` — harvester Discard Hopper. `HarvesterDiscardHopperCommand.h:18`  
`/harvesterGetResourceData` — harvester Get Resource Data. `HarvesterGetResourceDataCommand.h:19`  
`/harvesterSelectResource` — harvester Select Resource. `HarvesterSelectResourceCommand.h:18`  

## City & Politics (4)
`/cityban` — cityban. `CitybanCommand.h:18`  
`/citypardon` — citypardon. `CitypardonCommand.h:18`  
`/grantZoningRights` — grant Zoning Rights. `GrantZoningRightsCommand.h:18`  
`/registerWithLocation` — register With Location. `RegisterWithLocationCommand.h:18`  

## Waypoints, Friends & Misc (43)
`/addFriend` — add Friend. `AddFriendCommand.h:21`  
`/addIgnore` — add Ignore. `AddIgnoreCommand.h:20`  
`/bet` — bet. `BetCommand.h:19`  
`/claimVeteranReward` — claim Veteran Reward. `ClaimVeteranRewardCommand.h:13`  
`/combatModeCheck` — combat Mode Check. `CombatModeCheckCommand.h:18`  
`/find` — find. `FindCommand.h:15`  
`/findFriend` — find Friend. `FindFriendCommand.h:23`  
`/forceMeditate` — force Meditate. `ForceMeditateCommand.h:18`  
`/forceOfWill` — force Of Will. `ForceOfWillCommand.h:41`  
`/getVeteranRewardTime` — get Veteran Reward Time. `GetVeteranRewardTimeCommand.h:13`  
`/insertItemIntoShipComponentSlot` — insert Item Into Ship Component Slot. `InsertItemIntoShipComponentSlotCommand.h:18`  
`/installShipComponent` — install Ship Component. `InstallShipComponentCommand.h:16`  
`/intimidate1` — intimidate 1. `Intimidate1Command.h:18`  
`/intimidate2` — intimidate 2. `Intimidate2Command.h:18`  
`/newbiehelper` — newbiehelper. `NewbiehelperCommand.h:18`  
`/npcConversationSelect` — npc Conversation Select. `NpcConversationSelectCommand.h:16`  
`/npcConversationStart` — npc Conversation Start. `NpcConversationStartCommand.h:16`  
`/npcConversationStop` — npc Conversation Stop. `NpcConversationStopCommand.h:15`  
`/removeFriend` — remove Friend. `RemoveFriendCommand.h:16`  
`/removeIgnore` — remove Ignore. `RemoveIgnoreCommand.h:16`  
`/requestBadges` — request Badges. `RequestBadgesCommand.h:18`  
`/requestBiography` — request Biography. `RequestBiographyCommand.h:19`  
`/requestCharacterMatch` — request Character Match. `RequestCharacterMatchCommand.h:18`  
`/requestCharacterSheetInfo` — request Character Sheet Info. `RequestCharacterSheetInfoCommand.h:18`  
`/requestWaypointAtPosition` — request Waypoint At Position. `RequestWaypointAtPositionCommand.h:16`  
`/setBiography` — set Biography. `SetBiographyCommand.h:13`  
`/setBiographyAdmin` — set Biography Admin. `SetBiographyAdminCommand.h:13`  
`/setGodMode` — set God Mode. `SetGodModeCommand.h:18`  
`/setIntimidate` — set Intimidate. `SetIntimidateCommand.h:16`  
`/setName` — set Name. `SetNameCommand.h:15`  
`/setSpokenLanguage` — set Spoken Language. `SetSpokenLanguageCommand.h:16`  
`/setWarcry` — set Warcry. `SetWarcryCommand.h:16`  
`/setWaypointActiveStatus` — set Waypoint Active Status. `SetWaypointActiveStatusCommand.h:18`  
`/setWaypointName` — set Waypoint Name. `SetWaypointNameCommand.h:18`  
`/sitServer` — sit Server. `SitServerCommand.h:19`  
`/synchronizedUiListen` — synchronized Ui Listen. `SynchronizedUiListenCommand.h:16`  
`/synchronizedUiStopListening` — synchronized Ui Stop Listening. `SynchronizedUiStopListeningCommand.h:18`  
`/tumbleToKneeling` — tumble To Kneeling. `TumbleToKneelingCommand.h:18`  
`/tumbleToProne` — tumble To Prone. `TumbleToProneCommand.h:16`  
`/tumbleToStanding` — tumble To Standing. `TumbleToStandingCommand.h:16`  
`/uninstallShipComponent` — uninstall Ship Component. `UninstallShipComponentCommand.h:16`  
`/warcry1` — warcry 1. `Warcry1Command.h:18`  
`/warcry2` — warcry 2. `Warcry2Command.h:18`  

## Deep-Dives

### Combat & Group

`/assist <player>`  
Sets your target to your target's target and immediately queues an attack on it. Target must be a creature, not yourself, and must itself have a valid target. Example: `/assist` while targeting the tank opens up on whatever the tank is fighting. `AssistCommand.h:39`

`/duel <player>`  
Sends a duel request to a targeted player; both must consent via this mutual challenge system. Fails in no-duel areas (e.g. starter cities), beyond 25m, if already dueling, or if either player ignores the other. Example: `/duel` with a rival targeted starts the challenge countdown once accepted. `DuelCommand.h:46`

`/endDuel [player]`  
Ends an active duel, cancels your own open challenge, or rejects an incoming challenge from the target. With no valid target it clears your entire duel list at once. `EndDuelCommand.h:52`

`/steadyaim`  
Squad leader only (must be group leader). Costs HAM scaled by group size minus the steadyaim skill mod; grants every group member with a ranged weapon +5+skillMod private aim for 300 seconds. Example: `/steadyaim` before a long boss fight. `SteadyaimCommand.h:55`

`/volleyFire <target>`  
Group leader command: leader attacks the target with a temporary accuracy bonus (2x volley skill mod), then all combat-ready members within 128m auto-execute the same attack on that target. Costs 100 HAM adjusted by group modifier. `VolleyFireCommand.h:45`

`/rally`  
Group leader squad-leader ability, 25% chance to fail. On success all members get a 30s buff: +50 to their weapon's accuracy modifier and +30 ranged/melee group defense, plus the rallied state. Leader pays ~100 HAM per use. `RallyCommand.h:44`

`/formup`  
Group leader command that cures Dizzy and Stunned states on every valid group member. Costs 50 base HAM scaled by group size. Optional shout message fires on a 30s shared cooldown. `FormupCommand.h:43`

`/retreat`  
Group leader gives the whole group a burst-run effect. Blocked when mounted, inside dungeon1, or if any member already has burst run / force run buffs. Cost scales down with the leader's group_burst_run mod. `RetreatCommand.h:78`

`/boostmorale`  
Group leader wipes every valid member's wounds across all 9 attributes and redistributes them evenly among the group — net wound reduction for the group as a whole. Does nothing if no member has wounds. Costs 100 HAM. `BoostmoraleCommand.h:56`

`/rescue <groupmember>`  
Medic taunt-swap: pulls aggro off the targeted groupmate onto yourself by applying a TAUNTED threat state for 5 seconds. Success chance is random(50) + rescue skill mod vs 30 (capped mod 100). Requires you grouped and the target in combat. `RescueCommand.h:58`

`/invite <player>`  
Invites target (or named player when galaxy-wide grouping is on) to a group; also works on ships, inviting the ship owner. Silently fails if the invitee ignores you unless you are staff. `InviteCommand.h:41`

`/join`  
Accepts a pending group invitation. Fails if already grouped; join runs deferred through the task manager. `JoinCommand.h:22`

`/leaveGroup`  
Leaves your current group. No arguments; errors if not grouped. `LeaveGroupCommand.h:29`

`/disband`  
Leader disbands the whole group; any non-leader using it just leaves instead — no confirmation prompt. `DisbandCommand.h:36`

`/makeLeader <player>`  
Transfers group leadership to the targeted member (leader-only). With galaxy-wide grouping enabled accepts a first name argument instead of a target. Runs asynchronously via task. `MakeLeaderCommand.h:70`

`/dismissGroupMember <player>`  
Kicks the targeted player or pet from the group. Galaxy-wide grouping allows kicking by name without targeting. `DismissGroupMemberCommand.h:75`

`/groupLoot`  
Group-only: leaders get a SUI menu to pick loot rule — Free For All, Master Looter, Lottery, Random. Non-leaders just get told the current rule. `GroupLootCommand.h:46`

`/loot [all]`  
Opens the targeted dead corpse's inventory (16m range); appending `all` loots everything you own rights to. Non-owners can still retrieve items left for them due to full inventories; empty owned corpses trigger scheduled destruction. `LootCommand.h:60`

`/harvestCorpse [meat|hide|bone]`  
Scout harvest of a dead creature corpse; specifying a type picks that resource, omitting it picks randomly among what the corpse carries. Requires harvest rights and a non-DNA-depleted corpse. `HarvestCorpseCommand.h:58`

### Death & Incapacitation

`/deathBlow <incapacitated target>`  
Finishes an incapacitated player or pet within 5m and line of sight, killing them outright (clone/respawn). Fails on feigning-death targets and already-dead ones. PvP-flagged context applies. `DeathBlowCommand.h:64`

`/feignDeath`  
Combat-only trick: arms a one-shot buff so your next incoming hit triggers feigned death (you drop out of combat appearing dead). Sets private_defense massively negative until triggered; reusable toggle removes the state if already feigning. `FeignDeathCommand.h:22`

`/revivePlayer <dead player>`  
Resuscitates a recently-dead player within 7m using a Resuscitation Kit from your inventory (200 mind cost, focus-adjusted). Requires grouping or consent; Jedi targets always demand consent. Applies a 60s grogginess debuff (-100 all attributes) to the revived player; awards half-power medical XP. `RevivePlayerCommand.h:36`

`/dragIncapacitatedPlayer <body>`  
Drags an incapacitated or dying player to your position, up to 5m per drag, requiring consent or grouping (medic injury speed 2 skill gates the command). Enforces height difference limits and repositions along the floor with a forced transform update. `DragIncapacitatedPlayerCommand.h:63`

### Healing

`/tendDamage`  
Basic medic heal: restores ~50 health and ~50 action damage within 6m for 200 mind cost and 5 focus/willpower wounds, sharing the standard 20s injury-treatment cooldown via TendCommand base class. Works on self or others. `TendDamageCommand.h:15`

`/quickHeal [target]`  
No-medicine heal for 150-750 health AND action damage (random roll), costing 1000 focus-adjusted mind plus 10 wounds each to focus and willpower, 6m range. Falls back to self-heal when the target is invalid, an unowned AI, or attackable. `QuickHealCommand.h:149`

`/firstAid [target]`  
Cures Bleeding only — heals bleeding DOT by healing_injury_treatment skillmod x3. Zero mind cost, 6m range, requires line of sight to others. `FirstAidCommand.h:136`

`/diagnose <patient>`  
Free inspection tool: opens a listbox showing the target's wound levels on all 9 attributes plus Battle Fatigue, within 6m. Cannot diagnose droids, walkers, or vehicles; enemy-faction targets must be healable by you. `DiagnoseCommand.h:74`

`/healPet [stimpackOID]`  
Heals a tamed creature pet with a pet stimpack found automatically in your inventory (or by object ID), 5m range, flat 50 focus-adjusted mind cost. Creature pets only — droids/faction pets invalid — and triggers the 20s injury treatment cooldown. `HealPetCommand.h:130`

`/sampleDNA <creature>`  
Bio-engineer DNA sampling of a live, non-baby, non-pet, out-of-combat aggressive-to-you creature within 16m and line of sight. Needs outdoors_bio_engineer_novice; corpses rejected. Hands off to the creature manager's timed sample minigame. `SampleDNACommand.h:39`

### Scout & Wilderness

`/areatrack`  
Ranger-only area scan (outdoors_ranger_novice): opens a menu to track animals, NPCs (harvest_02 skill), or players (harvest_04). Outdoor cells only; has its own cooldown and won't stack with an open track window. `AreatrackCommand.h:29`

`/maskScent`  
Toggles scent concealment from creatures: duration = 12 x (mask_scent mod / 2) seconds, doubling as its own cooldown. Blocked while concealed, already masked, in combat, or on cooldown with a wait-time message. Attacking breaks it. `MaskscentCommand.h:50`

`/conceal [player]`  
Applies visual camouflage from a zone-correct Camo Kit in inventory (10m range to others): duration = 40 x (camouflage mod / 2), granting private_conceal. Refused if hostiles are within 32m, target is scent-masked, or on mask-scent cooldown. Self-target with existing buff removes it. `ConcealCommand.h:180`

`/forage`  
Starts scout foraging at your location via the forage manager — rolls for scouts' items/resources where permitted. `ForageCommand.h:31`

`/fish`  
Casts your line if not already fishing; the fishing minigame then drives subsequent actions. `FishCommand.h:35`

`/burstRun`  
Temporary movement-speed burst handled by PlayerManager::doBurstRun. Explicitly tells mounted riders burst run is unavailable rather than failing silently. `BurstRunCommand.h:19`

### Pets & Creatures

`/tame <baby creature>`  
Attempts to tame a baby creature within 8m via the creature manager. The `adult` argument works only for characters with the admin ability. `TameCommand.h:44`

`/tellpet <message>`  
Issues a voice/chat command to ALL your active pets within 128m — the text is parsed by PetManager against trained commands (follow, stay, attack, etc.). `TellpetCommand.h:33`

`/mount <vehicle|pet>`  
Mounts your own vehicle or pet within 7m with line of sight; refuses disabled/incapacitated/dead mounts and anyone else's. Using it while already riding issues a dismount instead. Sets a 2s mount/dismount cooldown. `MountCommand.h:30`

`/dismount`  
Gets off your mount: strips the riding state, removes mounted-combat slow, reapplies suppressed speed buffs (gallop, burst run, retreat, force run), and corrects client height desync. Shares the 2s mount_dismount cooldown. `DismountCommand.h:39`

### Travel

`/boardShuttle [ticketOID]`  
Boards the nearest shuttle (128m travel point lookup, 25m board range): consumes a matching ticket from inventory, or pops a ticket-selection window if several match. No mounting, no pets, shuttle departure timing enforced (Theed Spaceport exempt). `BoardShuttleCommand.h:73`

`/purchaseTicket <departurePlanet> <departurePoint> <arrivalPlanet> <arrivalPoint> [single]`  
Buys travel tickets from a travel terminal within 8m — round trip by default, `single`/`0` buys one-way. Charges planet fare plus city travel tax, checks city bans at BOTH ends, validates route existence, requires inventory space, and supports new-player travel coupons for one-way fares. Bank-first payment with cash fallback. Example: `/purchaseTicket corellia coronet naboo_theed single`. `PurchaseTicketCommand.h:37`

### Housing & Structures

`/placeStructuremode` / `/placeStructure <deedID> <x> <y> <angle>`  
Places a structure deed: angle 0-3 only, placement must be within 100m and 10m height of you, outside buildings, off mounts, outside client city regions and no-build zones. Logs and rejects out-of-range placement attempts server-side. `PlaceStructureCommand.h:54`

`/declareResidence`  
Declares the building you're standing in as your residence. Rejected inside GCW bases; accepts normal structures and theaters. `DeclareresidenceCommand.h:28`

`/setPermission <list> <player>`  
Shorthand toggling a player on a structure permission list (ENTRY, ADMIN, VENDOR, etc.) — must be near/in a structure you have admin on; civic structures and GCW bases refused. Has its own command cooldown. Delegates internally to PermissionListModify. `SetpermissionCommand.h:53`

`/paymaintenance [amount]`  
Pays maintenance into the targeted/nearby installation or building (admin rights required): with an integer amount pays directly, without one prompts a transfer box. Cash-only check up front; civic structures and GCW bases excluded. `PaymaintenanceCommand.h:60`

`/structurestatus`  
Reports full status of a nearby/targeted structure you administer — maintenance pool, decay, permission lists — and POB ships have a parallel report path. Staff can append `export <reason>` to dump the structure to JSON. `StructurestatusCommand.h:55`

`/nameStructure <name>`  
Owner-only rename of a structure (not GCW bases/turrets/minefields): name passes the profanity filter and 128-char cap, updates the sign object on buildings. `NameStructureCommand.h:62`

`/transferStructure <newOwner>`  
Hands structure ownership to a targeted player: owner-only, must be inside buildings being transferred, blocks GCW bases/turrets/minefields, banned recipients, and any no-trade items left inside. `TransferstructureCommand.h:47`

`/destroyStructure`  
Initiates structured demolition of a targeted structure you administer — confirmation flow handled by StructureManager with redeed/refund rules. `DestroystructureCommand.h:1`

`/setPrivacy`  
Admin-only toggle of a building between public and private. Always-public templates, faction HQs, and buildings containing vendors cannot go private; broadcasts cell permission changes on success. `SetprivacyCommand.h:36`

`/moveFurniture <FORWARD/BACK/UP/DOWN> <distance>`  
Moves a targeted item 1-500 units inside your building or POB ship (admin required): collision-checked per cell so items can't end up in walls; vendors must be picked up instead. `MoveFurnitureCommand.h:88`

`/rotateFurniture <LEFT/RIGHT> <degrees>`  
Rotates targeted furniture 1-180 degrees. With EnhancedFurnitureRotate config enabled: yaw/pitch/roll accepted in -180..180 plus `reset 1` to restore original orientation. `RotateFurnitureCommand.h:30`

### Cities & Social

`/cityBan <player>`  
Militia members ban a non-citizen player from the current city (both must be in it), stripping specialization modifiers and blocking city services. CSR-protected players refuse bans and notify the target. `CitybanCommand.h:48`

`/cityPardon <player>`  
Militia counterpart to cityBan: lifts a ban and reapplies city specialization modifiers. `CitypardonCommand.h:43`

`/grantZoningRights <player>`  
Mayor/militia grant a non-mayor player 24-hour placement zoning rights in the city; using it again on the same player revokes them. `GrantZoningRightsCommand.h:39`

`/consent <name>[,name2...]`  
Grants healing/loot/resurrection consent: accepts comma-separated names or a targeted player, capped at 20 entries, self-consent blocked, names validated live. Consented players may revive and drag you. `ConsentCommand.h:49`

`/unconsent <name>[,...]`  
Revokes consent by names or target; tolerant loop continues past unknown names instead of aborting. `UnconsentCommand.h:65`

`/addFriend <name>`  
Adds a player to your friends list (max 1000): rejects ignored players, duplicates, nonexistent names, and online god-mode staff. `AddFriendCommand.h:32`

`/tip <player> <amount>|bank <amount>`  
Cash tips hand over instantly within 190m (no fee, ignore-list respected). `bank` wires credits account-to-account with a 5% surcharge behind a confirmation box; same-account transfers can be made free via config. `TipCommand.h:23`

`/bet <amount> <slot>`  
Places a roulette bet at a gambling table: amount then bet label matched against the roulette table's slots; only works while actively playing and betting is open. `BetCommand.h:39`

`/imageDesign [player]`  
Opens an image-design session with a targeted player — designer needs social_entertainer_novice and must be in the SAME GROUP as the subject. Dead or invisible targets refused; one outstanding offer at a time. `ImagedesignCommand.h:30`

`/holoEmote <name|help|delete>`  
Fires your installed holo-emote generator animation (from an image-design purchase): consumes a charge, 30s cooldown, `help` lists emotes, `delete` uninstalls. `kitty` aliases technokitty; `holoemote_all` generators play anything. `HoloEmoteCommand.h:56`

`/guildremove [player]`  
With kick permission removes another member (confirmation prompt); without arguments or target it just leaves the guild yourself. `GuildremoveCommand.h:69`

`/giveItem <item> <containerOID>`  
Server-side item move used by the client's give-item flow: moves an item you can remove into a target container, with special handling to load attachments onto wearables, stims into droid modules, and vendor stock. Not normally typed by hand. `GiveItemCommand.h:42`

### Entertainer

`/startDance [danceName]`  
Begins dancing: no argument lists dances you know, an unknown or unskilled dance name is rejected. Refuses while already performing. `StartDanceCommand.h:61`

`/startMusic [songName]`  
Starts playing an equipped instrument; no argument lists songs. In a group, automatically matches the band's current song for your instrument type instead of starting your own selection. `StartMusicCommand.h:55`

`/flourish <1-8>`  
Performs flourish N mid-performance; each flourish must be a learned ability (flourish+N). Command duration scales down with healing_music/dance_ability — 5s baseline to 2s at master. `FlourishCommand.h:37`

### Trader & Inventory

`/survey <resource>`  
Runs a survey sweep for the named resource with your open survey tool (session required — no-tool users get an error). Blocked while an active sampling task runs; the client supplies the resource name argument. `RequestSurveyCommand.h:26`

`/sample`  
Client-side handshake for repetitive sampling; the real sampling loop lives in SampleTask sessions started from the survey tool. Explicitly cancels with a message if you're under attack. `SampleCommand.h:16`

`/applyPowerup <weapon>`  
Slaps a targeted Powerup onto an owned weapon in your inventory/hands: type-matched (ranged pup on ranged gun, etc.), one powerup per weapon, Jedi weapons excluded. Consumes the powerup. `ApplyPowerupCommand.h:65`

`/factoryCrateSplit <count>`  
Splits N items out of a targeted factory crate you own into a new crate — parent container must have space and admin rights apply inside structures. `FactoryCrateSplitCommand.h:26`

`/resourceContainerSplit <count>`  
Splits a resource crate into two containers of your choosing: newStackSize >= 1, crate must be in inventory (not on the ground), destination inventory not full. `ResourceContainerSplitCommand.h:26`

### Waypoints & Finding

`/find [category|clear]`  
Opens the planetary find menu, or jumps straight to a map category/subcategory (underscore or colon separated, e.g. `cloning_facility`, `trainer:marksman`). `find clear` removes the special find waypoint. `FindCommand.h:35`

`/requestWaypointAtPosition <planet> <x> <z> <y> <name>`  
Creates an active waypoint at coordinates clamped to +/-8192 with a free-form final-token name. Example: `/requestWaypointAtPosition tatooine -1200 12 -3500 My Spot`. `RequestWaypointAtPositionCommand.h:24`

`/setWaypointName <name>`  
Renames a targeted waypoint (yours only, max 150 chars); renaming a special-type waypoint clears its special designation. `SetWaypointNameCommand.h:40`

### Terminals & Vendors

`/insure`  
Opens the insurance terminal menu (within 15m of an insurance terminal) listing insurable equipped/inventory items with Insure All option. `InsureCommand.h:51`

`/createVendor`  
Starts the vendor-creation session inside a public building where you hold ADMIN or VENDOR permissions; one creation session at a time. `CreateVendorCommand.h:35`

## Method
Derived from `CommandConfigManager.cpp` command registrations cross-checked against classes in `src/server/zone/objects/creature/commands/`; deep-dive entries above were written directly from those header sources with verified file:line cites. Totals: 283 inventory entries across all groups, 66 argumented/notable commands covered in Deep-Dives.
