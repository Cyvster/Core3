# Custom Skills -- Menu System (SUI)

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## Architecture

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation

- **C++ owned** (`CustomSkillsMenu`, `CustomSkillsSuiCallback`) -- not Lua screenplay
- **Rationale**: Avoids Lua as second calculation path; queries same typed API as gameplay hooks
- **Entry**: `CustomSkillsCommand` -> Lua bridge `CustomSkills:openMenu(pPlayer)` -> C++ menu creation
- **Character-scoped**: SUI page stored in invoking character's `PlayerObject` SUI map (one player's page cannot resolve from another's)

---

## Main Page Layout

```
Custom Skills

Accumulated Bonuses
+7% Critical Chance
+4 Melee Defense
+2% Movement Speed

Badges
```

- **Accumulated Bonuses**: Only non-zero totals from acquired badges
- **Never shows**: Total-possible values (what you'd get with all badges)
- **Disabled modifiers**: Omitted from active summary (or shown as inactive per config)

---

## Modifier Hierarchy

```
Custom Skills
  Offense
    Critical Chance
    Critical Multiplier
    Double Attack Chance
    Triple Attack Chance
    Quad Attack Chance
    Armor Penetration
  Defense and Equipment
    Defense Cap Increase
    SEA Cap Increase
    Armor Degradation Reduction
    Weapon Degradation Reduction
  Utility
    Movement Speed
    Buff Duration
    Experience Bonus
    Practice Mode Experience Bonus
  Crafting and Gathering
    Crafting Speed
    Crafting Amazing Success Chance
    Amazing Results
    Gathering Quantity
```

**Display rules**:
- Sort by stable configured display order (not badge index)
- Zero-valued categories hidden unless server config requests them
- Disabled mechanics visible but marked inactive

---

## Modifier Detail Page

```
Custom Skills > Offense > Critical Chance

Total                                      +3.00%
Ben Kenobi's Old Home                      +1.00%
Master Pistoleer                           +1.00%
Rebel Corvette: Destroy                    +1.00%
```

- **Total + source rows** from `CustomSkillsModifiers` (same as gameplay)
- **Sort source rows**: Contribution magnitude (desc), then display name
- **Multiple bonuses**: Joined with commas in stable modifier display order

---

## Badge Hierarchy

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

### Category Details

| Category | Contents | Order |
|----------|----------|-------|
| **Milestone Badges** | 7 accumulation milestones (`count_5` through `count_125`) + 5 exploration milestones | Numeric threshold ascending |
| **Exploration** | Milestone Exploration first, then 10 planets alphabetically | Planet alpha; badges within planet alpha by display name |
| **Profession** | 6 families (Combat, Crafting, Outdoors, Science, Social, Pilot) in configured order; professions alpha within family | Configured family order |
| **Quest** | 4 lines (Hero of Tatooine, Warren, Theme Parks, Corellian Corvette) in configured order; badges in story/progression order | Configured line order |
| **Event** | 4 groups (Cries of Alderaan, Accolades, Librarian, Racing, Death Star) in configured order; chronological/progression within multi-stage | Configured group order |

---

## Badge Page Layout

```
Badges > Exploration > Tatooine

Accumulated Bonuses
+3% Critical Chance
+2 Melee Defense

O  Ben Kenobi's Old Home  (+1% Critical Chance)
X  Pool Beneath Fort Tusken  (+1% Critical Chance)
O  Lars Homestead  (+2% Critical Chance, +2 Melee Defense)
```

- **Accumulated Bonuses**: Recursive total from acquired descendant badges
- **Category entry shows**: Acquired descendant badge count: `Tatooine (2)`, `Exploration (12)`, `Badges (20)`
- **Badge rows**: `O` (green, owned) or `X` (red, unowned) prefix; only marker colored
- **Bonus suffix**: `(+X% Modifier)` in module summary color; omitted if no bonus; multiple joined by commas

---

## Aggregation Rules

| Level | Aggregates |
|-------|------------|
| Planet page | Its acquired badges |
| Exploration | All exploration milestones + all planets |
| Profession | All profession families |
| Quest | All quest lines |
| Event | All event groups |
| Badges (root) | All badge categories |

- **Only acquired badges contribute** to category totals
- **Totals from shared modifier service** -- honors stacking, caps, rounding
- **Menu never independently adds** display strings

---

## SUI Navigation Rules

| Control | Behavior |
|---------|----------|
| **OK** (select row) | Opens subpage |
| **Back** | Returns one level |
| **Cancel** | Exits menu completely |
| **Refresh** (badge lists) | Reopens current page (OK without valid selection also refreshes) |

**Stock `Script.listBox` limitation**: Server event fires on button submit, not row selection change.

---

## Multi-Window Support

- Multiple `/customskills` windows can be open simultaneously
- Each character's SUI page stored separately in their `PlayerObject`
- No global `currentPlayer`, selected row, or open-page record in Lua

---

## Safety Requirements (from SCREENPLAYS.md)

- Validate `pPlayer` is player creature in every entry point
- Validate player ghost before reading character data
- Treat cancel, missing args, negative rows, unknown actions as no-op
- Store stable action IDs in SUI row data (not display text)
- Dispatch only through explicit action table (never arbitrary function names)
- Use dedicated SUI window type if module later needs to close/replace only its own window

---

## Implementation Classes

### CustomSkillsMenu
- Creates SUI list boxes for each page
- Builds rows from `CustomSkillsModifiers` data
- Assigns action IDs to row data for callback dispatch
- Sends pages to specific player only

### CustomSkillsSuiCallback
- Handles `SuiCallback` events
- Validates player, event type, arguments
- Dispatches via action table to page handlers
- Manages navigation stack (Back/Cancel/Refresh)

### CustomSkillsCommand
- `/customskills` slash command handler
- Validates caller is player creature
- Invokes Lua `CustomSkills:openMenu(pPlayer)` bridge
- Registered via `CommandConfigManager.cpp` special command hook

---

## Adding a New Menu Page

1. Add modifier to `CustomSkillsModifierType` enum
2. Add display name to `CustomSkillsModifiers::getModifierName()`
3. Add formatting to `formatModifierBonus()`
4. Add category placement in `CustomSkillsMenu` (modifier hierarchy)
5. Add badge assignments in `MENU_SYSTEM.md + BADGE_REFERENCE.md`
6. Add badge keys to `config.lua` modifier table
7. Verify recursive aggregation works for new category

---

## Client-Side Improvements (Deferred)

| Feature | Requires |
|---------|----------|
| Single-click navigation | Client SUI script/template modification |
| Right-click Back | Client modification |
| Reliable double-click submission | Client modification |

Currently deferred to client-mod phase. Stock client uses OK/Back/Cancel buttons only.

---

## Color Scheme

| Element | Color |
|---------|-------|
| Owned badge marker (`O`) | Green (`\#00FF00` via `customSummaryColor`) |
| Unowned badge marker (`X`) | Red |
| Modifier bonus text | `customSummaryColor` (default `00FF00`) |
| Badge names / category text | Default client text color |
| Combat spam labels | Same as native (not overridden) |

---

## Development Labels (Temporary)

| Effect | Label | Status |
|--------|-------|--------|
| Critical hit | `(CRIT)` | Remove after dev |
| Double attack | `(DOUBLE)` | Remove after dev |
| Triple attack | `(TRIPLE)` | Remove after dev |
| Quad attack | `(QUAD)` | Remove after dev |

Configurable via `combatSpamLabel` in `config.lua`. Native damage spam colors untouched.
