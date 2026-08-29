# BRIEF-017 -- Independent verification and completion of the badge reconfiguration

- Status: DELIVERED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. The executor of the prior badge work committed technical
  errors mid-stream (partial script writes, formatting damage, a lost
  edit). An independent session must verify the final state against this
  specification, complete anything missing, and correct any
  discrepancy.

## Context

Owner directives driving this state:

1. No hardcoded badge bonuses in C++ (ERR-005/007 lineage).
2. No active `badgeOverrides` in shipped defaults -- every modifier uses
   one uniform `badgeBonus` over its badge list. Each modifier still
   carries an INACTIVE commented `badgeOverrides` placeholder with usage
   notation so owners never have to guess the syntax.
3. Anything previously in overrides is squashed into the uniform system;
   where that changed reachability, values were recalibrated to keep
   caps attainable.
4. Faction-restricted badge pairs mean reachable totals may differ from
   listed totals by design; annotate where relevant, do not treat as
   drift.
5. Dates use MMDDYYYY ([DIRECTIVE 08242026]); no overrides stack --
   override replaces badgeBonus for that key ([PROC R6.10] verified).

## Required reading

Workspace-root `readme.md` ->
[customskills/LLM_GUIDE.md](customskills/LLM_GUIDE.md) ->
[rules/process.md](rules/process.md) ->
[rules/project-design.md](rules/project-design.md). Then this brief in
full.

## The specification to verify (target state)

All paths relative to Core3 root; config =
`MMOCoreORB/bin/scripts/customskills/config.lua`.

### A. Modifier table -- verify each row exactly

| # | Modifier | badgeBonus | Cap | Badge list | Expected total |
|---|----------|-----------:|----:|------------|----------------|
| 1 | criticalChance | 100 | 6000 | 60 achievement badges (12 milestones + combat masters + quest lines + dangerous/Jedi sites + outdoors/science masters) | 6000 = cap exact |
| 2 | criticalMultiplier | 1250 | 15000 | 12 milestones | 15000 = cap exact |
| 3 | doubleAttackChance | 2000 | 10000 | Warren (2) + Theme Parks (4) | 12000 -> clamped 10000 |
| 4 | tripleAttackChance | 500 | 8500 | 17 = 12 combat + 5 Hero of Tatooine | 8500 = cap exact |
| 5 | quadAttackChance | 300 | 5100 | same 17 as triple | 5100 = cap exact |
| 6 | armorPenetration | 1 | 3 | 9 Corvette badges | 9 -> clamped 3 |
| 7 | defenseCapIncrease | 20 | 100 | Hero of Tatooine ONLY (5 POI) | 100 = cap exact |
| 8 | armorDegradeReduction | 1250 | 10000 | Talus (4) + Rori (4) | 10000 = cap exact |
| 9 | weaponDegradeReduction | 1250 | 10000 | Dantooine (4) + Endor (4) | 10000 = cap exact |
| 10 | seaCapIncrease | 20 | 100 | Tat dangerous (4) + Dat Sarlacc | 100 = cap exact |
| 11 | movementSpeed | 2500 | uncapped | Naboo exploration (4) | +100% speed |
| 12 | buffDuration | 2500 | uncapped | 13 badges (Tat 3 / Yavin 2 / Science 3 / Social 5) | +325% duration |
| 13 | experienceMultiplier | 10000 | uncapped | Corellia exploration (5) | +500% (6x XP) |
| 14 | practiceExperienceBonus | 10000 | uncapped | Tat easy (3) + Yavin (2) + Social (5) | +1000% practice XP |
| 15 | craftingSpeed | 1000 | uncapped | 9 crafting masters incl Shipwright? NO -- Combat Medic replaced it: 8 ground crafting + Doctor + Combat Medic | +100% craft speed |
| 16 | amazingSuccessChance | 500 | 10000 | Exar Kun (1) + Lok (3) + Dathomir (6) | 5000 = 50% (cap headroom intentional) |
| 17 | amazingResults | 1000 | 10000 | same 10-badge set | 10000 = cap exact |
| 18 | gatheringQuantity | 10000 | uncapped | Tat easy (3) + Yavin (2) + Outdoors (5) + Science (3) | ~130000 bp (~1300%, ~14x) |

### B. Structural requirements

- ZERO active `badgeOverrides` blocks in config.lua (grep
  `^\s*badgeOverrides\s*=` must return nothing).
- EVERY modifier carries an INACTIVE commented placeholder block:
  three-line notation explaining `{ "badge_key", basisPointValue }`
  replacement semantics plus one example line using a key from its own
  list.
- No badge key appears twice within one modifier's list.
- Faction-paired badges (Corvette Imp/Reb, COA Rebel/Imperial) are
  annotated as partially exclusive wherever their reachable total
  differs from listed total.

## Deliverables

1. **Verification pass**: compare every modifier block in config.lua
   against table A and requirement B above. List every mismatch found.
