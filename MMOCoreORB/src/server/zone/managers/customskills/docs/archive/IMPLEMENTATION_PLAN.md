# Custom Skills Hook Implementation Plan

This document converts the modifier registry into an implementation sequence.
`MODIFIER_REGISTRY.md` remains authoritative for player-facing behavior and hook
requirements.

## Objectives

- Keep badge assignments, configuration, calculations, and eligibility rules in
  module-owned files.
- Keep existing Core3 edits to small delegation calls at native lifecycle
  boundaries.
- Use one calculation for menu display and runtime behavior.
- Preserve native behavior exactly when a modifier is disabled or totals zero.
- Preserve native ordering unless the modifier specification explicitly changes
  it.
- Keep the standalone `customskills-mod/package` copy synchronized with Core3.

## Consolidated module services

### Configuration and modifier authority

`CustomSkillsConfig` loads enabled flags, per-badge values, caps, multipliers,
and eligible badge mappings from `scripts/customskills/config.lua`.

`CustomSkillsModifiers` is the sole character/badge aggregation authority. It
returns typed totals in basis points, whole cap points, or armor levels. Menu
presentation and runtime services consume these same totals.

### Runtime services

1. `CustomSkillsCombat`
   - Landed damage: critical and repeat-damage procs.
   - Effective defender armor rating.
   - Primary and secondary defense caps.
2. `CustomSkillsDurability`
   - Armor/PSG degradation-event chance.
   - Weapon degradation-event chance.
3. `CustomSkillsProgression`
   - General positive-XP multiplier.
4. `CustomSkillsCrafting`
   - Practice XP.
   - Personal and factory duration.
   - Amazing Success chance.
   - Amazing Results progress and ceiling behavior.
5. `CustomSkillsBuffs`
   - Initial and explicit-renewal duration.
   - One shared positive-buff eligibility classifier.
6. `CustomSkillsMovement` and `CustomSkillsSkillMods`
   - Client-effective and server-validated movement speed.
   - Server-effective and client-visible wearable SEA caps.
7. `CustomSkillsGathering`
   - Forage resource quantity.
   - Milk resource quantity.

## Consolidation decisions

- H02 is not implemented. Double/triple/quad effects use landed-damage H01.
- Armor and weapon degradation share chance math but retain separate hooks.
- General and practice XP retain separate hooks; no signature expansion of the
  widely used `awardExperience` API is required.
- Personal and factory crafting speed share duration math but retain separate
  lifecycle hooks.
- Assembly and experimentation use the same Amazing chance policy at their two
  native roll sites.
- Amazing Results uses one policy but delegates from both resource and genetic
  laboratory assembly/experimentation paths because progress and ceilings live
  there.
- Initial buff activation and explicit renewal share eligibility and duration
  math but retain separate hooks.
- Client movement and server validation share one effective-speed calculation
  but retain separate hooks.
- Server and client-visible SEA values share one wearable-cap calculation but
  retain separate hooks.
- Forage and milk share quantity math but retain separate hooks.

## Runtime ordering

### Combat

1. Resolve the native landed hit.
2. Resolve custom critical chance and multiplier.
3. Resolve one configured repeat-damage tier.
4. Apply the finalized custom damage result through the native damage pipeline.

Armor Penetration changes only the defender's effective armor rating immediately
before the native Armor Piercing comparison. Defense Cap Increase changes only
the native primary/secondary cap. Neither operation changes raw skill mods.

### Experience

The general multiplier is the final factor in Core3's existing positive-XP
modifier expression. Practice crafting applies native 5%, then the custom
practice modifier, before entering that central expression.

### Crafting

Duration is divided by the effective speed multiplier and clamped to one second.
Factory duration is snapshotted from the activating character for the run.

Amazing Success Chance adds percentage points to native chance. Amazing Results
runs only after an actual Amazing Success and closes the configured fraction of
the distance from the native completed result to the absolute schematic cap,
raising the affected resource ceiling only enough to retain the result.

### Buffs

Duration is snapshotted once at initial activation. An explicit renewal applies
the recipient's current bonus to the newly supplied native duration. Persistence
reload and internal task rescheduling reuse the stored duration without scaling.

## Implementation sequence

1. Expand Lua configuration and typed modifier aggregation.
2. Implement no-op-safe runtime service APIs and focused unit-testable math.
3. Add combat, durability, and progression delegations.
4. Add crafting delegations, including factory persistence behavior.
5. Add buff eligibility and lifecycle delegations.
6. Add synchronized movement and SEA server/client delegations.
7. Add forage and milk delegations.
8. Extend the menu summary/category/badge presentation from the same modifier
   authority.
9. Synchronize package files and integration documentation.
10. Build, run focused tests, inspect the complete diff, and prepare one commit.

## Verification matrix

- Every disabled or zero-value modifier reproduces native output.
- All configured values shown by `/customskills` equal runtime values.
- Negative XP and `applyModifiers == false` remain unchanged.
- Practice mode retains native 5% with zero custom bonus.
- Factory speed survives logout/restart and changes only on a new run.
- Non-amazing crafting remains resource-capped.
- Buff downers/debuffs and excluded families retain native durations.
- Movement agrees between client speed and server validation in every travel
  mode and does not double on mount transitions.
- SEA overflow agrees between server mechanics and client-visible values.
- Discrete forage loot is never duplicated by Gathering Quantity.

