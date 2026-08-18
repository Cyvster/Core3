# Custom Skill Module

This module is independent of Core3's normal `SkillManager` and SWG skill tree.

## Current prototype

The `/customskills` slash command opens a SUI list containing five placeholder
custom skills. Selecting one displays its level and description.

Levels are stored through the existing `PlayerObject` screenplay-data storage,
so this module does not add fields to `PlayerObject.idl` or require a database
schema change.

## Porting

Copy the `server/zone/managers/customskill` directory into another Core3
source tree and add the small command integration from
`CommandConfigManager.cpp`:

1. Include `CustomSkillCommand.h`.
2. Add `new CustomSkillCommand("customskills", server)` to the special slash
   command list.

No existing SWG skills are modified or required.

## Adding real custom skills

Replace the five definitions in `CustomSkillManager::loadDefaultDefinitions()`
with the server's custom skill definitions. The IDs are used as persistent
screenplay-data keys and should remain stable once players have progression.
