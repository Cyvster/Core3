# Brief 031 -- SWGEMU Options viewer: static display registry in menu

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)
- Depends on: BRIEF-023 (CONFIG_OPTIONS.md inventory, delivered 887cdd7e5e)
- XPP/R6.9 note: discoveries during implementation get captured into
  customskills/CODE_REFERENCE.md per authoring rules.

## Goal

Populate the existing SWGEMU_OPTIONS page (already wired into the Page enum
and titles[] at CustomSkillsMenu.cpp:396/403, currently renders "No options
configured yet.") with a curated, read-only view of notable Core3 server
options so operators can inspect live server behavior from the mod menu.

## Design decisions

1. **Static registry table** in CustomSkillsMenu.cpp:
   `struct SwgemuOpt { const char* label; const char* key; char type; bool restart; }`
   type: 'b'=bool 'i'=int 's'=string. Read via ConfigManager::instance()->
   getBool/getInt/getString with the code default from CONFIG_OPTIONS.md.
2. **Curated subset only** (~25 entries), NOT all 159: secrets/credentials
   (DBPass, DBSecret, APIToken) are EXCLUDED by policy. Chosen for operator
   interest: gameplay toggles (JTLEnabled, GalaxyWideGrouping,
   AdvancedWaypoints, DisableGroupVisibility, WipeFillingOnClone,
   MaxCharactersPerGalaxy, EnableTutorial, useCovertOvertSystem,
   CharacterBuilderEnabled, PvpMode, AllowBankPayments,
   EnhancedFurnitureRotate, AlwaysSafeLogout, LinkDeadDelay, etc.).
3. **Display format** matches house menu style: one line per option,
   `Label: VALUE` with color state for bools (green ENABLED / red DISABLED),
   plain for int/string; suffix `\#999999(restart)\#.` when restart=true.
4. **No writes.** Read-only view; config changes stay in config files.
   Rationale: writing config from a player-facing SUI is a security smell.

## Deliverables

- [ ] Registry + renderer implemented in CustomSkillsMenu.cpp
      (SWGEMU_OPTIONS branch of buildSummary + category list).
- [ ] Compiles clean (brace balance + full rebuild on Linux host pending).
- [ ] CODE_REFERENCE.md updated (registry location, how to add an option).
- [ ] USER_GUIDE.md one-liner for the new page content.
- [ ] Commit `[BRIEF-026]`, push.
