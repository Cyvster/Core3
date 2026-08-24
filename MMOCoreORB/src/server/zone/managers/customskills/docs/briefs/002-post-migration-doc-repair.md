# BRIEF-002 -- Post-migration documentation & governance repair

- Status: DELIVERED -- ox-alpha (opencode/x-preview-f-free), 2026-08-23; commit 49a22bd54a
- Created: 2026-08-23 by ox-alpha (opencode/x-preview-f-free)
- Prerequisite: [BRIEF-001](001-migrate-mod-into-core3.md) DELIVERED. All
  paths in this brief are post-migration, relative to
  `Core3/MMOCoreORB/src/server/zone/managers/customskills/` (= "module dir").

## Problem

The migration moved the docs tree into the Core3 repo. Internal links among
docs survived (whole tree moved together), but every reference that pointed
OUTSIDE the old `customskills-mod/` folder is now wrong or stale, and the
governance rules still describe a dual-tree world that no longer exists.

## Deliverables

1. **Governance rewrite** (`docs/rules/process.md`, `docs/rules/project-design.md`):
   - R6.5: single tree now -- all changes commit directly in Core3 under
     traceability tags; delete the package-mirror workflow text.
   - R6.6 table: remove "mirror into `package/MMOCoreORB/...`" rows; keep and
     re-target doc-update obligations (ARCHITECTURE hook inventory,
     MODIFIER_REFERENCE, MANIFEST, INSTALLATION).
   - Repository & Release Conventions section: replace "package must remain
     installable without git history" with the new distribution story: the
     module IS part of the repo; distribution = copy module dir + the two
     script files + apply the patch.
   - project-design.md Integration Policy: same treatment; note containment
     rule (mod files stay in module dir unless runtime requires otherwise;
     the two script locations are the sanctioned exceptions).
2. **Workspace-root START-HERE.md**: rewrite for the new layout (no more
   customskills-mod; paths into `Core3/MMOCoreORB/src/server/zone/managers/customskills/...`;
   keep tech-stack, reading order, sign-in). Verify every link resolves.
3. **INSTALLATION.md** (`docs/installation/`): rewrite install/remove for
   single-tree reality (no package copy step; patch path is now
   `../integration/core3-hooks.patch` relative to module dir; scripts already
   in place in this repo). Keep config reference intact.
4. **MANIFEST.md** (module dir root): update header (drop mirror language),
   fix self-referential paths to new layout.
5. **CORE3_USER_GUIDE.md**: rewrite "Deploying Module Changes" for single-tree
   workflow (commit + build + deploy binary/scripts from THIS tree).
6. **ARCHITECTURE.md**: update Physical Layout package-mirror note.
7. **errata.md quick start / process references**: wording like "the Core3
   dev tree (`../Core3`)" -> "this repository"; verify no other stale
   cross-tree references (grep for `customskills-mod`, `package/MMOCoreORB`,
   `../Core3`) outside archive/.
8. **Trackers**: objectives.md -- mark the package-sync objective OBSOLETE
   (superseded by migration) rather than deleted; add archive entry. Update
   reconciliation stamps on every touched doc ([PROC R6]).

## Acceptance criteria

- Repo-wide grep (excluding `docs/archive/` and googletest) finds ZERO live
  references to `customskills-mod`
- Every markdown link resolves from its own file's directory (run a
  link-check sweep; include workspace-root START-HERE.md)
- process.md contains no two-tree/mirror instructions
- Stamps updated on all modified documents

## Delivery notes (2026-08-23)

All eight deliverables completed in commit `49a22bd54a`: governance rewrite
(R6.5/R6.6/containment/release conventions), master-doc Integration Policy,
workspace START-HERE rewritten, INSTALLATION single-tree install/remove,
MANIFEST header+paths, CORE3_USER_GUIDE deploy section, ARCHITECTURE layout
note + patch-management wording, errata verification wording. Trackers:
package-sync objective marked OBSOLETE; git-history objective RESOLVED;
stamps updated on all touched docs.