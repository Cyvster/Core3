# Custom Skills — Badge Reference

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Complete badge catalog from `badge_map.iff` with Core3 types, menu organization, and integration requirements.

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

---

## Badge Inventory Summary

| Core3 Enum Type | Table Strings | Records | Show=1 | Show=0 |
|-----------------|---------------|---------|--------|--------|
| `ACCUMULATION` | `accumulation` | 12 | 7 | 5 |
| `INTEREST` | `interest` | 5 | 5 | 0 |
| `EXPLORATION` | `exploration_easy`, `exploration_dangerous`, `exploration_jedi` | 45 | 45 | 0 |
| `MASTER` | `master` | 43 | 43 | 0 |
| `EVENT` | `events` | 20 | 3 | 17 |
| `CONTENT` | `content` | 15 | 11 | 4 |
| **Total** | | **140** | **124** | **16** |

- Indices: contiguous 0–139, no duplicates
- Columns: index, stable key, music, client category, show flag, type
- **Always use stable keys** — never hard-code indices (fragile across TRE changes)

---

## Exact Key Inventory

### Accumulation (12)

| Index | Key | Notes |
|-------|-----|-------|
| 0 | `count_5` | |
| 1 | `count_10` | |
| 2 | `count_25` | |
| 3 | `count_50` | |
| 4 | `count_75` | |
| 5 | `count_100` | |
| 6 | `count_125` | |
| 100 | `bdg_exp_10_badges` | Exploration milestone |
| 101 | `bdg_exp_20_badges` | Exploration milestone |
| 102 | `bdg_exp_30_badges` | Exploration milestone |
| 103 | `bdg_exp_40_badges` | Exploration milestone |
| 104 | `bdg_exp_45_badges` | Exploration milestone |

---

### Interest (5) — Hero of Tatooine Progression

| Index | Key |
|-------|-----|
| 7 | `poi_rabidbeast` |
| 8 | `poi_prisonbreak` |
| 9 | `poi_twoliars` |
| 10 | `poi_factoryliberation` |
| 11 | `poi_heromark` |

---

### Exploration (45)

#### Jedi (3)
| Index | Key |
|-------|-----|
| 12 | `exp_tat_bens_hut` |
| 23 | `exp_yav_temple_exar_kun` |
| 30 | `exp_dan_jedi_temple` |

#### Dangerous (5)
| Index | Key |
|-------|-----|
| 13 | `exp_tat_tusken_pool` |
| 14 | `exp_tat_krayt_skeleton` |
| 16 | `exp_tat_sarlacc_pit` |
| 18 | `exp_tat_krayt_graveyard` |
| 26 | `exp_dat_sarlacc` |

#### Easy (37)
| Index | Key |
|-------|-----|
| 15 | `exp_tat_escape_pod` |
| 17 | `exp_tat_lars_homestead` |
| 19 | `exp_nab_gungan_sacred_place` |
| 20 | `exp_cor_agrilat_swamp` |
| 21 | `exp_yav_temple_woolamander` |
| 22 | `exp_yav_temple_blueleaf` |
| 24 | `exp_lok_volcano` |
| 25 | `exp_dat_tarpit` |
| 27 | `exp_dat_escape_pod` |
| 28 | `exp_dat_misty_falls_1` |
| 29 | `exp_dat_misty_falls_2` |
| 31 | `exp_dan_rebel_base` |
| 75 | `bdg_exp_nab_theed_falls_bottom` |
| 76 | `bdg_exp_nab_deeja_falls_top` |
| 77 | `bdg_exp_nab_amidalas_sandy_beach` |
| 78 | `bdg_exp_cor_rebel_hideout` |
| 79 | `bdg_exp_cor_rogue_corsec_base` |
| 80 | `bdg_exp_cor_tyrena_theater` |
| 81 | `bdg_exp_cor_bela_vistal_fountain` |
| 82 | `bdg_exp_dat_crashed_ship` |
| 83 | `bdg_exp_dat_imp_prison` |
| 84 | `bdg_exp_dan_dantari_village1` |
| 85 | `bdg_exp_dan_dantari_village2` |
| 86 | `bdg_exp_end_ewok_tree_village` |
| 87 | `bdg_exp_end_ewok_lake_village` |
| 88 | `bdg_exp_end_dulok_village` |
| 89 | `bdg_exp_end_imp_outpost` |
| 90 | `bdg_exp_tal_creature_village` |
| 91 | `bdg_exp_tal_imp_base` |
| 92 | `bdg_exp_tal_imp_vs_reb_battle` |
| 93 | `bdg_exp_tal_aqualish_cave` |
| 94 | `bdg_exp_ror_kobala_spice_mine` |
| 95 | `bdg_exp_ror_rebel_outpost` |
| 96 | `bdg_exp_ror_imp_camp` |
| 97 | `bdg_exp_ror_imp_hyperdrive_fac` |
| 98 | `bdg_exp_lok_imp_outpost` |
| 99 | `bdg_exp_lok_kimogila_skeleton` |

