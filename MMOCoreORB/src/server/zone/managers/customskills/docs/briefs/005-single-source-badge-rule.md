# BRIEF-005 -- Document single-source-of-truth rule for badge-backed modifiers

- Status: DELIVERED
- Created: 2026-08-23 by hy3-free (opencode/hy3-free)
- Prerequisite: [ERR-005](../rules/errata.md) (resolved 2026-08-23; owner
  directed fix direction (a))

## Problem

ERR-005 showed `CustomSkillsCombat::applyDamage` computed Critical Chance from
a hardcoded `combatProfessionBadges` array in `CustomSkillsModifiers.cpp`,
while the SUI menu computed it from the config badge map via
`getModifierTotal`. The two paths disagreed, violating [CS-3] (single source
of truth) and the "Menu = runtime" verification test. The fix (retire the
static array; `getCriticalChance` now reads `CustomSkillsConfig::
getBadgeBonuses(CRITICAL_CHANCE)`) is applied, but nothing in the docs tells a
future contributor that badge-backed modifier values MUST resolve through the
config map. This brief records that rule so the divergence cannot silently
recur.

## Deliverables

1. ARCHITECTURE.md -- add an explicit single-source rule for badge-backed
   modifiers: every gameplay hook and the SUI menu MUST read badge-driven
   values from `CustomSkillsConfig` (`getBadgeBonuses` / `getModifierTotal`);
   C++ must never hardcode badge key lists or per-badge rates for a modifier.
   Reference [CS-3]. Update the "Combat helpers" bullet under
   `CustomSkillsModifiers` to state `getCriticalChance` reads the config badge
   map (same source as the menu).
2. MODIFIER_REFERENCE.md -- update the Critical Chance (`CRITICAL_CHANCE`)
   property table and default-badge list to the actual current
   `config.lua` values (12 milestone exploration badges via
   `criticalChance.badges` + `badgeOverrides` at 100 bp each, cap 6000), and
   note that both combat and menu read the config map. Remove the
   Critical-Chance-specific "wrong until resolved" caveat from the staleness
   banner wording if present (config is authoritative now).
3. Honor [PROC R6.6]: behavior change (ERR-005) carries its doc updates; this
   brief is that documentation work.

## Acceptance criteria

- ARCHITECTURE.md states the no-hardcoded-badge-lists rule and points at
  `getBadgeBonuses` / `getModifierTotal` as the single source ([CS-3]).
- MODIFIER_REFERENCE Critical Chance table matches `config.lua`
  (milestone exploration badges, badgeOverrides 100 bp, cap 6000).
- No remaining doc claim that combat and menu disagree on Critical Chance.

## Notes

- This rule generalizes beyond Critical Chance: any future badge-backed
  modifier must follow the same path or it will fail the "Menu = runtime"
  verification test ([ARCHITECTURE.md] Testing & Verification Matrix).

---

**Last reconciled:** 2026-08-23 by hy3-free (opencode/hy3-free) -- created
and delivered same session as the ERR-005 fix.

## Contributors

- **Daniel** -- project owner; directed ERR-005 fix direction (a)
- hy3-free (opencode/hy3-free), 2026-08-23 -- created and delivered
