# Excluded Badge List

Badges listed here must not grant any custom modifier. The menu still displays
them, but they contribute zero bonus value.

## Exclusion rules

1. **Pilot / Jump to Lightspeed badges** -- JTL gameplay is not yet
   implemented. These badges cannot be reliably earned and must not grant
   bonuses until JTL is available on the server.

2. **Admin and special-event badges** -- Badges awarded through admin-run
   events, live events, or one-time server promotions are excluded because
   they are not part of normal gameplay progression.

## Excluded pilot / JTL badges

| Badge key | Notes |
| --- | --- |
| `pilot_imperial_navy_corellia` | Imperial Navy pilot -- Corellia |
| `pilot_imperial_navy_naboo` | Imperial Navy pilot -- Naboo |
| `pilot_imperial_navy_tatooine` | Imperial Navy pilot -- Tatooine |
| `pilot_neutral_corellia` | Neutral pilot -- Corellia |
| `pilot_neutral_naboo` | Neutral pilot -- Naboo |
| `pilot_neutral_tatooine` | Neutral pilot -- Tatooine |
| `pilot_rebel_navy_corellia` | Rebel Navy pilot -- Corellia |
| `pilot_rebel_navy_naboo` | Rebel Navy pilot -- Naboo |
| `pilot_rebel_navy_tatooine` | Rebel Navy pilot -- Tatooine |

## Excluded admin / special-event badges

These badges have `show = 0` in the badge map or are awarded through
admin-run or one-time events rather than normal gameplay.

| Badge key | Notes |
| --- | --- |
| `acc_brave_soldier` | Admin event accolade |
| `acc_fascinating_background` | Admin event accolade |
| `acc_good_samaritan` | Admin event accolade |
| `acc_interesting_personage` | Admin event accolade |
| `acc_professional_demeanor` | Admin event accolade |
| `bdg_accolade_home_show` | Home show event |
| `bdg_accolade_live_event` | Live event |
| `bdg_library_trivia` | Librarian badge |
| `bdg_racing_agrilat_swamp` | Racing event |
| `bdg_racing_keren_city` | Racing event |
| `bdg_racing_lok_marathon` | Lok marathon event |
| `bdg_racing_mos_espa` | Racing event |
| `bdg_racing_narmle_memorial` | Narmle memorial event |
| `bdg_racing_nashal_river` | Nashal river event |
| `destroy_deathstar` | Death Star event |
| `event_coa2_imperial` | Cries of Alderaan 2 -- Imperial |
| `event_coa2_rebel` | Cries of Alderaan 2 -- Rebel |
| `event_coa3_imperial` | Cries of Alderaan 3 -- Imperial |
| `event_coa3_rebel` | Cries of Alderaan 3 -- Rebel |
| `event_project_dead_eye_1` | Project Dead Eye |

## How to apply

When populating badge lists in `config.lua`, never include any badge key from
this list. The Lua config loader does not filter automatically; the
configuration author must omit excluded keys.

## Updating this list

Add or remove entries here when:
- JTL becomes available and pilot badges should grant bonuses.
- A new admin event badge is introduced.
- An existing event badge becomes part of normal gameplay progression.
