# Challenge Tier Skill Trees

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Three new fourbyfour skill trees for the Challenge Tier system using the
`challenge_tier` XP type.

## Agreed framework

- **3 trees**, each fourbyfour (novice + 4 branches x 4 tiers + master)
- **Trees stack sequentially**: master one tree to unlock the next (Survivor -> Veteran -> Paragon)
- **XP type**: `challenge_tier`
- **POINTS_REQUIRED**: 0 (all entries)

### challenge_tier XP acquisition (agreed 08212026)

- Awarded **only while the player is in Challenge Tier 1 or higher** (opted into the system).
  Zero XP at tier 0 / opted out.
- Amount per qualifying kill = the **creature's combat level**, multiplied by the active
  Challenge Tier XP bonus and any other standard multipliers.
- **Composition rule (agreed)**: badge Experience Bonus **multiplies against** the CT XP bonus
  (standard project convention -- separate multipliers compose multiplicatively).
- **Planning assumptions (agreed)**: every player carries the full badge set (~ **6x**) -- it is
  among the first things anyone acquires; late-game CT bonus ~ **1000%** (**11x**).
  Combined planning rate: CL x 11 x 6 = **CL x 66** per kill at endgame.
- Self-scaling loop: higher challenge tiers spawn/tolerate higher-CL content, which pays
  proportionally more XP toward the trees.
- Design intent: the trees are a **long-term stretch goal** -- players work toward them after
  completing everything else. Progression speed must be gated behind *high* challenge tiers;
  low-tier farming should not be able to fund meaningful tree progress.

## Challenge Tier ladder - APPROVED 08242026

Ten tiers; players opt in by entering Tier 1 (`challenge_tier` XP requires Tier 1+).
XP bonus per tier, applied to qualifying kills (composes with badge Experience Bonus
per the agreed composition rule):

| Tier | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|------|---|---|---|---|---|---|---|---|---|----|
| XP bonus | +25% | +50% | +100% | +175% | +275% | +400% | +550% | +700% | +850% | +1000% |

Top of ladder = 11x, exactly the planning assumption behind XP costs v2
(CL x 11 x ~6 badge multiplier = CL x 66 endgame rate). Early tiers are
deliberately small so opted-in low-tier farming stays weak until deep tiers,
preserving the gating intent.

## XP costs v2 - APPROVED 08212026; implemented in skills.csv

Per user direction: requirements **increase per tree** but are **uniform within a tree**
(every box in a tree costs the same, novice through master).

| Tree | Cost per box (x18) | Tree total |
|---|---:|---:|
| Survivor | 100,000 | 1,800,000 |
| Veteran | 350,000 | 6,300,000 |
| Paragon | 1,000,000 | 18,000,000 |
| **Full mastery** | | **26,100,000 `challenge_tier` XP** |

Grind projections at the agreed planning rate (CL x 66):

| Content | XP/kill | Kills per box | Kills for full mastery |
|---|---:|---:|---:|
| CL 250 endgame | 16,500 | ~6-61 depending on tree | **~1,580** |
| CL 150 high-tier | 9,900 | ~10-101 | ~2,640 |
| CL 50 low-tier (gated out) | 3,300 | ~30-303 | ~7,900 |

~1,600 best-case endgame kills = weeks/months of dedicated play; low-tier farming is
economically excluded. Scale factor is a single dial -- double all three constants for a
~3,200-kill ceiling if playtests feel too fast.

**Implementation status**: all 57 rows (3 hidden tree roots + 54 boxes) appended to
`G:\Data\swgemu server\TRE builder\datatables\skill\skills.csv` on 08212026. Verified:
unique names, 27 columns per row, per-mod sums exactly match the Distribution v1 targets,
per-tree XP totals 1.8M / 6.3M / 18M = 26.1M grand total. Rebuild `skills.iff` with the
TRE builder to deploy. Banking cap comes from `xp_limits.iff` (`challenge_tier` = 5,100,000,
already present); every box also declares XP_CAP 5100000 as a self-contained fallback.

~~v1 (SUPERSEDED 08212026): tier-escalating costs 25k/50k/85k/130k, masters 250k,
tree factors x1/x2/x3, grand total 8.5M -- rejected in favor of uniform-within-tree pricing.~~

