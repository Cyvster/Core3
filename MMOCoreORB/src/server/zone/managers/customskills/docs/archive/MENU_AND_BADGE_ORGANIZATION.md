# Menu and Badge Organization

## Separation of concerns

The project needs two related but distinct hierarchies:

1. **Modifier hierarchy** -- organizes the totals a character wants to inspect.
2. **Badge hierarchy** -- organizes the 140 badge definitions that may
   contribute to those totals.

The main `/customskills` interface starts with the character's accumulated
bonuses and a Badges entry. Additional systems may add more entries later.

## Proposed main menu

```text
Custom Skills

Accumulated Bonuses
+7% Critical Damage
+4 Melee Defense
+2% Movement Speed

Badges
```

The summary contains only bonuses currently granted by badges the character
has acquired. It never shows a total-possible value.

## Modifier pages

### Offense

- Critical Chance
- Critical Multiplier
- Double Attack Chance
- Triple Attack Chance
- Quad Attack Chance
- Armor Penetration

### Defense and equipment

- Defense Cap Increase
- SEA Cap Increase
- Armor Degradation Reduction
- Weapon Degradation Reduction

### Utility

- Movement Speed
- Buff Duration
- Experience Bonus
- Practice Mode Experience Bonus

### Crafting and gathering

- Crafting Speed
- Crafting Amazing Success Chance
- Amazing Results
- Gathering Quantity

No modifier in this document is approved for gameplay merely by being listed.
Each requires separately specified semantics, stacking, caps, and engine hooks.

## Modifier detail page

Each selectable modifier opens a source breakdown:

```text
Custom Skills > Offense > Critical Chance

Total                                      +3.00%
Ben Kenobi's Old Home                      +1.00%
Master Pistoleer                           +1.00%
Rebel Corvette: Destroy                    +1.00%
```

The total and rows must come from the same modifier service used by gameplay.

## Badges menu hierarchy

Core3's six internal badge types remain authoritative metadata, but the player
interface maps them into four understandable groups:

```text
Badges
  Milestone Badges
  Exploration
    Milestone Exploration
    Tatooine
    Naboo
    Corellia
    Yavin IV
    Lok
    Dathomir
    Dantooine
    Endor
    Talus
    Rori
  Profession
  Quest
  Event
```

Core3's accumulation records are real, automatically awarded badges. They are
presented in two explicit menu categories:

### Milestone Badges (12) -- +12.5% Crit Multiplier, +1% Crit Chance each

Accumulation:

- `count_5`
- `count_10`
- `count_25`
- `count_50`
- `count_75`
- `count_100`
- `count_125`

Exploration:

- `bdg_exp_10_badges`
- `bdg_exp_20_badges`
- `bdg_exp_30_badges`
- `bdg_exp_40_badges`
- `bdg_exp_45_badges`

### Exploration > Milestone Exploration (included in Milestone Badges above)

- `bdg_exp_10_badges`
- `bdg_exp_20_badges`
- `bdg_exp_30_badges`
- `bdg_exp_40_badges`
- `bdg_exp_45_badges`

### Exploration > planets

The Exploration page lists planets. Selecting a planet lists its associated
badges. Risk subtypes (`easy`, `dangerous`, and `jedi`) remain badge metadata
and can be shown in the badge detail text, but are not navigation levels.

#### Tatooine (3 easy) -- +100% Practice XP, +25% Buff Duration, +25% Gathering Quantity each

Easy:

- `exp_tat_escape_pod`
- `exp_tat_lars_homestead`
- `exp_tat_bens_hut`

#### Tatooine (4 dangerous) -- +15 SEA Cap each

- `exp_tat_tusken_pool`
- `exp_tat_krayt_skeleton`
- `exp_tat_sarlacc_pit`
- `exp_tat_krayt_graveyard`

#### Naboo (4) -- +25% Movement Speed each (100% max)

- `exp_nab_gungan_sacred_place`
- `bdg_exp_nab_theed_falls_bottom`
- `bdg_exp_nab_deeja_falls_top`
- `bdg_exp_nab_amidalas_sandy_beach`

#### Corellia (5) -- +100% Experience Bonus each (500% max)

- `exp_cor_agrilat_swamp`
- `bdg_exp_cor_rebel_hideout`
- `bdg_exp_cor_rogue_corsec_base`
- `bdg_exp_cor_tyrena_theater`
- `bdg_exp_cor_bela_vistal_fountain`

#### Yavin IV (3)

- `exp_yav_temple_exar_kun` -- +5 Defense Cap, +5% Amazing Success, +10% Amazing Results
- `exp_yav_temple_woolamander` -- +100% Practice XP, +25% Gathering Quantity, +25% Buff Duration
- `exp_yav_temple_blueleaf` -- +100% Practice XP, +25% Gathering Quantity, +25% Buff Duration

