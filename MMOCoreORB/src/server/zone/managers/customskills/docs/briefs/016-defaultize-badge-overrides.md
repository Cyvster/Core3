# BRIEF-016 -- Defaultize badge configuration: uniform badgeBonus everywhere; badgeOverrides become dormant placeholders

- Status: UNCLAIMED
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
   | defenseCapIncrease | 5 | 15 | 75 pts (uncapped) |
   | armorDegradeReduction | 1250 (unchanged) | 8 | 100% |
   | weaponDegradeReduction | 1250 (unchanged) | 8 | 100% |
   | seaCapIncrease | 15 (unchanged) | 5 | 75 pts |
   | movementSpeed | 2500 (unchanged) | 4 | 100% |
   | buffDuration | 2500 | 13 | 325% |
   | experienceMultiplier | 10000 (unchanged) | 5 | 500% |
   | practiceExperienceBonus | 10000 (unchanged) | 10 | 1000% |
   | craftingSpeed | 1000 (unchanged) | 10 | 100% |
   | amazingResults covered above; gatheringQuantity below |
   | gatheringQuantity | 2500 | 13 | 325% (uncapped) |

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

4. **Known balance shifts to document** (accepted by owner): Defense Cap
   100 -> 75 pts; Buff Duration 300% -> 325%; Gathering Quantity 1000% ->
   325%. Record these in the commit message and CODE_REFERENCE Appendix A
   totals table.

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
- Adding new badges to any list (lists stay as-is per the recalibration
  table above).