---

### Master (43)

#### Ground Professions (33)
| Index | Key |
|-------|-----|
| 42 | `combat_1hsword_master` |
| 43 | `combat_2hsword_master` |
| 44 | `combat_bountyhunter_master` |
| 45 | `combat_brawler_master` |
| 46 | `combat_carbine_master` |
| 47 | `combat_commando_master` |
| 48 | `combat_marksman_master` |
| 49 | `combat_pistol_master` |
| 50 | `combat_polearm_master` |
| 51 | `combat_rifleman_master` |
| 52 | `combat_smuggler_master` |
| 53 | `combat_unarmed_master` |
| 54 | `crafting_architect_master` |
| 55 | `crafting_armorsmith_master` |
| 56 | `crafting_artisan_master` |
| 57 | `crafting_chef_master` |
| 58 | `crafting_droidengineer_master` |
| 59 | `crafting_merchant_master` |
| 60 | `crafting_tailor_master` |
| 61 | `crafting_weaponsmith_master` |
| 62 | `outdoors_bio_engineer_master` |
| 63 | `outdoors_creaturehandler_master` |
| 64 | `outdoors_ranger_master` |
| 65 | `outdoors_scout_master` |
| 66 | `outdoors_squadleader_master` |
| 67 | `science_combatmedic_master` |
| 68 | `science_doctor_master` |
| 69 | `science_medic_master` |
| 70 | `social_dancer_master` |
| 71 | `social_entertainer_master` |
| 72 | `social_imagedesigner_master` |
| 73 | `social_musician_master` |
| 74 | `social_politician_master` |

#### Space Professions (10)
| Index | Key |
|-------|-----|
| 129 | `crafting_shipwright` |
| 130 | `pilot_rebel_navy_naboo` |
| 131 | `pilot_rebel_navy_corellia` |
| 132 | `pilot_rebel_navy_tatooine` |
| 133 | `pilot_imperial_navy_naboo` |
| 134 | `pilot_imperial_navy_corellia` |
| 135 | `pilot_imperial_navy_tatooine` |
| 136 | `pilot_neutral_naboo` |
| 137 | `pilot_neutral_corellia` |
| 138 | `pilot_neutral_tatooine` |

---

### Events (20)

