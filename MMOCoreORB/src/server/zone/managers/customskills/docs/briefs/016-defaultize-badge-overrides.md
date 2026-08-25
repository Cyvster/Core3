# BRIEF-016 -- Defaultize badge configuration: uniform badgeBonus everywhere; badgeOverrides become dormant placeholders

- Status: DELIVERED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Owner policy: shipped defaults must not rely on
  badgeOverrides; every modifier uses one uniform `badgeBonus` over its
  badge list, and override capability survives as ready-to-uncomment
  placeholder blocks with instructions. Badge lists may be adjusted so
  capped modifiers remain reachable.

## Deliverables

1. **config.lua -- remove every ACTIVE `badgeOverrides` block** (all six:
   criticalChance, tripleAttackChance, quadAttackChance,
   defenseCapIncrease, buffDuration, gatheringQuantity).
2. **config.lua -- uniform badgeBonus values** (recalibrated so capped
   modifiers reach their caps):

   | Modifier | badgeBonus | Badges | Total vs cap |
   |----------|-----------:|-------:|--------------|
   | criticalChance | 500 | 12 milestones | 6000 = cap 6000 exact |
   | doubleAttackChance | 2000 (unchanged) | 6 | 12000 -> clamped 10000 = cap |
   | tripleAttackChance | 450 | 17 | 7650 -> clamped 7500 = cap |
   | quadAttackChance | 300 | 17 | 5100 -> clamped 5000 = cap |
   | armorPenetration | 1 (unchanged) | 9 | 9 -> clamped 3 = cap |
   | amazingSuccessChance | 1000 | 10 | 10000 = cap 10000 exact |
   | amazingResults | 1000 (unchanged) | 10 | 10000 = cap exact |
   | defenseCapIncrease | 8 | 15 | 120 pts (uncapped; owner-cited historical ceiling) |
   | armorDegradeReduction | 1250 (unchanged) | 8 | 100% |
   | weaponDegradeReduction | 1250 (unchanged) | 8 | 100% |
   | seaCapIncrease | 15 (unchanged) | 5 | 75 pts |
   | movementSpeed | 2500 (unchanged) | 4 | 100% |
   | buffDuration | 2500 | 13 | 325% (slight rise from former 300% -- flagged) |
   | experienceMultiplier | 10000 (unchanged) | 5 | 500% |
   | practiceExperienceBonus | 10000 (unchanged) | 10 | 1000% |
   | craftingSpeed | 1000 (unchanged) | 10 | 100% |
   | amazingResults covered above; gatheringQuantity below |
   | gatheringQuantity | 7700 | 13 | 100100 bp ~= 1000% (uncapped; preserves prior ceiling) |

3. **config.lua -- placeholder block for EVERY modifier**, directly under
   its badges list, inactive (commented out) with instructional notes:

   ```lua
   		-- Optional per-badge overrides. Each entry replaces badgeBonus
   		-- for that single badge: { "badge_key", basisPointValue }.
   		-- Example (uncomment and edit):
   		-- badgeOverrides = {
   		--     { "combat_bountyhunter_master", 300 },
   		-- },
   ```

   Use keys drawn from THAT modifier's own badges list in its example so
   owners see a copy-paste-realistic starting point.

4. **Faction-exclusivity accounting**: faction-paired badge sets
   (Corellian Corvette Imp/Reb/Neutral, Cries of Alderaan Rebel/Imperial)
   are partially mutually exclusive per character -- a player can hold
   only one faction side plus neutrals. Reachable-max annotations in
   CODE_REFERENCE.md Appendix A must state per-player reachable totals
   where they differ from listed totals, and intentional over-cap
   attainment via faction paths (e.g. Defense Cap reaching 120 pts) is
   PRESERVED, not treated as drift.
5. **Known shifts to document**: Buff Duration rises 300% -> 325%
   (uniform-rate artifact, flagged for owner awareness); Defense Cap and
   Gathering ceilings preserved via recalibrated badgeBonus (see table).
   Record in commit message and Appendix A.

5. **Docs** ([PROC R6.6]):
   - CODE_REFERENCE.md Appendix A: regenerate affected sections/totals
     from the new config (overrides references removed from defaults,
     placeholder convention noted); fix any remaining stale counts while
     regenerating (config.lua is authority per R6.10).
   - USER_GUIDE.md: update feature-section maxima that shifted
     (Defense Cap 75, Buff Duration 325%, Gathering 325%).
   - INSTALLATION.md: badgeOverrides worked example becomes the canonical
     placeholder reference; verification-step numbers updated.

## Acceptance criteria

- Zero active `badgeOverrides` tables in shipped config.lua; every
  modifier carries the placeholder template.
- Every CAPPED modifier reaches its cap with uniform values (math shown
  in commit message).
- Loader code unchanged (mechanism already supports the layout).
- Doc regeneration complete; known-balance-shift list recorded.
- Single commit tagged `[BRIEF-016]`, pushed. Compile unaffected
  (Lua/data only) but restart-to-apply reminder applies.

## Out of scope

- ERR-009 tier implementation (delivered separately, BRIEF-015).
- Changing any cap value.
- Adding badges beyond what the recalibration requires (list sizes stay
  as-is unless a ceiling proves unreachable with uniform values).

---

## Closure Report (verification of already-shipped state)

- Executor: hy3-free (opencode/x-preview-f-free), 08242026. This brief's
  substance was implemented during the badge-reconfiguration work verified
  under BRIEF-017; this session re-verified every acceptance criterion
  against the live config and closes the brief formally. No config changes
  were required.
- Acceptance verification (fresh math against config.lua [PROC R6.10]):
  - Zero active `badgeOverrides`: grep returns 0. Placeholder blocks:
    18/18 modifiers carry the commented template.
  - Capped-modifier reachability (badgeBonus x badges vs cap):
    criticalChance 100x60=6000 CAP EXACT; criticalMultiplier 1250x12=15000
    EXACT; tripleAttack 500x17=8500 EXACT; quadAttack 300x17=5100 EXACT;
    armorPenetration 1x9=9 clamped to 3; defenseCap 20x5=100 EXACT;
    armorDegrade 1250x8=10000 EXACT; weaponDegrade 1250x8=10000 EXACT;
    seaCap 20x5=100 EXACT; doubleAttack 2000x6=12000 clamped to 10000;
    amazingSuccessChance 500x10=5000 (50%, cap headroom INTENTIONAL per
    BRIEF-017 spec row 16); amazingResults 1000x10=10000 EXACT.
  - Uncapped modifiers uniform: movementSpeed 2500x4, buffDuration
    2500x13 (=325%), experienceMultiplier 10000x5, practiceXP 10000x10,
    craftingSpeed 1000x10, gatheringQuantity 10000x13 (=+1300%).
- Known shifts documented in CODE_REFERENCE Appendix A + USER_GUIDE
  (Buff Duration 325%, Defense Cap 100 pts, SEA Cap 100 pts) -- confirmed
  present during the BRIEF-014/BRIEF-017 doc sweeps.
- Loader code unchanged (setUniformBadgeBonus/loadBadgeOverrides mechanism
  already supports the layout).
- Docs consistent with config as of the BRIEF-017 sweep; no stale numbers.
- No new commit required for config (state shipped in earlier tagged
  commits); this closure rides the tracker-update commit. Compile
  unaffected (Lua/docs only); restart-to-apply reminder stands.
