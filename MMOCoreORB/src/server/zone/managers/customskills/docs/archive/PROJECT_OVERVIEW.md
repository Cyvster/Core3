# Custom Skills -- Project Overview

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

## What Is This?

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** -- Initial creation

Custom Skills is a **self-contained SWGEmu Core3 module** that adds badge-derived character bonuses across multiple gameplay systems. It provides:

1. **Gameplay modifiers** -- 17 bonus types affecting combat, crafting, gathering, movement, buffs, XP
2. **Configuration system** -- Server owners enable/disable modifiers and assign badge values via Lua
3. **In-game menu** -- `/customskills` SUI showing character-specific totals and badge-source breakdowns
4. **Core3 integration** -- 19 minimal delegation hooks at stable lifecycle boundaries

**Current status**: All 17 modifiers implemented with hooks; only Critical Chance enabled by default.

---

## Repository Structure

```
customskills-mod/
|-- package/                          # Distributable module (mirrors Core3 paths)
|   `-- MMOCoreORB/
|       |-- bin/scripts/
|       |   |-- commands/customSkills.lua      # Command registration
|       |   `-- customskills/config.lua        # Server configuration
|       `-- src/server/zone/managers/customskills/
|           |-- combat/         # Combat hooks (crit, repeat-damage, armor pen, defense cap)
|           |-- buffs/          # Buff duration eligibility & calculation
|           |-- crafting/       # Crafting speed, amazing success/results, practice XP
|           |-- durability/     # Armor/weapon degradation reduction
|           |-- gathering/      # Forage & milk quantity
|           |-- movement/       # Client & server movement speed
|           |-- progression/    # Experience multiplier
|           |-- skillmods/      # SEA cap, visible skill mods
|           |-- CustomSkillsCommand.h/.cpp      # /customskills command
|           |-- CustomSkillsConfig.h/.cpp       # Singleton config loader
|           |-- CustomSkillsModifiers.h/.cpp    # Central modifier authority
|           |-- CustomSkillsModifierType.h      # 17-type enum
|           |-- CustomSkillsMenu.h/.cpp         # C++ SUI menu
|           |-- CustomSkillsSuiCallback.h/.cpp  # SUI event handling
|           `-- MENU_DESIGN.md                  # Menu design notes
|-- integration/
|   |-- INSTALL.md              # Install/remove/verify instructions
|   `-- core3-hooks.patch       # 19-file patch for Core3 integration
|-- docs/
|   |-- rules/                     # Governing docs: design master, process, errata
|   |-- reference/                 # Developer documentation (this folder)
|   |-- tracking/                  # Objectives & task state (living tracker)
|   |-- briefs/                    # Delegable task queue
|   |-- installation/              # Installation guide + how-it-works
|   |-- user-guide/                # Player-facing mechanics guide
|   |-- feature-planning/          # Active feature design (Challenge Tier)
|   `-- archive/                   # Superseded documents (historical)
|-- MANIFEST.md                 # Every distributed file & integration point
`-- README.md                   # Module summary & directory roles
```

---

## Core Design Principles

| # | Principle | Implementation |
|---|-----------|----------------|
| 1 | **Isolation** | Module-owned code in dedicated `customskills/` directories |
| 2 | **Minimal Core3 edits** | Only generic delegation hooks at stable boundaries |
| 3 | **Single source of truth** | `CustomSkillsModifiers` serves both gameplay hooks AND menu |
| 4 | **Table-driven** | Badge->modifier mappings in Lua config, not hardcoded C++ |
| 5 | **Basis points** | 100 = 1.00% for precision; convert to display only at UI boundary |
| 6 | **No new persistence** | Reads existing badge bitmasks on `PlayerObject` directly |
| 7 | **Stable public API** | `/customskills` and `CustomSkills:openMenu()` are stable |

---

## 17 Modifier Types

| Category | Modifiers (enum order) |
|----------|------------------------|
| **Offense** | `CRITICAL_CHANCE`, `DOUBLE_ATTACK_CHANCE`, `TRIPLE_ATTACK_CHANCE`, `QUAD_ATTACK_CHANCE`, `ARMOR_PENETRATION` |
| **Defense** | `DEFENSE_CAP_INCREASE`, `ARMOR_DEGRADE_REDUCTION`, `WEAPON_DEGRADE_REDUCTION`, `SEA_CAP_INCREASE` |
| **Utility** | `MOVEMENT_SPEED`, `BUFF_DURATION`, `EXPERIENCE_MULTIPLIER`, `PRACTICE_EXPERIENCE_BONUS` |
| **Crafting** | `CRAFTING_SPEED`, `AMAZING_SUCCESS_CHANCE`, `AMAZING_RESULTS` |
| **Gathering** | `GATHERING_QUANTITY` |

All use **basis points** except: `ARMOR_PENETRATION` (whole levels), `DEFENSE_CAP_INCREASE` (points), `SEA_CAP_INCREASE` (points), `EXPERIENCE_MULTIPLIER` (multiplier in bp), `CRAFTING_SPEED` (multiplier in bp).

---

## Module Services (Runtime)

| Service | Responsibilities | Hooks |
|---------|------------------|-------|
| `CustomSkillsCombat` | Landed damage (crit, repeat-tiers), effective armor rating, defense caps | H01, H03, H04 |
| `CustomSkillsDurability` | Armor/PSG & weapon degradation chance reduction | H05, H06 |
| `CustomSkillsProgression` | Experience multiplier (all positive XP) | H07 |
| `CustomSkillsCrafting` | Practice XP, personal/factory speed, amazing chance/results | H08, H09A, H09B, H10, H11 |
| `CustomSkillsBuffs` | Initial & renewal duration for eligible buff families | H12A, H12B |
| `CustomSkillsMovement` | Client-effective & server-validated run speed | H13A, H13B |
| `CustomSkillsSkillMods` | Server SEA cap + client-visible skill mods | H14A, H14B |
| `CustomSkillsGathering` | Forage & milk quantity multiplication | H15A, H15B |
| `CustomSkillsModifiers` | Central badge aggregation, modifier totals, config access | All |

---

## Configuration System

**File**: `scripts/customskills/config.lua`

- Loaded once at startup by `CustomSkillsConfig` singleton
- **Cached for combat performance** -- restart required after changes
- Missing/invalid values -> safe defaults + server-log warning
- Per-modifier tables: `enabled`, `badgeBonus`, `cap`, `badges[]`, optional `combatSpamLabel`

---

## Badge System

- **140 badges** from `badge_map.iff` (6 Core3 types: ACCUMULATION, INTEREST, EXPLORATION, MASTER, EVENT, CONTENT)
- **111 eligible** for bonuses (excludes 9 pilot/JTL + 20 admin/event badges)
- Menu organizes into: Milestone, Exploration (by planet), Profession, Quest, Event
- Metadata (type, subtype, show flag) separate from modifier assignments

---

## Integration Hooks (19 Core3 Files)

| Area | Files Modified | Hook IDs |
|------|----------------|----------|
| Command registration | `CommandConfigManager.cpp`, `CommandConfigManager3.cpp`, `commands.lua` | -- |
| Combat (damage, armor, defense cap) | `CombatManager.h/.cpp` | H01, H03, H04 |
| Crafting (amazing chance/results) | `CraftingManagerImplementation.cpp`, `SharedLabratory.cpp` | H10, H11 |
| Factory | `FactoryObject.idl`, `FactoryObjectImplementation.cpp` | H09B |
| Player XP & speed | `PlayerManagerImplementation.cpp` | H07, H13B |
| Foraging | `ForageManagerImplementation.cpp` | H15A |
| Creature (skill mods, buffs) | `CreatureObjectImplementation.cpp` | H14A, H14B, H12A |
| Buff activation/renewal | `BuffImplementation.cpp` | H12A, H12B |
| Weapon degradation | `WeaponObjectImplementation.cpp` | H06 |
| Milk | `MilkCreatureTask.h` | H15B |
| SkillModList | `SkillModList.h` | H14A/H14B helpers |
| Packets (client sync) | `CreatureObjectMessage4.h`, `CreatureObjectDeltaMessage4.h` | H13A, H14B |

**Patch**: `integration/core3-hooks.patch` -- self-contained, reversible, `--ignore-space-change`

---

## Key Files for Developers

| File | Purpose |
|------|---------|
| `CustomSkillsModifierType.h` | Enum of all 17 modifier types |
| `CustomSkillsConfig.h/.cpp` | Config loading, defaults, Lua parsing |
| `CustomSkillsModifiers.h/.cpp` | Central modifier API (totals, badge queries, formatting) |
| `MANIFEST.md` | Complete file list + integration points |
| `integration/core3-hooks.patch` | All Core3 edits |
| `docs/reference/IMPLEMENTATION_GUIDE.md` | Hook details, service architecture, verification |
| `docs/reference/MODIFIER_REFERENCE.md` | Per-modifier specs, config, badge assignments |

---

## Milestones

| Milestone | Status |
|-----------|--------|
| **M1: Command-opened menu** | [OK] Complete -- shell tested in-game |
| **M2: First effective modifier (Critical Chance)** | [OK] Complete -- 12 combat badges, 3% each, 150% multiplier |
| **M3: All 17 modifiers + hooks implemented** | [OK] Complete -- all services & Core3 delegations done |
| **M4: Badge assignments & config** | [OK] Complete -- 111 badges mapped per `MENU_SYSTEM.md + BADGE_REFERENCE.md` |
| **M5: Challenge Tier system** | [CYCLE] Design phase (see `CHALLENGE_TIER.md`) |
| **M6: Challenge Tier skill trees** | [CYCLE] Design phase (see `CHALLENGE_TIER_SKILLS.md`) |

---

## Branching & Release

- Development on `cyvster3` branch
- Before deleting: tag completed module, install on clean integration branch, verify
- Package (`package/`) must remain installable without Git history
- `MANIFEST.md` and `INSTALL.md` kept current with every material change
