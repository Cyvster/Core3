# Custom Skills Modifier Registry

This is the authoritative planning list for Custom Skills modifiers. Update it
whenever a modifier is added, renamed, rejected, implemented, or assigned to a
badge. Configuration percentages use basis points: `100 = 1.00%`.

## Status definitions

- **Implemented**: active module code and menu display exist.
- **Planned**: accepted candidate without an implementation yet.
- **Research**: mechanics or a safe integration point still need investigation.
- **Rejected**: retained for project history but will not be implemented.

## Offense

| Modifier | Status | Proposed unit or behavior | Notes |
| --- | --- | --- | --- |
| Critical Chance | Implemented | Basis points | Twelve ground combat mastery badges grant a configurable 300 basis points each by default. |
| Critical Multiplier | Implemented | Basis points | Configurable base value; defaults to 15000 (150.00%). |
| Double Attack Chance | Implemented | Basis points | Repeats landed-hit damage for a total of two applications. Checked after Quad and Triple; only one tier can trigger. |
| Triple Attack Chance | Implemented | Basis points | Repeats landed-hit damage for a total of three applications. Checked after Quad and before Double; only one tier can trigger. |
| Quad Attack Chance | Implemented | Basis points | Repeats landed-hit damage for a total of four applications. Checked first; only one tier can trigger. |
| Armor Penetration | Implemented | Whole armor levels | Proposed to reduce no/light/medium/heavy armor by one level per point, to a floor of no armor. |

## Defense and equipment

| Modifier | Status | Proposed unit or behavior | Notes |
| --- | --- | --- | --- |
| Defense Cap Increase | Implemented | Points | Raises the normal hard cap for both primary and secondary defenses. Core3 currently hard-caps both combat calculations at 125 before above-cap bonuses are added. |
| Armor Degradation Reduction | Implemented | Basis points | Reduces the chance that an otherwise eligible armor degradation event applies condition loss. Native armor degradation starts at a 100.00% event chance. |
| Weapon Degradation Reduction | Implemented | Basis points | Reduces the native weapon degradation event chance by a multiplicative percentage; it does not reduce condition loss after a successful event. A 50% reduction on the native 5% chance yields 2.5%; a 100% reduction eliminates degradation entirely. |
| SEA Cap Increase | Implemented | Points | Raises only the total wearable Skill Enhancing Attachment/tape contribution cap above its native +25 limit. SEA/tape bonuses retain their native ability to contribute beyond normal defense caps. This does not raise the normal defense cap. |

## Character and utility

| Modifier | Status | Proposed unit or behavior | Notes |
| --- | --- | --- | --- |
| Movement Speed | Implemented | Basis points | Applies to the character's final native movement speed on foot, while mounted, and while driving a vehicle. Cap remains undecided. |
| Buff Duration | Implemented | Basis points | Applies only to beneficial doctor/medical, entertainer/performance, food (including drink), spice-up, and Jedi/Force buffs. It never affects negative buffs, spice downer phases, debuffs, states, traps, DoTs, cooldown/control markers, or other buff families such as skill-item, innate, Squad Leader, concealment, gallop, and vehicle buffs. |
| Experience Bonus | Implemented | Multiplier in basis points | Applies multiplicatively to every positive experience award that uses Core3's normal modifier path. It composes with species, buff, local, group/precomputed, and server-global rates; it never adds to or replaces them. Example: character 5.00x and server 2.00x produce 10.00x. Awards with `applyModifiers == false` and zero/negative XP are unchanged. |
| Practice Mode Experience Bonus | Implemented | Basis points | Applies specifically when crafting with `createItem == false`. Core3 already applies its native 5% practice bonus first. The custom practice bonus then modifies that XP before the central award path; interaction with the normal Experience Bonus is additive or multiplicative according to shared configuration policy. |

## Crafting and gathering

