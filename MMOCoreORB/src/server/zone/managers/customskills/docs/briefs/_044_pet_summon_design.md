# BRIEF-044 Research: Pet Range Summon Feasibility + Design

Status: RESEARCH (no code changes)
Branch: cyvster3
All paths relative to MMOCoreORB/src/server/zone/ unless noted.

=====================================================================
1. WHERE PET FOLLOW LOGIC LIVES + "LEFT BEHIND" DISTANCE
=====================================================================

Pet AI runs a Lua behavior tree per pet, driven by AiBehaviorEvent:

- BT driver: objects/creature/ai/AiAgentImplementation.cpp:3846
  (activateAiBehavior) -> behaviorEvent->schedule(Math::max(10,
  nextBehaviorInterval)) at :3867/:3872.
- Tick intervals: objects/creature/ai/AiAgent.idl:215-217
    BEHAVIORINTERVALMIN = 400 ms   (FOLLOWING / moving pets)
    BEHAVIORINTERVALMID = 700 ms   (WATCHING)
    BEHAVIORINTERVALMAX = 1000 ms  (PATROLLING/RESTING)
  Interval selection at AiAgentImplementation.cpp:2971-2979 — a moving
  FOLLOWING pet ticks on the 400 ms floor, throttled by travel distance.

The pet trees live in bin/scripts/ai/pet.lua:
- movePet (pet.lua:119-134): node id=580291233 CheckOwnerInRange
  condition=128.0 (:130); if owner NOT in 128 m, AlwaysFail ->
  PetReturn (:131-132) which re-acquires follow target.
- awarePet (pet.lua:41-44): same CheckOwnerInRange 128.0 -> PetReturn.
- targetPet (pet.lua:191): CheckTargetInOwnerRange 128.0 drops targets
  out of owner range.

CheckOwnerInRange implementation:
objects/creature/ai/bt/leaf/Checks.cpp:507-523 — casts controlDevice to
PetControlDevice, compares commander range vs checkVar (128.0 from Lua).

PetReturn: bt/leaf/FollowActions.h:717-775 — resets follow object to
linked creature (or last commander/target per stored lastCommand).

FINDING: Vanilla ALREADY self-heals following pets left behind: any pet
in the MOVE/AWARE branch whose owner exceeds 128 m gets PetReturn'd and
runs after them. Pets that genuinely get stranded are ones under PET_STAY,
GUARD, PATROL commands, low-power droids (DroidObjectImplementation.cpp:
193-208 handleLowPower -> setOblivious()), or pets whose movement is
blocked/pathing-failed while owner outruns the 128m recovery window.
Owner-perceived "pets stop following" is most likely pathing failure +
the 128 m check only firing inside the BT tick. Confirm exact repro with
owner before implementing.

Follow-range computation: UpdateRangeToFollow,
bt/leaf/FollowActions.h:247-278 writes blackboard "followRange" each
equip tick (squared 2D distance minus template radii).

=====================================================================
2. EXISTING TELEPORT-TO-OWNER CODE (CALL FLOW)
=====================================================================

Call flow (creature/droid/faction pets all use PetControlDevice):
- CallPetTask.h:20-32 — pending task "call_pet"; aborts if player
  in combat; calls device->spawnObject(player).
- PetControlDeviceImplementation.cpp:396 spawnObject():
  :415 position offset chosen as
      initializePosition(playerX + System::random(5) - 2, playerZ,
                         playerY + System::random(5) - 2);
  i.e. random offset -2..+3 m on X/Y around the player — exactly the
  pattern to reuse (parameterize the spread).
  :449-454 transferObject into player's parent cell if player indoors,
  else zone transfer. NOTE: this means indoor summon already works via
  same-cell transfer; no building-interior special-casing needed beyond
  matching the player's cell.
  :506 setFollowObject(player); :523 activateRecovery(); :531 sets
  lastCommand = FOLLOW.

Mounts use VehicleControlDeviceImplementation.cpp:120 spawnObject():
:148-150 places vehicle AT player world position (no offset),
:131 blocks call while player indoors OR in combat ("cant_call_vehicle").
Creature-trained MOUNTS, however, are PetControlDevice with
trainedAsMount (PetControlDeviceImplementation.cpp:439) — they go
through the pet path above.

There is no existing mid-world "teleport pet to me" routine; SceneObject
teleport(x,z,y,parentID) exists (SceneObject.idl:657) but the cleanest
reuse is: controlledObject->initializePosition(offset) followed by
zone/cell transferObject — identical to spawnObject()'s placement code,
without touching link/faction/bitmask setup.

