# Brief 035 -- Crafting tool repeat-craft viability research

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- NOTE: the owner originally requested this topic during a disconnect window
  in which no agent was connected -- no brief was ever created or lost; it
  simply never got filed. Filed now per owner's re-statement. If an older
  draft surfaces elsewhere, reconcile.

## Goal

Research viability of adding a REPEAT option to the crafting tool that
launches from player inventory: after crafting an item, optionally start
the same schematic again with the same settings, so mass-producing N
identical items doesn't require re-clicking through the full session flow
each time. Research first; implementation is a follow-up brief.

## Key source files (verified present)

- `src/server/zone/objects/tangible/tool/CraftingTool.idl` /
  `CraftingToolImplementation.cpp` -- inventory crafting tool state machine
- Crafting session objects/managers under `src/server/zone/objects/`
  (crafting session) and `src/server/zone/managers/crafting/` if present
- Relevant client packets: CraftCustomization, CreatePrototype,
  RequestCraftingSession (`src/server/zone/packets/object/`)
- Our existing hooks: CustomSkillsConfig (Crafting Speed / Amazing Success
  already modify sessions -- same injection neighborhood)

## Research questions

1. Session lifecycle: map the inventory-tool crafting state machine
   (tool state field, session object creation, assembly -> experimentation
   -> prototype create -> finish). Where does a session end and what
   would "repeat" need to snapshot?
2. Snapshot feasibility: what defines "same craft" -- schematic ID, draft
   slots/resources used, experiment points allocation, customization?
   Which of these can be legally reused vs must be re-validated
   (resources may no longer exist in inventory on repeat #N)?
3. Client interaction constraints: the client drives the session via UI
   packets. Can the server re-open/re-run without new client UI input,
   or does every craft require a fresh client-side session request?
   (This decides whether repeat = server-side loop or just a QoL shortcut
   that pre-fills the next session.)
4. Vanilla precedents: factories already mass-produce from a schematic +
   components; examine FactoryObject logic for reusable patterns.
5. Safety/abuse: interaction with Crafting Speed bonus, practice-mode XP
   farming risk, resource exhaustion mid-loop, tool damage/degradation.
6. Verdict + implementation sketch: server-side loop vs assisted-session;
   config knobs (max repeats, per-craft delay); where our mod would hook.

## Deliverables

- [ ] `_035_findings.md` answering questions 1-5 with file:line cites.
- [ ] R6.9 capture into customskills/CODE_REFERENCE.md if durable facts
      about the crafting session lifecycle emerge.
- [ ] Feasibility verdict + recommended approach for a follow-up impl brief.
- [ ] Commit `[BRIEF-035]`, push.

## Constraints

- Research only; no code changes.
