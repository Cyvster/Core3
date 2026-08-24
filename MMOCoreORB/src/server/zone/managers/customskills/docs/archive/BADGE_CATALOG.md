# Badge Catalog from the Active `badge_map.iff`

This catalog is based on the 140-row `badge_map.iff` extract supplied for this
server. It supersedes public-guide estimates.

## Conclusions

Badges do not fall into only Exploration and Profession categories. Core3 maps
the table's type strings into six enum types:

| Core3 enum type | Table strings | Records |
|---|---|---:|
| `ACCUMULATION` | `accumulation` | 12 |
| `INTEREST` | `interest` | 5 |
| `EXPLORATION` | `exploration_easy`, `exploration_dangerous`, `exploration_jedi` | 45 |
| `MASTER` | `master` | 43 |
| `EVENT` | `events` | 20 |
| `CONTENT` | `content` | 15 |
| **Total** | | **140** |

Indices are contiguous from 0 through 139 with no duplicates. There are 124
rows with `show = 1` and 16 with `show = 0`. Visibility is metadata and must
not be treated as proof that a badge can or cannot be awarded.

The columns are index, stable key, music, client category, show flag, and type.
Modifier definitions should store the stable key and resolve it through
`BadgeList`; hard-coding indices would be fragile across modified TRE files.

## Exact key inventory

### Accumulation (12)

- `0 count_5`; `1 count_10`; `2 count_25`; `3 count_50`
- `4 count_75`; `5 count_100`; `6 count_125`
- `100 bdg_exp_10_badges`; `101 bdg_exp_20_badges`
- `102 bdg_exp_30_badges`; `103 bdg_exp_40_badges`; `104 bdg_exp_45_badges`

### Interest (5)

These are Hero of Tatooine progression records, not librarian or race badges.

- `7 poi_rabidbeast`; `8 poi_prisonbreak`; `9 poi_twoliars`
- `10 poi_factoryliberation`; `11 poi_heromark`

### Exploration (45)

#### Jedi (3)

- `12 exp_tat_bens_hut`; `23 exp_yav_temple_exar_kun`; `30 exp_dan_jedi_temple`

#### Dangerous (5)

- `13 exp_tat_tusken_pool`; `14 exp_tat_krayt_skeleton`
- `16 exp_tat_sarlacc_pit`; `18 exp_tat_krayt_graveyard`; `26 exp_dat_sarlacc`

#### Easy (37)

- `15 exp_tat_escape_pod`; `17 exp_tat_lars_homestead`
- `19 exp_nab_gungan_sacred_place`; `20 exp_cor_agrilat_swamp`
- `21 exp_yav_temple_woolamander`; `22 exp_yav_temple_blueleaf`
- `24 exp_lok_volcano`; `25 exp_dat_tarpit`; `27 exp_dat_escape_pod`
- `28 exp_dat_misty_falls_1`; `29 exp_dat_misty_falls_2`; `31 exp_dan_rebel_base`
- `75 bdg_exp_nab_theed_falls_bottom`; `76 bdg_exp_nab_deeja_falls_top`
- `77 bdg_exp_nab_amidalas_sandy_beach`; `78 bdg_exp_cor_rebel_hideout`
- `79 bdg_exp_cor_rogue_corsec_base`; `80 bdg_exp_cor_tyrena_theater`
- `81 bdg_exp_cor_bela_vistal_fountain`; `82 bdg_exp_dat_crashed_ship`
- `83 bdg_exp_dat_imp_prison`; `84 bdg_exp_dan_dantari_village1`
- `85 bdg_exp_dan_dantari_village2`; `86 bdg_exp_end_ewok_tree_village`
- `87 bdg_exp_end_ewok_lake_village`; `88 bdg_exp_end_dulok_village`
- `89 bdg_exp_end_imp_outpost`; `90 bdg_exp_tal_creature_village`
- `91 bdg_exp_tal_imp_base`; `92 bdg_exp_tal_imp_vs_reb_battle`
- `93 bdg_exp_tal_aqualish_cave`; `94 bdg_exp_ror_kobala_spice_mine`
- `95 bdg_exp_ror_rebel_outpost`; `96 bdg_exp_ror_imp_camp`
- `97 bdg_exp_ror_imp_hyperdrive_fac`; `98 bdg_exp_lok_imp_outpost`
- `99 bdg_exp_lok_kimogila_skeleton`

