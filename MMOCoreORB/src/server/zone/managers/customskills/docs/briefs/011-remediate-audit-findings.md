# BRIEF-011 -- Remediate BRIEF-008 audit findings (menu/config single-source, title-table safety, loader consistency)

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), produced by
  the BRIEF-008 deep audit. Prerequisite reading: BRIEF-008 (audit report
  with evidence anchors) and the errata log.

## Phase A -- correctness (AUDIT-1 / ERR-008)

The `/customskills` menu duplicates config assignments in 24 hardcoded
`badges*` arrays (`CustomSkillsMenu.cpp` anonymous namespace) and computes
per-page badge lists, acquired counts, and bonus totals from that copy.
Assignments live in ONE authoritative place: `config.lua`. Drift is live
(Critical Chance page shows a 24-badge subset; gameplay applies 60).

Fix (choose and document which):
1. Derive menu grouping from the config map at runtime -- iterate
   `getBadgeBonuses(type)` keys and group by a single static
   key->menu-group table (grouping is presentation, assignments are not
   duplicated); or
2. Keep arrays as presentation-only but add boot-time validation in
   `CustomSkillsConfig::load()` (or first menu open) that warns loudly on
   any drift between union-of-arrays and each modifier's config keys.

Option 1 preferred ([CS-3]). After either: menu BONUS_CRIT_CHANCE shows
all 60 badges and totals equal combat totals for every modifier.

Resolve ERR-008 after applying + self-verification per
[DIRECTIVE 08242026] if no second party is available.

## Phase B -- robustness (AUDIT-3)

- `CustomSkillsMenu.cpp getTitle()`: add `static_assert` equating array
  length to `CustomSkillsMenu::Page::SWGEMU_OPTIONS + 1` (or COUNT-style
  sentinel) and bounds-guard returning "Custom Skills" on overflow.
- `CustomSkillsModifiers.cpp getModifierName()`: same static_assert
  against `CustomSkillsModifierType::COUNT`.
- Replace `getParent()` range-comparison chains with an explicit
  parent-map initializer (order-independence), or add a comment +
  compile-time check justifying range reliance.

## Phase C -- consistency/craft (AUDIT-4/5)

- Consolidate criticalChance loading onto shared helpers with
  `loadModifier` semantics where practical; unify negative-cap handling.
- Replace the magic `20000` defaultBadgeBonus for gatheringQuantity with
  a named constant or move the value into config.lua only.
- Document (one comment line) that badgeOverrides entries may introduce
  keys absent from badges[].
- Edge cleanups: `divideDuration` 0-second input; degrade-roll
  granularity unification (random(9999) both paths).
- Preserve behavior everywhere in this phase; no balance changes.

## Acceptance criteria

- ERR-008 RESOLVED (verified per [DIRECTIVE 08242026] if solo) with menu
  totals provably equal to gameplay totals for at least Critical Chance.
- No behavioral change outside Phase A's intended fix (Phase C is
  behavior-preserving; state any exception explicitly).
- Compile verified or deferral documented (engine3 submodule caveat).
- Docs touched only where behavior visibly changed (e.g., menu now lists
  all assigned badges).
- Single commit tagged `[BRIEF-011]`, pushed.
