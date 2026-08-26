# Brief -- Add mod config options to the /customskills options menu

- Status: OPEN
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)

## Problem

The `/customskills` menu has a SWGEMU_OPTIONS page listing vanilla
ConfigManager options (CustomSkillsMenu.cpp `addSwgemuOptionItems`, a static
table of {label, key, type, restart} rows). MOD-owned knobs added since then
(loot, surveying, training, structures, missions tables) are NOT in the menu.
Every new mod option currently requires remembering CONFIG_OPTIONS.md --
operators cannot discover or verify settings in-game. This recurs each time a
feature lands; the process gap is that no rule requires menu registration.

## Task

1. Add a second static table (or extend the existing one with an owner
   column) for MOD options, reading from CustomSkillsConfig (not
   ConfigManager): at minimum all knobs added 08252026:
   loot.creditsToTopDamager, loot.nonHumanoidCredits, loot.creditMultiplier,
   loot.attachmentAutoName, surveying.maxRange, training.trainersTeachAll,
   structures.accountSharedLots, missions table (missionOptionsEnabled,
   directionOptionEnabled, difficultyOptionEnabled, missionListSize,
   descriptiveTitles).
   Show current value + restart-required flag per row; read-only display is
   acceptable for v1 (matching SWGEMU_OPTIONS behavior).
2. PROCESS RULE (prevents recurrence): amend the brief-authoring rules /
   delivery checklist so every new config knob MUST either register itself in
   the options menu or state why it is excluded (internal-only). Coordinator
   grep duty extended accordingly.
3. Update CODE_REFERENCE.md (menu registry mechanics) + CONFIG_OPTIONS.md if
   any knob descriptions change.

## Deliverables

- [ ] Mod-options section live in the /customskills menu.
- [ ] Authoring-rule amendment landed (both repos per XPP if policy-level;
      file Alice mirror brief only -- do not edit Alice while another LLM
      owns it).
- [ ] Commit `[BRIEF-051]`, push origin cyvster3.
