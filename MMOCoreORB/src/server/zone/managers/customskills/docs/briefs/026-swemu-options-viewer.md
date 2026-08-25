# BRIEF-026 -- SWGEMU Options category: server-configurable options viewer

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive.
- Priority: MEDIUM-HIGH (new player/operator-facing feature; feeds future
  routing of hidden swgemu options through the mod).

## Owner intent (verbatim constraints)

- Populate the menu's existing **SWGEMU Options** category with server
  configurable options originating from **swgemu Core3 code** -- even if a
  setting later routes through our mod, ORIGIN determines the category.
- This is a read-only **viewer**: players can see what options the server
  operator has selected. Nothing here is editable in-game.
- **Exclude private/secret settings** -- e.g. DB credentials, DBSecret,
  account/API keys, anything in config.lua marked secret or sensitive.
  When in doubt, leave it out and note it as excluded.
- Categorize so things are **easy to find**; top pane follows the same
  architecture as Mod Options (category pages -> leaf rows showing
  name + current value + ENABLED/DISABLED style status coloring).
- **Standardize in documentation** so future additions fit the format.

## Data source

The verified inventory already exists:
`docs/customskills/_config_options_draft.md` (BRIEF-023 draft, 159 keys,
classified (a) documented / (b) hidden-functional / (c) dead, each with
type/default/consumer file:line). Use it as the single source of truth;
re-verify any option's live value at display time via ConfigManager getters
(do not hard-code values in the menu).

## Task

1. **Curation pass over the inventory**: select player-safe options worth
   surfacing. Suggested initial categories (adjust during implementation):
   - Gameplay: JTL enabled, XP/gathering/crafting modifiers,
     PlayerBountyCooldown, purge cadence
   - Characters: MaxCharactersPerGalaxy, EnableTutorial, starting locations
   - Groups/Social: GalaxyWideGrouping, DisableGroupVisibility
   - World: spawner windows, travel settings, structure maintenance knobs
   - Combat/PvP: GCW/PvP toggles, TEF windows
   - Performance/Threading: zone thread counts (read-only display)
   - Client compatibility: ValidClientVersion
2. **Exclusion list**: DB* credentials, DBSecret, webusers/adminusers,
   REST/API keys, MakeLogin/ping/status ports if sensitive, anything
   authentication-related. Record exclusions in the brief delivery report.
3. **Menu architecture** (mirror Mod Options):
   - `SWGEMU_OPTIONS` page becomes a category index (sub-pages per group).
   - New Page enum entries per group; `addPageItems` renders one row per
     option: `<Option Label>: <value>` with green ENABLED/red DISABLED for
     bools, raw value otherwise.
   - Values read live via ConfigManager at page render (hot-reload aware --
     [dyn] options reflect changes without restart; [startup] ones note
     "restart required" in the row suffix).
4. **Standardization doc**: new section in customskills/CODE_REFERENCE.md
   ("SWGEMU Options viewer") defining: where the option registry lives in
   code (a static table struct {label, configKey, type, group, restartFlag}),
   how to add an entry, exclusion rules, and the origin rule (swgemu-origin
   stays here even if routed through mod later).
5. R6.9: capture any SUI layout facts learned into CODE_REFERENCE.

## Deliverables

- [ ] SWGEMU_OPTIONS category populated with grouped, categorized,
      live-valued option rows (no secrets).
- [ ] Top-pane architecture mirrors Mod Options (categories -> detail pages).
- [ ] Standardization section in customskills/CODE_REFERENCE.md.
- [ ] Exclusion list documented in delivery report.
- [ ] Build verified Linux host; commit tagged `[BRIEF-026]`; pushed.

## Estimate

1-2 sessions (curation + menu plumbing are mechanical once the registry
table exists).
