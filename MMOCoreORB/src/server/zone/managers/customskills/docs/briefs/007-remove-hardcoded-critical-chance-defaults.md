# BRIEF-007 -- Remove residual hardcoded critical-chance defaults; restore cap reachability via explicit per-badge overrides

- Status: CLAIMED by ox-alpha (opencode/x-preview-f-free), 08242026
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive (work session, Central Time). Context: while rebalancing
  badge bonuses it was discovered that the ERR-005-era cleanup did NOT
  remove all hardcoded critical-chance sources from C++.

## Problem

`CustomSkillsConfig.cpp` constructor still seeds gameplay state:

1. A `combatBadges[]` array (all 12 combat mastery keys) is looped into
   `modifierBadgeBonuses[CRITICAL_CHANCE]` at
   `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE` (= 300 bp, defined in
   `CustomSkillsConfig.h`) -- the pre-ERR-005 hardcoded design, alive as
   a constructor default.
2. `criticalChanceEnabled = true` and the seed mean that a MISSING or
   invalid `customSkillsConfig`/`criticalChance` table silently runs
   crit chance at 12 combat badges x 3% = 36%.
3. In `load()`, `setUniformBadgeBonus(CRITICAL_CHANCE, badgeBonus)`
   rescales those seeded entries to whatever `badgeBonus` says -- so the
   old shipped config (`badgeBonus = 400`) gave full-badge players an
   undocumented +48% from combat masteries on top of the documented
   +12% from milestones (exactly the 60% cap).

Owner policy: there must be NO hardcoded badge bonuses in code; every
badge's value is declared in `config.lua`, per-badge via
`badgeOverrides`.

## Starting state (already committed)

- Commit `049ef1d0d0`: `config.lua` `criticalChance.badgeBonus = 0`
  (explicit no-silent-grant fallback) + truthful comments.
- Behavioral note: because of the seed, this commit temporarily drops
  full-badge players from 60% to 12% until THIS brief restores
  reachability via explicit overrides.

## Deliverables

1. **C++ (`CustomSkillsConfig.cpp/.h`)**:
   - Delete the constructor `combatBadges[]` seed block and the
     `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE` constant.
   - Constructor leaves the CRITICAL_CHANCE bonus map EMPTY; keep
     `criticalChanceEnabled = true` and the cap default (6000).
   - Missing/invalid config states: feature enabled but zero bonuses
     (no silent grants). Replace the `getIntField("badgeBonus",
     DEFAULT_...)` fallback with explicit 0 default and keep the
     range warning.
   - If trivial while touching these lines, rename stale identifiers
     (`criticalChancePerCombatBadge`, `...PER_COMBAT_BADGE`) to
     fallback-appropriate names; otherwise leave for a later cleanup
     and note it.
2. **`config.lua`**: expand `criticalChance` to the owner-specified
   60-badge set below. Every key listed in BOTH `badges` and
   `badgeOverrides` at 100 bp; `badgeBonus` stays 0; `cap` stays 6000.
   Result: 60 x 100 bp = 6000 bp = exactly the cap, no clamp reliance.
3. **Documentation** ([PROC R6.6], same commit set):
   - `docs/customskills/CODE_REFERENCE.md` Appendix A: rewrite the
     Critical Chance section (60 badges @ 100 bp via overrides,
     badgeBonus = 0 fallback semantics) and correct the Maximum
     Theoretical Totals row (1200 -> 6000). While regenerating from
     `config.lua`, also fix known-stale Amazing Success/Results rows
     (config has 10 badges each, not 6; Results already totals 100%).
   - `docs/customskills/USER_GUIDE.md`: update the Critical Chance
     feature section (sources + 60% maximum).
   - `docs/installation/INSTALLATION.md`: update any cited values.
4. **Errata**: file ERR-007 (OPEN) documenting the survived-hardcode
   finding with evidence (constructor snippet + commit references);
   cross-link this brief. Resolution/verification follows the normal
   two-party lifecycle after delivery.

## The 60-badge set (each at 100 bp via badgeOverrides)

| Group | Count | Keys |
|-------|-------|------|
| Accumulation milestones | 7 | `count_5` .. `count_125` |
| Exploration milestones | 5 | `bdg_exp_10_badges` .. `bdg_exp_45_badges` |
| Combat profession masteries | 12 | `combat_*_master` |
| Hero of Tatooine | 5 | `poi_rabidbeast`, `poi_prisonbreak`, `poi_twoliars`, `poi_factoryliberation`, `poi_heromark` |
| Warren | 2 | `warren_compassion`, `warren_hero` |
| Theme Parks | 4 | `bdg_thm_park_jabba/imperial/rebel/nym_badge` |
| Corellian Corvette | 9 | `bdg_corvette_*` (9 faction/mission combos) |
| Exploration -- dangerous sites | 5 | `exp_tat_tusken_pool`, `exp_tat_krayt_skeleton`, `exp_tat_sarlacc_pit`, `exp_tat_krayt_graveyard`, `exp_dat_sarlacc` |
| Exploration -- Jedi sites | 3 | `exp_tat_bens_hut`, `exp_yav_temple_exar_kun`, `exp_dan_jedi_temple` |
| Outdoors masteries | 5 | `outdoors_bio_engineer/creaturehandler/ranger/scout/squadleader_master` |
| Science masteries | 3 | `science_combatmedic/doctor/medic_master` |
| **Total** | **60** | |

No excluded badges (pilot/JTL, admin/event) are used. Multi-assignment
alongside other modifiers is expected and harmless (per-modifier maps
are independent).

## Acceptance criteria

- Repo-wide grep: no `combatBadges` seed, no
  `DEFAULT_CRITICAL_CHANCE_PER_COMBAT_BADGE`.
- Module compiles (cmake build target core3 or at minimum the module
  objects compile clean).
- Boot-behavior reasoning documented in the commit message: missing
  config -> enabled + zero bonuses; present config -> only explicit
  values apply.
- Math check: sum of override values for the 60 keys = 6000 bp = cap;
  no clamping required.
- Appendix A Critical Chance section + Max Totals row match
  `config.lua`; USER_GUIDE + INSTALLATION consistent.
- ERR-007 filed OPEN with evidence and brief cross-link.
- Single commit tagged `[BRIEF-007]`; pushed ([PROC R6.5]).

## Out of scope

- Gap fixes for other modifiers (Triple 75%, Quad 50%, Amazing Success
  100%) -- owner will direct separately; do not touch their values.
- Changing any cap value.
- Renaming `criticalChancePerCombatBadge` if it turns non-trivial.
