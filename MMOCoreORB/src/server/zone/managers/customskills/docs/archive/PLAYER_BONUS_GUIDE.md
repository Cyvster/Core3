# Custom Skills: Player Bonus Guide

This guide explains how Custom Skills bonuses affect your character. It is a
gameplay guide, not an installation or server-administration guide.

The server owner controls which bonuses are enabled, which badges grant them,
and how large each bonus is. Values shown by the `customskills` command are the
authoritative values for your character on the server you are playing.

Some mechanics in this guide are still planned and may not yet be enabled. The
guide will be updated as those mechanics are released.

## Badges and character bonuses

Bonuses are character-specific. A badge earned by one character does not grant
its bonus to another character on the same account.

- Only acquired badges contribute to your totals.
- Badges remain visible in the menu even if they do not grant a bonus.
- Unobtainable badges do not grant bonuses.
- Pilot and Jump to Lightspeed badges do not grant bonuses while JTL gameplay is
  unavailable.
- A server owner may disable a bonus or change its value.

Open the menu with:

```text
/customskills
```

The main page shows your acquired bonus totals. Badge categories and individual
badge entries show the bonuses that contribute to those totals.

## Understanding percentages and multipliers

A percentage chance, such as `+3% Critical Chance`, adds percentage points to
that chance.

A multiplier, such as `5x Experience`, multiplies the applicable value. It does
not replace other multipliers. Unless a section says otherwise, a missing or
disabled bonus behaves as `1x` and leaves the normal game mechanic unchanged.

## Offensive bonuses

### Critical Chance

Critical Chance is the chance for a landed attack to become a custom critical
hit. The default configuration grants `+3%` for each eligible ground-combat
mastery badge. With all twelve eligible badges, the default total is `36%`.

The server owner may change the per-badge value or disable custom critical hits.

### Critical Multiplier

Critical Multiplier determines the damage of a custom critical hit. The default
multiplier is `150%`, meaning a critical hit deals 1.5 times the finalized
landed-hit damage.

The multiplier is displayed even when your Critical Chance is zero because it
is a property of the enabled critical-hit system, not an earned chance by
itself.

### Double, Triple, and Quad Attack Chance

These effects repeat the finalized damage of an attack that has already landed:

- Double Attack applies the finalized damage two times in total.
- Triple Attack applies it three times in total.
- Quad Attack applies it four times in total.

They are damage repeats, not complete new attacks. Repeated damage does not make
new accuracy rolls and does not separately rerun armor mitigation, states,
wounds, damage-over-time effects, or weapon-degradation rolls.

Only one repeat tier can trigger for a landed hit. The server checks Quad,
then Triple, then Double; it stops at the first successful roll. Critical damage
is calculated first, so a repeat tier repeats the critical-adjusted damage when
both effects trigger. Temporary `(DOUBLE)`, `(TRIPLE)`, and `(QUAD)` combat
labels identify these development effects.

### Armor Penetration

Armor Penetration lowers the target's effective armor level for damage
mitigation. Each point lowers armor by one level:

```text
Heavy -> Medium -> Light -> No Armor
```

The effective level cannot fall below No Armor. This bonus does not increase the
weapon's native Armor Piercing rating; Armor Penetration and Armor Piercing are
different mechanics.

## Defensive and equipment bonuses

### Defense Cap Increase

Core3 normally caps both primary and secondary defense calculations at `125`
before applicable above-cap bonuses are added. Defense Cap Increase raises that
normal limit by the displayed number of points.

This bonus does not increase your defense value by itself. It only allows more
of an existing primary or secondary defense value to count.

### SEA Cap Increase

Skill Enhancing Attachments and tapes are a separate bonus source. Their native
wearable contribution is capped at `+25`. SEA Cap Increase raises only this
wearable contribution limit.

For example, `+5 SEA Cap` changes the applicable wearable cap from `+25` to
`+30`. It does not provide five points of a skill mod by itself; the character
must still equip attachments or tapes containing those skill mods.

