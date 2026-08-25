# Custom Skills -- LLM Contributor Guide

Audience: LARGE LANGUAGE MODEL SESSIONS working on this repository --
and nobody else. Human players read `USER_GUIDE.md`; developers read
`CODE_REFERENCE.md`; server administrators read
`../installation/INSTALLATION.md`. If you are an LLM asked to do any
work here, this document is your entry point.

---
## Identity and voice

You are one contributor among many concurrent LLM sessions plus the
human owner (Daniel). One entity, one voice: you edit only within your
claimed scope ([PROC R2]), never sign or complete content for another,
and route disagreements through errata instead of editing over them
([PROC R7]). All workers commit under the shared machine identity, so
document signatures are the ONLY attribution record -- sign everything
per [rules/process.md](../rules/process.md) -> Contributor Recording
Conventions.

---
## Required reading order

1. [rules/project-design.md](../rules/project-design.md) -- master
   document; principles [CS-N]. Several intentional design choices look
   like bugs if unread (basis points, native fallback, parity).
2. [rules/process.md](../rules/process.md) -- collaboration, claiming,
   traceability tags, completeness ([R6.6]), discovery capture
   ([R6.9]).
3. [rules/errata.md](../rules/errata.md) -- dispute/correction protocol.
4. [../tracking/objectives.md](../tracking/objectives.md) -- current
   focus and open objectives.
5. [CODE_REFERENCE.md](CODE_REFERENCE.md) -- module technical reference:
   hooks H01-H16, services, menu.

Deeper reference is indexed in [../README.md](../README.md).

---
## Session protocol

1. Read the OPEN entries in errata FIRST; attempt the oldest eligible
   one before other work ([ERR duty], errata.md -> Worker quick start).
2. Check coordination state: briefs index for UNCLAIMED work, recent
   commits, and stagger scopes while another session is active
   ([PROC R8/E5] interim rule).
3. Claim before working: signed status line in the file, commit AND push
   at claim time ([PROC R6.5]; unpushed claims may be re-claimed).
4. Deliver completely: behavior changes carry their documentation in the
   same commit set ([R6.6]); newly learned toolchain/environment facts
   land in their owning reference doc same-session ([R6.9]).
5. Record where work landed; update trackers with reconciliation stamps.
6. If nothing is claimable or eligible: STOP and report back -- do not
   invent work ([briefs pickup protocol step 9]).

---
## Code tour (15 minutes)

Start inside the module dir
(`MMOCoreORB/src/server/zone/managers/customskills/`):

| File | Purpose |
|------|---------|
| `CustomSkillsModifierType.h` | Modifier enum |
| `CustomSkillsConfig.h/.cpp` | Config loading, defaults |
| `CustomSkillsModifiers.h/.cpp` | Central modifier API |
| `MANIFEST.md` | Every module file + integration point |

Each hunk in `integration/core3-hooks.patch` is a small delegation call;
read one to understand the pattern. Build/run/test procedures:
[../../swgemu/CODE_REFERENCE.md](../swgemu/CODE_REFERENCE.md)
(Part II). Server data-archive mechanics (TRE builder, stringfiles):
same document -> Client data archives.

---
## Common failure modes (learned the hard way)

- Restating formats/rules instead of referencing their authority
  ([PROC R6.7] violation; grep-sweeps will catch it).
- Signing with the harness name instead of the model/persona identifier
  (slot semantics in process.md; see ERR-006 history).
- Editing another contributor's historical record (errata evidence,
  delivered briefs) to "fix" paths or formats -- records are protected;
  fix live documents only.
- Leaving ISO dates or stale paths after a convention change -- sweep
  your own delivery before committing.
- Pushing is part of committing: an unpushed commit does not exist for
  coordination purposes ([PROC R6.5/P1]).

---
## Badge & modifier integration rules

When working on badge assignments or modifier configuration
(`config.lua`), follow these rules (moved here from the old badge
reference; they are worker instructions, not reference data):

### For config authors (`config.lua`)

1. **Use stable keys only** -- `BadgeList::get(key)` resolves index;
   never hard-code indices (fragile across TRE changes).
2. **Never include excluded badges** in `badges[]` arrays (pilot/JTL +
   admin/event exclusions -- see CODE_REFERENCE Appendix B excluded
   list).
3. **Pilot badges**: only add if JTL is actually implemented on the
   target server.
4. **Basis points**: 100 = 1.00% for percentages; whole units for armor
   penetration / defense cap / SEA cap.
5. **Cap values**: 0 = uncapped; applied after badge aggregation.

### For developers

- **BadgeDefinition** (metadata): key, resolved index, Core3 type,
  exploration subtype, source group, show flag.
- **BadgeModifierAssignment** (gameplay): badge key, modifier ID,
  amount, stacking rule.
- **Keep separate** -- prevents menu grouping from becoming gameplay
  logic; allows other servers to replace badge indices without
  rewriting modifier definitions.

---

**Last reconciled:** 08242026 by ox-alpha (opencode/x-preview-f-free) --
initial guide, owner-directed audience split (swgemu/customskills/LLM);
absorbs the workspace-root START-HERE role; 08242026 badge/modifier
integration rules moved in from BADGE_REFERENCE fold (owner directive).

---
## Contributors

- ox-alpha (opencode/x-preview-f-free), 08242026 -- initial guide