| Index | Key | Show | Notes |
|-------|-----|------|-------|
| 32 | `event_project_dead_eye_1` | 0 | Project Dead Eye |
| 33 | `acc_good_samaritan` | 0 | Admin accolade |
| 34 | `acc_fascinating_background` | 0 | Admin accolade |
| 35 | `acc_brave_soldier` | 0 | Admin accolade |
| 36 | `acc_interesting_personage` | 0 | Admin accolade |
| 37 | `acc_professional_demeanor` | 0 | Admin accolade |
| 40 | `event_coa2_rebel` | 0 | Cries of Alderaan 2 |
| 41 | `event_coa2_imperial` | 0 | Cries of Alderaan 2 |
| 109 | `event_coa3_rebel` | 0 | Cries of Alderaan 3 |
| 110 | `event_coa3_imperial` | 0 | Cries of Alderaan 3 |
| 111 | `bdg_library_trivia` | 1 | Librarian |
| 121 | `bdg_racing_agrilat_swamp` | 1 | Racing |
| 122 | `bdg_racing_keren_city` | 1 | Racing |
| 123 | `bdg_racing_mos_espa` | 1 | Racing |
| 124 | `bdg_accolade_live_event` | 0 | Live event |
| 125 | `bdg_racing_lok_marathon` | 0 | Lok marathon |
| 126 | `bdg_racing_narmle_memorial` | 0 | Narmle memorial |
| 127 | `bdg_racing_nashal_river` | 0 | Nashal river |
| 128 | `destroy_deathstar` | 0 | Death Star event |
| 139 | `bdg_accolade_home_show` | 0 | Home show |

---

### Content (15)

| Index | Key | Notes |
|-------|-----|-------|
| 38 | `warren_compassion` | Warren |
| 39 | `warren_hero` | Warren |
| 105 | `bdg_thm_park_jabba_badge` | Theme Park: Jabba |
| 106 | `bdg_thm_park_imperial_badge` | Theme Park: Imperial |
| 107 | `bdg_thm_park_rebel_badge` | Theme Park: Rebel |
| 108 | `bdg_thm_park_nym_badge` | Theme Park: Nym |
| 112 | `bdg_corvette_imp_destroy` | Corvette: Imperial Destroy |
| 113 | `bdg_corvette_imp_rescue` | Corvette: Imperial Rescue |
| 114 | `bdg_corvette_imp_assassin` | Corvette: Imperial Assassin |
| 115 | `bdg_corvette_neutral_destroy` | Corvette: Neutral Destroy |
| 116 | `bdg_corvette_neutral_rescue` | Corvette: Neutral Rescue |
| 117 | `bdg_corvette_neutral_assassin` | Corvette: Neutral Assassin |
| 118 | `bdg_corvette_reb_destroy` | Corvette: Rebel Destroy |
| 119 | `bdg_corvette_reb_rescue` | Corvette: Rebel Rescue |
| 120 | `bdg_corvette_reb_assassin` | Corvette: Rebel Assassin |

---

## Menu Organization (Player-Facing Groups)

Core3's 6 internal types → 4 menu groups:

```
Badges
  Milestone Badges          (Accumulation: 7 count_* + 5 bdg_exp_*_badges)
  Exploration
    Milestone Exploration   (bdg_exp_10/20/30/40/45_badges)
    Tatooine (7)            → 3 easy, 4 dangerous
    Naboo (4)
    Corellia (5)
    Yavin IV (3)
    Lok (3)
    Dathomir (7)            → 6 easy, 1 dangerous
    Dantooine (4)
    Endor (4)
    Talus (4)
    Rori (4)
  Profession
    Combat (12)
    Crafting (9)            → 8 ground + Shipwright
    Outdoors (5)
    Science (3)
    Social (5)
    Pilot (9)               → EXCLUDED (no JTL)
  Quest
    Hero of Tatooine (5)    → Interest: poi_*
    Warren (2)              → Content: warren_*
    Theme Parks (4)         → Content: bdg_thm_park_*
    Corellian Corvette (9)  → Content: bdg_corvette_*
  Event
    Cries of Alderaan (5)   → Events: event_coa2/3_*
    Accolades (7)           → EXCLUDED (admin)
    Librarian (1)           → bdg_library_trivia
    Racing (6)              → 3 visible, 3 EXCLUDED
    Death Star (1)          → EXCLUDED
```

---

## Eligibility for Bonuses

| Status | Count | Badges |
|--------|-------|--------|
| **Eligible** | 111 | All except pilot/JTL + admin/event exclusions |
| **Excluded: Pilot/JTL** | 9 | All 9 `pilot_*_master` badges |
| **Excluded: Admin/Event** | 20 | All accolades, live events, most racing, Death Star, Cries of Alderaan |

