# Brief 041 -- Repeat-craft as /customskills subcommand + radial option + autocraft surface research

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directives
- Supersedes the top-level `/repeatcraft` command (BRIEF-036 delivery)

## Owner directives driving this brief

1. NO new top-level slash commands going forward: adding a top-level command
   requires a client TRE change; subcommands of the already-registered
   `/customskills` do not. ALL mod commands must live under
   `/customskills <sub>` from now on.
2. A clickable "repeat" affordance is wanted (typing is clunky): evaluate a
   radial-menu option on the crafting tool.
3. Research other crafting objects that could serve autocrafting -- bypassing
   the handheld crafting tool entirely if there's a good candidate.

## Tasks

1. **Subcommand conversion**: implement argument parsing on the existing
   customskills command path (`CustomSkillsCommand.h` / its queue handler):
   `/customskills repeatcraft [tool]` routes to
   `CustomSkillsCrafting::doRepeatCraft`. Remove the standalone
   RepeatCraftCommand.h registration + repeatCraft.lua command file.
   Check how CustomSkillsCommand receives arguments (QueueCommand
   maxArgsToParse / the lua `name` args) and wire accordingly.
2. **Radial menu option**: CraftingToolImplementation::fillObjectMenuResponse
   (:48) already injects a server radial (see hopper-output example :52).
   Add "Repeat Craft" radial that calls doRepeatCraft for players with a
   stored snapshot. Radial codes: use an unused SERVER_ITEM_OPTIONS-range
   code; handle in the tool's ObjectMenuRequest handling or our own
   observer. Verify client renders server-item radials without TRE edits.
3. **Autocraft surface research** (document, don't implement yet):
   - Droid fabrication module / crafting droids: can they drive sessions?
   - Factory + schematic route: already mass-produces; what's missing to
     make it convenient for simple repeats (schematic upload flow)?
   - Structure-based crafting stations with hoppers: unattended input/output
     potential.
   Deliver findings in this brief's delivery notes; promising routes become
   their own brief.
4. Docs: CODE_REFERENCE + USER_GUIDE updated (new invocation forms); remove
   /repeatcraft references.

## Constraints

- No TRE/client changes. If a desired affordance proves impossible without
  one, document it as such honestly.
- Brace balance zero; commit `[BRIEF-041]`, push.