#### Lok (3) -- +5 Defense Cap, +5% Amazing Success, +10% Amazing Results each

- `exp_lok_volcano`
- `bdg_exp_lok_imp_outpost`
- `bdg_exp_lok_kimogila_skeleton`

#### Dathomir (7)

#### Dathomir (6 easy) -- +5 Defense Cap, +5% Amazing Success, +10% Amazing Results each

Easy:

- `exp_dat_tarpit`
- `exp_dat_escape_pod`
- `exp_dat_misty_falls_1`
- `exp_dat_misty_falls_2`
- `bdg_exp_dat_crashed_ship`
- `bdg_exp_dat_imp_prison`

Dangerous (1) -- +15 SEA Cap:

- `exp_dat_sarlacc`

#### Dantooine (4) -- +12.5% Weapon Degradation Reduction each

- `exp_dan_jedi_temple`
- `exp_dan_rebel_base`
- `bdg_exp_dan_dantari_village1`
- `bdg_exp_dan_dantari_village2`

#### Endor (4) -- +12.5% Weapon Degradation Reduction each

- `bdg_exp_end_ewok_tree_village`
- `bdg_exp_end_ewok_lake_village`
- `bdg_exp_end_dulok_village`
- `bdg_exp_end_imp_outpost`

#### Talus (4) -- +12.5% Armor Degradation Reduction each

- `bdg_exp_tal_creature_village`
- `bdg_exp_tal_imp_base`
- `bdg_exp_tal_imp_vs_reb_battle`
- `bdg_exp_tal_aqualish_cave`

#### Rori (4) -- +12.5% Armor Degradation Reduction each

- `bdg_exp_ror_kobala_spice_mine`
- `bdg_exp_ror_rebel_outpost`
- `bdg_exp_ror_imp_camp`
- `bdg_exp_ror_imp_hyperdrive_fac`

### Profession

#### Combat (12) -- +4% Crit Chance, +2% Triple, +2% Quad each (bounty hunter gets +3% Triple, +3% Quad)

- `combat_1hsword_master`
- `combat_2hsword_master`
- `combat_bountyhunter_master` -- +4% Crit, +3% Triple, +3% Quad
- `combat_brawler_master`
- `combat_carbine_master`
- `combat_commando_master`
- `combat_marksman_master`
- `combat_pistol_master`
- `combat_polearm_master`
- `combat_rifleman_master`
- `combat_smuggler_master`
- `combat_unarmed_master`

#### Crafting (9) -- +10% Crafting Speed each

- `crafting_architect_master`
- `crafting_armorsmith_master`
- `crafting_artisan_master`
- `crafting_chef_master`
- `crafting_droidengineer_master`
- `crafting_merchant_master`
- `crafting_shipwright`
- `crafting_tailor_master`
- `crafting_weaponsmith_master`

#### Outdoors (5) -- +115% Gathering Quantity each

- `outdoors_bio_engineer_master`
- `outdoors_creaturehandler_master`
- `outdoors_ranger_master`
- `outdoors_scout_master`
- `outdoors_squadleader_master`

#### Science (3) -- +100% Gathering Quantity, +25% Buff Duration each; Doctor also gets +10% Crafting Speed

- `science_combatmedic_master`
- `science_doctor_master` -- +10% Crafting Speed
- `science_medic_master`

#### Social (5) -- +20% Buff Duration, +100% Practice XP each

- `social_dancer_master`
- `social_entertainer_master`
- `social_imagedesigner_master`
- `social_musician_master`
- `social_politician_master`

#### Pilot (9) -- excluded from bonuses until JTL is available

- `pilot_imperial_navy_corellia`
- `pilot_imperial_navy_naboo`
- `pilot_imperial_navy_tatooine`
- `pilot_neutral_corellia`
- `pilot_neutral_naboo`
- `pilot_neutral_tatooine`
- `pilot_rebel_navy_corellia`
- `pilot_rebel_navy_naboo`
- `pilot_rebel_navy_tatooine`

### Quest

#### Hero of Tatooine (5) -- +10% Triple Attack, +5% Quad Attack, +10 Defense Cap each

- `poi_rabidbeast`
- `poi_prisonbreak`
- `poi_twoliars`
- `poi_factoryliberation`
- `poi_heromark`

#### Warren (2) -- +20% Double Attack Chance each

- `warren_compassion`
- `warren_hero`

#### Theme Parks (4) -- +20% Double Attack Chance each

- `bdg_thm_park_jabba_badge`
- `bdg_thm_park_imperial_badge`
- `bdg_thm_park_rebel_badge`
- `bdg_thm_park_nym_badge`

#### Corellian Corvette (9) -- +1 Armor Penetration each

