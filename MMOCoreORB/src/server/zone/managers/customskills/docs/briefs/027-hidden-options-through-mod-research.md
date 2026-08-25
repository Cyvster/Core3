# BRIEF-027 -- Research: routing hidden swgemu options through the mod (prefilled mod config)

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive.
- Priority: RESEARCH first -- no implementation in this brief; findings feed
  a decision record and, if adopted, an implementation brief.
- Priority tier: MEDIUM-HIGH.

## Owner intent

Most admins don't know the hidden swgemu options exist (e.g.
`Core3.PlayerManager.DisableGroupVisibility`, `WipeFillingOnClone`,
`GalaxyWideGrouping`, `AdvancedWaypoints` -- set today in
config.lua/config-local.lua by hand). The owner wants to research moving
these into the Custom Skills mod:

- The mod ships a **prefilled Lua file** containing every hidden option,
  each with its real key, default, and notation.
- Admins turn features on/off by editing `true`/`false` in one obvious
  place -- no C++ knowledge, no forum archaeology.
- Long-term synergy with BRIEF-026 (the SWGEMU Options menu viewer would
  display these same options live).

## Research questions (answer all, with evidence file:line)

1. **Mechanism**: ConfigManager reads only from its configured lua files
   (`loadConfigData`). Can the mod add a SECOND lua file that sets these
   keys without touching stock config.lua? Investigate:
   - Does ConfigManager support multiple config files / include order?
     (`ConfigManager::loadConfigData`, `setConfigFile`, configVersion
     hot-reload path)
   - If single-file only: can the mod's existing lua hook
     (`lua.runFile("scripts/customskills/config.lua")`) set global
     ConfigManager keys via a registered function, or does
     `getBool(key, default)` only see the parsed config table?
2. **Timing**: which hidden options are read [startup] vs [dyn]? Startup
   reads (e.g. zone thread counts) cannot route through a mod-loaded file
   unless the mod loads before those consumers run. Map the BRIEF-023 draft
   inventory's ~76 class-(b) options into startup/dynamic buckets.
3. **Precedence**: if both config-local.lua and a mod file define the same
   key, which wins? (Last-load-wins vs first?) Determine safe merge order.
4. **Safety**: any keys that must NOT be exposed (secrets rule per ERR-014 /
   BRIEF-026 exclusions) or that destabilize when toggled at runtime?
5. **Deliverable shape**: proposed mod config file layout (grouped, fully
   notated per the owner's documentation standards: what it does, default,
   possible values, restart-vs-dynamic note), and whether toggling requires
   server restart per option.

## Method

- Read ConfigManager load/hot-reload code paths (src/conf/ConfigManager.*).
- Trace 2-3 representative consumers per bucket (startup + dynamic) to
  confirm read timing empirically from code.
- Prototype check (no commit): manually verify a test key set from a second
  lua source is honored, if mechanism 1 is viable. Timebox this; if not
  quickly confirmable statically, mark as "needs runtime experiment on Linux
  host".
- Produce a findings report + recommendation (feasible as-designed /
  feasible-with-caveats / infeasible) with the option-bucket table.

## Deliverables

- [ ] Findings report committed under docs/proposals/ (decision-ready:
      mechanism, precedence, timing buckets, risks, recommended file layout
      sketch).
- [ ] Seed list: which of the ~76 hidden options are routable now vs blocked
      (with reasons).
- [ ] No behavior changes to shipped code in this brief.
