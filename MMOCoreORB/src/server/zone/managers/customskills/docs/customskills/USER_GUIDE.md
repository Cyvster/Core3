# Custom Skills -- Player User Guide
*Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.*

## What Is Custom Skills?
Custom Skills is a server-side addon that grants your character bonuses based on the badges you've earned. Bonuses apply to combat, crafting, gathering, movement, buffs, and experience.
**Open the menu anytime with:** `/customskills`

## Key Concepts
**Character-specific** — badges earned by one character don't affect other characters on your account.  
**Only acquired badges count** — unowned badges show in the menu but grant zero bonus.  
**Server-controlled** — the server owner decides which bonuses are enabled, which badges grant them, and how large each bonus is.  
**Authoritative source** — values shown by `/customskills` are the true values for your character on this server.

## Features (server-owner toggles)
Every feature below is individually enable/disable-able by the server owner. If a feature is disabled on your server, its entry appears inactive in `/customskills` and has no gameplay effect. Enabled state, badge assignments, and values always come from the server's configuration; what `/customskills` displays is what your server actually runs. Each feature section answers three questions: what it does for you, which badges power it, and how big it can get on a default setup.

## Understanding the Menu
**Main Page:**
```
Custom Skills
Badges
Bonuses
Server Config
```
**Badges** — browse badges by category to see which ones contribute to your totals.  
**Bonuses** — your current accumulated totals, grouped by Combat, Utility, and Crafting. Only shows non-zero values.  
**Server Config** — server-side toggle state (e.g., Rarity Naming).  
**Navigation:** OK (select row) opens subpage; Back returns one level; Cancel closes menu; Refresh (on badge lists) reopens current page.  
**Ownership markers:** every badge row is prefixed with `O` (green) = you own this badge, or `X` (red) = you don't. Badge names and bonus text use normal color; only the marker is colored.

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
**Category Totals:** each category page shows Accumulated Bonuses from all acquired descendant badges (recursive). Example:
```
Badges > Exploration > Tatooine
Accumulated Bonuses
+50% Movement Speed
O  Ben Kenobi's Old Home  (+25% Movement Speed)
X  Pool Beneath Fort Tusken  (+25% Movement Speed)
O  Lars Homestead  (+25% Movement Speed)
```
Only **acquired** badges contribute to category totals. Totals use the same math as gameplay (stacking, caps, rounding). Category entries show acquired badge count: `Tatooine (2)`, `Exploration (12)`, `Badges (20)`.

## Feature Sections

### Offense
**Critical Chance** — gives landed attacks a chance to become critical hits, dealing bonus damage. Powered by: 60 achievement badges across the whole game (all badge-count milestones 7+5, all 12 combat profession masteries, Hero of Tatooine 5, Warren 2, Theme Parks 4, Corellian Corvette 9, dangerous exploration sites 5, Jedi sites 3, Outdoors masteries 5, Science masteries 3). Typical maximum: +60% critical chance with the full set.
**Critical Multiplier** — increases damage dealt BY critical hits (on top of base 150%). Powered by: same 12 milestone badges as Critical Chance. Typical maximum: up to 300% total crit damage.
**Double Attack Chance** — chance for an attack to strike twice. Powered by: Warren (2), Theme Parks (4). Typical maximum: +100% (always double).
**Triple Attack Chance** — chance for an attack to strike three times. Powered by: all 12 combat profession masteries + 5 Hero of Tatooine quest badges. Typical maximum: +85%.
**Quad Attack Chance** — chance for an attack to strike four times. Powered by: same set as Triple Attack. Typical maximum: +51% (full 17-badge set).  
*How tiers work:* Quad is checked first, then Triple, then Double — only one tier triggers per hit. Tiers apply after critical damage adjustment. Armor Penetration (below) applies before the native armor comparison.
**Consolidated Strike** — Double/Triple/Quad no longer land as several separate hits: each tier delivers ONE bigger hit (x2/x3/x4 damage) with escalating flytext (bigger and brighter — yellow → orange → red, gold on crits) and an `x2`/`x3`/`x4` tag in your combat spam. Total damage is the same as the old repeat hits; armor just mitigates the single big hit instead of each small one.
**Armor Penetration** — each point lowers the target's effective armor one level (Heavy → Medium → Light → None). Never drops below No Armor; does not change the weapon's Armor Piercing rating. Powered by: the 9 Corellian Corvette badges. Typical maximum: 3 levels.

### Defense & Equipment
**Defense Cap Increase** — raises the normal 125 hard cap on primary/secondary defense calculations; does not affect attachment/tape bonuses. Powered by: the 5 Hero of Tatooine badges only, +20 defense points each. Typical maximum: +100 defense points.
**SEA Cap Increase** — raises the wearable-attachment ("SEA"/tape) contribution cap above the native +25; independent of Defense Cap. Powered by: Tatooine dangerous sites (4) and the Dathomir Sarlacc. Typical maximum: +100 SEA cap points.
**Armor Degradation Reduction** — lowers the chance worn armor loses condition on use (native 100%); covers armor, PSGs, NPC armor, vehicle armor. Powered by: Talus exploration (4) and Rori exploration (4).
**Weapon Degradation Reduction** — reduces weapon degradation event chance multiplicatively; condition loss amount per event unchanged. Powered by: Dantooine exploration (4) and Endor exploration (4).

