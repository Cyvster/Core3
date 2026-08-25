# Brief 033 -- Menu performance research: per-open cost and burst-load profile

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Question to answer: what does one menu open cost server-side, and what
  happens at 1000 simultaneous opens -- compared against routine actions
  like a combat swing or inventory open?

## Why

The menu runs many checks per open (modifier aggregation across all badge
bonuses, config reads, StringId lookups, SUI construction). Owner wants a
quantified cost model, not intuition, before deciding whether optimization
is warranted.

## Research plan

1. **Map the call graph of one open.** CustomSkillsMenu::open ->
   addBadgeItems/addBonusItems/getPromptText -> per-type
   getModifierTotal (how many? ~30 modifier types x badges?), plus
   CustomSkillsConfig reads. Count total operations: ConfigManager::getBool
   calls (each takes a map lock?), StringId lookups, VectorMap iterations.
2. **Cost each primitive.**
   - ConfigManager get*: lock + hash lookup -- measure or estimate ns;
     note [dyn-cache] getters avoid re-parse but still lock.
   - getModifierTotal: iterates bonus maps per modifier type; quantify
     loop counts with current data sizes (~40+ badges, ~11 categories).
   - StringBuffer/SUI construction + packet serialization size (the SUI
     script payload grows with every line we append -- BRIEF-026 added 20+
     lines; measure packet bytes).
   - Object database reads, if any (badges come from PlayerObject in
     memory -- verify no DB hits).
3. **Burst model (the 1000-player question).** Estimate: opens/sec x
   per-open microseconds vs a combat swing's known cost (CombatManager
   does damage calc, TO-DAMAGE packets to N defenders, DOT tasks). Compare
   both against the zone task queue budget. Identify whether menu work runs
   on zone executor or player task queue and whether 1000 opens would
   starve combat ticks.
4. **Measure, don't just estimate** where possible: wrap timed sections
   behind an #ifdef or a temporary instrumented build; log min/max/avg
   over 1000 opens on the dev box. Cite actual numbers in the report.
5. **Identify cheap wins if needed**: memoize getPromptText per player
   with dirty-flag invalidation, lazy-build pages only on navigation,
   precompute static registry text once (BRIEF-026's option list is fully
   static except live values), cap StringId lookups.

## Deliverables

- [ ] Cost table: per-operation estimates AND measured numbers where
      instrumented (cite method for each).
- [ ] Comparison column: menu open vs combat swing vs inventory open.
- [ ] 1000-concurrent-open projection: total ms of queued work, worst-case
      frame impact, verdict (NEGLIGIBLE / TUNABLE / PROBLEM).
- [ ] If TUNABLE or PROBLEM: prioritized optimization list with expected
      savings; implementation goes to a follow-up brief, not this one.
- [ ] Findings captured into customskills/CODE_REFERENCE.md (R6.9).
- [ ] Commit `[BRIEF-033]`, push.

## Constraints

- No behavior changes in this brief -- research only.
- Any instrumentation code must be removable / #ifdef-gated.