| Modifier | Status | Proposed unit or behavior | Notes |
| --- | --- | --- | --- |
| Crafting Speed | Implemented | Speed multiplier in basis points | Applies to both personal crafting-tool creation and factory per-item production. Divide native duration by the character multiplier and clamp the scheduled duration to a minimum of 1 second. Personal crafting uses the crafter; factory production snapshots the activating character's multiplier when that character starts the run. |
| Crafting Amazing Success Chance | Implemented | Basis points | Adds percentage points to the native Amazing Success chance for assembly and experimentation; it does not multiply the native chance. Badge values and a configurable final cap must allow a maximum-bonus character to reach at least a 50% final chance. A candidate badge value is +10 percentage points per badge. |
| Amazing Results | Implemented | Basis points | Enhances actual Amazing Success results. For each affected attribute, close the configured percentage of the distance between the native amazing result and the schematic's absolute valid cap, and raise that attribute's resource-derived ceiling only enough to retain the enhanced value. At 100% strength, poor resources can produce perfect affected attributes without exceeding legitimate schematic limits. |
| Gathering Quantity | Implemented | Basis points | Each qualifying badge grants +200% quantity by default (`20000` basis points). Badge bonuses add together, then modify forage and creature-milking resource quantities through two separate hooks. One badge therefore produces 300% of the native amount (3x total); two produce 500% (5x total). Apply after each native quantity calculation, round down to a whole resource unit, and never reduce the native result. Discrete forage loot objects are unaffected. |

## Global design rules

- Disabled or unobtainable badge sources grant no modifier value.
- Pilot/JTL badges grant no bonuses while JTL is unavailable.
- All badges remain visible in the menu even when they grant no modifier.
- Combat and menu values must come from the same configuration-backed service.
- Existing Core3 files receive only generic delegation hooks required to enter
  module-owned code.
- Temporary combat-spam labels are development diagnostics and will be removed
  when testing is complete.

## Server Config menu guidelines

When adding new server config options, follow these conventions:

### Page hierarchy
- Server Config > Category (Mod Options, SWGEMU Options) > Option detail page
- Each category is a Page enum value; each option detail page is its own Page enum
- Place option enums after their parent category enum in the Page enum

### Acquired count (parentheses in bottom pane)
- Category items show enabled option count: `Mod Options (1)`
- `countEnabledOptions()` returns total enabled across all categories
- `getAcquiredCount()` must have cases for each category summing its enabled options

### Top pane (prompt text)
- **Server Config page**: Shows `--- Category ---` headers, each listing options with
  ENABLED/DISABLED status. Not stat summary.
- **Category page** (e.g. Mod Options): Shows options directly with ENABLED/DISABLED
  status. No category header since we are at the bottom of the category tree.
- **Option detail page** (e.g. Rarity Naming): Shows description, current state,
  current values, and default values for comparison.

### Bottom pane (listbox items)
- Category items: `Category Name (N)` where N is enabled option count
- Option items at category level: `Option Name ENABLED` or `Option Name DISABLED`
  (all caps, color coded green/red). No count -- these are not categories.
- Option detail items: current values with color swatches where applicable

### Config loading
- Add config fields to `CustomSkillsConfig.h` (public getters + private fields)
- Add defaults in `setDefaults()`, loading in `load()`
- Add `rarityNaming`-style table in `config.lua` with documented defaults
- Sync both config.lua copies (Core3 and package)

## Core3 hook inventory

This is the implementation backlog for Core3 entry points. A hook is a small,
generic delegation into a module-owned service; modifier rules and configuration
must remain outside Core3. No new hook should be added until this inventory has
been reviewed for consolidation.

