# Cyvster2 Modification Inventory & Migration Ledger

> BRIEF-037 deliverable. Permanent tracking list: every NET modification on
> `remotes/origin/cyvster2` (vs fork base `7ff869ccd2`) and its migration
> status into this mod. Update status as items port.
> Method + scope: [briefs/037-cyvster2-modification-inventory.md](../briefs/037-cyvster2-modification-inventory.md)
> Analysis date: 08252026 -- ox-alpha (opencode/x-preview-f-free).
>
> Status legend: IN-MOD = already a mod feature | TODO = port candidate |
> WONT-PORT = obsolete/upstream-caught-up/server-specific tuning.
> Complexity: TRIVIAL = config-like value change | MODERATE = needs hook |
> HARD = deep engine change.
> Quality (missions entries): OK / SMELLY / BAD -- implementation-quality
> assessment of the cyvster2 code itself. Per owner direction, cyvster2 code
> is treated as a SPECIFICATION ONLY, never a quality reference.

## Summary Stats

Total post-fork commits analyzed: 610 (607 Cyvster, 3 Marc Joyce --
mission-direction fix series). Net diff: 269 files, ~3783 insertions /
~2255 deletions. Undo/revert churn: ~11 undo/attempt commits identified;
all recorded here as NET changes only (churn counted, not itemized).
Estimated net distinct modifications: **~48** across categories:
FEATURE 12 | TWEAK 28 | FIX 3 | DEAD (net-zero chains, counted only) 5.
Status snapshot at writing: IN-MOD 4 | TODO 30 | WONT-PORT 14
(server-tuning multipliers dominate the WONT-PORT set; see notes per item).

## Highest-Value Ports (seeds BRIEF-038)

