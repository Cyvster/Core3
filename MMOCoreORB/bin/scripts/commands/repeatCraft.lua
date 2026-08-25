-- Registers the repeat-craft assisted pre-fill command (BRIEF-036).
-- Starts a fresh crafting session pre-filled from the last successful
-- craft on the targeted crafting tool; player confirms in the normal
-- crafting window. Gated server-side by customSkillsConfig.repeatCraft.

RepeatCraftCommand = {
	name = "repeatcraft",
}

AddCommand(RepeatCraftCommand)