## Selected skill mods

Values below are the **maximum totals across all three trees** (a character who masters
everything). Distribution across trees/branches/tiers is fixed by Distribution v1 below.

| Group | Mod | Total | Notes |
|-------|-----|------:|-------|
| Primary | `melee_defense` | **100** | |
| Primary | `ranged_defense` | **100** | |
| Primary | `melee_accuracy` | **100** | |
| Primary | `ranged_accuracy` | **100** | |
| Secondary | `combat_equillibrium` | **100** | |
| Secondary | `dodge` | **100** | weapon-dependent (1H/pistol-class) |
| Secondary | `block` | **100** | weapon-dependent (polearm-class) |
| Secondary | `counterattack` | **100** | weapon-dependent (2H-class) |
| Secondary | `force_luck` | **100** | crafting/loot luck rolls |
| Secondary | `stun_defense` | **100** | |
| Secondary | `dizzy_defense` | **100** | |
| Secondary | `blind_defense` | **100** | |
| Secondary | `knockdown_defense` | **100** | |
| Secondary | `intimidate_defense` | **100** | |
| Secondary | `posture_change_down_defense` | **100** | vs knockdown/prone forces |
| Secondary | `posture_change_up_defense` | **100** | vs forced standing |
| Avoidance | `dodge_attack` | **10** | deliberately low -- adds *above* the 125 defense cap, so every point counts at endgame |
| DoT resistance | `bleed_resist` | **100** | |
| DoT resistance | `fire_resist` | **100** | |
| DoT resistance | `poison_disease_resist` | **100** | stacks with native `resistance_poison` / `resistance_disease` in DoT potency checks |
| Gathering | `creature_harvesting` | **1000** | high-rate server decision (08212026). Mod = % of corpse max yield, so 1000 ~ 10x base corpse yield before density (up to x1.25) and group (up to x1.4) multipliers -- up to ~19.5x with full bonuses. Natives reach only ~115, so tree masters see an order-of-magnitude jump. Verify resource-container stack limits handle the yields during testing |

### Mod pool reference

### Primary
- `melee_defense`
- `ranged_defense`
- `melee_accuracy`
- `ranged_accuracy`

### Secondary
- `combat_equillibrium`
- `dodge`
- `block`
- `counterattack`
- `force_luck`
- `stun_defense`
- `dizzy_defense`
- `blind_defense`
- `knockdown_defense`
- `intimidate_defense`
- `posture_change_down_defense`
- `posture_change_up_defense`

### Additional (confirmed engine-readable, added 08212026)

All verified as generic `getSkillMod()` readers -- work from learned-skill (SKILLBOX) grants:

**Avoidance**
- `dodge_attack` -- adds to primary defense *above* the 125 hard cap

**State resistance** (the five state defenses above are already the complete set; resisted states fail silently)
- *(already listed)* `knockdown_defense`, `dizzy_defense`, `stun_defense`, `blind_defense`, `intimidate_defense`

**Regeneration / DoT resistance**
- `bleed_resist` -- reduces bleeding DoT application
- `fire_resist` -- reduces fire DoT application
- `poison_disease_resist` -- reduces poison and disease DoT application

**Gathering**
- `creature_harvesting` -- creature resource harvest yield (`CreatureManagerImplementation`)

### Excluded from these trees (buff-only readers -- do nothing from SKILLBOX grants)
`xp_increase`, `craft_bonus`, `experiment_bonus`, `heal_recovery`, `incap_recovery`,
`reduce_spice_downtime`, `food_reduce`, `wookiee_roar`, `reduce_clone_wounds` (dead),
`mitigate_damage` (works, but emits combat-spam chat line per mitigated hit -- excluded by design decision).

### Excluded after investigation

- **`enhanced_regen`** -- rejected 08212026. It is only read by the Trandoshan innate `/regen`
  command (`RegenerationCommand.h`): converts points to CONSTITUTION on the 5-min innate buff
  (1 point = +1.75 CON, cap 100), which then regenerates ~CON x 0.00619 HP/s. Species-locked and
  command-gated, so it does nothing for any other species and nothing passively. No generic
  passive regen mod exists in the HAM tick -- it reads only CON/STAMINA/WILLPOWER.