M01 Mission level choice | FEATURE | mission | Player picks destroy-mission
difficulty tier from terminal radial menu ("Choose Mission Level", radial 112);
choice persists via ScreenPlayData `mission_level_choice/levelChoice`; when set,
overrides group-level difficulty calc (`diffDisplay += levelChoice` at
MissionManagerImplementation.cpp:807-817) and overrides playerLevel in
`getRandomLairSpawn` (line ~1896-1905). Tiers: Reset/Easiest(1)/Mid1(2)/Mid2(12)/
Mid3(25)/High1(35)/High2(45)/High3(60)/Hard(135)/Hardest(200) --
bin/scripts/screenplays/tools/mission_level_choice.lua (new file, 84 lines).
Complexity MODERATE. Quality SMELLY: Lua SUI flow itself is fine and cheap, but
the C++ side calls DirectorManager Lua instance synchronously inside radial-menu
select on the zone executor (MissionTerminalImplementation.cpp:87-107); the
levelChoice read uses `Integer::valueOf("")` with no empty-string guard --
first use before any choice crashes/parses to 0 silently. Status IN-MOD (BRIEF-043, 08252026).
M02 Mission direction choice | FEATURE | mission | Player picks compass
direction (N/NE/E/SE/S/SW/W/NW or Reset) from terminal radial 113; persisted as
`mission_direction_choice/directionChoice`; destroy-mission start position is
placed within ±5 degrees of chosen heading instead of random 360
(MissionManagerImplementation.cpp:768-777 new `getMissionPosition()`, and
857-883 deviation logic replacing `player->getWorldCoordinate(distance,
random(360))`). bin/scripts/screenplays/tools/mission_direction_choice.lua
(new file, 94 lines). Complexity MODERATE. Quality SMELLY: same synchronous
Lua-call-on-radial-select pattern as M01; direction math was fixed twice by
collaborator commits (608116821c, db6b0b6c56 -- angle convention off-by-90°
initially), evidence the hand-rolled trig was fragile; ±5° deviation uses two
System::random calls per mission gen (trivial cost). The STUTTER the owner
remembers is most plausibly NOT these two menus (they are one cheap Lua call +
one SUI build) but rather what happens right after: choosing a direction/level
then opening the terminal triggers full mission-bag regeneration (24+ missions
generated per request, see M05) including lair-spawn scans and zone boundary
checks per mission -- synchronous, on the zone executor, per terminal open.
BRIEF-038 should treat regeneration cost, not menu construction, as the
performance suspect. Status IN-MOD (BRIEF-043, 08252026).
M03 Mission bag expansion + custom titles | FEATURE | mission | Mission bag
raised 12->24 general (MissionManagerImplementation.cpp:206), factional recon
bonus 6->24 (214), destroy/deliver split points moved 6/12 -> 24/30 general
and 6/12/36 faction (588-, 734-blocks); destroy missions get CUSTOM readable
titles: `"CL<displayLevel>  Destroy the <first-mobile-name> camp/lair."`
built from first entry of lair mobiles map, replacing StringId table titles
(~950-970 block). Complexity TRIVIAL-MODERATE. Quality SMELLY: title built per
mission per terminal open with `replaceAll("_", " ")` string alloc; fine, but
the 24-slot bag doubles generation work per open. Status TODO (pairs with UPDATE 08252026: descriptive-titles half now IN-MOD via BRIEF-043; bag expansion still deferred.
M01/M02 as the terminal UX package).
M04 Bounty reward x100 + city bonus stacking | TWEAK | mission |
`mission->setRewardCredits(reward * cityBonus * 100)` hard-coded in
randomizeBountyTerminalMissions (~line 709-713); commit history shows 10x
first then raised to 100x (8a0487becc). Complexity TRIVIAL. Quality BAD:
hard-coded magic multiplier inline; belongs in config. Status TODO as a
config option (value itself server-specific).
M05 Destroy mission distance reduction | TWEAK | mission |
bin/scripts/managers/mission/mission_manager.lua: baseDistance 1000->800,
randomDistance 1000->300 (lines ~83-86). Complexity TRIVIAL (pure lua config,
already upstream-configurable keys). Status IN-MOD-ADJACENT (config exists
upstream; mod need not touch) -> WONT-PORT.
M06 Same-account bounty missions enabled | TWEAK | mission |
mission_manager.lua: `enable_same_account_bounty_missions = "true"` (line 73).
Complexity TRIVIAL (existing upstream flag). Status WONT-PORT (config-only).

## Missions Area -- Other Entries

M07 Mission reward shared to whole group anywhere | TWEAK/FIX | mission |
MissionObjectiveImplementation.cpp:273 proximity check 128m -> `>= 0`
(everyone qualifies) and divisor forced to 1 so reward is not divided
(~line 309-315). Complexity TRIVIAL. Quality OK (simple, deliberate).
Status WONT-PORT as-is (game-balance choice) but note as pattern for any
group-reward mod option.
M08 Mission objective completion distance check changed | FIX | mission |
commit fe3874225c "Change distance check for mission objective completion"
-- net effect superseded by M07's >=0 check; net diff shows only M07 lines.
Counted within M07; no separate net change remains.

## Movement & Speed (mostly DEAD churn area)

S01 Pet run speed x5 | TWEAK | creature/PetManagerImplementation.cpp:
`runSpeed *= 5.0f` (net after three failed mount-speed attempts undone);
plus PetDeedImplementation.cpp minor stat tweak. Commit trail: eb0c287cfd ->
3d190595a9 undo -> 02dd3f6efb -> a19a1d5c5c attempts. Complexity TRIVIAL.
Quality BAD (hard-coded). Movement speed IS an existing mod feature
(CustomSkillsMovement H13A/H13B covers players; pets/mounts not covered).
Status TODO (extend movement service to pets/mounts via multiplier).
S02 Creature run speed doubling | DEAD->partially-live | creature | Commits
acc a665f68/db c541e6e double default run speed, bd61166d30/005f/a28f/158b
undo chains -- net zero in most files except CreatureObject.idl single-line
tweak. Complexity n/a. Counted DEAD (net-zero chain), 1 residual line folded
into movement TODO above.
S03 Vehicle/mount speed x2 then decay nerf | TWEAK | VehicleControlDevice/
VehicleDecayTask.h: decayRate forced to 1 (vehicles barely decay);
speederbike_swoop_deed.lua stat tweaks. Complexity TRIVIAL. Quality BAD.
Status TODO (fold into movement/travel options) -- low priority.
S04 ForceRun2/3 speed tweaks | TWEAK | commands ForceRun2Command.h/
ForceRun3Command.h single-line each. Complexity TRIVIAL. Status WONT-PORT
(balance tuning; mod movement multiplier already scales buffs transitively).
S05 Board shuttle range 25m -> 150m | TWEAK | BoardShuttleCommand.h.
Complexity TRIVIAL. Quality OK. Status TODO (nice QoL, one constant --
could be a mod travel QoL or upstream config suggestion).

