# BRIEF-003 -- Reconcile hook inventory, patch coverage, and modifier count

- Status: UNCLAIMED
- Created: 2026-08-23 by ox-alpha (opencode/x-preview-f-free)
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