**Total badge_map.iff**: 140 = 111 eligible + 9 pilot + 20 excluded

---

## Excluded Badge List (Must Not Grant Modifiers)

### Pilot / JTL (9) — No bonuses until JTL available
```
pilot_imperial_navy_corellia
pilot_imperial_navy_naboo
pilot_imperial_navy_tatooine
pilot_neutral_corellia
pilot_neutral_naboo
pilot_neutral_tatooine
pilot_rebel_navy_corellia
pilot_rebel_navy_naboo
pilot_rebel_navy_tatooine
```

### Admin / Special Event (20) — Not part of normal progression
```
acc_brave_soldier
acc_fascinating_background
acc_good_samaritan
acc_interesting_personage
acc_professional_demeanor
bdg_accolade_home_show
bdg_accolade_live_event
bdg_library_trivia
bdg_racing_agrilat_swamp
bdg_racing_keren_city
bdg_racing_lok_marathon
bdg_racing_mos_espa
bdg_racing_narmle_memorial
bdg_racing_nashal_river
destroy_deathstar
event_coa2_imperial
event_coa2_rebel
event_coa3_imperial
event_coa3_rebel
event_project_dead_eye_1
```

---

## Visibility Policy

| Rule | Description |
|------|-------------|
| `show = 1` | May appear in normal badge browsing |
| `show = 0` | Hidden from general browser by default |
| Hidden + enabled in config | Still shown in `/customskills` modifier breakdown (so totals stay explained) |
| Visibility ≠ ownership | `show` flag is metadata only; does not determine if badge is owned |

---

## Badge-to-Modifier Assignments (menu grouping in ARCHITECTURE.md; gameplay assignments below)

### Milestone Badges (12) — +12.5% Crit Multiplier, +1% Crit Chance each
- `count_5` through `count_125` (7)
- `bdg_exp_10/20/30/40/45_badges` (5)

### Exploration Planets (45)

| Planet | Badges | Bonus (per badge) |
|--------|--------|-------------------|
| **Tatooine easy** (3) | `exp_tat_escape_pod`, `exp_tat_lars_homestead`, `exp_tat_bens_hut` | +100% Practice XP, +25% Buff Duration, +25% Gathering |
| **Tatooine dangerous** (4) | `exp_tat_tusken_pool`, `exp_tat_krayt_skeleton`, `exp_tat_sarlacc_pit`, `exp_tat_krayt_graveyard` | +15 SEA Cap |
| **Naboo** (4) | `exp_nab_gungan_sacred_place`, `bdg_exp_nab_theed_falls_bottom`, `bdg_exp_nab_deeja_falls_top`, `bdg_exp_nab_amidalas_sandy_beach` | +25% Movement Speed (100% max) |
| **Corellia** (5) | `exp_cor_agrilat_swamp`, `bdg_exp_cor_rebel_hideout`, `bdg_exp_cor_rogue_corsec_base`, `bdg_exp_cor_tyrena_theater`, `bdg_exp_cor_bela_vistal_fountain` | +100% Experience (500% max) |
| **Yavin IV** (3) | `exp_yav_temple_exar_kun`: +5 Def Cap, +5% Amazing Success, +10% Amazing Results | |
| | `exp_yav_temple_woolamander`: +100% Practice XP, +25% Gathering, +25% Buff Duration | |
| | `exp_yav_temple_blueleaf`: +100% Practice XP, +25% Gathering, +25% Buff Duration | |
| **Lok** (3) | All: +5 Def Cap, +5% Amazing Success, +10% Amazing Results | |
| **Dathomir easy** (6) | `exp_dat_tarpit`, `exp_dat_escape_pod`, `exp_dat_misty_falls_1`, `exp_dat_misty_falls_2`, `bdg_exp_dat_crashed_ship`, `bdg_exp_dat_imp_prison` | +5 Def Cap, +5% Amazing Success, +10% Amazing Results |
| **Dathomir dangerous** (1) | `exp_dat_sarlacc` | +15 SEA Cap |
| **Dantooine** (4) | All: +12.5% Weapon Degradation Reduction | |
| **Endor** (4) | All: +12.5% Weapon Degradation Reduction | |
| **Talus** (4) | All: +12.5% Armor Degradation Reduction | |
| **Rori** (4) | All: +12.5% Armor Degradation Reduction | |

