# Distribution Manifest

This manifest is a living tracker: it is updated whenever a module-owned
file or integration point is added, removed, or renamed ([PROC R6.6] in
`docs/rules/process.md`).

> Subordinate to [docs/rules/project-design.md](docs/rules/project-design.md).

**Layout (08232026, BRIEF-001):** the module lives inside the Core3 repo.
This manifest sits at the module dir root; paths below are relative to a
Core3 checkout root. The former standalone mirror (`customskills-mod/`,
`package/`) was dissolved; its successor mapping table at the bottom remains
as history.

## Module-owned files

Paths relative to `MMOCoreORB/`:

- `src/server/zone/managers/customskills/CustomSkillsCommand.h`
- `src/server/zone/managers/customskills/CustomSkillsMenu.h`
- `src/server/zone/managers/customskills/CustomSkillsMenu.cpp`
- `src/server/zone/managers/customskills/CustomSkillsSuiCallback.h`
- `src/server/zone/managers/customskills/CustomSkillsSuiCallback.cpp`
- `src/server/zone/managers/customskills/CustomSkillsConfig.h`
- `src/server/zone/managers/customskills/CustomSkillsConfig.cpp`
- `src/server/zone/managers/customskills/CustomSkillsModifiers.h`
- `src/server/zone/managers/customskills/CustomSkillsModifiers.cpp`
- `src/server/zone/managers/customskills/CustomSkillsModifierType.h`
- `src/server/zone/managers/customskills/MENU_DESIGN.md`
- `src/server/zone/managers/customskills/combat/CustomSkillsCombat.h`
- `src/server/zone/managers/customskills/combat/CustomSkillsCombat.cpp`
- `src/server/zone/managers/customskills/buffs/CustomSkillsBuffs.h`
- `src/server/zone/managers/customskills/buffs/CustomSkillsBuffs.cpp`
- `src/server/zone/managers/customskills/crafting/CustomSkillsCrafting.h`
- `src/server/zone/managers/customskills/crafting/CustomSkillsCrafting.cpp`
- `src/server/zone/managers/customskills/durability/CustomSkillsDurability.h`
- `src/server/zone/managers/customskills/durability/CustomSkillsDurability.cpp`
- `src/server/zone/managers/customskills/gathering/CustomSkillsGathering.h`
- `src/server/zone/managers/customskills/gathering/CustomSkillsGathering.cpp`
- `src/server/zone/managers/customskills/movement/CustomSkillsMovement.h`
- `src/server/zone/managers/customskills/movement/CustomSkillsMovement.cpp`
- `src/server/zone/managers/customskills/progression/CustomSkillsProgression.h`
- `src/server/zone/managers/customskills/progression/CustomSkillsProgression.cpp`
- `src/server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.h`
- `src/server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.cpp`
- `bin/scripts/commands/customSkills.lua`
- `bin/scripts/customskills/config.lua`

## Existing Core3 integration points

- `integration/core3-hooks.patch` is the self-contained patch that applies or
  reverses every edit to existing Core3 files listed below.

Core3 integration edits:

1. `MMOCoreORB/src/server/zone/managers/objectcontroller/command/CommandConfigManager3.cpp`
2. `MMOCoreORB/bin/scripts/commands/commands.lua`
3. `MMOCoreORB/src/server/zone/managers/objectcontroller/command/CommandConfigManager.cpp`
4. `MMOCoreORB/src/server/zone/managers/combat/CombatManager.h`
5. `MMOCoreORB/src/server/zone/managers/combat/CombatManager.cpp`
6. `MMOCoreORB/src/server/zone/managers/player/PlayerManagerImplementation.cpp`
7. `MMOCoreORB/src/server/zone/managers/minigames/ForageManagerImplementation.cpp`
8. `MMOCoreORB/src/server/zone/managers/crafting/CraftingManagerImplementation.cpp`
9. `MMOCoreORB/src/server/zone/managers/crafting/labratories/SharedLabratory.cpp`
10. `MMOCoreORB/src/server/zone/objects/creature/CreatureObjectImplementation.cpp`
11. `MMOCoreORB/src/server/zone/objects/creature/buffs/BuffImplementation.cpp`
12. `MMOCoreORB/src/server/zone/objects/creature/events/MilkCreatureTask.h`
13. `MMOCoreORB/src/server/zone/objects/creature/variables/SkillModList.h`
14. `MMOCoreORB/src/server/zone/objects/installation/factory/FactoryObject.idl`
15. `MMOCoreORB/src/server/zone/objects/installation/factory/FactoryObjectImplementation.cpp`
16. `MMOCoreORB/src/server/zone/objects/player/sessions/crafting/CraftingSessionImplementation.cpp`
17. `MMOCoreORB/src/server/zone/objects/tangible/weapon/WeaponObjectImplementation.cpp`
18. `MMOCoreORB/src/server/zone/packets/creature/CreatureObjectMessage4.h`
19. `MMOCoreORB/src/server/zone/packets/creature/CreatureObjectDeltaMessage4.h`
20. `MMOCoreORB/src/server/zone/managers/loot/LootManagerImplementation.cpp` (H16 rarity naming)

Ground-combat damage enters the module through one delegation wrapper in
`CombatManager::applyDamage()`. The original implementation is retained as
`applyVanillaDamage()` and invoked by the custom combat pipeline.

Exact patch snippets and removal instructions are recorded in
`docs/installation/INSTALLATION.md`.

## Project records

Historical documents superseded during the 08232026 documentation
restructuring (and the same-day compression pass) are preserved under
`docs/archive/` and are not current authority. Current successors:

| Superseded (archive/) | Current authority |
|-----------------------|-------------------|
| `MODIFIER_REGISTRY.md` | `docs/customskills/CODE_REFERENCE.md` Appendix A |
| `IMPLEMENTATION_PLAN.md` | `docs/customskills/CODE_REFERENCE.md` |
| `PLAYER_BONUS_GUIDE.md` | `docs/customskills/USER_GUIDE.md` |
| `SCREENPLAYS.md` | `docs/customskills/CODE_REFERENCE.md` (SUI Menu System section) |
| `BADGE_CATALOG.md` | `docs/customskills/CODE_REFERENCE.md` Appendix B |
| `MENU_AND_BADGE_ORGANIZATION.md` | `docs/customskills/CODE_REFERENCE.md` (Part I + Appendix B) |
| `EXCLUDED_BADGES.md` | `docs/customskills/CODE_REFERENCE.md` Appendix B excluded list |
| `SWGEMU_CODE_GUIDE.md` | `docs/swgemu/CODE_REFERENCE.md` (merged: former CORE3_CODE_REFERENCE + CORE3_USER_GUIDE) |
| `IMPLEMENTATION_GUIDE.md`, `MENU_SYSTEM.md`, `PROJECT_OVERVIEW.md`, `DEVELOPER_QUICKSTART.md`, `integration-INSTALL.md` | `docs/customskills/CODE_REFERENCE.md`, `docs/customskills/LLM_GUIDE.md`, `docs/installation/INSTALLATION.md`, `docs/rules/process.md` |

Full mapping: [docs/README.md](docs/README.md).

---

**Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
successor-table paths updated for the owner-directed guide restructure
(docs/ split into swgemu/ + customskills/ audience subfolders; reference/
and user-guide/ dissolved). Prior: 08232026 -- BRIEF-002: header + paths
rewritten for single-tree layout; --
links updated to post-restructuring locations; known package divergence
recorded above pending BRIEF-001; 08232026 compression pass -- successor
table updated for consolidated reference set; install pointer redirected to
docs/installation/INSTALLATION.md.