- `bdg_corvette_imp_destroy`
- `bdg_corvette_imp_rescue`
- `bdg_corvette_imp_assassin`
- `bdg_corvette_neutral_destroy`
- `bdg_corvette_neutral_rescue`
- `bdg_corvette_neutral_assassin`
- `bdg_corvette_reb_destroy`
- `bdg_corvette_reb_rescue`
- `bdg_corvette_reb_assassin`

### Event

#### Cries of Alderaan (5)

- `event_coa2_imperial`
- `event_coa2_rebel`
- `event_coa3_imperial`
- `event_coa3_rebel`
- `event_project_dead_eye_1`

#### Accolades (7) -- admin event badges, excluded from bonuses

- `acc_brave_soldier`
- `acc_fascinating_background`
- `acc_good_samaritan`
- `acc_interesting_personage`
- `acc_professional_demeanor`
- `bdg_accolade_home_show`
- `bdg_accolade_live_event`

#### Librarian (1)

- `bdg_library_trivia`

#### Racing (6) -- 3 visible, 3 admin event (excluded from bonuses)

Visible:

- `bdg_racing_agrilat_swamp`
- `bdg_racing_keren_city`
- `bdg_racing_mos_espa`

Admin event (excluded):

- `bdg_racing_lok_marathon`
- `bdg_racing_narmle_memorial`
- `bdg_racing_nashal_river`

#### Death Star (1) -- admin event, excluded from bonuses

- `destroy_deathstar`

## Maximum bonus totals

Total eligible badges: **111**

The table below shows the maximum attainable bonus for each modifier when a
character has acquired all 111 eligible badges. Actual values depend on which
badges are assigned to each modifier in `config.lua`.

| Modifier | Unit | Per-badge value | Max badges | Maximum total |
| --- | --- | --- | --- | --- |
| Critical Chance | Basis points | 400 | 12 | 6000 (60.00%) |
| Critical Multiplier | Basis points | 1250 | 12 | 15000 (150.00%) bonus (300.00% total) |
| Double Attack Chance | Basis points | 2000 | 6 | 10000 (100.00%) |
| Triple Attack Chance | Basis points | 200 | 17 | 7500 (75.00%) |
| Quad Attack Chance | Basis points | 200 | 17 | 5000 (50.00%) |
| Armor Penetration | Levels | 1 | 9 | 9 levels (effective max 3) |
| Defense Cap Increase | Points | 5 | 15 | 100 points |
| SEA Cap Increase | Points | 15 | 5 | 75 points |
| Armor Degradation Reduction | Basis points | 1250 | 8 | 10000 (100.00%) |
| Weapon Degradation Reduction | Basis points | 1250 | 8 | 10000 (100.00%) |
| Movement Speed | Basis points | 2500 | 4 | 10000 (100.00%) |
| Buff Duration | Basis points | 2500 | 11 | 30000 (300.00%) |
| Experience Bonus | Multiplier | 10000 | 5 | 50000 (500.00%) |
| Practice Mode Experience Bonus | Basis points | 10000 | 8 | 100000 (1000.00%) |
| Crafting Speed | Multiplier | 1000 | 10 | 10000 (100.00%) |
| Crafting Amazing Success Chance | Basis points | 500 | 10 | 5000 (50.00%) |
| Amazing Results | Basis points | 1000 | 10 | 10000 (100.00%) |
| Gathering Quantity | Basis points | 11500 | 13 | 100000 (1000.00%) |

### Badge pool by category

| Category | Eligible badges | Notes |
| --- | ---: | --- |
| Milestone Badges | 7 | Accumulation milestones |
| Exploration Milestones | 5 | Exploration count milestones |
| Exploration Planets | 45 | All planet exploration badges |
| Combat | 12 | Ground combat mastery |
| Crafting | 9 | Ground crafting mastery + Shipwright |
| Outdoors | 5 | Outdoors mastery |
| Science | 3 | Science mastery |
| Social | 5 | Social mastery |
| Quest | 20 | Hero, Warren, Theme Parks, Corvette |
| **Total** | **111** | |

## Visibility policy

- `show = 1` badges may appear in normal badge browsing.
- `show = 0` badges remain hidden from a general browser by default.
- A hidden badge must not grant a modifier unless it is explicitly enabled in
  the modifier registry.
- If an enabled hidden badge contributes to a character, its source row should
  still be shown on that character's modifier breakdown; otherwise the total
  would be unexplained.
- Visibility does not determine whether the underlying badge is owned.

## Registry organization

Badge metadata and modifier assignments must remain separate:

```text
BadgeDefinition
  key
  resolved index
  Core3 type
  exploration subtype
  source group
  show flag

BadgeModifierAssignment
  badge key
  modifier ID
  amount
  stacking rule
```