Store-side safety precedent (mirror these checks):
PetControlDeviceImplementation.cpp:557-566 storeObject() refuses when
pet/player in combat, player dead, or within 1 s petCallOrStoreCooldown.

=====================================================================
3. WHICH CONTROL DEVICE CLASSES COVER WHICH PETS
=====================================================================

- Creature pets, faction pets, helper droids, TRAINED MOUNTS:
  PetControlDevice (objects/intangible/PetControlDevice.idl /
  PetControlDeviceImplementation.cpp). petType branches at :508-520
  (CREATUREPET / DROIDPET / HELPERDROIDPET / FACTIONPET);
  trainedAsMount flag at :439. ONE class covers creature pets + droids +
  creature mounts.
- Vehicles/speeders: VehicleControlDeviceImplementation.cpp (separate
  class; not a following entity — out of scope for "outrun" problem).
- Ships: ShipControlDevice — irrelevant.

RECOMMENDATION: implement against PetControlDevice only. That single
surface covers creature pets, droid pets, AND creature mounts. Exclude
vehicle control devices entirely (they don't follow). Gate by petType
if owner wants droids excluded (e.g. power droids shouldn't teleport).

=====================================================================
4. HOOK POINT OPTIONS ("PET GOT TOO FAR" DETECTION)
=====================================================================

a) Periodic task per active pet (e.g. 5 s Task scheduled from
   CustomSkills module iterating ghost->getActivePets()):
   Cost: one cheap squared-distance compare per pet per tick.
   Active pets per player <= 3 (vanilla pet limit); server population
   small. Negligible cost even at 100 players online (~300 checks/5 s).
   No lock-order risk if implemented like CallPetTask (Locker(player)
   then device + controlledObject clocker).

b) Player POSITIONCHANGED observer: fires on EVERY movement update of
   every player (CreatureObjectImplementation.cpp:3092 shows observer
   dispatch per position change). High-frequency, and observers must be
   attached/detached per pet lifecycle — more plumbing, more failure
   modes than value.

c) Inside the pet AI BT tick (400 ms for following pets): the natural
   place distance is already known (UpdateRangeToFollow /
   CheckOwnerInRange), BUT it means shadowing or extending the Lua BT +
   C++ Behavior leaves — violates the mod's delegation-hook pattern
   ([R6.11]: don't fork engine managers/AI) and couples us to upstream
   AI changes.

RECOMMENDED: option (a) — a mod-owned recurring Task (CustomSkillsPetSummonTask),
scheduled every N seconds (config, default 5000 ms) per online player
with active pets. Simplest, zero engine coupling, trivially killable via
config knob. Distance check uses squared 2D distance (pattern:
FollowActions.h:266) to avoid sqrt. On trigger, reuse spawnObject-style
placement: initializePosition with configurable arrivalOffset ring,
transferObject into player's cell/zone, re-issue setFollowObject(player).

=====================================================================
5. SAFETY RULES
=====================================================================

Mirror vanilla call/store guards; skip summon when ANY of:
- pet isInCombat() or has defenders (storeObject precedent
  PetControlDeviceImpl.cpp:559)
- player isInCombat() / isDead() / isIncapacitated()
  (CallPetTask.h:26-27 precedent)
- pet dead/incapacitated (posture check)
- lastCommand == STAY/GUARD/PATROL (owner explicitly placed the pet;
  only auto-summon pets whose lastCommand == FOLLOW/FOLLOWOTHER — read
  controlDevice->getLastCommand(), values in FollowActions.h:741-762)
- droid handleLowPower state (setOblivious, DroidObjectImpl.cpp:199) —
  do not resurrect an intentionally parked low-power droid
- pet currently being stored (PetControlDeviceStoreTask pending) or
  device status != summoned (updateStatus(1) marks called,
  PetControlDeviceImpl.cpp:456)
- player trading (TradeSession check, PetControlDeviceImpl.cpp:409-413)
- respect petCallOrStoreCooldown to avoid spamming the 1 s gate

Building/interior handling: NO same-building geometry test needed —
placement into player's parent CellObject (PetControlDeviceImpl.cpp:
449-452) inherently summons into the correct interior. But DO skip when
player is indoors and pet is outdoors? Vanilla call permits cross-context
summon (call works anywhere), so simplest correct rule: always place in
PLAYER'S cell context (matches call-pet behavior owners already accept).

