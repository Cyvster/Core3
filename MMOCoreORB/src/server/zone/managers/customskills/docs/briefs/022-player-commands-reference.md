# BRIEF-022 -- Player command reference (PLAYER_COMMANDS.md)

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner-approved follow-up
  to BRIEF-021.
- Priority: ERRATA > BRIEFS > anything else. No live errata blocking.

## Problem

BRIEF-021's sweep extracted the full command inventory from
`command_command_table` (`sql/datatables.sql`): **~3,036 player commands**
(characterAbility != 'admin') vs 133 admin. Owner directive during BRIEF-021:
player commands found while sweeping should be documented too — there is a
document slot for it (`swgemu/PLAYER_COMMANDS.md`, not yet created).

The working list exists at repo root `_player_cmds.txt` (uncommitted).

## Scope reality check

3,036 commands is an order of magnitude larger than the admin pass, and most
fall into a few huge mechanical families:

- **Combat special attacks** (~400+): `*Shot1/2`, `*Slash1/2`, `unarmedCombo*`,
  `melee*`, `creature*Attack` — parsed generically by CombatManager via command
  table fields (damage multiplier, action cost, state effects), NOT per-class
  argument parsing; most have thin/no `*Command.h` logic.
- **Skill-granted abilities with no arguments**: `/tumbleToStanding`,
  `/centerOfBeing`, `/warcry1`, innates, etc.
- **Real argumented player commands** worth deep docs: `/tip`, `/trade`,
  `/consent`, `/duel`, `/survey`, `/harvest`-adjacent, `/bandFlourish`,
  `/watch`, `/listen`, pet/droid control family, vendor/auction family,
  city-vote family.

A per-class parse of all 3,036 would mostly restate the combat pipeline
already covered in `swgemu/CODE_REFERENCE.md`. The value for private server
operators is (a) complete *inventory*, (b) deep syntax for the *argumented*
subset.

## Goal

Create `swgemu/PLAYER_COMMANDS.md` in house format with two tiers:

1. **Full inventory section** (complete, from `command_command_table` +
   `bin/scripts/commands/*.lua` cross-ref): every player command, grouped
   (Combat / Social+Entertainer / Trader+Crafting / Scout / Pets & Droids /
   Jedi / City / Travel & Movement / Vendor-Auction / Misc), one line each:
   name + one-clause description. Mechanical families may share a single
   descriptive line listing members ("burstShot1, burstShot2, ... — special
   attack variants, see Combat pipeline").
2. **Deep-dive sections** for the ~100 genuinely argumented commands: exact
   syntax, behavior, example, file:line cite — same evidence bar as BRIEF-021.

Also capture for operators:
- How abilities gate on skills (characterAbility -> skill grant) vs admin.
- The stub/no-op subset among player commands.

## Method

- Reuse the BRIEF-021 pipeline: table extraction script -> delegated batch
  parsing (opencode/bots) -> orchestrator spot-checks per R6.10.
- Batch by category; verify sample size scales with batch (>=5 each).
- Combat-family claims must cite CombatManager/table fields, not invent
  per-command behavior.
- R6.9: any new gating/discovery facts go to swgemu/CODE_REFERENCE.md.

## Acceptance criteria

- [ ] PLAYER_COMMANDS.md created in swgemu/, house format.
- [ ] Complete inventory (count reported, reconciled against table extract).
- [ ] >=100 argumented commands at full evidence bar (syntax/example/cite).
- [ ] Mechanical families documented as families with pipeline cites.
- [ ] Stub subset identified.
- [ ] Committed + pushed; brief marked DELIVERED.

## Estimate

2-4 sessions given batch delegation and verification overhead. Can be split:
inventory-only first commit, deep-dives incrementally after.
