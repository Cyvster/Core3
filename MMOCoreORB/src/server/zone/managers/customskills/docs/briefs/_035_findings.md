# BRIEF-035 Findings -- Crafting tool repeat-craft viability

- Status: COMPLETE
- Date: 08/25/2026
- Scope: research only, no code changes.

## Q1. Session lifecycle / state machine

**Tool states** (`CraftingTool.idl:41-43`): `TOOL_READY=0`, `TOOL_WORKING=1`,
`TOOL_FINISHED=2`. Tool holds at most one finished prototype in its container
(`CraftingToolImplementation.cpp:56-61`); radial "hopper output" transfers it to
inventory and resets to READY (`handleObjectMenuSelect`,
CraftingToolImplementation.cpp:70-101). On server restart status is derived
from container contents (`initializeTransientMembers`, :36-46).

**Session object**: `server/zone/objects/player/sessions/crafting/CraftingSession`
(created fresh per craft in `RequestCraftingSessionCommand.h:120-131`; any prior
session is cancelled first, :114-119).

**State machine** (`state` field, advanced by client commands):

| state | meaning | entered by |
|---|---|---|
| 1 | schematic list shown | `startSession()` CraftingSessionImplementation.cpp:113-184 |
| 2 | resource/ingredient screen | `selectDraftSchematic()` :329-391 |
| 3 | assembly done, experimentation available (station present + exp rows or factory-allowed) | `initialAssembly()` :841-846 |
| 4 | assembly done, no experimenting / post-experiment | :846, `finishAssembly()` :985-1007 |
| 5 | customization done | `customization()` sets `state = 5` :1330 |
| 6 | final stage | `finishStage1` :1350 / `nextCraftingStage` :722-732 |

Progression is driven entirely by **client queue commands**:
`requestcraftingsession` -> `selectdraftschematic <index>` ->
`craftingingredient` add (per slot, packet 0x107;
CraftingAddIngredientCallback) -> `nextcraftingstage` (assembly,
NextCraftingStageCommand.h:58 -> `nextCraftingStage()`) -> optional
`requestcraftingexperiment` (packet 0x106 -> `experiment()` :1010) ->
`craftingcustomization` (packet 0x15A -> `customization()` :1211) ->
`createprototype <0|1>` (CreatePrototypeCommand.h:71 -> `createPrototype()` :1372).

**Where a session ends**: `createPrototype()` (:1372-1416) starts the timed
creation tasks (`startCreationTasks` :1418-1450, timer =
complexity*2 modified by our `CustomSkillsCrafting::getPersonalCraftingDuration`),
awards XP, marks the manf schematic completed, then **always calls
`cancelSession()`** (:1415) which drops the CRAFTING facade from crafter and
tool (:186-209) and destroys the manf schematic + satchel (`clearSession()`
:216-280). The tool then goes WORKING until `CreateObjectTask` fires
(events/CreateObjectTask.h), which transfers the prototype to inventory and
`setReady()`, or parks it FINISHED if inventory is full.

So: one CraftingSession == exactly one craft. There is no vanilla "stay open"
path.

## Q2. What defines "same craft" (snapshot content)

Snapshot candidates:

- **Draft schematic**: identified by index into `currentSchematicList`
  (`startSession()` :136-138 rebuilds it each session via
  `filterSchematicList`). Snapshot the DraftSchematic **server CRC**, re-resolve
  the index each repeat (list ordering should be stable but verify).
