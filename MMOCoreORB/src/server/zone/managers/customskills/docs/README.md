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
| `swgemu/` | Emulator-facing guides, kept separate from module content: player user guide + merged Core3 developer code reference (Part I patterns / Part II build-run-test) + operator admin-command reference (`ADMIN_COMMANDS.md`) | Permanent; updated when emulator knowledge changes ([PROC R6.9]) |
| `customskills/` | Module-facing guides: player user guide, module code reference (with modifier/badge specification appendices), LLM contributor guide ([LLM_GUIDE.md](customskills/LLM_GUIDE.md)) | Permanent; updated when code changes ([PROC R6.6]) |
| `tracking/` | Current objectives and task state | Living tracker; reconciliation-stamped per [PROC R6] |
| `briefs/` | Task brief definitions (claim/deliver) | Permanent once delivered; archived periodically |
| `installation/` | Server-administrator guide: install, config, verify, remove | Permanent; updated with every integration change |
| `feature-planning/` | Active feature design (Challenge Tier) | Living design docs; graduate into reference/rules or archive |
| `archive/` | Superseded documents (historical reference only) | Frozen; never cited as current authority |

Audience separation is a governing constraint (owner directive 08242026):
emulator content stays under `swgemu/`; module content under
`customskills/`; the two sets do not bleed into each other.

## Entry Point for Contributors

Start with the workspace-root ``START-HERE.md``, then read
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
| SWGEmu/emulator guide (player or developer) | `swgemu/<TOPIC>.md` | ALL_CAPS_SNAKE matching existing names |
| Module guide (player, developer, or LLM) | `customskills/<TOPIC>.md` | ALL_CAPS_SNAKE matching existing names |
| Feature design (pre-implementation) | `feature-planning/<NAME>.md` | Descriptive name |
| Temporary note, scratch, draft | `temp/` (create as needed) | Descriptive name + lifespan marker |

### Required reading before creating documents

1. The workspace-root ``START-HERE.md``
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
| workspace-root `START-HERE.md` | Contributor entry point, required reading order, code tour |
| `../README.md` | Module summary and directory roles |
| `../MANIFEST.md` | Every module-owned file + integration points (living tracker) |
| `installation/INSTALLATION.md` | Canonical install/configure/verify/remove guide (the archived `integration/INSTALL.md` was its duplicate) |
| `../integration/core3-hooks.patch` | Core3 integration patch (file list in MANIFEST) |
| `../../../bin/scripts/customskills/config.lua` | Server configuration |
| `../../../bin/scripts/commands/customSkills.lua` | Command registration |

---

## Contributing to Documentation

- Keep audience separation clear: swgemu/ = emulator players + Core3
  developers; customskills/ = module players, developers, and LLMs;
  installation = admins; rules = governing authority. The two guide sets
  stay self-contained (owner directive 08242026).
- Update the relevant guide when changing behavior ([PROC R6.6]).
- Record newly discovered toolchain/environment facts in their owning
  reference document same-session ([PROC R6.9]).
- Prefer consolidating into the existing reference documents over
  creating new ones; archive rather than delete (preserves history) and
  update links that pointed to archived docs.
- **Update `swgemu/CODE_REFERENCE.md`** when discovering new Core3
  patterns useful for future researchers, and its Part II when
  build/test/deploy operations change ([PROC R6.9] capture applies).
  Factual errors in either are filed via [rules/errata.md](rules/errata.md);
  the historical log for the retired SWGEMU_CODE_GUIDE lives in
  [archive/CODE_GUIDE_ERRATA.md](archive/CODE_GUIDE_ERRATA.md).
