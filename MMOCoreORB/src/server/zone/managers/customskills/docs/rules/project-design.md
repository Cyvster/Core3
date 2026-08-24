# Custom Skills -- Project Design (Master Document)

> **Status:** Ratified
> **Created:** 08232026 by ox-alpha (opencode/x-preview-f-free)
> **Last reconciled:** 08232026 by ox-alpha (opencode/x-preview-f-free);
> 08232026 BRIEF-002 -- Integration Policy rewritten for single-tree layout

## Master Document Notice

This is the **master document** for all project rules and design decisions.
Every other document in this module's `docs/` tree is subordinate to it. When
a
subdocument contradicts this document, this document prevails. Subdocuments
may provide operational detail, procedural steps, or implementation guidance,
but they may not redefine principles, override decisions, or introduce
requirements that conflict with what is stated here.

### Subordinate Documents

| Citation | Document | Purpose | When to Use |
|----------|----------|---------|-------------|
| [PROC R#] | docs/rules/process.md | Collaboration, recording, and package-sync rules | Before any multi-session or delegated work |
| [ERR-NNN] | docs/rules/errata.md | Correction protocol and dispute resolution | When you believe a document or claim is wrong |
| [OBJ] | docs/tracking/objectives.md | Current objectives and task state | At session start; update on delivery |
| [BRIEF-NNN] | docs/briefs/README.md | Delegable task queue | When picking up discrete work items |

Reference material (not governing): `docs/reference/*`, `docs/installation/*`,
`docs/user-guide/*`, `docs/feature-planning/*`, `MANIFEST.md`.

## Scope

This document contains ONLY design content:

- **IS design content:** mission, core principles [CS-N], architectural
  decisions, integration and distribution policy, gameplay conventions
  (units, caps, stacking), milestone charters.
- **IS NOT design content** (lives elsewhere; referenced, not restated):
  contributor collaboration and recording conventions
  (`docs/rules/process.md`); correction records (`docs/rules/errata.md`);
  current task state (`docs/tracking/objectives.md`); technical deep-dives
  (`docs/reference/*`).
- **When a fact could be either:** design states WHAT the module is and WHY;
  process states HOW contributors work; errata records what was corrected.

## Mission

Extend SWGEmu Core3 with badge-derived character bonuses and supporting
systems as a **self-contained, distributable module**: minimal edits to
existing Core3 files, all module-owned code isolated under dedicated
directories, configuration table-driven via Lua, distribution installable
without git history.

## Core Design Principles

**[CS-1] Isolation** -- module-owned code lives in dedicated `customskills/`
directories; behavior never leaks into ad-hoc Core3 locations.

**[CS-2] Minimal Core3 edits** -- existing Core3 files receive only generic,
reversible delegation hooks at stable boundaries, captured in one
self-contained patch (`integration/core3-hooks.patch`).

**[CS-3] Single source of truth** -- `CustomSkillsModifiers` serves both
gameplay hooks and the SUI menu; menu values must equal runtime values.

**[CS-4] Table-driven configuration** -- modifier definitions, badge
assignments, caps, and enablement live in `scripts/customskills/config.lua`,
never hardcoded in C++ logic.

**[CS-5] Basis points** -- percentages use bp (100 = 1.00%), multipliers use
bp (10000 = 1.00x); convert to display units only at the UI boundary.

**[CS-6] No new persistence** -- read existing badge bitmasks on
PlayerObject; do not add IDL/db changes until a feature requires it with a
migration plan.

**[CS-7] Character-scoped resolution** -- every displayed or applied value
resolves from the invoking character; no global player state.

**[CS-8] Client/server parity** -- wherever the client is told a value
(speed, visible skill mods), server validation MUST use the identical
calculation (H13A/H13B pattern).

**[CS-9] Native fallback** -- disabled or zero modifiers must reproduce
native Core3 output exactly.

**[CS-10] Stable public API** -- `/customskills` command and
`CustomSkills:openMenu()` are stable interfaces; changes require owner
ratification recorded in this document or an ERR entry.

## Integration Policy

- Hook inventory H01-H16 (H02 rejected) is defined in
  `docs/reference/ARCHITECTURE.md`; that document is the operational
  authority on hook placement and MUST stay consistent with the applied patch.
- `MANIFEST.md` (module dir root) lists every module-owned file and every
  edited Core3 file. It is updated whenever a module file or integration
  point is added, removed, or renamed -- in the same change (per
  [PROC R6.6]).
- The module lives inside this repository at
  `MMOCoreORB/src/server/zone/managers/customskills/`. **Containment rule**:
  module-owned files stay in that directory except where Core3's runtime
  layout requires otherwise; the sanctioned exceptions are the two script
  files under `MMOCoreORB/bin/scripts/` and the delegation hooks inside
  existing Core3 sources. Distribution = copy module dir + scripts + apply
  `integration/core3-hooks.patch`.
- The standalone mirror folder (`customskills-mod/`, `package/`) was
  dissolved on 08232026 ([BRIEF-001]); references to it are historical.

## Gameplay Conventions

- All 18 modifiers (CRITICAL_MULTIPLIER promoted to standalone 08232026) are enumerated in
  `docs/reference/MODIFIER_REFERENCE.md`, which is the authority on per-
  modifier units, defaults, caps, and badge assignments.
- Badge pool: 111 eligible badges of 140 total (9 pilot/JTL and 20
  admin/event badges excluded).
- Config loads once at startup and is cached; restart required after
  `config.lua` changes. Missing or invalid values fall back to safe defaults
  plus a server-log warning.

## Milestone Charters

| Milestone | Charter | Status |
|-----------|---------|--------|
| M1 | Command-opened SUI menu | Complete |
| M2 | First effective modifier (Critical Chance) | Complete |
| M3 | All modifiers + hooks implemented | Complete |
| M4 | Badge assignments & config (111 badges) | Complete |
| M5 | Challenge Tier system | Design phase |
| M6 | Challenge Tier skill trees | Design phase |

Current objective state lives in `docs/tracking/objectives.md` [OBJ].

## Open Design Areas

- Challenge Tier tier count and per-tier XP bonus (blocks M5/M6 cost
  calibration; planning assumption documented in
  `docs/feature-planning/CHALLENGE_TIER_SKILLS.md`).
- Skill display strings per Challenge Tier box.

Changes to open areas are decided by Daniel and recorded either here (if
they become design rules) or in the feature-planning document (if they
remain feature-local decisions).

## Contributors

- **Daniel** -- project owner; final authority on design decisions
- ox-alpha (opencode/x-preview-f-free), 08232026 -- initial structure,
  consolidating principles formerly spread across ARCHITECTURE.md and
  archive/DESIGN.md