| ID | Lifecycle / proposed module call | Modifiers served | Candidate Core3 entry point | Status and constraints |
| --- | --- | --- | --- | --- |
| H01 | `CustomSkillsCombat::modifyLandedDamage(context)` | Critical Chance; Critical Multiplier; Double/Triple/Quad Attack | Existing landed-hit wrapper around `CombatManager::applyDamage` | Implemented. Double/triple/quad scale landed-hit damage through this hook and do not rerun accuracy, mitigation, states, wounds, DoTs, or weapon degradation as complete attacks. Precedence is Quad, Triple, Double; only one tier triggers, after critical adjustment. |
| H02 | No hook | None | Not applicable | Rejected as unnecessary. Double/triple/quad are damage repeats handled by H01, not complete additional attack executions. |
| H03 | `CustomSkillsCombat::getEffectiveArmorRating(context, nativeRating)` | Armor Penetration | `CombatManager::getArmorReduction`, after the applicable armor layer and its native rating are known but before native armor-piercing comparison | Implemented. Reduces defender armor level Heavy -> Medium -> Light -> None, floor None. It must not alter weapon/attacker Armor Piercing. The hook must cover worn armor, PSG/Force armor, NPC armor, and vehicle armor only where explicitly eligible. |
| H04 | `CustomSkillsCombat::getDefenseCap(defender, defenseType, nativeCap)` | Defense Cap Increase | Shared cap operation used by primary and secondary defense calculations in `CombatManager` | Implemented. Core3 currently applies a literal 125 hard cap in both paths. Replace duplicated cap use with one native helper if practical, then delegate through it. Increase the normal cap only; do not absorb or re-cap SEA/tape and other native above-cap contributions. |
| H05 | `CustomSkillsDurability::shouldDegradeArmor(context, nativeChance)` | Armor Degradation Reduction | Immediately before eligible armor/PSG condition-loss `inflictDamage` calls in combat mitigation | Implemented. Native eligible armor degradation has a 100.00% event chance. The module reduces that chance, and a failed roll skips the existing condition loss without changing its amount. Multiple native armor paths should call the same module hook. |
| H06 | `CustomSkillsDurability::shouldDegradeWeapon(context, nativeChance)` | Weapon Degradation Reduction | `WeaponObjectImplementation::decay`, at the native degradation chance roll | Implemented. Reduce the native event chance (currently base 5%, with the native powerup adjustment) multiplicatively: `effectiveChance = nativeChance * (10000 - reduction) / 100`. Leave the existing condition loss unchanged when the roll succeeds. This remains separate from H05 because weapon and armor degradation have different entry points and native chances. |
| H07 | `CustomSkillsProgression::getExperienceMultiplier(player, xpType)` | Experience Bonus | `PlayerManagerImplementation::awardExperience`, inside the existing `applyModifiers` calculation beside `globalExpMultiplier` | Implemented. Multiply the existing expression by the character multiplier: `amount * speciesModifier * buffMultiplier * localMultiplier * globalExpMultiplier * customMultiplier`. Default is 1.00x. Apply to every positive XP type on the normal modifier path; do not alter XP costs/losses or calls that explicitly set `applyModifiers == false`. |
| H08 | `CustomSkillsCrafting::modifyPracticeExperience(crafter, nativePracticeAmount)` | Practice Mode Experience Bonus | `CraftingSessionImplementation::createPrototype`, inside the existing `createItem == false` branch after Core3's native `xp * 1.05` adjustment and before `PlayerManager::awardExperience` | Required and straightforward. The existing `createItem` branch already identifies practice mode, so no new session state or `awardExperience` signature is needed. H08 modifies only practice XP; H07 may subsequently apply the general Experience Bonus using the shared stacking policy. |
| H09A | `CustomSkillsCrafting::getPersonalCraftingDuration(crafter, nativeSeconds)` | Crafting Speed | `CraftingSessionImplementation::startCreationTasks`, before countdown and creation tasks are scheduled | Implemented. Divide the native inventory-tool crafting duration by the crafter's configured multiplier, then clamp to at least 1 second. Use the same modified duration for the countdown and final creation task. |
| H09B | `CustomSkillsCrafting::getFactoryProductionDuration(activator, factory, schematic, nativeSeconds)` | Crafting Speed | Factory activation path into `FactoryObjectImplementation::startFactory`, after native `schematic complexity * 8` timing is calculated and before the first `CreateFactoryObjectTask` is scheduled | Implemented. Snapshot the activating character's multiplier into the factory's existing persisted per-item `timer`, clamp it to at least 1 second, and reuse that duration for all subsequent batch-item reschedules. Badge changes do not alter an active run; stop and restart the factory to snapshot a new multiplier. Database-load restart must preserve the stored modified timer instead of recalculating it without the activator. No permanent activator ID is required. |
| H10 | `CustomSkillsCrafting::getAmazingChanceBonus(crafter, phase)` | Crafting Amazing Success Chance | Native Amazing Success checks in `SharedLabratory::calculateAssemblySuccess` and `CraftingManagerImplementation::calculateExperimentationSuccess` | Implemented. Add the configured basis-point bonus to the native probability; do not multiply it. Use the same module policy at both native roll sites and clamp to the configured final chance cap. Native city, food, Force, and other crafting effects remain part of the starting chance. |
| H11 | `CustomSkillsCrafting::applyAmazingResults(crafter, phase, affectedAttributes, strength)` | Amazing Results | Resource and genetic laboratory assembly-value initialization and experimentation-row application, only when the result is `AMAZINGSUCCESS` | Implemented. A numerical-result hook alone is insufficient because Core3 separately clamps progress to a resource-derived ceiling. For each affected attribute, interpolate from the completed native amazing value toward the absolute schematic cap by the configured strength, raise the stored ceiling only enough to preserve that enhanced value, and never exceed the legitimate cap. Assembly affects all applicable initial attributes; experimentation affects only the selected row. Non-amazing results remain entirely resource-limited. |
| H12A | `CustomSkillsBuffs::modifyInitialDuration(recipient, buff, nativeDuration)` | Buff Duration | `BuffImplementation::activate`, before the base `scheduleBuffEvent` call | Implemented. All included specialized buffs ultimately call base activation. Apply once using an explicit eligibility classifier: beneficial `MEDICAL`, `PERFORMANCE`, `FOOD` (including drinks), spice-up, and positive `JEDI`/Force effects. Type alone is insufficient because spice downers and harmful Force effects share buff infrastructure, so use explicit class/CRC exclusions or an allowlist. Database reload scheduling must preserve the already-snapshotted duration. |
| H12B | `CustomSkillsBuffs::modifyRenewedDuration(recipient, buff, nativeDuration)` | Buff Duration | `CreatureObjectImplementation::renewBuff`, before assigning and scheduling an explicitly supplied renewal duration | Required for included buffs that renew without reactivation, particularly Jedi/Force abilities. Apply the character bonus to the newly supplied native duration once. Preserve the original native duration when recursively renewing secondary linked buffs so each is independently scaled once. Do not treat database reload, ordinary event rescheduling, or an already-scaled stored duration as a new native renewal. The same eligibility classifier and calculation used by H12A must be used here. |
| H13A | `CustomSkillsMovement::getEffectiveRunSpeed(character, nativeSpeed, travelMode)` | Movement Speed | Player run-speed value sent in `CreatureObjectMessage4` / `CreatureObjectDeltaMessage4`, after native on-foot, mount, or vehicle speed selection | Implemented. `CreatureObjectImplementation::getRunSpeed` feeds the client packet, but changing that getter alone is insufficient. Apply the character's bonus once to the final native speed in every travel mode; do not modify the mount/vehicle object and then apply it again to the rider. Exact placement should avoid changing unrelated AI/object callers of the getter. |
| H13B | `CustomSkillsMovement::getEffectiveAllowedSpeed(character, nativeSpeed, travelMode)` | Movement Speed | `PlayerManagerImplementation::checkPlayerSpeedTest`, after it selects player, mount, or vehicle native speed and modifiers but before comparison with parsed movement | Implemented. Mounted and vehicle validation bypasses the rider's `getRunSpeed()` and reads the parent object's speed directly, so H13A alone would allow the client to move faster while the server rejects that movement. H13A and H13B must use the same module calculation. |
| H14A | `CustomSkillsSkillMods::getEffectiveSkillMod(character, modName, nativeTotal, rawWearableValue)` | SEA Cap Increase | `CreatureObjectImplementation::getSkillMod`, after native `SkillModList` aggregation | Implemented. `SkillModManager` caps the `WEARABLE` source at +25, but `SkillModList` has no owning-character context. Leave the generic container unchanged: calculate the difference between the native-clamped wearable contribution and the character's custom-capped wearable contribution, then add only that difference to the native total. This raises only SEA/tape contribution and preserves all other source caps. |
| H14B | `CustomSkillsSkillMods::getEffectiveVisibleSkillMod(character, modName, nativeEntry, rawWearableValue)` | SEA Cap Increase | Character skill-mod delta/baseline preparation and badge-cap refresh | Required for client consistency. H14A changes server calculations, but the client-visible `SkillModEntry` is produced separately by `SkillModList::getVisibleSkillMod` and remains capped at +25. Apply the same wearable-cap difference when preparing character-visible skill mods, and refresh affected entries when the character's badge-derived SEA cap changes. |
| H15A | `CustomSkillsGathering::modifyForageQuantity(player, resource, nativeQuantity)` | Gathering Quantity | `ForageManagerImplementation::forageGiveResource`, after the native 10-40 resource-unit roll and before `harvestResourceToPlayer` | Implemented. Multiply only resource quantity, round down to a whole unit, and never return less than the native quantity. This covers flora, shellfish, and lair-egg resource awards. It does not duplicate food, bait, medical components, rare items, or live-creature loot objects. |
| H15B | `CustomSkillsGathering::modifyMilkQuantity(player, creature, resource, nativeQuantity)` | Gathering Quantity | `MilkCreatureTask::giveMilkToPlayer`, after creature-template quantity and local-density adjustment but before `harvestResourceToPlayer` | Implemented. Multiply the completed native milk quantity, round down to a whole unit, and never return less than the native quantity. Native creature eligibility, milk state, resource type, and density scaling remain unchanged. |
| H16 | `CustomSkillsModifiers::applyRarityNaming(object, excMod, legendaryThreshold, exceptionalThreshold)` | Server Config: Rarity Naming | `LootManagerImplementation::setCustomObjectName`, after base name is set but before suffix logic | Implemented. When enabled via config, replaces the default "(Legendary)"/"(Exceptional)" name suffix with colored text, fixing quest turn-in compatibility. Returns true to skip original suffix logic, false to let Core3 handle it normally. Single delegation call; zero behavioral change when disabled. |