SEA Cap Increase does not raise the normal primary or secondary Defense Cap.
Defense Cap Increase and SEA Cap Increase are independent bonuses. Applicable
SEA/tape defenses retain their normal ability to contribute beyond the ordinary
defense cap.

### Armor Degradation Reduction

Eligible armor normally receives a degradation event every time the native
armor-degradation conditions are met. Its starting event chance is therefore
`100%`.

Armor Degradation Reduction lowers the chance that the event occurs. If the roll
is avoided, no condition loss is applied. If it occurs, the normal amount of
condition loss is applied; this bonus does not reduce the amount after a
successful degradation roll.

### Weapon Degradation Reduction

Weapons already use a chance-based degradation roll for eligible actions or
hits. Weapon Degradation Reduction lowers that event chance by a multiplicative
percentage.

For example, with the native 5% base chance:

- A 25% reduction yields a 3.75% chance.
- A 50% reduction yields a 2.50% chance.
- A 100% reduction eliminates weapon degradation entirely.

Native rules such as weapon powerup adjustments remain part of the starting
chance before the reduction is applied.

If degradation occurs, the weapon takes its normal condition loss. The bonus
does not reduce the amount of condition loss after a successful roll.

Armor and weapon degradation use separate rolls. A bonus to one does not affect
the other.

## Movement

### Movement Speed

Movement Speed applies to the character's final native speed in every travel
mode:

- On foot
- Riding a creature mount
- Driving a vehicle

The character's bonus is applied once. It is not first added to the mount or
vehicle and then applied a second time to the rider. Existing movement effects,
including slows and native speed modifiers, remain part of the calculation.

## Experience

### General Experience Bonus

The General Experience Bonus is a character multiplier applied to all positive
experience awards that use the game's normal modifier system. It multiplies
with--not adds to, replaces, or overrides--the server's XP rate and other native
multipliers.

Example:

```text
Base award:             100 XP
Server bonus weekend:     2x
Character XP bonus:       5x
Final award:           1,000 XP
```

The calculation is `100 x 2 x 5 = 1,000`. The combined rate is `10x`, not `7x`,
and the character's `5x` bonus does not replace the server's `2x` rate.

The multiplier covers normal positive awards from combat, crafting, harvesting,
scouting, fishing, quests, apprenticeship, and other systems using the central
XP-award path. It does not multiply XP deductions, conversion costs, or special
awards that explicitly bypass normal modifiers.

### Practice Mode Experience Bonus

Crafting practice mode already grants a native `5%` XP increase. That native
bonus remains in effect even if the character has no custom Practice Mode
Experience Bonus.

The order is:

1. Calculate the base crafting XP.
2. Apply Core3's native 5% practice increase.
3. Apply the custom Practice Mode Experience Bonus, if any.
4. Process the award through the normal XP system, including server and General
   Experience multipliers.

The custom practice bonus multiplies the amount after the native 5% increase.
The General Experience Bonus then multiplies that result through the central XP
award path, so neither custom bonus replaces the other.

## Crafting and factories

### Personal Crafting Speed

Personal Crafting Speed applies when creating an item with a crafting tool from
your inventory. The native crafting duration is divided by your speed
multiplier.

For example, a `2x` crafting-speed multiplier reduces a 10-second creation to 5
seconds. A personal crafting action can never be reduced below 1 second.

The modified duration is used for both the crafting countdown and the completed
item task.

### Factory Production Speed

Factory Production Speed affects the per-item production time of a factory. It
uses the bonus belonging to the character who starts that production run--not
necessarily the factory owner.

When a character activates the factory:

- The factory calculates its native per-item duration.
- The activating character's current speed bonus is applied.
- The result is limited to a minimum of 1 second per item.
- That duration is captured for the entire active production run.

Logging out does not remove the captured bonus. A server restart does not change
the captured production time. Gaining or losing a qualifying badge while the
factory is active does not alter the current run. Stop and restart the factory
to capture the activating character's current bonus again.

