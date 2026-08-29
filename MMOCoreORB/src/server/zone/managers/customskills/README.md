# Custom Skills Module

> Subordinate to [docs/rules/project-design.md](docs/rules/project-design.md)
> -- the master document for project rules. New contributors start with
> the workspace-root ``readme.md`` (project-customskills readme.md).

This directory IS the Custom Skills module, inside the Core3 repository:
module code, its documentation (`docs/`), and the integration patch
(`integration/core3-hooks.patch`) used to install it onto other Core3
checkouts. Runtime-required files outside this directory are limited to two
script files under `MMOCoreORB/bin/scripts/` (see [PROC R6.5] containment
rule in `docs/rules/process.md`).

## Current milestone

Challenge Tier system design (M5/M6): three fourbyfour skill trees driven by
`challenge_tier` XP. Design state lives in
`docs/feature-planning/CHALLENGE_TIER_SKILLS.md`; objective tracking lives
in `docs/tracking/objectives.md`.

## Directory roles

- `*.h/.cpp` + service subdirs (`combat/`, `buffs/`, ...) -- module code.
- `docs/rules/` contains governing documents (design master, process, errata).
- `docs/swgemu/` contains emulator-facing guides: player user guide +
  Core3 developer code reference (kept separate from module content).
- `docs/customskills/` contains module-facing guides: player user guide,
  module code reference (with modifier/badge specification appendices),
  LLM contributor guide.
- `docs/tracking/`, `docs/briefs/` contain task state and delegable work.
- `docs/installation/` serves server administrators.
- `docs/archive/` preserves superseded documents.
- `integration/` holds the patch of minimal edits made in existing Core3 files.
- `MANIFEST.md` lists every module file and integration point.

## Document map

Authoritative index: [docs/README.md](docs/README.md). Superseded historical
documents (badge catalog, modifier registry, implementation plan, etc.) are
preserved under `docs/archive/` -- they are not current authority.

---

**Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
owner-directed guide restructure: docs/ split into swgemu/ + customskills/
audience subfolders. Prior: 08232026 -- BRIEF-002: rewritten for
single-tree layout after migration into Core3 repo.