## Progression model

- Trees **stack sequentially**: master Tree 1 to unlock Tree 2; master Tree 2 to unlock Tree 3.
- Design arc: **Survive** (Tree 1) -> **Fight** (Tree 2) -> **Prosper** (Tree 3). Universal passives
  come first, combat power second, prestige/economy rewards last.
- Unlock flag: each tree's `master` box is the prerequisite for the next tree's novice.
- No cross-tree skill gates otherwise; internal branch prereqs only.

## Distribution v1 - APPROVED 08212026

Budgets escalate with progression depth: **600 / 1010 / 1300** = 2910 total points,
exactly matching the sum of all mod targets in the table above.

### Tree 1 -- SURVIVOR (600)

| Box | Iron Blood (bleed/fire/poison) | Unshakeable (knockdown_def) | Anchored (posture-down def) | Rising (posture-up def) |
|---|---|---|---|---|
| **Novice** | all six mods +5 each *(single box)* | | | |
| Tier 1 | each DoT resist +10 | +15 | +15 | +15 |
| Tier 2 | each DoT resist +15 | +20 | +20 | +20 |
| Tier 3 | each DoT resist +20 | +25 | +25 | +25 |
| Tier 4 | each DoT resist +25 | +30 | +30 | +30 |
| **Master** | each DoT resist +25, knockdown +5, posture-down +5, posture-up +5 | | | |

Per-mod totals: bleed/fire/poison/knockdown/posture-down/posture-up = **100 each**. Tree total 600.

### Tree 2 -- VETERAN (1010)

| Box | Bulwark | Deadeye | Weaponwork | Senses |
|---|---|---|---|---|
| **Novice** | melee_def +5, ranged_def +5, melee_acc +5, ranged_acc +5, dodge +5 *(single box)* | | | |
| Tier 1 | melee_def & ranged_def +15 each | melee_acc & ranged_acc +15 each | block, counter, dodge +10 each | stun/dizzy/blind +10 each; dodge_attack +2 |
| Tier 2 | +20 each | +20 each | +15 each | states +15 each; dodge_attack +2 |
| Tier 3 | +25 each | +25 each | +20 each | states +20 each; dodge_attack +3 |
| Tier 4 | +30 each | +30 each | +25 each | states +25 each; dodge_attack +3 |
| **Master** | all four primaries +5, block +30, counter +30, dodge +25, each state def +30 | | | |

Per-mod totals: four primaries, block/counter/dodge, stun/dizzy/blind = **100 each**;
dodge_attack = **10**. Tree total 1010.

Weapon-dependent trio (block/counter/dodge) consolidated into one Weaponwork branch so
mixed-weapon players get everything in one line.

### Tree 3 -- PARAGON (1300)

| Box | Composure (equilibrium) | Resolve (intimidate def) | Fortuna (force_luck) | Harvester |
|---|---|---|---|---|
| **Novice** | eq +5, intim +5, luck +5, **harvest +50** *(single box)* | | | |
| Tier 1 | +15 | +15 | +15 | harvest +150 |
| Tier 2 | +20 | +20 | +20 | harvest +200 |
| Tier 3 | +25 | +25 | +25 | harvest +250 |
| Tier 4 | +30 | +30 | +30 | harvest +300 |
| **Master** | eq +5, intim +5, luck +5, **harvest +50** | | | |

Per-mod totals: equilibrium/intimidate/force_luck = **100 each**; creature_harvesting =
**1000**. Novice and master boxes deliberately mirror at 65 points each. Tree total 1300.

## Open questions

- ~~What is the parent skill for these trees?~~ -- proposed: no parent (standalone root trees)
- ~~What does each tree represent?~~ -- Survivor / Veteran / Paragon (approved)
- ~~What secondary defense mods go in which tree?~~ -- see Distribution v1 (approved)
- ~~Prerequisites between trees?~~ -- sequential stacking via master boxes (approved)
- ~~What are the XP costs and caps per tier?~~ -- uniform per-tree pricing, see XP costs v2 (approved + implemented)
- ~~How many Challenge Tiers exist, and what bonus does each tier grant?~~ --
  ten tiers, +25% to +1000%; see Challenge Tier ladder (approved 08242026)