### Crafting Amazing Success Chance

This bonus adds percentage points to the native chance of an Amazing Success
during eligible assembly and experimentation rolls. It does not multiply the
native chance.

For example, a native 5% chance plus a custom 10% bonus produces a 15% final
chance. Badge assignments and server configuration are intended to allow a
maximum-bonus character to reach at least a 50% final chance. A candidate value
is +10 percentage points per qualifying badge. The server owner may configure
the badge value and final chance cap.

### Amazing Results

Amazing Results is separate from Amazing Success Chance. Chance determines
whether an Amazing Success occurs. Amazing Results determines how powerful that
successful result is.

Core3 normally limits crafting progress according to resource quality. Merely
adding more progress would not let poor resources produce perfect items because
the extra progress would stop at that resource-derived ceiling. On an actual
Amazing Success, Amazing Results therefore enhances both the result and the
ceiling needed to retain it.

For each affected attribute, the bonus closes the displayed percentage of the
distance between the completed native amazing result and the schematic's
absolute valid maximum:

```text
Native result after Amazing Success: 28%
Perfect schematic value:            100%
Amazing Results strength:            50%
Enhanced result:                     64%
```

The calculation is `28% + ((100% - 28%) x 50%) = 64%`. The resource ceiling for
that attribute rises only enough to retain the 64% result.

- At 0% strength, Amazing Success behaves exactly as it does in native Core3.
- At 50% strength, the result closes half the remaining distance to perfection.
- At 100% strength, even poor resources can produce perfect affected attributes.
- The result cannot exceed the schematic's legitimate absolute maximum.
- Non-amazing results remain fully limited by normal resource quality.

During assembly, Amazing Results applies to all applicable initial attributes.
During experimentation, it applies only to the attributes in the selected row.
Other rows remain unchanged unless they receive their own Amazing Success.

## Buffs and gathering

### Buff Duration

Buff Duration increases the initial duration of these beneficial buff families:

- Doctor and medical buffs
- Entertainer and performance buffs
- Food buffs
- Drink buffs, which Core3 treats as food buffs
- The beneficial up phase of spice
- Positive Jedi and Force buffs

It does not extend negative buffs or spice downer phases. It also excludes
poison, disease, bleeding, fire and other damage-over-time effects; hostile
debuffs; combat states; traps; cooldown and internal control markers; and
single-use or effectively permanent effects.

Skill-item buffs, innate/species buffs, Squad Leader buffs, concealment, gallop,
and vehicle buffs are not included. An eligible duration is modified once when
the buff is initially applied. Explicit renewals apply the same bonus to the
newly supplied native duration; database reload and internal event rescheduling
preserve the stored duration without multiplying it again.

### Gathering Quantity

Gathering Quantity increases resource amounts obtained from two activities:

- Foraging resources, including flora, shellfish, and lair eggs
- Milking creatures

The default bonus is `+200%` per qualifying badge. These percentage bonuses add
together before being applied to the native quantity:

```text
No qualifying badges: 100% of native quantity (1x)
One qualifying badge: 300% of native quantity (3x)
Two qualifying badges: 500% of native quantity (5x)
```

For example, if an activity normally awards 20 resource units, one qualifying
badge increases the result to 60 units. The server owner may change the
per-badge value.

The bonus is applied after the activity completes its normal quantity
calculation. For forage, this means after the native resource-quantity roll. For
milk, it means after the creature's milk amount has been adjusted for local
resource density. The resulting quantity is rounded down to a whole resource
unit and will never be lower than the native result.

Gathering Quantity does not change the chance of successfully foraging or
milking, the type of resource found, or whether a creature can be milked.

Foraged food, bait, medical components, rare items, and live creatures are
discrete loot objects rather than resource quantities. Gathering Quantity does
not duplicate those items.

## Reading future updates

This guide describes the intended player-facing rules. Server configuration may
change bonus values or disable individual systems. When a configured value and
an example in this guide differ, the value displayed by `/customskills` on that
server takes precedence.