### Character & Utility
**Movement Speed** — multiplies final movement speed everywhere (on foot, mounted, vehicles); applied once, never doubled. Powered by: Naboo exploration (4). Typical maximum: +100% speed.
**Buff Duration** — extends beneficial buffs only (medical, entertainer, food/drink, spice-up, positive Jedi/Force); does NOT extend debuffs, DoTs, states, traps, cooldowns, innates, Squad Leader, concealment, gallop, or vehicle buffs. Powered by: Tatooine easy (3), Yavin IV Woolamander/Blueleaf (2), Science masteries (3), Social masteries (5). Typical maximum: +325% buff duration.
**Experience Bonus** — multiplies ALL positive XP (combat, crafting, harvesting, quests); stacks multiplicatively with server and buff rates: 100 XP × 2x server × 5x character = 1,000 XP (10x total, not 7x). Powered by: Corellia exploration (5). Typical maximum: +500%.
**Practice Mode XP Bonus** — boosts XP from practicing crafts (no-item mode); applies after native 5% practice bonus, before general Experience multipliers. Powered by: Tatooine easy (3), Yavin IV (2), Social masteries (5).

### Crafting
**Crafting Speed** — speeds up crafting sessions (duration divided by bonus, minimum 1s); in factories the run snapshots the STARTING player's bonus — persists through logout/restart, doesn't change mid-run. Powered by: all 9 crafting masteries (plus Shipwright) and Doctor mastery.
**Amazing Success Chance** — adds percentage points to the natural Amazing Success roll during assembly and experimentation. Powered by: Exar Kun's Temple, Lok dangerous (3), Yavin IV Woolamander/Blueleaf (2).
**Amazing Results** — when an Amazing Success happens, each affected attribute lands closer to schematic cap; at high bonuses even poor resources produce near-perfect results. Assembly affects all attributes; experimentation affects the selected line only. Powered by: same badges as Amazing Success Chance.

### Gathering
**Gathering Quantity** — multiplies resource quantity from foraging (flora/shellfish/lair eggs) and milking; every badge adds its own yield bonus (+77% to +100%) before the multiply — full set reaches ~14x native yields; never reduces yields, rounded down; does not affect discrete loot (food, bait, rare items, live creatures). Powered by: Outdoors masteries (5), Science masteries (3), Tatooine easy (3), Yavin IV (2) — all uniform rate.

### Rarity Naming
Replaces "(Exceptional)"/"(Legendary)" suffixes on crafted item names with color (blue/purple by default); also fixes quest turn-ins that match items by exact name (suffixes break those matches). Toggle is fully independent of badge bonuses; configured by the server owner.

## Percentage vs Multiplier
`+3% Critical Chance` — adds 3 percentage points to the chance.  
`5x Experience` — multiplies the applicable value; missing/disabled = `1x` (no change).

## Important Notes
**Pilot/JTL badges** — grant no bonuses while Jump to Lightspeed is unavailable.  
**Double/Triple/Quad Attack** — listed in menu and config; the repeat-tier proc is not yet implemented server-side ([ERR-009](../rules/errata.md)).  
**Hidden badges** (`show = 0`) — still visible in `/customskills` if they contribute to your totals (so totals stay explained).  
**Disabled features** — show in menu as inactive but don't affect gameplay.  
**Config changes** — require server restart; values you see in `/customskills` always match what the server is actually using.

## Quick Reference: Badge -- Bonus Assignments
*Complete catalog with exact badge keys lives in [CODE_REFERENCE.md](CODE_REFERENCE.md) → Appendix B (developer-facing).*
**Milestones (12)** — +1% Crit Chance, +12.5% Crit Multiplier each.  
**Combat Mastery (12)** — +5% Triple, +3% Quad each (uniform).  
**Crafting Mastery (9)** — +10% Crafting Speed each.  
**Outdoors Mastery (5)** — +100% Gathering Quantity each.  
**Science Mastery (3)** — +100% Gathering, +25% Buff Duration (Doctor also +10% Crafting Speed).  
**Social Mastery (5)** — +25% Buff Duration, +100% Practice XP each.  
**Corellian Corvette (9)** — +1 Armor Penetration each.  
**Hero of Tatooine (5)** — +5% Triple, +3% Quad, +20 Def Cap total.  
**Theme Parks (4)** — +20% Double Attack each.  
**Warren (2)** — +20% Double Attack each.

## Reading Updates
This guide describes intended player-facing rules. **Server configuration always takes precedence** — when a configured value differs from examples here, trust what `/customskills` shows on your server.

## Contributors
**Nemotron 3.5 Lightning Free (AI)** — initial creation.  
ox-alpha (opencode/x-preview-f-free), 08242026 — feature-section restructure; modifier/badge reference folded in per owner directive.  
hy3-free (opencode/hy3-free), 08242026 — BRIEF-014 doc removals (combat spam label references); BRIEF-020 dense-format pass.
*Last reconciled 08242026 by hy3-free (opencode/hy3-free) — BRIEF-020.*

## SWGEMU Options page

Menu path Server Config > SWGEMU Options shows a read-only list of notable Core3 server options with live values (green ENABLED / red DISABLED for bools); entries marked "(restart required)" only change after a server restart. Full inventory: customskills/CONFIG_OPTIONS.md.
