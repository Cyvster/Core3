# Custom Skills Module

> Subordinate to [docs/rules/project-design.md](docs/rules/project-design.md)
> -- the master document for project rules. New contributors start with
> [../START-HERE.md](../START-HERE.md) (workspace root).

This directory is the standalone project and portable distribution copy of
the SWGEmu Core3 Custom Skills module.

The development installation is maintained in `../Core3` (branch `cyvster3`).
The distributable copy is maintained under `package/`, using the same
relative paths the files have inside a Core3 checkout. Both trees must stay
in sync ([PROC R6.6] in `docs/rules/process.md`).

## Current milestone

Challenge Tier system design (M5/M6): three fourbyfour skill trees driven by
`challenge_tier` XP. Design state lives in
`docs/feature-planning/CHALLENGE_TIER_SKILLS.md`; objective tracking lives
in `docs/tracking/objectives.md`.

## Directory roles

- `package/` contains copies of files owned by the module.
- `docs/rules/` contains governing documents (design master, process, errata).
- `docs/reference/` contains architecture and developer reference material.
- `docs/tracking/`, `docs/briefs/` contain task state and delegable work.
- `docs/installation/`, `docs/user-guide/` serve admins and players.
- `docs/archive/` preserves superseded documents.
- `integration/` documents the minimal edits required in existing Core3 files.
- `MANIFEST.md` lists every distributed file and integration point.

The package must remain installable without copying this repository's Git
history or unrelated Core3 files.

## Document map

Authoritative index: [docs/README.md](docs/README.md). Superseded historical
documents (badge catalog, modifier registry, implementation plan, etc.) are
preserved under `docs/archive/` -- they are not current authority.

---

**Last reconciled:** 2026-08-23 by ox-alpha (opencode/x-preview-f-free).