- **Resources**: slot->tano OID mapping is NOT reusable; ingredients are
  consumed into the tool's `crafted_components` satchel during assembly
  (`addIngredient` :533-617 -> `manufactureSchematic->addIngredientToSlot`;
  consumed in `initialAssembly` path). Each iteration must re-resolve
  slot requirements (`DraftSlot`) against current inventory contents.
  Resource type/experimentation-quality choice per slot IS legitimately
  reusable (pick "same resource class + same specific resource name if still
  present").
- **Experiment allocation**: `experiment(rowsAttempted, expAttempt, counter)`
  takes a string of `row points` pairs (:1043-1091). Snapshotting the row/point
  pairs is legal; points total is recomputed per session from skill mods
  (:753-756), so clamp to the new budget.
- **Customization**: `customization(name, templateChoice, schematicCount, custString)`
  (:1211) -- name, template choice, manufacture limit, and the customization
  var/value pairs are all plain data and fully reusable.
- Assembly/experimentation rolls are re-rolled every time (they're inside the
  stage handlers), which is correct -- a repeat must NOT replay results.

Must re-validate every iteration: schematic still known/use-count available
(`decreaseSchematicUseCount` :973), resources actually in inventory and
moveable (`addIngredient` checks :583-600), complexity vs tool/station,
inventory space for output (checked late in CreateObjectTask -- a full
inventory parks the tool FINISHED, CreateObjectTask.h transfer block).

## Q3. Client interaction constraints -- THE DECISIVE QUESTION

The session's progression methods are plain server methods; nothing in them
parses client-only state. However:

1. The session is created and initialized only by the `requestcraftingsession`
   queue command (RequestCraftingSessionCommand.h:120-131).
2. Each stage transition arrives as a separate client ObjectController command
   (registered in ZonePacketHandler.cpp:207-210 for the callback family; stage
   commands are queue commands: NextCraftingStageCommand, SelectDraftSchematicCommand,
   CreatePrototypeCommand, etc.).
3. The client crafting window is driven by the outbound DPLAY9/DMSCO3/DMSCO7/
   OC packets; if the server runs stages internally without the client sending
   the matching commands, the client UI simply never opens -- the packets are
   informational and the code does not require the window to be open.
4. Vanilla already proves autonomous completion: once `createPrototype` runs,
   `CreateObjectTask` finishes the item with zero further client input.

**Verdict**: a pure hands-off server-side loop is *technically* possible
(invoke initializeSession + selectDraftSchematic + programmatic
addIngredientToSlot + nextCraftingStage chain + customization + createPrototype
from a task), BUT the stock client will not display anything and any client
UI interaction mid-loop (cancel attempt, movement is fine, but closing windows,
starting another craft) desyncs badly. Vanilla deliberately made every step
client-initiated as a flow-control mechanism.

**Recommended shape = "assisted pre-fill" (hybrid)**: keep the client in
charge of session *boundaries* only. After `createPrototype`, instead of just
cancelling, offer the player a repeat (radial on tool / SUI confirm). On
accept, the server starts the NEXT session itself (same call sequence as
RequestCraftingSessionCommand), auto-selects the snapshotted schematic,
auto-fills identical ingredients from inventory (re-validated), and stops at
state 2 showing the normal window pre-filled -- the player presses Assemble
(or we optionally auto-advance through createPrototype for N repeats with a
config-gated "full auto"). This respects the client-packet flow while removing
~90% of the clicking.

## Q4. Vanilla precedent: factories

`FactoryObjectImplementation::createNewObject()` (:664-774): the factory owns a
persisted ManufactureSchematic plus its stored prototype
(:676-688), then loops on `CreateFactoryObjectTask`: verify inputs via
`schematic->canManufactureItem(type, displayedName)` (:714-719), clone output
into crates or standalone items (:728-754), decrement
`schematic->getManufactureLimit()` and destroy schematic at 0 (:756-766),
reschedule (:768-773). Ingredient consumption happens against the input hopper.

Reusable patterns for us:
- **Persisted ManufactureSchematic + prototype** as the "recipe" record --
  exactly what `createManufactureSchematic(int clientCounter)`
  (CraftingSessionImplementation.cpp:1452-1505) puts into the datapad when the
  player chooses "create manufacturing schematic". A repeat feature could reuse
  this same object as its snapshot instead of inventing a new format.
- **manufactureLimit countdown** as the natural "N repeats" knob.
- **Crate stacking of identical serials** (locateCrateInOutputHopper :776-798)
  -- optional QoL for repeat output.

## Q5. Abuse surface

- **Practice-mode XP farming**: practice awards xp * 1.05
  (:1398-1402) plus our `modifyPracticeExperience` hook, with NO resource cost
  recovery difference -- practice consumes resources identically (resources
  removed at initialAssembly :952 comment "Remove all resources - Not
  recovering them"), so XP-per-resource is unchanged by repeating. Repeat does
  remove the *time* cost between crafts though: with Crafting Speed stacked,
  timer -> min 1s (getPersonalCraftingDuration clamps >=1s), so an unattended
  full-auto repeat loop becomes an AFK XP printer. Mitigations: disallow
  practice mode in repeats (owner directive: server operator config option,
  defaulting to allowed -- AFK farming is already possible via macros and is
  NOT being blocked), or cap repeats/session, or require the assisted
  pre-fill (player present pressing buttons) rather than full auto.
- **Crafting Speed interplay**: `startCreationTasks` (:1418-1450) applies our
  multiplier before scheduling; repeats inherit it automatically. Full-auto
  back-to-back crafts compress to ~1s each + client round-trips; fine for
  assisted mode, another reason to gate full auto.
- **Resource exhaustion mid-loop**: ingredients are consumed at assembly; if
  repeat #N lacks resources, `addIngredientToSlot` returns failure codes --
  the repeat controller must treat PARTIALASSEMBLE (:700) /
  slot-failure messages as a clean stop-and-return-ingredients, never retry.
  Inventory-full output is handled by CreateObjectTask parking the tool
  FINISHED; the repeat controller must detect tool != READY and halt.
- **Tool damage/degradation**: none in vanilla (no condition field on
  CraftingTool) -- no extra concern.
- **forceCriticalAssembly/Experiment consumables** (:768-771, :1101-1107):
  each repeat consumes charges normally; snapshot must NOT duplicate them.

## Verdict

**PARTIAL / FEASIBLE-AS-ASSISTED-PREFILL.**

A fully server-side autonomous repeat loop (no client involvement) is not
recommended: the entire stage machine is designed around client-initiated
commands, the session self-destructs after every craft
(createPrototype -> cancelSession, :1415), and an invisible server loop risks
client desync and AFK automation abuse.

**Recommended approach sketch for the implementation brief:**

1. After a successful `createPrototype(clientCounter, true)` (non-practice),
   stash a lightweight `RepeatRecipe` on the tool/player: draft schematic CRC,
   per-slot resource-name choices, experiment row/point pairs, customization
   (name/templateChoice/manufactureLimit/custString). Expire on logout/zone.
2. Add a radial/SUI option on the crafting tool ("Repeat last craft"): server
   performs the RequestCraftingSessionCommand sequence, auto-selects the
   schematic (resolve index from CRC in the freshly filtered list), and
   programmatically fills slots via `manufactureSchematic->addIngredientToSlot`
   with re-validated inventory resources, leaving the normal crafting window
   open at state 2, pre-filled. Player confirms as usual.
3. Optional config-gated "auto-confirm" tier (`maxRepeats`, `repeatMode =
   off|prefill|auto`, `autoMaxRepeats` default small): server chains sessions
   through createPrototype with a short inter-craft delay, halting on any slot
   failure, critical failure, inventory-full (tool FINISHED), or exhausted
   schematic uses. Practice mode excluded from auto tier.
4. Hooks live beside existing CustomSkillsCrafting call sites
   (CraftingSessionImplementation.cpp includes customskills/crafting/CustomSkillsCrafting.h
   already, lines 10, 777, 1127, 1401, 1421) -- no new injection neighborhood needed.
