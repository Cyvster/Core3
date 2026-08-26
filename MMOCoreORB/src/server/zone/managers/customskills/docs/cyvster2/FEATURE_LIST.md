# Cyvster2 Feature List -- Plain-Language Port Roadmap

> Temporary companion to MIGRATION_LEDGER.md (same data, organized for
> decision-making rather than archaeology). Delete or archive once the TODO
> items are dispositioned.

## Already in the mod

Forage quantity x100 (exact match) | Crafting speed / instant crafting |
Practice XP + global XP multipliers | Mission direction choice |
Mission difficulty choice (ten tiers) | Descriptive mission titles

## Worth porting (ranked by value-per-effort)

1. Bounty rewards x100 -- hard-coded today; make it a config knob. TRIVIAL.
2. Mission bag 12 -> 24 slots -- more missions per terminal. TRIVIAL.
3. Mission list size 3 -> 6 -- knob already exists (`missionListSize`);
   just set it in config. DONE by config edit alone.
4. Credits go straight to top-damage player instead of corpse loot --
   nice solo/group QoL. MODERATE.
5. AA/CA attachments auto-named ("[AA] strength: 25") from highest skill
   mod. Small, genuinely nice UX. TRIVIAL-MODERATE.
6. Survey tool range extended (384 -> 2624 with finer granularity).
   Clean standalone gathering QoL. TRIVIAL-MODERATE.
7. Harvester / creature harvest x10 -- natural third hook alongside our
   forage + milk bonuses. TRIVIAL.
8. Skill trainers teach everything incl. elite/master trees -- big player
   QoL; trainerData.lua extension + SUI callback already designed on
   cyvster2. MODERATE.
9. Board shuttle range 25m -> 150m -- one constant. TRIVIAL.
10. Slicing upgrades: XP boost + guaranteed dual slices (weapon gets both
    damage and speed; armor both effectiveness and encumbrance). Could be
    a proper mod option rather than cyvster2's pinned-switch hack. MODERATE.
11. Pet run speed x5 -- extend our movement modifier service to pets and
    mounts (players are covered; pets/mounts are not). TRIVIAL-MODERATE.
12. Entertainer buffs: full 4-stat buffs, duration cap 2h -> 4h -- could be
    a buff-service feature. MODERATE.
13. Account-shared structure lots -- all characters on one account share
    the lot pool, admin permission on placement for all account chars.
    BIGGEST ITEM but needs careful design: cyvster2's version caused
    stutters (full recursive structure fetch per lots query). Do NOT port
    implementation; design properly cached. HARD.
14. Escape pod hatch for ships -- radial eject, owner self-destruct. Only
    if space content matters. TRIVIAL (self-contained lua).
15. FRS XP drip -- Jedi Knights+ passively gain FRS XP from combat.
    MODERATE.

## Server tuning (config values, not code)

Loot credits x5 | XP caps raised to 10M | Political XP boosted |
Veteran rewards day 1 | Heal potency tweaks | City system loosened
(citizens, intervals, discounts, planet restrictions) | Shuttle retimes |
Corellia weather off | Krayt / DWB / Geonosian loot stat buffs |
Lightsaber damage + armor piercing | Combat command retunes |
Recruiter conversation swaps

## Deliberately not porting

DNA system gutted to constant 1000s | God-loot (all mods maxed 25,
loot-chance gate bypassed) | Legendary suffix removal | CLOSEOBJECTRANGE
192->512 (zone-load amplifier, likely general-sluggishness contributor) |
Account-lots fan-out implementation (stutter source; concept may return
properly designed) | FRS manager stubbing | Character-creation throttle
removal | Force-run speed tweaks

## Dead ends recorded (do not revisit)

Run-speed doubling saga | Mount-speed triple attempts | Resource-density
experiment chain | Council robe duplicate commits | Armor-piercing
per-file commit spam
