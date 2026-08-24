# Core3 Screenplay and Character-State Research

## Lifecycle

Core3 initializes a Lua environment through `DirectorManager` and loads
`scripts/screenplays/screenplays.lua`. That loader imports the SUI helpers and,
at its end, imports the custom screenplay loader. Each server Lua context loads
the module definitions.

`ScreenPlay:new` creates a Lua table inheriting common screenplay helpers. The
table is not a separate object for each player. `registerScreenPlay(name,
start)` records the table name and whether its `start` function should run as a
global screenplay during server startup.

The Custom Skills menu does not need global startup work. It should be loaded
through the custom screenplay loader and invoked only when a character uses
`/customskills`. It may be registered with `start = false` for conventional
bookkeeping, but registration is not what makes callbacks work.

## SUI request and callback path

1. The C++ `/customskills` bridge receives the invoking `CreatureObject`.
2. It invokes `CustomSkills:openMenu(pPlayer)` in Lua.
3. Lua creates a `SuiListBox`, assigns a Lua callback name, and sends it only to
   `pPlayer`.
4. Core3 stores that SUI page in the invoking character's `PlayerObject` SUI
   map.
5. The client response identifies the page ID.
6. `SuiManager` resolves that page from the same character's `PlayerObject` and
   calls the Lua callback with that character, the SUI page, the event, and the
   submitted arguments.

This path is character-specific by construction. One player's page and
callback cannot be resolved from another player's `PlayerObject` SUI map.

## Character-specific persistence options

Core3 exposes two existing character-owned stores to Lua:

### PlayerObject screenplay data

`writeScreenPlayData`, `readScreenPlayData`, and `deleteScreenPlayData` operate
on the invoking player's `PlayerObject`. Values are strings stored under a
combined screenplay and variable key. The underlying map is not transient, so
it is part of the persisted player object.

This is suitable for small, namespaced, module-specific values when:

- string serialization is sufficient;
- access is primarily per-character;
- values do not require database-wide querying; and
- engine systems do not need a high-frequency typed data structure.

### CreatureObject screenplay state

`getScreenPlayState` and `setScreenPlayState` provide a character-owned map of
unsigned integer values. Existing content commonly uses these as flags or
bitmasks. This is appropriate for compact state flags, not a general custom
statistics schema.

### Stores to avoid for authoritative character state

- Lua global tables are shared module definitions, not character records.
- Director shared memory is process-wide and should not be treated as durable
  character persistence, even if keys contain a character object ID.
- SUI stored data exists to route one menu interaction and is not progression
  storage.

## Re-evaluated architectural decision

The project now includes badge-derived bonuses that must affect gameplay, such
as critical-hit chance, and a menu that must show the same effective totals and
their sources. For that requirement, a C++-owned modifier service and C++ SUI
menu are preferred over a screenplay-owned menu.

An on-demand Lua SUI by itself would not be a serious resource problem. The
risk is allowing Lua to become a second calculation path or invoking it from
high-frequency combat and movement logic. A C++ menu also lets the module query
the exact same typed API used by gameplay hooks and keeps the current module in
one source folder.

Screenplay Lua remains a valid optional extension point for low-frequency
presentation or server-specific scripting. It must not become the source of
truth for badge ownership, derived modifier totals, or combat calculations.

Badge ownership already persists on each `PlayerObject` as bitmasks. The module
should query those badges directly and derive modifiers without creating new
screenplay data.

## Safety requirements

- Validate that `pPlayer` is a player creature in every public entry point.
- Validate the player ghost before reading character data.
- Treat cancel, missing arguments, negative rows, and unknown stored actions as
  no-op events.
- Store stable action IDs in SUI row data rather than dispatching directly from
  display text.
- Dispatch only through an explicit action table; never call an arbitrary
  function name supplied by the client.
- Do not keep a global `currentPlayer`, selected row, or open-page record in
  Lua.
- Use a dedicated SUI window type if the module later needs to close or replace
  only its own open window.
- Keep stat IDs stable after persistence begins and document migrations.
