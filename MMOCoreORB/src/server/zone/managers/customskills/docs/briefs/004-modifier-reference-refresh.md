# BRIEF-004 -- Refresh MODIFIER_REFERENCE against current config.lua

- Status: DELIVERED -- hy3-free (opencode/hy3-free), 08232026
- Created: 08232026 by ox-alpha (opencode/x-preview-f-free) during
  [BRIEF-003](003-reconcile-hooks-patch-modifier-count.md) delivery
- Prerequisite: none (can run in parallel with nothing else touching
  MODIFIER_REFERENCE)
- Delivered: 08232026 by hy3-free (opencode/hy3-free) -- refreshed
  MODIFIER_REFERENCE.md against config.lua (all 18 modifiers, caps/badges,
  badgeOverrides + rarityNaming conventions, regenerated Maximum Theoretical
  Totals, removed staleness banner); documented rarityNaming + badgeOverrides
  in INSTALLATION.md; corrected ARCHITECTURE.md SUI menu (Badges/Bonuses/Server
  Config, Combat/Utility/Crafting, Rarity Naming page) and the Critical Chance
  detail example. The criticalChance code-vs-config divergence was resolved in
  ERR-005 (combat path now reads the config badge map) -- no dead config.

## Problem

`bin/scripts/customskills/config.lua` evolved well past what
[../reference/MODIFIER_REFERENCE.md](../reference/MODIFIER_REFERENCE.md)
documents. Discovered during BRIEF-003:

- ALL generic modifiers are now `enabled = true` with full badge assignments
  (docs still say "all disabled except Critical Chance")
- `criticalChance` restructured: milestone exploration badges at 100 bp each
  via `badgeOverrides`, cap 6000, badgeBonus 400 -- NOT the 12 combat mastery
  badges / 300 bp / cap 10000 the reference documents. NOTE: code
  (`CustomSkillsModifiers.cpp` combatProfessionBadges static array +
  static_assert==12) must be reconciled with this config intent during this
  brief; determine which is authoritative and flag any dead config.
- New feature: `badgeOverrides` ({ "badgeKey", value } pairs) -- undocumented
  anywhere in docs/
- New server config section: `rarityNaming` (enabled, legendaryColor,
  exceptionalColor) -- only partially documented via H16 inventory entry
- Per-modifier caps/badges differ from reference tables across the board

## Deliverables

1. Rewrite every per-modifier entry in MODIFIER_REFERENCE.md from the actual
   `config.lua` values (units, enabled state, badgeBonus, overrides, cap,
   badge list summary). Keep behavior paragraphs; verify each against code
   before restating.
2. Document `badgeOverrides` semantics (loader location in
   CustomSkillsConfig.cpp; interaction with badgeBonus) in Configuration
   Conventions + INSTALLATION.md config reference.
3. Document the `rarityNaming` server-config section in INSTALLATION.md.
4. Reconcile criticalChance documentation with the code path actually used;
   if config fields are dead (ignored by loader), mark them explicitly and
   file an ERR entry rather than silently choosing a side.
5. Update the "Maximum Theoretical Totals" table to match new assignments or
   remove it with a note if no longer meaningful.
6. Remove the staleness banner at the top of MODIFIER_REFERENCE.md once true.
7. Document the in-code menu categories (Bonuses category on main menu,
    Server Config menu with rarity detail page) in ARCHITECTURE.md's SUI
    Menu System section -- deferred here from BRIEF-003 delivery notes.
8. Pass USER_GUIDE.md mechanics text for the same staleness (Critical
    Multiplier description and any per-modifier examples).

## Acceptance criteria

- Every MODIFIER_REFERENCE property table matches config.lua exactly
- badgeOverrides + rarityNaming documented in both MODIFIER_REFERENCE (or
  ARCHITECTURE) and INSTALLATION
- No unresolved code-vs-config contradictions without an ERR entry
- Staleness banner removed; stamp updated
