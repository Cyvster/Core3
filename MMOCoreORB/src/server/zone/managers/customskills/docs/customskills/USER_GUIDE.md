# Custom Skills -- Player User Guide

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## What Is Custom Skills?

Custom Skills is a server-side addon that grants your character bonuses based on the badges you've earned. Bonuses apply to combat, crafting, gathering, movement, buffs, and experience.

**Open the menu anytime with:**
```
/customskills
```

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation
- ox-alpha (opencode/x-preview-f-free), 08242026 -- feature-section
  restructure; modifier/badge reference material folded in per owner
  directive

## Key Concepts

| Concept | Description |
|---------|-------------|
| **Character-specific** | Badges earned by one character don't affect other characters on your account |
| **Only acquired badges count** | Unowned badges show in the menu but grant zero bonus |
| **Server-controlled** | The server owner decides which bonuses are enabled, which badges grant them, and how large each bonus is |
| **Authoritative source** | Values shown by `/customskills` are the true values for your character on this server |

## Features (server-owner toggles)

Every feature below is individually enable/disable-able by the server
owner. If a feature is disabled on your server, its entry appears
inactive in `/customskills` and has no gameplay effect -- skip its
section here. Enabled state, badge assignments, and values always come
from the server's configuration; what `/customskills` displays is what
your server actually runs.

Each feature section answers three questions: what it does for you,
which badges power it, and how big it can get on a default setup.

---

## Understanding the Menu

### Main Page

```
Custom Skills

Badges
Bonuses
Server Config
```

- **Badges** -- Browse badges by category to see which ones contribute to your totals.
- **Bonuses** -- Your current accumulated totals, grouped by Combat, Utility, and Crafting. Only shows non-zero values.
- **Server Config** -- Server-side toggle state (e.g., Rarity Naming).

### Navigation

| Action | Result |
|--------|--------|
| **OK** (select row) | Opens subpage |
| **Back** | Returns one level |
| **Cancel** | Closes menu completely |
| **Refresh** (on badge lists) | Reopens current page |

**Ownership markers**: Every badge row is prefixed with:
- `O` (green) = You own this badge
- `X` (red) = You don't own this badge

Badge names and bonus text use normal color; only the marker is colored.

---

## Badge Categories

```
Badges
  Milestone Badges
  Exploration
    Milestone Exploration
    Corellia
    Dantooine
    Dathomir
    Endor
    Lok
    Naboo
    Rori
    Talus
    Tatooine
    Yavin IV
  Profession
  Quest
  Event
```

### Category Totals

Each category page shows **Accumulated Bonuses** from all acquired descendant badges (recursive). Example:

```
Badges > Exploration > Tatooine

Accumulated Bonuses
+50% Movement Speed

O  Ben Kenobi's Old Home  (+25% Movement Speed)
X  Pool Beneath Fort Tusken  (+25% Movement Speed)
O  Lars Homestead  (+25% Movement Speed)
```

- Only **acquired** badges contribute to category totals
- Totals use the same math as gameplay (stacking, caps, rounding)
- Category entries show acquired badge count: `Tatooine (2)`, `Exploration (12)`, `Badges (20)`

---

# Feature Sections

## Offense

### Critical Chance

- **What it does**: Gives landed attacks a chance to become critical hits, dealing bonus damage.
- **Powered by**: 60 achievement badges across the whole game -- all badge-count milestones (7 accumulation + 5 exploration), all 12 combat profession masteries, Hero of Tatooine (5), Warren (2), Theme Parks (4), Corellian Corvette (9), dangerous exploration sites (5), Jedi sites (3), Outdoors masteries (5), and Science masteries (3).
- **Typical maximum**: +60% critical chance with the full set.
- **Combat spam**: `(CRIT)` appears when it triggers (on its own combat message line; server owner can turn labels off).

### Critical Multiplier

- **What it does**: Increases damage dealt BY critical hits (on top of the base 150%).
- **Powered by**: Same 12 milestone badges as Critical Chance.
- **Typical maximum**: up to 300% total crit damage.

### Double Attack Chance

- **What it does**: Chance for an attack to strike twice.
- **Powered by**: Warren (2 badges), Theme Parks (4 badges).
- **Typical maximum**: +100% chance (always double).
- **Combat spam**: `(DOUBLE)`.

### Triple Attack Chance

- **What it does**: Chance for an attack to strike three times.
- **Powered by**: All 12 combat profession masteries (Bounty Hunter counts extra), plus the 5 Hero of Tatooine quest badges.
- **Typical maximum**: +85%.
- **Combat spam**: `(TRIPLE)`.

### Quad Attack Chance

- **What it does**: Chance for an attack to strike four times.
- **Powered by**: Same set as Triple Attack.
- **Typical maximum**: +51% (with the full 17-badge set).
- **Combat spam**: `(QUAD)`.

**How repeats interact**: Quad is checked first, then Triple, then Double -- only one tier triggers per hit. Repeats apply after critical damage adjustment. Armor Penetration (below) applies before the native armor comparison.

### Armor Penetration

- **What it does**: Each point lowers the target's effective armor one level (Heavy -> Medium -> Light -> None). Never drops below No Armor; does not change the weapon's Armor Piercing rating.
- **Powered by**: The 9 Corellian Corvette badges.
- **Typical maximum**: 3 levels.

## Defense & Equipment

### Defense Cap Increase

- **What it does**: Raises the normal 125 hard cap on primary/secondary defense calculations. Does not affect attachment/tape bonuses.
- **Powered by**: The 5 Hero of Tatooine badges only, +20 defense points each.
- **Typical maximum**: +100 defense points.

### SEA Cap Increase

