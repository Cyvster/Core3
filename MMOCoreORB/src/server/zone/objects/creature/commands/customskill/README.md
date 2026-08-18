# Custom Skills prototype

This is a standalone prototype for a new skill system. It does not use Core3's existing SkillManager or skill tree.

## Command

`/customskills`

## Integration points

Only these existing files are modified:

- `src/server/zone/managers/objectcontroller/command/CommandConfigManager3.cpp`
  - one include for `CustomSkillsCommand.h`
  - one `commandFactory.registerCommand` line
- `bin/scripts/commands/commands.lua`
  - one `RunSlashCommandsFile("customSkills.lua")` line

The command registration Lua file and implementation are new files.
