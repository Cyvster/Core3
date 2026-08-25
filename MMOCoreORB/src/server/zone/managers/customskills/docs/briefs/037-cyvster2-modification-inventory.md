# Brief 037 -- Cyvster2 production-branch archaeology: full modification inventory

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Purpose: the owner's previous production server ran for ~2 years on branch
  `cyvster2` with many direct edits to SWGEmu code. This mod (cyvster3) is an
  attempt to move those features/QoL changes into a proper mod instead of
  direct edits. This brief builds the master inventory of what cyvster2
  changed, so we can track migration progress into the mod.

## Source material

- Branch: `remotes/origin/cyvster2` (also `cyvster2backup`, `cyvster2test` --
  note their existence; primary analysis on `cyvster2`)
- Fork base from upstream: merge-base with `origin/unstable` =
  `7ff869ccd26701fdfb078f2bec7e27557e1bbe76`
- Owner's commits are identifiable: they post-date the git-svn import tail and
  carry informal messages ("attempt to...", "undo previous...", "Add 100x...").
  Expect MANY undo/redo pairs -- the inventory must record NET changes, not
  every attempt (owner self-describes the history as mistake-heavy; be
  non-judgmental in docs and focus on net effect).

## Method

1. Commit triage: classify all post-fork commits as upstream-sync vs
   owner-edit vs revert-pairs. Build a NET-diff per functional area.
2. Diff-based sweep: `git diff <fork-base>..origin/cyvster2 --stat` grouped
   by directory (managers/missions, objects/, lua templates, bin/conf, etc.)
   to catch changes commit messages don't describe.
3. Categorize every net modification:
   - FEATURE (new behavior, e.g. "mission direction/difficulty options")
   - TWEAK (value changes: multipliers, distances, rewards)
   - FIX (bug fixes, some may already exist upstream now)
   - DEAD (undo-chains that net to zero; skip but count them)
4. For each item record: short name, category, area/file(s), one-line
   description, rough complexity to port to mod architecture
   (TRIVIAL=config-like / MODERATE=needs hook / HARD=deep engine change),
   and whether the current mod ALREADY has it (e.g. movement speed,
   crafting speed, gathering quantity ARE already mod features).
5. Produce a migration-tracking table with status per item:
   IN-MOD / TODO / WONT-PORT (upstream caught up or obsolete).

## Deliverables

- [ ] `docs/cyvster2/MIGRATION_LEDGER.md` -- the permanent tracking list:
      categorized inventory + migration status column (dense house format;
      this doc will be updated over time as items port).
- [ ] Summary stats: total net modifications, counts by category/status.
- [ ] Flag the highest-value ports (owner called out mission terminal
      direction/difficulty options specifically -- ensure that area is
      thoroughly documented; it seeds BRIEF-038).
- [ ] R6.9 capture: notable durable facts -> appropriate reference docs.
- [ ] Commit `[BRIEF-037]`, push.

## Constraints

- Read-only on both branches; no code changes anywhere.
- The ledger lives under docs/cyvster2/ (new section) so mod docs stay clean.
