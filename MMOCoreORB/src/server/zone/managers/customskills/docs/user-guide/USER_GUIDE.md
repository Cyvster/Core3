# Custom Skills — Player User Guide

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## What Is Custom Skills?

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

Custom Skills is a server-side addon that grants your character bonuses based on the badges you've earned. Bonuses apply to combat, crafting, gathering, movement, buffs, and experience.

**Open the menu anytime with:**
```
/customskills
```

---

## Key Concepts

| Concept | Description |
|---------|-------------|
| **Character-specific** | Badges earned by one character don't affect other characters on your account |
| **Only acquired badges count** | Unowned badges show in the menu but grant zero bonus |
| **Server-controlled** | The server owner decides which bonuses are enabled, which badges grant them, and how large each bonus is |
| **Authoritative source** | Values shown by `/customskills` are the true values for your character on this server |

---

## Understanding the Menu

### Main Page

```
Custom Skills

Badges
Bonuses
Server Config
```

- **Badges** — Browse badges by category to see which ones contribute to your totals.
- **Bonuses** — Your current accumulated totals, grouped by Combat, Utility, and Crafting. Only shows non-zero values.
- **Server Config** — Server-side toggle state (e.g., Rarity Naming).

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

## Modifier Categories & Mechanics

### Offense

| Modifier | What It Does | Default |
|----------|--------------|---------|
| **Critical Chance** | Chance for a landed attack to become a critical hit | +1% per milestone exploration badge (max +12%) |
| **Critical Multiplier** | Damage multiplier on critical hits | 150% (1.5x damage) |
| **Double/Triple/Quad Attack** | Repeats finalized damage 2/3/4 times total | Enabled — combat masteries +2% Triple/Quad (Bounty Hunter +3%); Theme Parks/Warren +20% Double |
| **Armor Penetration** | Lowers target's effective armor by 1 level per point | Enabled — +1 level per Corellian Corvette badge |

**Critical hits**: Checked first, then repeat tiers (Quad — Triple — Double, only one triggers). Repeats apply to critical-adjusted damage.

**Armor Penetration**: Heavy — Medium — Light — No Armor. Floor: No Armor. Does not affect weapon's Armor Piercing.

### Defense & Equipment

| Modifier | What It Does |
|----------|--------------|
| **Defense Cap Increase** | Raises the normal 125 cap on primary/secondary defense calculations |
| **SEA Cap Increase** | Raises the wearable (attachments/tapes) contribution cap above +25 |
| **Armor Degradation Reduction** | Lowers chance of armor condition loss events (native 100% chance) |
| **Weapon Degradation Reduction** | Lowers weapon degradation event chance multiplicatively |

**SEA Cap vs Defense Cap**: Independent. SEA Cap only affects attachment/tape contributions; Defense Cap affects primary/secondary defense totals.

### Utility

| Modifier | What It Does |
|----------|--------------|
| **Movement Speed** | Multiplies final native speed (on foot, mounted, in vehicle) — applied once, not doubled |
| **Buff Duration** | Extends beneficial buffs only (doctor, entertainer, food/drink, spice-up, positive Jedi/Force). Excludes debuffs, DoTs, states, traps, cooldowns, skill-item/innate/Squad Leader/concealment/gallop/vehicle buffs |
| **Experience Bonus** | Multiplies **all positive XP awards** (combat, crafting, harvesting, quests, etc.). Stacks multiplicatively with server/buff/species rates. Example: 100 XP — 2x server — 5x character = 1,000 XP (10x total, not 7x) |
| **Practice Mode XP Bonus** | Applies after Core3's native 5% practice bonus, before general XP multipliers |

### Crafting & Factories

| Modifier | What It Does |
|----------|--------------|
| **Crafting Speed** | Divides native duration by your multiplier (min 1 second). Personal: uses crafter. Factory: snapshots **activator's** bonus at start — survives logout/restart, doesn't change mid-run |
| **Amazing Success Chance** | Adds percentage points to native Amazing Success chance (assembly & experimentation). Does not multiply. Target: max-bonus character can reach =50% |
| **Amazing Results** | On actual Amazing Success: closes % of distance between native result and schematic cap, raises resource ceiling to retain it. At 100%, poor resources can yield perfect attributes. Assembly: all attributes. Experimentation: selected row only |

### Gathering

| Modifier | What It Does |
|----------|--------------|
| **Gathering Quantity** | Multiplies resource quantity from foraging (flora/shellfish/lair eggs) and milking. Default +200% per badge (additive: 1 badge = 3x, 2 badges = 5x). Rounded down, never below native. Does not affect discrete loot (food, bait, rare items, live creatures) |

---

## Percentage vs Multiplier

| Format | Meaning |
|--------|---------|
| `+3% Critical Chance` | Adds 3 percentage points to the chance |
| `5x Experience` | Multiplies the applicable value. Missing/disabled = `1x` (no change) |

---

## Important Notes

- **Pilot/JTL badges**: Grant no bonuses while Jump to Lightspeed is unavailable
- **Hidden badges** (`show = 0`): Still visible in `/customskills` if they contribute to your totals (so totals stay explained)
- **Disabled modifiers**: Show in menu as inactive but don't affect gameplay
- **Config changes**: Require server restart. Values you see in `/customskills` always match what the server is actually using

---

## Quick Reference: Badge — Bonus Assignments

*See `BADGE_REFERENCE.md` for the full list. Highlights:*

| Badge Group | Example Bonuses |
|-------------|-----------------|
| **Combat Mastery (12)** | +2% Triple, +2% Quad each (Bounty Hunter: +3% Triple/Quad); no longer grants Critical Chance |
| **Crafting Mastery (9)** | +10% Crafting Speed each |
| **Outdoors Mastery (5)** | +115% Gathering Quantity each |
| **Science Mastery (3)** | +100% Gathering, +25% Buff Duration (Doctor also +10% Crafting Speed) |
| **Social Mastery (5)** | +20% Buff Duration, +100% Practice XP each |
| **Corellian Corvette (9)** | +1 Armor Penetration each |
| **Hero of Tatooine (5)** | +2% Triple, +2% Quad, +5 Defense Cap each |
| **Theme Parks (4)** | +20% Double Attack each |
| **Warren (2)** | +20% Double Attack each |

---

## Reading Updates

This guide describes intended player-facing rules. **Server configuration always takes precedence**. When a configured value differs from examples here, trust what `/customskills` shows on your server.