### Profession Mastery (43)

| Family | Badges | Bonus |
|--------|--------|-------|
| **Combat** (12) | All 12 ground combat mastery | +4% Crit Chance, +2% Triple, +2% Quad (BH: +3%/+3%) |
| **Crafting** (9) | 8 ground + Shipwright | +10% Crafting Speed |
| **Outdoors** (5) | All 5 | +115% Gathering Quantity |
| **Science** (3) | All 3 | +100% Gathering, +25% Buff Duration (Doctor: +10% Crafting Speed) |
| **Social** (5) | All 5 | +20% Buff Duration, +100% Practice XP |
| **Pilot** (9) | All 9 | EXCLUDED (no JTL) |

### Quest (20)

| Line | Badges | Bonus |
|------|--------|-------|
| **Hero of Tatooine** (5) | `poi_rabidbeast`, `poi_prisonbreak`, `poi_twoliars`, `poi_factoryliberation`, `poi_heromark` | +10% Triple, +5% Quad, +10 Def Cap |
| **Warren** (2) | `warren_compassion`, `warren_hero` | +20% Double Attack |
| **Theme Parks** (4) | `bdg_thm_park_jabba/imperial/rebel/nym_badge` | +20% Double Attack |
| **Corellian Corvette** (9) | All 9 faction/mission combos | +1 Armor Penetration |

### Event

| Group | Badges | Bonus |
|-------|--------|-------|
| **Cries of Alderaan** (5) | `event_coa2/3_imperial/rebel`, `event_project_dead_eye_1` | (configured per server) |
| **Accolades** (7) | All `acc_*` + `bdg_accolade_*` | EXCLUDED |
| **Librarian** (1) | `bdg_library_trivia` | (configured per server) |
| **Racing** (6) | 3 visible: `bdg_racing_agrilat/keren/mos_espa` | (configured per server) |
| | 3 excluded: `bdg_racing_lok/narmle/nashal` | EXCLUDED |
| **Death Star** (1) | `destroy_deathstar` | EXCLUDED |

---

## Integration Requirements

### For Config Authors (`config.lua`)

1. **Use stable keys only** — `BadgeList::get(key)` resolves index
2. **Never include excluded badges** in `badges[]` arrays
3. **Pilot badges**: Only add if JTL is actually implemented on your server
4. **Basis points**: 100 = 1.00% for percentages; whole units for armor pen/def cap/SEA cap
5. **Cap values**: 0 = uncapped; applied after badge aggregation

### For Developers

- **BadgeDefinition** (metadata): key, resolved index, Core3 type, exploration subtype, source group, show flag
- **BadgeModifierAssignment** (gameplay): badge key, modifier ID, amount, stacking rule
- **Keep separate** — prevents menu grouping from becoming gameplay logic
- **Allows** other servers to replace badge indices without rewriting modifier definitions

### Resolution at Runtime

```cpp
// CustomSkillsModifiers::getModifierTotal()
const BadgeList* badgeList = BadgeList::instance();
const Badge* badge = badgeList->get(badgeKey);  // stable key → index
if (badge && ghost->hasBadge(badge->getIndex()))
    total += bonus;
```

---

## Client Category Mapping (Reference)

| Client Category | Core3 Types Included | Records |
|-----------------|---------------------|---------|
| 0 | Accumulation (12) | 7 shown, 5 hidden |
| 1 | Interest (5) + some Content | 6 shown |
| 2 | Exploration (45) + most Content | 54 shown |
| 3 | Master (43) | 43 shown |
| 4 | Event (14) | 3 shown, 11 hidden |
| 5 | Event (11) | 11 shown |

*Client category is separate from Core3 type enum — used for client-side badge browser only.*
