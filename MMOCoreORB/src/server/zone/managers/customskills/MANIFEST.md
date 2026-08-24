# Distribution Manifest

This manifest is a living tracker: it is updated whenever a module-owned
file or integration point is added, removed, or renamed ([PROC R6.6] in
`docs/rules/process.md`).

> Subordinate to [docs/rules/project-design.md](docs/rules/project-design.md).

**Known divergence (2026-08-23):** Core3 commits
`4b441424ba..504bb6cb15` (Bonuses category, Server Config menu, H16 rarity
naming in LootManagerImplementation.cpp, CRITICAL_MULTIPLIER promotion) are
NOT yet reflected in `package/` or in the lists below. Reconciliation is
tracked as [BRIEF-001](docs/briefs/001-sync-package-with-core3.md); until it
delivers, the Core3 dev tree is authoritative for current behavior.

## Module-owned files

- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsCommand.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsMenu.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsMenu.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsSuiCallback.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsSuiCallback.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsConfig.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsConfig.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsModifiers.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsModifiers.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/CustomSkillsModifierType.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/MENU_DESIGN.md`
- `package/MMOCoreORB/src/server/zone/managers/customskills/combat/CustomSkillsCombat.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/combat/CustomSkillsCombat.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/buffs/CustomSkillsBuffs.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/buffs/CustomSkillsBuffs.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/crafting/CustomSkillsCrafting.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/crafting/CustomSkillsCrafting.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/durability/CustomSkillsDurability.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/durability/CustomSkillsDurability.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/gathering/CustomSkillsGathering.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/gathering/CustomSkillsGathering.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/movement/CustomSkillsMovement.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/movement/CustomSkillsMovement.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/progression/CustomSkillsProgression.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/progression/CustomSkillsProgression.cpp`
- `package/MMOCoreORB/src/server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.h`
- `package/MMOCoreORB/src/server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.cpp`
- `package/MMOCoreORB/bin/scripts/commands/customSkills.lua`
- `package/MMOCoreORB/bin/scripts/customskills/config.lua`

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

Ground-combat damage enters the module through one delegation wrapper in
`CombatManager::applyDamage()`. The original implementation is retained as
`applyVanillaDamage()` and invoked by the custom combat pipeline.

Exact patch snippets and removal instructions are recorded in
`docs/installation/INSTALLATION.md`.

## Project records

Historical documents superseded during the 2026-08-23 documentation
restructuring (and the same-day compression pass) are preserved under
`docs/archive/` and are not current authority. Current successors:

| Superseded (archive/) | Current authority |
|-----------------------|-------------------|
| `MODIFIER_REGISTRY.md` | `docs/reference/MODIFIER_REFERENCE.md` |
| `IMPLEMENTATION_PLAN.md` | `docs/reference/ARCHITECTURE.md` |
| `PLAYER_BONUS_GUIDE.md` | `docs/user-guide/USER_GUIDE.md` |
| `SCREENPLAYS.md` | `docs/reference/ARCHITECTURE.md` (SUI Menu System section) |
| `BADGE_CATALOG.md` | `docs/reference/BADGE_REFERENCE.md` |
| `MENU_AND_BADGE_ORGANIZATION.md` | `docs/reference/ARCHITECTURE.md` + `docs/reference/BADGE_REFERENCE.md` |
| `EXCLUDED_BADGES.md` | `docs/reference/BADGE_REFERENCE.md` |
| `SWGEMU_CODE_GUIDE.md` | `docs/reference/CORE3_CODE_REFERENCE.md` + `CORE3_USER_GUIDE.md` |
| `CODE_GUIDE_ERRATA.md` | `docs/rules/errata.md` (log preserved as history) |
| `IMPLEMENTATION_GUIDE.md`, `MENU_SYSTEM.md`, `PROJECT_OVERVIEW.md`, `DEVELOPER_QUICKSTART.md`, `integration-INSTALL.md` | `docs/reference/ARCHITECTURE.md`, `START-HERE.md`, `docs/installation/INSTALLATION.md`, `docs/rules/process.md` |

Full mapping: [docs/README.md](docs/README.md).

---

**Last reconciled:** 2026-08-23 by ox-alpha (opencode/x-preview-f-free) --
links updated to post-restructuring locations; known package divergence
recorded above pending BRIEF-001; 2026-08-23 compression pass -- successor
table updated for consolidated reference set; install pointer redirected to
docs/installation/INSTALLATION.md.