- **What it does**: Raises the wearable-attachment ("SEA"/tape) contribution cap above the native +25. Independent of Defense Cap.
- **Powered by**: Tatooine dangerous sites (4) and the Dathomir Sarlacc.
- **Typical maximum**: +100 SEA cap points.
- **Typical maximum**: +100 SEA cap points.

### Armor Degradation Reduction

- **What it does**: Lowers the chance that worn armor loses condition on use (native chance is 100%). Covers armor, PSGs, NPC armor, vehicle armor.
- **Powered by**: Talus exploration (4) and Rori exploration (4).

### Weapon Degradation Reduction

- **What it does**: Reduces weapon degradation event chance multiplicatively. Condition loss amount per event is unchanged.
- **Powered by**: Dantooine exploration (4) and Endor exploration (4).

## Character & Utility

### Movement Speed

- **What it does**: Multiplies your final movement speed everywhere -- on foot, mounted, and in vehicles (applied once, never doubled).
- **Powered by**: Naboo exploration (4 badges).
- **Typical maximum**: +100% speed.

### Buff Duration

- **What it does**: Extends beneficial buffs only -- medical, entertainer, food/drink, spice-up, positive Jedi/Force buffs. Does NOT extend debuffs, DoTs, states, traps, cooldowns, innates, Squad Leader, concealment, gallop, or vehicle buffs.
- **Powered by**: Tatooine easy (3), Yavin IV Woolamander/Blueleaf (2), Science masteries (3), Social masteries (5).
- **Typical maximum**: +325% buff duration.

### Experience Bonus

- **What it does**: Multiplies ALL positive XP you earn (combat, crafting, harvesting, quests...). Stacks multiplicatively with server and buff rates: 100 XP x 2x server x 5x character = 1,000 XP (10x total, not 7x).
- **Powered by**: Corellia exploration (5 badges).
- **Typical maximum**: +500%.

### Practice Mode XP Bonus

- **What it does**: Boosts XP from practicing crafts (the no-item mode). Applies after the native 5% practice bonus, before general Experience multipliers.
- **Powered by**: Tatooine easy (3), Yavin IV (2), Social masteries (5).

## Crafting

### Crafting Speed

- **What it does**: Speeds up crafting sessions (duration divided by your bonus, minimum 1 second). In factories, the run snapshots the STARTING player's bonus -- it persists through logout/restart and doesn't change mid-run.
- **Powered by**: All 9 crafting masteries (plus Shipwright) and Doctor mastery.

### Amazing Success Chance

- **What it does**: Adds percentage points to the natural Amazing Success roll during assembly and experimentation.
- **Powered by**: Exar Kun's Temple, Lok dangerous (3), Yavin IV Woolamander/Blueleaf (2).

### Amazing Results

- **What it does**: When an Amazing Success actually happens, each affected attribute lands closer to the schematic cap -- at high bonuses even poor resources can produce near-perfect results. Assembly affects all attributes; experimentation affects the selected line only.
- **Powered by**: Same badges as Amazing Success Chance.

## Gathering

### Gathering Quantity

- **What it does**: Multiplies resource quantity from foraging (flora/shellfish/lair eggs) and milking. Every badge adds its own yield bonus (+77% to +100%) before the multiply -- full set reaches roughly 14x native yields. Never reduces yields, rounded down. Does not affect discrete loot (food, bait, rare items, live creatures).
- **Powered by**: Outdoors masteries (5), Science masteries (3), Tatooine easy (3), Yavin IV (2) -- all at uniform rate.

## Rarity Naming

- **What it does**: Instead of appending "(Exceptional)"/"(Legendary)" to crafted item names, item names display in color (blue/purple by default). Also fixes quest turn-ins that match items by exact name -- suffixes break those matches.
- **Toggle**: Fully independent of badge bonuses; configured by the server owner.

---

## Percentage vs Multiplier

| Format | Meaning |
|--------|---------|
| `+3% Critical Chance` | Adds 3 percentage points to the chance |
| `5x Experience` | Multiplies the applicable value. Missing/disabled = `1x` (no change) |

## Important Notes

- **Pilot/JTL badges**: Grant no bonuses while Jump to Lightspeed is unavailable
- **Double/Triple/Quad Attack**: Listed in menu and configuration; the repeat-tier proc is not yet implemented server-side ([ERR-009](../rules/errata.md))
- **Hidden badges** (`show = 0`): Still visible in `/customskills` if they contribute to your totals (so totals stay explained)
- **Disabled features**: Show in menu as inactive but don't affect gameplay
- **Config changes**: Require server restart. Values you see in `/customskills` always match what the server is actually using

## Quick Reference: Badge -- Bonus Assignments

*Complete catalog with exact badge keys lives in
[CODE_REFERENCE.md](CODE_REFERENCE.md) -> Appendix B (developer-facing).*

| Badge Group | Example Bonuses |
|-------------|-----------------|
| **Milestones (12)** | +1% Crit Chance, +12.5% Crit Multiplier each |
| **Combat Mastery (12)** | +2% Triple, +2% Quad each (Bounty Hunter: +3%/+3%) |
| **Crafting Mastery (9)** | +10% Crafting Speed each |
| **Outdoors Mastery (5)** | +115% Gathering Quantity each |
| **Science Mastery (3)** | +100% Gathering, +25% Buff Duration (Doctor also +10% Crafting Speed) |
| **Social Mastery (5)** | +20% Buff Duration, +100% Practice XP each |
| **Corellian Corvette (9)** | +1 Armor Penetration each |
| **Hero of Tatooine (5)** | +10% Triple, +5% Quad, +10 Def Cap total |
| **Theme Parks (4)** | +20% Double Attack each |
| **Warren (2)** | +20% Double Attack each |

## Reading Updates

This guide describes intended player-facing rules. **Server configuration always takes precedence**. When a configured value differs from examples here, trust what `/customskills` shows on your server.
