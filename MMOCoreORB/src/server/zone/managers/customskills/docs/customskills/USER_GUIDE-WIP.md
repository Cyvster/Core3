# Custom Skills -- Player User Guide
*Subordinate to project-design.md -- the master document for project rules.*

## What Is Custom Skills?
Server-side addon granting character bonuses based on earned badges. Bonuses: combat, crafting, gathering, movement, buffs, XP. Open menu: `/customskills`
## Key Concepts
**Character-specific** -- badges are per-character, not per-account. **Only acquired badges count** -- unowned badges show but grant zero. **Server-controlled** -- owner decides which bonuses are enabled, which badges grant them, and bonus sizes. **Authoritative** -- `/customskills` shows true values for your character.
## Menu Navigation
`/customskills` opens: Badges (browse by category), Bonuses (your totals: Combat/Utility/Crafting, non-zero only), Server Config (toggle state). Navigation: OK=subpage, Back=one level, Cancel=close, Refresh=reopen. Ownership markers: `O`=green=owned, `X`=red=not owned. Badge names and bonus text use normal color; only the marker is colored.
## Badge Categories
Tree: Badges > Milestone Badges | Exploration > Milestone Exploration, Corellia, Dantooine, Dathomir, Endor, Lok, Naboo, Rori, Talus, Tatooine, Yavin IV | Profession | Quest | Event. Category pages show accumulated bonuses from all acquired descendant badges (recursive). Only acquired badges contribute. Totals use same math as gameplay (stacking, caps, rounding). Entries show count: `Tatooine (2)`, `Exploration (12)`.
## Features
Every feature is individually toggleable by server owner. Disabled features appear inactive in menu with no gameplay effect.

### Offense
**Critical Chance** -- landed attacks can crit for bonus damage. 60 achievement badges. Max: +60%. **Critical Multiplier** -- increases crit damage (on top of base 150%). Same 12 milestones. Max: 300%. **Double Attack** -- chance to strike twice. Warren (2), Theme Parks (4). Max: +100%. **Triple Attack** -- chance to strike three times. 12 combat masteries + Hero of Tatooine (5). Max: +85%. **Quad Attack** -- chance to strike four times. Same set as Triple. Max: +51%. Tiers: Quad checked first, then Triple, then Double -- only one triggers per hit. **Consolidated Strike** -- multi-hit delivered as ONE bigger hit (x2/x3/x4) with escalating flytext (yellow > orange > red, gold on crits) and `x2`/`x3`/`x4` tag. Same total damage; armor mitigates once. **Armor Penetration** -- lowers target effective armor (Heavy > Medium > Light > None). Never below No Armor. 9 Corellian Corvette badges. Max: 3 levels.

### Defense & Equipment
**Defense Cap Increase** -- raises 125 hard cap on primary/secondary defense. 5 Hero of Tatooine badges, +20 each. Max: +100. **SEA Cap Increase** -- raises attachment/tape cap above native +25. Tatooine dangerous (4) + Dathomir Sarlacc. Max: +100. **Armor Degradation Reduction** -- lowers chance armor loses condition. Talus (4) + Rori (4). **Weapon Degradation Reduction** -- reduces weapon degradation chance multiplicatively. Dantooine (4) + Endor (4).

### Character & Utility
**Movement Speed** -- multiplies final speed everywhere (foot/mounted/vehicles), applied once. Naboo (4). Max: +100%. **Buff Duration** -- extends beneficial buffs only (medical, entertainer, food/drink, spice-up, positive Jedi/Force). NOT debuffs, DoTs, states, traps, cooldowns, innates, Squad Leader, concealment, gallop, vehicle. Tatooine easy (3), Yavin IV (2), Science (3), Social (5). Max: +325%. **Experience Bonus** -- multiplies ALL positive XP. Stacks multiplicatively: 100 x 2x server x 5x char = 1,000 (10x total). Corellia (5). Max: +500%. **Practice Mode XP Bonus** -- boosts practice craft XP (no-item mode). Applied after native 5%, before general XP multipliers. Tatooine easy (3), Yavin IV (2), Social (5).

### Crafting
**Crafting Speed** -- divides crafting session duration (min 1s). In factories, run snapshots STARTING player's bonus -- persists through logout/restart. 9 crafting masteries + Shipwright + Doctor. **Amazing Success Chance** -- adds percentage points to Amazing Success roll (assembly + experimentation). Exar Kun's Temple, Lok dangerous (3), Yavin IV (2). **Amazing Results** -- on Amazing Success, attributes land closer to schematic cap. Assembly affects all attributes; experimentation affects selected line only. Same badges as Chance.

### Gathering
**Gathering Quantity** -- multiplies resource quantity from foraging/milking. Every badge adds yield bonus (+77% to +100%) before multiply -- full set reaches ~14x native. Never reduces. Does not affect discrete loot. Outdoors (5), Science (3), Tatooine easy (3), Yavin IV (2).

### Rarity Naming
Replaces "(Exceptional)"/"(Legendary)" suffixes with color (blue/purple). Fixes quest turn-ins matching by exact name. Independent of badge bonuses.
## Percentage vs Multiplier
`+3% Critical Chance` -- adds 3 percentage points. `5x Experience` -- multiplies value; missing/disabled = `1x`.
## Important Notes
**Pilot/JTL badges** -- no bonuses while JTL unavailable. **Hidden badges** (`show=0`) -- visible in `/customskills` if they contribute to totals. **Disabled features** -- show inactive, no gameplay effect. **Config changes** -- `[dyn]` keys hot-reload without restart (values always match server); other keys need restart.
## Quick Reference: Badge -- Bonus Assignments
*Full catalog: CODE_REFERENCE.md Appendix B.*
Milestones (12): +1% Crit Chance, +12.5% Crit Multiplier each. Combat Mastery (12): +5% Triple, +3% Quad each. Crafting Mastery (9): +10% Crafting Speed each. Outdoors Mastery (5): +100% Gathering each. Science Mastery (3): +100% Gathering, +25% Buff Duration (Doctor also +10% Crafting). Social Mastery (5): +25% Buff Duration, +100% Practice XP each. Corellian Corvette (9): +1 Armor Pen each. Hero of Tatooine (5): +5% Triple, +3% Quad, +20 Def Cap. Theme Parks (4): +20% Double each. Warren (2): +20% Double each.
## Server Config
`/customskills` > Server Config > SWGEMU Options: read-only list of Core3 server options with live values (green ENABLED / red DISABLED for bools). Entries marked "(restart required)" change after restart. Full inventory: CONFIG_OPTIONS.md.
## Mission Terminal Options
Right-click mission terminal to set Mission Direction (spawn direction from city) and Mission Difficulty (ten tiers, very easy to Hardest). Saved per character, applies to new missions from any general/faction terminal. Set to "vanilla" for normal random missions.
## Reading Updates
This guide describes intended player-facing rules. **Server configuration always takes precedence** -- trust what `/customskills` shows on your server.
## Contributors
Nemotron 3.5 Lightning Free (AI) -- initial creation. ox-alpha (opencode/x-preview-f-free), 08242026 -- feature-section restructure. hy3-free (opencode/hy3-free), 08242026 -- doc removals, dense-format pass.
