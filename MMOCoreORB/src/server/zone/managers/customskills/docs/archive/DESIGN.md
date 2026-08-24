# Design Goals

1. Keep custom-skills behavior isolated from Core3's normal skill system.
2. Keep module-owned code in dedicated `customskills` directories.
3. Modify existing Core3 files only where a stable integration hook is needed.
4. Keep the command, modifier registry, calculations, and SUI navigation in a
   dedicated C++ module so the menu reads the same authoritative values used by
   engine systems.
5. Use table-driven modifier and menu definitions so future entries do not
   require large callback conditionals.
6. Validate player references, SUI events, and row selections before acting.
7. Add no persistence, IDL fields, or database changes until a feature requires
   them and a migration plan exists.
8. Treat `/customskills` and `CustomSkills:openMenu()` as stable public APIs.
9. Keep installation, upgrade, and removal instructions current with every
   material change.
10. Verify the module on a clean Core3 base before calling a release complete.
11. Treat the existing badge bitmasks as the source of truth for badge-granted
    modifiers; do not copy badge ownership or derived totals into screenplay
    data.
12. Resolve all displayed values from the character passed into the command or
    SUI callback.
13. Expose one modifier-query API used by both gameplay hooks and the menu so
    displayed and effective values cannot diverge.
14. Represent percentage modifiers in integer basis points: 100 basis points
    equals 1%. Convert to display text only at the UI boundary.
15. Begin with direct C++ badge checks grouped by modifier. Add caching only
    after profiling demonstrates a need and a complete invalidation design has
    been implemented.

## Physical layout

The current design keeps the installed module in one compiled-source location:

```text
MMOCoreORB/src/server/zone/managers/customskills/
```

The standalone `package/` directory mirrors this location. A runtime data file
will be introduced only if later configuration requirements outweigh the value
of a single-folder module.

See `SCREENPLAYS.md` for why screenplay Lua remains available for optional
presentation extensions but is no longer the recommended core architecture.
