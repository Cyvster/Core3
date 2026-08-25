# Brief 034 -- Consolidated strike: multi-hit as damage multiplier + FCT escalation

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Depends on: BRIEF-032 findings (ShowFlyText scale/RGB server-controlled;
  hook point CustomSkillsCombat::applyDamage, CombatManager.cpp:1420)
- Replaces the ERR-009 repeat-chain behavior in combat/CustomSkillsCombat.cpp

## Owner's design (verbatim intent)

Double/Triple/Quad should NOT perform extra hits. Extra applications are
invisible work -- overlapping client-side, unseeable. Instead each landed
tier is a DAMAGE MULTIPLIER on the single hit: same total damage as the
multi-hit sequence would deal, delivered in one strike.

## Behavior spec

1. Roll chain unchanged (Double -> Triple -> Quad sequential, capped bp
   totals, first failure ends chain) -- but instead of N x applyVanillaDamage,
   multiply `damage` by `repeats` BEFORE a single applyVanillaDamage call.
   Math equivalence: 3 hits of X == 1 hit of 3X (armor applies per hit today,
   so single-big-hit vs multi-small differs vs armor; ACCEPTED by owner --
   note it in doc).
2. Crit rolls first (existing), then repeats multiply on top.
3. FlyText escalation on the defender via ShowFlyText
   (packets/object/ShowFlyText.h): scale and color both increase with tier:
   - base hit: existing combat_effects hit_<location>, scale 1.0, current colors
   - Double: scale 1.15, tier-2 color
   - Triple: scale 1.3, tier-3 color
   - Quad: scale 1.5, tier-4 color
   - Critical adds +0.25 scale and shifts color toward the crit hue
   - Color scheme: escalating value along one axis (e.g. white -> yellow ->
     orange -> red for tiers; crit overlays gold). Exact palette = config.
   - Scale values are CONFIGURABLE multipliers (owner requirement: relative,
     respects client slider).
4. CHAT TAG (owner addendum): prepend tier tag to the combat spam number,
   e.g. `x4 4568` / `x2 1204` (crit keeps existing crit marker). Combat spam
   IS modifiable server-side (unlike flytext, which cannot render raw
   digits -- stf entries only per BRIEF-032). VERIFIED injection path:
   CombatSpam packet (packets/object/CombatSpam.h) carries %DI damage int +
   stf file/name + a COLOR byte (0=white, 1=auto green/red, 10=red,
   11=yellow) + an optional unicode custom-string ctor. Prefer: send the
   standard vanilla spam, then a second custom-unicode CombatSpam line
   carrying only the tier tag (`x4`), escalating the color byte with tier
   (white -> yellow 11 -> red 10). Full-replacement unicode spam is the
   fallback if double lines feel noisy in testing.
   LIMITATION (honest): chat lines render in one font size client-side --
   a smaller `x` than the digits is NOT possible in the chat box. That
   two-size effect IS achievable in the FLYTEXT instead: send the tier as
   a separate smaller-scale ShowFlyText line above the main hit flytext
   (two packets: small `x4` stf entry at scale ~0.8, big number-ish stf at
   escalated scale), since stf tables can hold an `x2/x3/x4` entry we ship.
5. Config knobs (CustomSkillsConfig, self-documenting per house standard):
   fctEnabled bool default true; fctScaleStepBp int (1500 = +15% per tier);
   fctCritBonusBp int (2500); tier colors as hex strings with defaults.
5. Implementation notes: send ShowFlyText from CustomSkillsCombat::applyDamage
   after computing final repeats+crit; broadcast to attacker + defenders-of-
   record same as vanilla hit flytext; keep flags byte 5 as vanilla uses.
   Do NOT touch CombatManager.cpp beyond what already exists.

## Deliverables

- [ ] Repeat chain converted to damage multiplier (single application).
- [ ] Chat spam tier tag (`x4 4568`) injected into damage spam line.
- [ ] Two-size flytext pair (small tier glyph + big hit line) if stf shipping accepted.
- [ ] Tiered ShowFlyText with config-driven scale/colors sent per qualifying hit.
- [ ] CustomSkillsConfig knobs added w/ self-documenting defaults.
- [ ] ERRATA entry: supersede ERR-009 behavior note (multiplier replaces repeats).
- [ ] CODE_REFERENCE.md updated (behavior + config keys); USER_GUIDE blurb.
- [ ] Brace-balance check + commit `[BRIEF-034]`, push.
