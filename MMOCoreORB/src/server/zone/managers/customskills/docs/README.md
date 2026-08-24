# Custom Skills -- Documentation Directory

> Subordinate to [rules/project-design.md](rules/project-design.md) -- the
> master document for project rules.

## Master Document

The authoritative source for all project rules and design decisions is
[rules/project-design.md](rules/project-design.md). Every other document in
this directory is subordinate to it. When a subdocument contradicts the
master, the master prevails.

## Directory Organization

| Folder | Contains | Lifecycle |
|--------|----------|-----------|
| `rules/` | Governing documents: design master, process rules, errata protocol | Permanent; versioned alongside project |
| `reference/` | Technical reference: architecture, modifier specs, hooks, menu, badges, Core3 code reference + user guide | Permanent; updated when code changes ([PROC R6.6]) |
| `tracking/` | Current objectives and task state | Living tracker; reconciliation-stamped per [PROC R6] |
| `briefs/` | Task brief definitions (claim/deliver) | Permanent once delivered; archived periodically |
| `installation/` | Server-administrator guide: install, config, verify, remove | Permanent; updated with every integration change |
| `user-guide/` | Player-facing guide: menu mechanics, badge bonuses | Permanent; updated when behavior changes |
| `feature-planning/` | Active feature design (Challenge Tier) | Living design docs; graduate into reference/rules or archive |
| `archive/` | Superseded documents (historical reference only) | Frozen; never cited as current authority |

## Entry Point for Contributors

Start with [`../../START-HERE.md`](../../START-HERE.md), then read
[rules/project-design.md](rules/project-design.md).

---

## Document Creation Rules

### Where to put new documents

| Document type | Location | Naming convention |
|--------------|----------|-------------------|
| Design principle or architectural decision | `rules/project-design.md` | Edited inline within existing sections |
| Process rule | `rules/process.md` | Next free R# number per [PROC R3] |
| Errata finding or resolution | `rules/errata.md` | Appended at bottom, next ERR-NNN |
| Objective or task item | `tracking/objectives.md` | Under the relevant milestone heading |
| Brief definition | `briefs/<NNN>-<name>.md` | Next sequential NNN number |
| Technical deep-dive / spec | `reference/<TOPIC>.md` | ALL_CAPS_SNAKE matching existing names |
| Feature design (pre-implementation) | `feature-planning/<NAME>.md` | Descriptive name |
| Temporary note, scratch, draft | `temp/` (create as needed) | Descriptive name + lifespan marker |

### Required reading before creating documents

1. [`../../START-HERE.md`](../../START-HERE.md)
2. [rules/project-design.md](rules/project-design.md) -- Core Design
   Principles [CS-1..10]
3. The section of the relevant existing document your change touches
4. This file's Document Creation Rules (you are here)

### Signing

Every substantive contribution is signed. Format, date standard, and time
zone are defined in
[rules/process.md](rules/process.md) -> Contributor Recording Conventions;
per [PROC R6.7] this document points there rather than restating them.

---

## Temporary Documents

Temporary documents are working notes, drafts, analysis output, or one-off
research not intended to become permanent documentation.

### Rules for temporary documents

1. **Location:** all temporary documents go under `docs/temp/`. Never place
   them in `rules/`, `reference/`, `tracking/`, or any permanent location.
2. **Marking:** the filename or first line must indicate intended lifespan
   (e.g. "draft", "scratch", "analysis").
3. **Destruction date:** include one if known; otherwise the owner may purge
   during periodic cleanup.
4. **No references:** permanent documents must not cite temporary documents.
   If information from a temp document must be preserved, copy it into the
   appropriate permanent document first.

---

## Document Destruction Policy

| Type | When destroyed | Who decides |
|------|---------------|-------------|
| `temp/` contents | Periodic cleanup | Daniel |
| Superseded feature-planning docs | Moved to `archive/` at feature completion | Daniel |
| Errata entries | Never deleted (permanent audit trail) | Nobody |
| Archive contents | Never deleted | Nobody |

---

## Related Files (Outside `docs/`)

| File | Purpose |
|------|---------|
| `../../START-HERE.md` | Contributor entry point, required reading order, code tour |
| `../README.md` | Module summary and directory roles |
| `../MANIFEST.md` | Every distributed file + integration points (living tracker) |
| `installation/INSTALLATION.md` | Canonical install/configure/verify/remove guide (the archived `integration/INSTALL.md` was its duplicate) |
| `../integration/core3-hooks.patch` | Core3 integration patch (file list in MANIFEST) |
| `../package/MMOCoreORB/bin/scripts/customskills/config.lua` | Server configuration |

---

## Contributing to Documentation

- Keep audience separation clear: installation = admins, user-guide =
  players, reference = developers, rules = governing authority.
- Update the relevant guide when changing behavior ([PROC R6.6]).
- Prefer consolidating into the existing five reference documents over
  creating new ones; archive rather than delete (preserves history) and
  update links that pointed to archived docs.
- **Update `reference/CORE3_CODE_REFERENCE.md`** when discovering new Core3
  patterns useful for future AI researchers, and
  `reference/CORE3_USER_GUIDE.md` when build/test operations change.
  Factual errors in either are filed via [rules/errata.md](rules/errata.md);
  the historical log for the retired SWGEMU_CODE_GUIDE lives in
  [archive/CODE_GUIDE_ERRATA.md](archive/CODE_GUIDE_ERRATA.md).