## Economy & Loot

E01 Loot credits x5 | TWEAK | LootManagerImplementation.cpp:304-310
`credits *= 5`. Complexity TRIVIAL. Status WONT-PORT (server balance value;
mod gathering/economy philosophy differs).
E02 All mobs drop credits directly to top-damage player | FEATURE |
CreatureManagerImplementation.cpp:684-block: NPC-credit-drop gate widened
(event mobs still excluded) AND credits awarded straight to highest-damage
player instead of corpse. Complexity MODERATE. Quality SMELLY:
copyThreatMap.getHighestDamagePlayer() called during destruction notify path
-- acceptable spot, but no null-faction/event guard beyond isEventMob; direct
credit award bypasses corpse-loot transaction expectations (trx logged
against corpse credits that never land there). Status TODO (good QoL feature
for the mod's loot/gathering service family).
E03 Legendary/Exceptional suffixes removed | TWEAK | 
LootManagerImplementation.cpp:265-275 suffix naming commented out; plus
ac7d94b00e lowered legendary threshold level 350->80 earlier in history
(net: disabled entirely). Complexity TRIVIAL. Status WONT-PORT (reverted a
feature rather than added one).
E04 Loot skill mods always max | TWEAK | LootManagerImplementation.cpp
setSkillMods(): random-mod chance branch commented out, `randomMods = 6`,
each mod value pinned to 25 (was random roll). Also createLootFromCollection
loot-chance gate bypassed for level > 120 (line ~745). Complexity TRIVIAL.
Quality BAD (hard-coded god-loot). Status WONT-PORT (server power-fantasy
tuning; document as anti-pattern).
E05 AA/CA attachment auto-naming | FEATURE | 
LootManagerImplementation.cpp createLootObject(): newly created attachments
get custom name `[AA]/<[CA]> <statname>: <value>` from highest skill mod
(lines ~446-476, credit comment cites SWGEmu discord/SWGFlurry/SWGInfinity).
Complexity TRIVIAL-MODERATE. Quality SMELLY: setObjectName called inside the
scan loop every time a higher value found (repeated StringId lookups) instead
of once after loop; harmless frequency (loot creation is rare) but sloppy.
Status TODO (genuinely nice UX feature, small port).
E06 Bazaar/vendor limits removed | TWEAK | AuctionManager.idl:
MAXBAZAARPRICE 20000->99999999, MAXSALES 25->99999999, vendor/commodity
expire periods -> 3600 days. Complexity TRIVIAL. Status TODO (config-style
QoL; consider mod config surface or upstream PR).
E07 Slicing XP x10 + guaranteed dual slices | TWEAK |
SlicingSessionImplementation.cpp: container/weapon/armor slice XP 250->2500,
terminal 100->1000, base 1000->10000; weapon slice applies BOTH damage and
speed slices (switch pinned to 0, both handlers called); armor slice applies
effectiveness AND encumbrance; armor min values retuned. Complexity TRIVIAL.
Quality SMELLY (pinned switch + duplicated handler call rather than a real
"dual slice" concept). Status WONT-PORT values / TODO concept (dual-slice
option could be a proper mod crafting/buffs feature).

## Progression & XP

P01 Global XP x5 | TWEAK | bin/scripts/managers/player_manager.lua
globalExpMultiplier = 5. Complexity TRIVIAL (upstream config key).
Status IN-MOD (mod EXPERIENCE_MULTIPLIER modifier covers personal scaling;
server-wide value stays server config) -> count IN-MOD-ADJACENT/WONT-PORT.
P02 XP caps effectively removed | TWEAK | PlayerObjectImplementation.cpp
addExperience(): xpCap pinned to 10,000,000 ignoring xpTypeCapList;
SkillManager::updateXpLimits capped at same 10M; canLearnSkill skill-point
requirement check disabled; skill point budget 250->5000 (award/surrender
wasted-points checks) and player_creation_manager.lua skillPoints=5000.
Complexity MODERATE. Quality BAD (blanket cap override breaks per-type caps;
commented-out rather than parameterized). Status WONT-PORT as written;
underlying desire (higher caps) noted for progression service design.
P03 Jedi XP changes | TWEAK | PlayerManagerImplementation.cpp:
jedi death XP loss flat -10000 (was -5% of cap); jedi_general share 0.2->0.4;
level-based combat XP cap disabled; NEW: FRS XP drip to Jedi Knights+
(`force_rank_xp += jedi_general * 0.005`) at line ~2155. Complexity MODERATE.
Quality SMELLY (dead commented block left in; flat loss hard-coded).
Status TODO for FRS-xp-drip concept only; rest WONT-PORT.
P04 Healing/craft/ID XP multipliers | TWEAK | Entertainer heal XP x100 for
group members (EntertainingSessionImplementation.cpp); crafting practice XP
x100 (CraftingSessionImplementation.cpp `xp * 100`); image designer XP x10;
healing XP commit 9fa41aa443 10x->100x. Complexity TRIVIAL. Status IN-MOD
(EXPERIENCE_MULTIPLIER / PRACTICE_EXPERIENCE_BONUS modifiers exist) for the
crafting-practice part; rest WONT-PORT server tuning.
P05 Political XP boosted | TWEAK | CityManagerImplementation.cpp: city-update
mayor XP 750->40000, voting XP votes*300->votes*300000. Complexity TRIVIAL.
Status WONT-PORT (values), concept (city play viability) noted.
P06 Veteran rewards at day 1 | TWEAK | player_manager.lua all milestones=1.
Complexity TRIVIAL. Status WONT-PORT.
P07 Skill trainers teach everything incl. elite/master trees | FEATURE |
trainerData.lua +145 lines: artisan trainer list extended with all crafting
elite/master skills; RecruitSkillTrainerCommand.h adds informant options 33-35
with new RecruitSkillTrainerSuiCallback.h handling. Complexity MODERATE.
Quality OK. Status TODO (trainer QoL is a real feature worth a mod home).
P08 FRP rank maintenance/XP costs gutted | TWEAK | frs_manager.lua:
petition/voting intervals 1 day->seconds, baseMaintCost 0, challenge/demotion
costs 20M XP, missed-vote penalty 0, rank member caps 10->100000, robe
requirements unchanged. Plus FrsManagerImplementation.cpp: validatePlayerData()
and deductMaintenanceXp() early-return `return;` stubbed, sendRankPlayerList
disabled with message ("rank list due to size causing issues" -- a live
performance admission), OVERT faction forcing commented out.
Complexity TRIVIAL but sprawling. Quality BAD (early-return stubs, dead code).
Status WONT-PORT (server-specific FRS sandboxing; performance lesson from
sendRankPlayerList noted in R6.9 capture below).

## Crafting & Resources

C01 Instant crafting | TWEAK | CraftingSessionImplementation.cpp:
startCreationTasks(complexity * 0) both one-shot and practice paths.
Complexity TRIVIAL. Status IN-MOD (Crafting Speed modifier H09A governs
duration; instant is an extreme value) -> count IN-MOD.
C02 Factory production instant + harvester/generator deed stats | TWEAK |
FactoryObjectImplementation.cpp timer = complexity*8 -> 1; 23 deed lua files
(harvester/generator) get extraction-rate/hopper tweaks (1-line each).
Complexity TRIVIAL. Status TODO-lite (crafting speed H09B already touches
factory duration; deed stats WONT-PORT data edits).
C03 Experimentation points x5 + genetics retune | TWEAK |
SharedLabratory.cpp results *= pointsAttempted*5; GeneticLabratory.cpp
fortitude armor-reset threshold 500->1001 and fortitude clamp; Genetics.h
DNA-derived damage formula simplified to maxValue*0.25. Complexity MODERATE.
Quality SMELLY (thresholds compared against 1001 sentinel because 1000 was
pinned elsewhere -- magic-number coupling). Status WONT-PORT mostly; DnaManager
all-genetics-pinned-to-1000 (see P09 below) related.
C04 DNA extraction all-stats-1000 | TWEAK | DnaManager.cpp: every genetic
stat derived value replaced with literal 1000. Complexity TRIVIAL. Quality
BAD (deleted the entire derivation system). Status WONT-PORT.
C05 Resource sampling x10 + density/value pinning | TWEAK |
ResourceSpawner.cpp samplingMultiplier 1->10; randomizeValue returns constant
1000 (resource stats pinned high); cf1021/fbb42/5b8ccf/ebb4c248 commit cluster
(getZoneResourceList density experiments) nets into SpawnDensityMap.h +
ResourceContainer.idl single-line tweaks. Complexity TRIVIAL-MODERATE.
Quality BAD (constant return kills variance). Status WONT-PORT values;
sampling multiplier could be a Gathering Quantity sibling (mod has forage +
milk hooks H15A/B; harvest/sampling hooks would be the analog).
C06 Survey tool ranges extended | TWEAK | SurveyToolImplementation.cpp +
SurveyToolSetRangeSuiCallback.h: range cap 384->2624, menu options become
64m x5pts / 576m x6pts etc., survey session + resource manager lua minor.
Complexity TRIVIAL-MODERATE. Quality OK. Status TODO (clean standalone QoL
feature, good mod candidate under gathering service).
C07 Harvester/droid harvest x10 | TWEAK | CreatureManagerImplementation.cpp
harvest()/droidHarvest(): quantityExtracted *= 10. Complexity TRIVIAL.
Status IN-MOD-ADJACENT: mod Gathering Quantity covers forage+milk; creature
harvest is a natural third hook -> TODO (extend gathering service).
C08 Forage quantity x100 | TWEAK | ForageManagerImplementation.cpp:406
`quantity *= 100`. Status IN-MOD (Gathering Quantity modifier, hook H15A
exactly replaces this). Explicit IN-MOD confirmation.

## Combat & Jedi Items

W01 Lightsaber damage modifiers increased | TWEAK | 40+ saber lua files
(gen1-5, one/two-handed/polearm, ranged variants): damage min/max + armor
piercing MEDIUM->HEAVY per-stat blocks; forceLightning* command lua damage
modifier bumps. Data-only. Status WONT-PORT (balance data; also several
upstream rebalances since fork may conflict).
W02 Mandalorian armor socketed + council robes buffed | FEATURE-data |
mandalorian armor lua + ArmorObjectMenuComponent.cpp color-palette loop fix +
NEW LuaTangibleObject::setSocketCount binding (LuaTangibleObject.cpp/.h,
WearableObject.idl/.cpp socket plumbing, 9-line idl change) letting scripts
set socket counts; council robes (robe_jedi_light/dark_s05.lua) given extra
skill mods. Complexity HARD (new engine binding) but upstream now has socket
tooling? unverified. Quality SMELLY (null-check AFTER dereference order bug:
`realObject->isWearableObject() && realObject != nullptr`; repeated comment
spam). Status TODO concept / WONT-PORT data (armor customization quality fix
itself is a decent micro-port).
W03 Crystal/jedi loot item stats | TWEAK | krayt pearl maxDamage, force
crystal HP/damage, geonosian/krayt/rancor/gorax/janta loot item lua stat
blocks (~25 files). Data-only. Status WONT-PORT.
W04 Combat command lua retunes | TWEAK | sprayShot/confusionShot/knockdownFire/
torsoShot/eyeShot/bleedingShot/underHandShot/fastBlast (+force armor/shield/
resist/speed/choke singles): action costs, damage mods, dot chances. Data-only.
Status WONT-PORT (upstream combat has moved far; conflicts certain).
W05 Heal command potency | TWEAK | HealDamage/HealWound/HealEnhanceCommand.h
single-line tweaks + EnhancePack/WoundPackImplementation.cpp potency lines.
Status WONT-PORT.
W06 Taunt/intimidate tweaks | TWEAK | taunt.lua +6 lines, intimidate2.lua.
Status WONT-PORT.
W07 Recruiter conversation changes | TWEAK | recruiterConvoHandler/
recruiterScreenplay lua faction item swaps. Status WONT-PORT.

## World & QoL

Q01 Character creation throttle removed | TWEAK | 
PlayerCreationManager.cpp:462/479: one-char-per-hour window 3600000ms -> 1000ms
(message text now lies). Complexity TRIVIAL. Quality SMELLY (error message
not updated to match behavior). Status WONT-PORT.
Q02 Account-shared structure lots | FEATURE |
PlayerObjectImplementation::getLotsRemaining() rewritten: lot pool =
maximumLots * account character count, decremented by every character's
owned structures (loops account character list + per-character owned
structures, fetching every StructureObject). StructureManager.cpp
placeStructure(): grants ADMIN permission to ALL account characters on
placement. Complexity HARD-ish (cross-account object graph walks). Quality
BAD-for-performance: full recursive structure fetch+lock fan-out per lots
query; getLotsRemaining runs on structure-placement/destroy UI paths --
this is a REAL stutter candidate on structure-heavy accounts and a textbook
unindexed-container-scan smell (flagged per owner request). Status TODO as a
properly cached/account-lot-service feature; do NOT port implementation.
Q03 Naboo building style palette fixes | FIX-data | 15 naboo building lua
files single-line (buildingType/palette corrections). Status WONT-PORT
(upstream likely absorbed; verify before ever caring).
Q04 City system loosened | TWEAK | city_manager.lua: CitizensPerRank {1..1},
update interval weekly->daily-ish 2520min, voting cycles 3->12, maintenance
discount 97.5%, cities allowed on dathomir/endor/yavin4, ALL city
specializations given identical merged mod sets (samplesize/sample rate/
assembly/medical/cloning/experimentation/missions/entertainer/defense 90).
Complexity TRIVIAL (data). Quality SMELLY (identical spec blocks = copy-paste
spec homogenization, defeats specialization choice). Status WONT-PORT data;
note private_spec_missions buff interacts with bounty rewards (M04).
Q05 Planet shuttles retimed + weather off | TWEAK | planet_manager.lua:
shuttleport away 300->10s, landed 120->7200s (landed long, gone short),
starport same; corellia weatherEnabled=0. Complexity TRIVIAL. Status
WONT-PORT data.
Q06 Death Watch Bunker / Geonosian lab / cave script edits | TWEAK-data |
deathWatchBunker.lua (repeated update commits fbf3a558..4b1797d128 -- loot/
door tweaks), geoLab.lua, squill/janta/kunga caves (respawn/loot),
tatooine_jabbas_palace_mobs.lua +1 line. Status WONT-PORT data edits.
Q07 Krayt dragon mobiles buffed | TWEAK-data | 6 tatooine krayt lua +
tatooine_hard_krayt_ne spawn. Status WONT-PORT.
Q08 Escape pod hatch component | FEATURE-space |
screenplays/space/escape_pod_hatch_menu_component.lua (new, 76 lines):
radial eject from ships, owner destroys ship vs passenger ejected.
Registered via screenplays.lua include. Complexity TRIVIAL (self-contained
lua, JTL-era). Quality OK. Status TODO if space content matters to the
server, else WONT-PORT.
Q09 Jedi village/FRS lua pacing | TWEAK | glowing.go_to_dathomir (+21 lines:
skip options), fs_intro/fs_outro/experience_converter/jedi_trials/trialData/
ForceShrineMenuComponent/hologrind/visibility_manager lua tweaks. Mostly
accelerated/unlocked Jedi progression. Status WONT-PORT individually; theme
(Jedi grind reduction) recorded.
Q10 CLOSEOBJECTRANGE 192->512 | TWEAK-HARD | ZoneServer.idl:192. Doubles-plus
close-object interest radius globally. Quality BAD-for-performance: global
awareness radius increase is a classic zone-load amplifier (quadratic-ish
proximity work per player) -- flagged as a likely contributor to general
server sluggishness and plausibly to perceived stutters around busy areas.
Status WONT-PORT (document as anti-pattern).
Q11 Misc single-line engine tweaks | TWEAK | BuildingObject.idl/.cpp,
CreatureObject.idl, TangibleObjectImplementation.cpp, DraftSchematicObjectTemplate.cpp,
TemplateManager.cpp (template load tweak), CombatManager.cpp 1-line,
CraftingManagerImplementation.cpp 1-line, MilkCreatureTask.h (milk QoL),
SampleDnaTask.h, ImageDesignSession, EntertainingSession flourish drain
(/2->/10) + performer buff duration cap 2h->4h with dance/mind + music
focus/willpower buff additions (net feature: full 4-stat entertainer buffs),
ArmorComponentImplementation.cpp +6, AttachmentImplementation.cpp,
GeneticComponentImplementation.cpp, PetControlDevice, WeaponObjectImplementation
+4, EnclaveCouncilRankSuiCallback +2, DestroyStructureRequestSuiCallback,
VisibilityManager.cpp: visibility gain suppressed unless OVERT (FEATURE-ish
PvP rule). Each TRIVIAL individually. Status: entertainer 4-buff extension =
TODO concept (buffs service); VisibilityManager OVERT-gate = WONT-PORT rule
but trivially portable if wanted; remainder WONT-PORT noise-level.

## DEAD Chains (counted only)

D1 Run-speed doubling saga (acc a665f68, dbc541e6e8, bd61166d30,
005f6b14d8, bdc22ae283, a28f147467 + re-attempts) -- net zero outside
residuals folded into S-items. D2 Mount/vehicle speed triple-attempts
(eb0c287cfd, 02dd3f6efb, 3d190595a9) -- superseded by S01 pet approach.
D3 Resource-density experiment chain (cf dcc1021, ebb4c248f6, 5b8ccf411d,
333f2ff28c) -- net collapsed into C05 residuals. D4 Council robe double-commit
(dd535b53ec, cfaf6f921f duplicate). D5 Armor piercing repeated per-file
multi-commits (dozens of same-message commits iterating) -- net captured in
W01 once.

## R6.9 Capture Notes (durable facts)

Upstream has since absorbed or obsoleted some cyvster2-era gaps (survey tool
range SUI existed pre-fork; legendary-item system pre-fork). Two durable
performance lessons for future briefs: (1) FrsManager sendRankPlayerList was
disabled in production due to payload size causing issues -- large SUI lists
built synchronously are a known stutter source on this codebase; (2)
account-shared lots (Q02) and enlarged CLOSEOBJECTRANGE (Q10) show that
per-query object-graph fan-outs and global awareness-radius inflation were
present on cyvster2 and should be presumed contributors to the remembered
"stutter," ahead of the mission-option UIs themselves. BRIEF-038 should
benchmark mission-terminal regeneration cost (bag of 24 synchronous mission
generations per open) before blaming the option menus.