2. **Corrections**: apply minimal fixes for each mismatch (wrong value,
   missing placeholder, stray active override, duplicated keys).
3. **Doc consistency sweep** ([PROC R6.10]):
   - `CODE_REFERENCE.md` Appendix A rows must match the table above.
   - `USER_GUIDE.md` feature maxima must match (60% crit, 85% triple,
     51% quad, 120 defense pts, 100 SEA pts, 325% buffs, ~1300%
     gathering, etc.).
   - No document may claim override-dependent defaults.
4. **Report**: append a signed section to this brief listing each check,
   its result, and any corrections applied.
5. Stamps on touched trackers ([PROC R6]).

## Acceptance criteria

- All 18 rows match config.lua exactly; grep proves zero active
  overrides and zero duplicate keys per modifier list.
- Placeholder blocks present for all 18 modifiers.
- Docs consistent with config (no stale numbers).
- Single commit tagged `[BRIEF-017]`, pushed ([PROC R6.5]).
- Compile unaffected (Lua/docs only) but restart-to-apply reminder
  stands for server owners.

## Out of scope

- Changing any value, cap, badge assignment, or tier mechanic.
- ERR-009 follow-ups beyond what BRIEF-015 already delivered.
- SWGEmu core files.

---

## Verification Report (independent execution)

- Executor: hy3-free (opencode/hy3-free), 08242026
- Scope note: the previous (broken) executor had left UNCOMMITTED working-copy
  edits to `CODE_REFERENCE.md` and `USER_GUIDE.md` (value updates carried over
  from the owner's final walkthrough rulings) plus corruption in `config.lua`.
  This session finished that work: completed the doc sweep, repaired the
  corruption, and committed everything under `[BRIEF-017]`.

### A. config.lua corruption found and fixed (byte-level, not value changes)

Three real tab-byte corruptions (the rest of the suspected `\t` entries were
JSON display artifacts -- the badge keys were actually clean):

| Line | Before | After | Effect if unfixed |
|------|--------|-------|-------------------|
| 8 | `customSummaryColor = "0<TAB>0FF00"` | `"00FF00"` | broken color string |
| 196 (seaCapIncrease) | `cap = 10<TAB>0` | `cap = 100` | Lua parse error (stray token) |
| 221 (buffDuration) | `cap = <TAB>0` | `cap = 0` | malformed numeric literal |

No badge KEY contained a tab. `bdg_exp_2_badges` (suspected digit typo) does
NOT exist -- line 103 already reads `bdg_exp_20_badges`. Cleared as
false-positive.

Structural checks (all pass, verified via grep/[PROC R6.10]):
- `grep -cP '^\s*badgeOverrides\s*='` => 0 active overrides (correct).
- All 18 modifiers carry an inactive commented placeholder block (18
  `Placeholder:` lines present).
- No duplicate badge key within any single modifier's `badges` list.

### B. Doc consistency sweep ([PROC R6.10])

The prior uncommitted edits had correctly updated Triple (500 bp / cap 8500),
Quad (300 bp / cap 5100), Buff Duration (325%), Gathering Quantity (1300%),
Amazing Results (10-badge set) in Appendix A and the USER_GUIDE quick
reference. This session repaired what they left broken/incomplete:

1. **Malformed markdown table headers** -- the prior run had doubled the pipes
   on 5 table headers (`|| Property | Value ||`). Restored to `| Property |
   Value |`. (5 in CODE_REFERENCE Appendix A, 1 in USER_GUIDE.)
2. **Stale `badgeOverrides` prose** -- replaced the "shipped defaults may
   include overrides" + `badgeOverrides` convention block (Appendix A
   Configuration Conventions) with the uniform `badgeBonus` + inactive
   placeholder convention mandated by BRIEF-016. Removed residual "via explicit
   badgeOverrides" / "via badgeOverride" phrasings in the Critical Chance and
   SUI menu detail sections.
3. **Two values left stale against config.lua** (verified against
   `config.lua` [PROC R6.10]):
   - Defense Cap Increase: was 5 pts / cap 0 -> now **20 pts / cap 100**
     (matches `defenseCapIncrease.badgeBonus = 20, cap = 100`).
   - SEA Cap Increase: was 15 pts / cap 0 -> now **20 pts / cap 100**
     (matches `seaCapIncrease.badgeBonus = 20, cap = 100`).
     Max-totals row already read 100 pts for both; the per-modifier rows now
     agree.

All 18 rows in Appendix A now match config.lua; no document claims
override-dependent defaults.

### C. Acceptance criteria

- [x] All 18 rows match config.lua; grep proves zero active overrides, zero
      duplicate keys.
- [x] Placeholder blocks present for all 18 modifiers.
- [x] Docs consistent with config (no stale numbers).
- [x] Single commit tagged `[BRIEF-017]`, pushed.
- [x] Compile unaffected; restart reminder noted.