Mount nuance:
- Creature mount (PetControlDevice trainedAsMount): if player is RIDING
  that mount it can't be "left behind" (it IS under them) — skip.
- If another player rides it, skip (don't yank a borrowed mount).
- Speeder bikes (VehicleControlDevice): out of scope.

Teleport mechanics note [skill gotcha]: do NOT use getWorldCoordinate()
for the arrival offset — it is facing-relative (SceneObjectImpl.cpp:1583).
Use fixed compass math or the spawnObject random-offset pattern.

=====================================================================
6. CONFIG SURFACE (house style, bin/scripts/customskills/config.lua)
=====================================================================

petAutoSummon = {
    -- enabled: master switch. Possible values: true | false.
    -- Default: false (opt-in; flip here and restart server).
    enabled = false,

    -- chaseRange: how far (meters) a FOLLOWING pet may fall behind the
    -- owner before auto-summon. Whole meters.
    -- Default: 120. Vanilla BT gives up chasing past 128 (pet.lua:130),
    -- so 120 catches stragglers BEFORE vanilla strands them.
    chaseRange = 120,

    -- arrivalOffset: distance (meters) from owner where pet appears.
    -- Pet spawns on a ring at this radius, never on top of player.
    -- Default: 10. Range 5..32 sensible.
    arrivalOffset = 10,

    -- scanIntervalMs: how often the checker task runs per player.
    -- Default: 5000. Lower = snappier, higher = cheaper.
    scanIntervalMs = 5000,

    -- includeDroids: whether DROIDPET/HELPERDROIDPET types participate.
    -- Default: true.
    includeDroids = true,

    -- includeMounts: whether trainedAsMount creature pets participate.
    -- Default: false (mounts usually summoned deliberately, not followers).
    includeMounts = false,
}

Loader: extend CustomSkillsConfig.cpp parse (getBooleanField/getIntField
pattern, CustomSkillsConfig.cpp:99-100 style) + header members in
CustomSkillsConfig.h with static defaults next to
DEFAULT_MISSION_LIST_SIZE (:63) pattern.

=====================================================================
RECOMMENDED ARCHITECTURE
=====================================================================

New file pair managers/customskills/movement/CustomSkillsPetSummon.{h,cpp}
(alongside CustomSkillsMovement):
- static configure(const CustomSkillsConfig&) cached at startup.
- startPlayer(CreatureObject*) / stopPlayer hooks wired where the mod
  already tracks sessions (one recurring Task per player with active
  pets; reschedule self while config enabled and pets active).
- run(): for each pet in ghost->getActivePets(): resolve
  PetControlDevice; apply safety gates (section 5); if
  dist2D(pet, owner) > chaseRange^2: place at compass-neutral ring point
  arrivalOffset from owner (x += sin(a)*d, y -= cos(a)*d pattern from
  ERR-022 lesson), matching player's cell/zone; setFollowObject(owner);
  optional system message to owner ("Your pet was summoned to you").
No engine file changes required except none — everything reachable via
public PetControlDevice/CreatureObject APIs. Zero delegation hooks into
engine managers needed (unlike combat work).

=====================================================================
OPEN QUESTIONS FOR OWNER
=====================================================================
1. Repro confirmation: vanilla already PetReturn's followers past 128 m
   (pet.lua:130-132) — what exactly did he see? Stay-commanded pets?
   Blocked pathing? A specific pet/mount type? This decides whether we
   need summon-on-strand (pathing fail detection) vs simple range check.
2. Should speeder-bike-style vehicles ever qualify? (Recommend no.)
3. Feedback to player on summon: system message, none, or flytext?
4. Include faction pets (AT-ST etc.)? Recommend yes (same class).
5. Cooldown between auto-summons per pet to avoid ping-pong when player
   stands still briefly then runs? Recommend reuse of 5 s scan interval
   as de facto cooldown; explicit knob unnecessary unless he wants one.

[R6.9] discoveries captured here:
- Vanilla pet BT self-recovers followers >128 m via CheckOwnerInRange +
  PetReturn (bin/scripts/ai/pet.lua:130-132, Checks.cpp:507) — owning
  doc: this brief; must inform implementation scope.
- spawnObject() random -2..+3 m placement + parent-cell transfer is the
  reusable summon primitive (PetControlDeviceImplementation.cpp:415,
  449-454) — owning doc: this brief section 2.
- Facing-relative getWorldCoordinate prohibition reaffirmed for offsets
  (skill ERR-022) — owning doc: swgemu-customskills-mod skill gotchas.
