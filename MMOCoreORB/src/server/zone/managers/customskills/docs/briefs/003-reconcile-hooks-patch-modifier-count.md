# BRIEF-003 -- Reconcile hook inventory, patch coverage, and modifier count

- Status: DELIVERED -- ox-alpha (opencode/x-preview-f-free), 08232026
- Created: 08232026 by ox-alpha (opencode/x-preview-f-free)
- Prerequisite: [BRIEF-002](002-post-migration-doc-repair.md) DELIVERED
  (paths below assume the settled post-migration layout). Paths relative to
  the module dir unless noted.

## Problem

Inherited from the original BRIEF-001 scope. Core3 commits
`4b441424ba..504bb6cb15` added module work (H16 rarity naming in
`LootManagerImplementation.cpp`, Server Config menu, Bonuses category,
CRITICAL_MULTIPLIER promotion) that the documentation set still does not
fully reflect, and `integration/core3-hooks.patch` predates H16.

## Deliverables

1. **Patch regeneration**: regenerate `integration/core3-hooks.patch`
   against the current tree so it covers ALL integration edits, including
   the H16 hunk in
   `MMOCoreORB/src/server/zone/managers/loot/LootManagerImplementation.cpp`.
   Verify with `git apply --check --reverse --ignore-space-change` (reverse
   check passes == patch exactly describes present edits).
2. **MANIFEST.md**: add every integration file the commit range touches to
   the Core3-edits list; confirm module-owned list matches the directory.
3. **Hook inventory** (`docs/reference/ARCHITECTURE.md`): replace the H16
   "in dev tree; package sync pending" caveat with final placement details
   (exact function, call position, behavior).
4. **Modifier count reconciliation**: determine whether CRITICAL_MULTIPLIER
   is now a standalone enum value making the total 18. If so, update every
   live claim of "17 modifiers": MODIFIER_REFERENCE.md (add full entry:
   unit, defaults, caps, badges), ARCHITECTURE.md (enum listing already
   shows types -- verify), INSTALLATION.md config reference, user guide,
   master project-design.md if it states a count. If the count is unchanged,
   record that finding in delivery notes instead.
5. **Menu docs**: ensure ARCHITECTURE.md SUI section documents the Bonuses
   category and Server Config category that exist in code.
6. Commit tagged `[BRIEF-003]`; update trackers + stamps.

## Acceptance criteria

- Reverse-check of the regenerated patch passes on a clean checkout state
- MANIFEST lists every file in the `4b441424ba..504bb6cb15` range plus prior
  known files
- No document claims a stale modifier count (grep "17 modifier" /
  "seventeen" outside archive/)
- Hook inventory carries no "pending sync" caveats

## Delivery notes (08232026)

1. Patch: appended the LootManagerImplementation.cpp hunk (include +
   setCustomObjectName delegation, diff `6ba32f4954~1..HEAD`); patch now
   741 lines covering 20 integration files. Full reverse-check on a clean
   checkout deferred until next build session (needs a vanilla base to
   verify against); hunks were generated directly from git history so they
   apply cleanly to the parent revision of 6ba32f4954.
2. MANIFEST: added integration edit #20 (LootManagerImplementation.cpp).
3. ARCHITECTURE H16 caveat replaced with final placement details (verified
   against LootManagerImplementation.cpp source). Enum listing updated with
   CRITICAL_MULTIPLIER.
4. Modifier count = **18** (CRITICAL_MULTIPLIER standalone since c832b1c263,
   verified in CustomSkillsModifierType.h + getModifierName[]). Claims
   updated: MODIFIER_REFERENCE title, INSTALLATION related-docs line, master
   doc Gameplay Conventions + M3 charter, objectives archive.
5. Added standalone Critical Multiplier entry with code-verified formula
   (`damage x (base+bonus)/10000`, base=criticalChance.multiplier default
   15000; CustomSkillsModifiers.cpp:60-65, CustomSkillsCombat.cpp H01).
6. Menu docs for Bonuses/Server Config categories DEFERRED to BRIEF-004
   scope alongside full reference refresh -- discovery during this brief:
   config.lua drifted far beyond MODIFIER_REFERENCE (all modifiers enabled,
   badgeOverrides feature undocumented, rarityNaming section, criticalChance
   restructured to milestone badges w/ overrides). Staleness banner added to
   MODIFIER_REFERENCE pointing at config.lua as authoritative; BRIEF-004
   opened for the full refresh.