This prevents menu grouping from becoming gameplay logic and lets another
server replace badge indices without rewriting modifier definitions.

## SUI navigation rules

- Keep pages short enough to scan in the stock list-box template.
- Prefix every badge with only a colored ownership marker: green `O` when
  acquired and red `X` when not acquired. Reset the color before the badge
  name so the name uses the normal client text color.
- Do not display a marker legend or the words `Acquired` / `Not Acquired`.
- Use one selection for one navigation action.
- Preserve Back and Cancel on all subpages.
- Continue allowing multiple Custom Skills windows.
- With the stock `Script.listBox`, OK opens the selected subpage, Back returns
  one level, and only Cancel exits. Badge-list pages label the action Refresh;
  submitting it or submitting without a valid selection reopens the page.
- Single-click navigation, right-click Back, and reliable double-click
  submission require a client-side SUI modification.
- Do not show zero-valued modifier categories on the final summary unless the
  server configuration requests them.
- Sort modifier totals by stable configured display order, not by badge index.
- Sort source rows by contribution magnitude and then display name.

## Badge display order

CRC values and badge indices are internal identifiers and must never determine
player-facing order. Use explicit configured order where progression matters
and alphabetical display-name order otherwise:

- `Badges`: Milestone Badges, Exploration, Profession, Quest, Event.
- `Milestone Badges`: numeric threshold ascending.
- `Exploration`: Milestone Exploration first, then planets alphabetically.
- Badges within each planet: alphabetically by display name.
- `Milestone Exploration`: numeric threshold ascending.
- `Profession`: profession family in configured order, then profession name
  alphabetically within the family.
- `Quest`: quest line in configured order; badges within a quest line follow
  story/progression order rather than alphabetical order.
- `Event`: event group in configured order; chronological/progression order
  within a multi-stage event and alphabetical order for independent badges.

Store an explicit display-order field so future additions do not accidentally
change established menu ordering when badge indices or keys change.

## First effective modifier

By default, each of the twelve ground combat profession mastery badges grants
300 basis points (3.00%) of Critical Chance. Owning all twelve grants 36.00% by
default. A successful critical attack deals 150.00% of its normal pre-armor
damage by default. Combat and menu display values must come from the same
modifier service.
The offense summary always lists base custom combat values such as Critical
Multiplier, even when the character has no acquired badge contribution. A
disabled mechanic remains visible but is marked inactive.
Critical hits temporarily produce a compact `(CRIT)` development label. Native
damage spam and its default color remain untouched. The label will be removed
after development because the stock client cannot append it to the damage line.

These defaults are configurable in `MMOCoreORB/bin/scripts/customskills/config.lua`.
Critical Chance can be disabled independently, and its badge bonus, multiplier,
and effect labels can be changed without editing C++. Values are
cached and reload on server restart.

## Badge and category bonus display

Append every configured badge bonus to the badge's row after its display name:

```text
O  Ben Kenobi's Old Home  (+1% Critical Damage)
X  Pool Beneath Fort Tusken  (+1% Critical Damage)
```

Only the ownership marker is colored. Badge names and bonuses use the normal
client text color. Omit the suffix when a badge has no configured bonus. Join
multiple bonuses with commas in stable modifier display order.

Every category page must show all accumulated bonuses granted by acquired
descendant badges. Put the full summary in the page prompt beneath the title,
not in a single category row, so it can wrap across lines:

```text
Badges > Exploration > Tatooine

Accumulated Bonuses
+3% Critical Damage
+2 Melee Defense
+1% Movement Speed

O  Ben Kenobi's Old Home  (+1% Critical Damage)
X  Pool Beneath Fort Tusken  (+1% Critical Damage)
O  Lars Homestead  (+2% Critical Damage, +2 Melee Defense)
```

Aggregation is recursive:

- a planet page totals its acquired badges;
- Exploration totals all exploration milestones and planets;
- Profession totals every profession family;
- Quest totals every quest line;
- Event totals every event group; and
- Badges totals every badge category.

Only acquired badges contribute to category totals. Totals must be obtained
from the shared modifier service and must honor the modifier's stacking, cap,
and rounding rules; the menu must not independently add display strings.

Every category entry also appends the character's acquired descendant-badge
count, recursively. For example: `Badges (20)`, `Exploration (12)`, and
`Rori (4)`. Do not display the possible badge count.

## Implementation order

1. Implement the typed modifier/category definitions without gameplay hooks.
2. Implement the badge metadata registry using stable keys.
3. Assign one test badge to one display-only modifier.
4. Replace placeholder menu rows with registry-driven rows.
5. Add modifier detail/source pages.
6. Validate no-badge, owned-badge, hidden-badge, and multi-window behavior.
7. Only then design the first effective gameplay modifier.