- ~~Skill names (display strings) per box?~~ -- scheme A approved 08242026;
  filled into the Trees tables below; ships via skill_n.stf at deployment

## Trees

*(Box-level tables filled from Distribution v1 + XP costs v2 -- every box in a tree costs the same.)*

### Tree 1: survivor - 100,000 XP per box (implemented)

| Skill | Name | XP Cost | XP Cap | Skill Mods |
|-------|------|---------|--------|------------|
| novice | Novice Survivor | 100000 | 5100000 | bleed_resist=5, fire_resist=5, poison_disease_resist=5, knockdown_defense=5, posture_change_down_defense=5, posture_change_up_defense=5 |
| ironblood_01-04 | Iron Blood I-IV | 100000 | 5100000 | bleed/fire/poison each =10/15/20/25 |
| unshakeable_01-04 | Unshakeable I-IV | 100000 | 5100000 | knockdown_defense=15/20/25/30 |
| anchored_01-04 | Anchored I-IV | 100000 | 5100000 | posture_change_down_defense=15/20/25/30 |
| rising_01-04 | Rising I-IV | 100000 | 5100000 | posture_change_up_defense=15/20/25/30 |
| master | Master Survivor | 100000 | 5100000 | bleed/fire/poison +25 each, knockdown +5, posture-down +5, posture-up +5 |

### Tree 2: veteran - 350,000 XP per box (implemented)

| Skill | Name | XP Cost | XP Cap | Skill Mods |
|-------|------|---------|--------|------------|
| novice | Novice Veteran | 350000 | 5100000 | melee_defense=5, ranged_defense=5, melee_accuracy=5, ranged_accuracy=5, dodge=5 |
| bulwark_01-04 | Bulwark I-IV | 350000 | 5100000 | melee_defense & ranged_defense =15/20/25/30 each |
| deadeye_01-04 | Deadeye I-IV | 350000 | 5100000 | melee_accuracy & ranged_accuracy =15/20/25/30 each |
| weaponwork_01-04 | Weaponwork I-IV | 350000 | 5100000 | block, counterattack, dodge =10/15/20/25 each |
| senses_01-04 | Senses I-IV | 350000 | 5100000 | stun/dizzy/blind_defense =10/15/20/25 each; dodge_attack=2/2/3/3 |
| master | Master Veteran | 350000 | 5100000 | primaries +5 each, block +30, counter +30, dodge +25, states +30 each |

### Tree 3: paragon - 1,000,000 XP per box (implemented)

| Skill | Name | XP Cost | XP Cap | Skill Mods |
|-------|------|---------|--------|------------|
| novice | Novice Paragon | 1000000 | 5100000 | combat_equillibrium=5, intimidate_defense=5, force_luck=5, creature_harvesting=50 |
| composure_01-04 | Composure I-IV | 1000000 | 5100000 | combat_equillibrium=15/20/25/30 |
| resolve_01-04 | Resolve I-IV | 1000000 | 5100000 | intimidate_defense=15/20/25/30 |
| fortuna_01-04 | Fortuna I-IV | 1000000 | 5100000 | force_luck=15/20/25/30 |
| harvester_01-04 | Harvester I-IV | 1000000 | 5100000 | creature_harvesting=150/200/250/300 |
| master | Master Paragon | 1000000 | 5100000 | eq +5, intim +5, luck +5, harvesting +50 |

### Display names - APPROVED 08242026

Owner-approved scheme A: `Novice <Tree>` / `<Branch> I-IV` / `Master <Tree>`
using the existing branch themes. Hidden tree roots
(`challenge_survivor` / `challenge_veteran` / `challenge_paragon`) display
as Survivor / Veteran / Paragon if ever surfaced. Internal skill keys are
unchanged; names ship as client stringfile entries (skill_n.stf source for
the TRE builder -- same mechanism as the challenge_tier entry in
exp_n.stf) during deployment.

---

**Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
owner decisions recorded: Challenge Tier ladder (10 tiers, +25%..+1000%)
and box display names (scheme A, filled into tree tables); open questions
resolved. Prior: 08232026 -- governance banners added.
