# BRIEF-001 -- Migrate customskills-mod into the Core3 repository

> Rewritten 08232026 by owner directive (Cyvster): the standalone
> `customskills-mod` folder is dissolved; the Core3 dev tree becomes the
> single home of the module. This replaces the former "sync package with
> Core3" scope -- the dual-tree problem this brief once tracked disappears
> with the migration.

- Status: DELIVERED -- ox-alpha (opencode/x-preview-f-free), 08232026; commit a682058144
- Created: 08232026 by ox-alpha (opencode/x-preview-f-free)
- Rewritten: 08232026 by ox-alpha (opencode/x-preview-f-free), owner
  directive

## Goal

All Custom Skills artifacts live inside the Core3 repository. Module-owned
code and docs are contained under
`MMOCoreORB/src/server/zone/managers/customskills/` except artifacts whose
runtime location requires otherwise (already true: see Containment audit).
After this brief, `G:\Data\swgemu customskills\customskills-mod\` is deleted.

## Pre-migration state (verified 08232026)

- Dev tree module code at `MMOCoreORB/src/server/zone/managers/customskills/`
  is AHEAD of the package mirror (contains H16 rarity naming, Server Config
  menu, Bonuses category, CRITICAL_MULTIPLIER work through commit
  `504bb6cb15`). **Dev tree wins; stale package copies are discarded
  deliberately.**
- External mod artifacts (all REQUIRED by Core3 runtime/design, none stray):
  - `MMOCoreORB/bin/scripts/customskills/config.lua`
  - `MMOCoreORB/bin/scripts/commands/customSkills.lua`
  - delegation hooks inside existing Core3 sources (documented via patch +
    MANIFEST)
- Working tree was clean on branch `cyvster3` as of audit time. Re-verify
  with `git status` before starting; stop and report if not clean.

## Deliverables

Execute from `G:\Data\swgemu customskills\` (paths below relative to it):

1. **Move docs** (whole tree, contents unchanged):
   `customskills-mod/docs/` ->
   `Core3/MMOCoreORB/src/server/zone/managers/customskills/docs/`
2. **Move module-root records** into the module directory:
   - `customskills-mod/MANIFEST.md` -> `Core3/.../managers/customskills/MANIFEST.md`
   - `customskills-mod/README.md` -> `Core3/.../managers/customskills/README.md`
   - `customskills-mod/integration/core3-hooks.patch` ->
     `Core3/.../managers/customskills/integration/core3-hooks.patch`
3. **Verify package redundancy, then discard**: for every file under
   `customskills-mod/package/MMOCoreORB/src/server/zone/managers/customskills/`,
   diff against the dev-tree counterpart. Expected: dev tree differs ONLY by
   the newer commits noted above (never the reverse). Record a one-line
   summary of the diff direction per changed file in your delivery notes.
   Then delete `customskills-mod/package/`. The two Lua script files under
   `package/MMOCoreORB/bin/scripts/` must byte-match their dev-tree twins;
   if they do not, STOP and report (that would mean unshipped local edits).
4. **Delete** the emptied `customskills-mod/` folder (nothing remains after
   steps 1-3 except archived docs, which move WITH docs in step 1).
5. **Commit** everything in Core3 as ONE commit tagged `[BRIEF-001]`, message
   noting: docs relocated into module directory; standalone mirror dissolved;
   dev tree confirmed authoritative.
6. Do NOT touch `readme.md` at the workspace root -- its repair is
   [BRIEF-002](002-post-migration-doc-repair.md) (it references the now-gone
   folder and would be broken until then; acceptable transient state within
   one session).
## Acceptance criteria

- `customskills-mod\` does not exist
- `Core3/MMOCoreORB/src/server/zone/managers/customskills/docs/` contains the
  full docs tree (rules/, reference/, tracking/, briefs/, installation/,
  user-guide/, feature-planning/, archive/)
- `MANIFEST.md`, `README.md`, `integration/core3-hooks.patch` sit directly
  under `Core3/.../managers/customskills/`
- No module-owned C++/Lua file moved or modified (build unaffected)
- Single `[BRIEF-001]` commit contains exactly the moves/deletions above
- Diff-direction evidence recorded in delivery notes (step 3)

## Out of scope

- Fixing links/governance text broken BY the move -> BRIEF-002
- Patch regeneration / modifier-count reconciliation -> BRIEF-003

## Delivery notes (08232026)

- Single commit `a682058144` on cyvster3: 37 files (docs tree, MANIFEST,
  README, patch) relocated into the module directory.
- Package redundancy evidence: every differing package file matched an
  ANCESTOR commit of HEAD exactly (Config.cpp=3327f4d3c6, Config.h=778c6d5fa7,
  Menu.cpp=882cdfa338, Menu.h=46efb45dbd, Modifiers.cpp=eb9980c79a,
  Modifiers.h=f36b211ac8, ModifierType.h=304231bf57,
  Combat.cpp=778c6d5fa7, config.lua=e92796c3ee) -- dev tree strictly newer
  in all cases; no reverse divergence; discarded per plan.
- customSkills.lua byte-identical package/dev; customskills-mod folder
  deleted; working tree clean after commit.