### Consolidation review questions

- Keep Double/Triple/Quad Attack in H01 as landed-damage repeats. H02 must not
  be implemented. The implemented precedence is Quad, Triple, Double; one tier
  may trigger, after critical adjustment.
- Consider one `CustomSkillsCombat` context API serving H01, H03, and H04 while
  retaining separate lifecycle calls. They cannot be one physical hook because
  damage, armor selection, and defense capping occur at different times.
- Use one `CustomSkillsDurability` service and shared chance-calculation policy
  for H05 and H06, but keep two Core3 hooks because armor and weapon degradation
  use different entry points and native chances.
- Keep H08 at the existing crafting practice branch and H07 at the central XP
  award path. This avoids changing the widely used `awardExperience` signature;
  define their additive or multiplicative stacking order once in module policy.
- Use one `CustomSkillsCrafting` service for H08-H11. Separate phase callbacks
  are still necessary because XP, scheduling, success selection, and result value
  occur at distinct lifecycle points.
- Implement Buff Duration through H12A for initial base activation and H12B for
  explicit renewals. Both must share one eligibility classifier and must not
  rescale persisted durations or internal event reschedules.
- Treat H13A and H13B as one movement feature with two mandatory Core3 entry
  points: one supplies the client-effective speed and one authorizes that same
  speed in server validation. Add transition tests for mount, dismount, vehicle,
  gallop/buffs, and reconnect to detect double application.
- Keep character context at the `CreatureObject` boundary for H14A/H14B. Do not
  add owner state, badge logic, or a global mutable cap to generic `SkillModList`.
  Both paths must use one module calculation so server mechanics and the client-
  visible skill-mod values remain identical.
