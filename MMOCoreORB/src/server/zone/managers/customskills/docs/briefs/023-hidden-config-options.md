# BRIEF-023 -- Deep dive: hidden/undocumented config.lua server options

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive: these
  options took "a tremendous amount of research" to find -- they are
  semi-hidden; document them in swgemu/CODE_REFERENCE.md so no operator has to
  rediscover them.
- Priority: ERRATA > BRIEFS > anything else. No live errata blocking.

## Owner's known options (seed set, from their config-local.lua)

    Core3.PlayerManager = {
        DisableGroupVisibility = true,
        WipeFillingOnClone = true,
        GalaxyWideGrouping = true,
        AdvancedWaypoints = true,
    }
    Core3.PlayerCreationManager = {
        MaxCharactersPerGalaxy = 3,
    }
    Core3.JTL = {
        JTLEnabled = true,
    }

These six-plus entries are the proof-of-concept: real options absent from the
shipped `bin/conf/config.lua` template and its comments. There are likely many
more.

## Problem

`config.lua` documents only a fraction of the options `ConfigManager`
actually reads. The rest are load-bearing booleans/ints scattered across
manager constructors and feature checks (`ConfigManager::get*` callsites).
Operators cannot find them without grepping C++.

## Goal

1. **Enumerate every config option read by ConfigManager**: parse
   `src/server/conf/ConfigManager.h` (getter list) + all
   `ConfigManager::instance()->get...` callsites across `src/`, extracting:
   option string, type (bool/int/float/string), default value, and the
   functional area that consumes it.
2. **Group by config table** (Core3.PlayerManager, Core3.PlayerCreationManager,
   Core3.JTL, Core3.CommandConfigManager, etc.) matching how callsites name
   them.
3. **Classify**: (a) documented already in shipped config.lua comments;
   (b) semi-hidden but functional (the target set); (c) dead/vestigial reads.
4. **Document in swgemu/CODE_REFERENCE.md** (new "Hidden config options"
   section): per option -- what it does, default, which code consumes it
   (file:line), any interactions/gotchas discovered while reading consumers.
5. **Flag behavior notes worth knowing**: e.g. whether an option is read at
   startup only vs dynamically, side effects of enabling (perf, exploits),
   inter-option dependencies.
6. R6.9: anything learned about ConfigManager internals goes in the same
   pass.

## Method

- Mechanical extraction is scriptable: regex over callsites; delegate batch
  verification of consumer semantics to opencode/bots per owner delegation
  preference; orchestrator spot-checks >=5 consumers directly against source
  (R6.10).
- Verify each candidate option actually appears in a live-read path (not a
  commented-out or test-only call) before documenting it as functional.

## Acceptance criteria

- [ ] Full option inventory extracted with type/default/consumer cites.
- [ ] Owner's six seed options verified in code (file:line) and documented.
- [ ] Hidden-options section added to swgemu/CODE_REFERENCE.md, grouped by
      config table, with startup-vs-dynamic read behavior noted.
- [ ] Dead reads identified but kept out of the functional doc (listed in
      delivery report only).
- [ ] Committed + pushed; brief marked DELIVERED.

## Estimate

1-2 sessions: extraction script + one delegated verification pass + doc write.