### Master (43)

#### Ground professions (33)

- `42 combat_1hsword_master`; `43 combat_2hsword_master`
- `44 combat_bountyhunter_master`; `45 combat_brawler_master`
- `46 combat_carbine_master`; `47 combat_commando_master`
- `48 combat_marksman_master`; `49 combat_pistol_master`
- `50 combat_polearm_master`; `51 combat_rifleman_master`
- `52 combat_smuggler_master`; `53 combat_unarmed_master`
- `54 crafting_architect_master`; `55 crafting_armorsmith_master`
- `56 crafting_artisan_master`; `57 crafting_chef_master`
- `58 crafting_droidengineer_master`; `59 crafting_merchant_master`
- `60 crafting_tailor_master`; `61 crafting_weaponsmith_master`
- `62 outdoors_bio_engineer_master`; `63 outdoors_creaturehandler_master`
- `64 outdoors_ranger_master`; `65 outdoors_scout_master`
- `66 outdoors_squadleader_master`; `67 science_combatmedic_master`
- `68 science_doctor_master`; `69 science_medic_master`
- `70 social_dancer_master`; `71 social_entertainer_master`
- `72 social_imagedesigner_master`; `73 social_musician_master`
- `74 social_politician_master`

#### Space professions (10)

- `129 crafting_shipwright`
- `130 pilot_rebel_navy_naboo`; `131 pilot_rebel_navy_corellia`
- `132 pilot_rebel_navy_tatooine`; `133 pilot_imperial_navy_naboo`
- `134 pilot_imperial_navy_corellia`; `135 pilot_imperial_navy_tatooine`
- `136 pilot_neutral_naboo`; `137 pilot_neutral_corellia`
- `138 pilot_neutral_tatooine`

### Events (20)

- `32 event_project_dead_eye_1` (`show = 0`)
- `33 acc_good_samaritan` (`show = 0`)
- `34 acc_fascinating_background` (`show = 0`)
- `35 acc_brave_soldier` (`show = 0`)
- `36 acc_interesting_personage` (`show = 0`)
- `37 acc_professional_demeanor` (`show = 0`)
- `40 event_coa2_rebel` (`show = 0`); `41 event_coa2_imperial` (`show = 0`)
- `109 event_coa3_rebel` (`show = 0`); `110 event_coa3_imperial` (`show = 0`)
- `111 bdg_library_trivia`
- `121 bdg_racing_agrilat_swamp`; `122 bdg_racing_keren_city`
- `123 bdg_racing_mos_espa`
- `124 bdg_accolade_live_event` (`show = 0`)
- `125 bdg_racing_lok_marathon` (`show = 0`)
- `126 bdg_racing_narmle_memorial` (`show = 0`)
- `127 bdg_racing_nashal_river` (`show = 0`)
- `128 destroy_deathstar` (`show = 0`)
- `139 bdg_accolade_home_show` (`show = 0`)

### Content (15)

- `38 warren_compassion`; `39 warren_hero`
- `105 bdg_thm_park_jabba_badge`; `106 bdg_thm_park_imperial_badge`
- `107 bdg_thm_park_rebel_badge`; `108 bdg_thm_park_nym_badge`
- `112 bdg_corvette_imp_destroy`; `113 bdg_corvette_imp_rescue`
- `114 bdg_corvette_imp_assassin`; `115 bdg_corvette_neutral_destroy`
- `116 bdg_corvette_neutral_rescue`; `117 bdg_corvette_neutral_assassin`
- `118 bdg_corvette_reb_destroy`; `119 bdg_corvette_reb_rescue`
- `120 bdg_corvette_reb_assassin`

## Client category totals

The numeric category column is separate from Core3's type enum:

| Category | Records | `show = 1` | `show = 0` |
|---:|---:|---:|---:|
| 0 | 12 | 7 | 5 |
| 1 | 6 | 6 | 0 |
| 2 | 54 | 54 | 0 |
| 3 | 43 | 43 | 0 |
| 4 | 14 | 3 | 11 |
| 5 | 11 | 11 | 0 |
