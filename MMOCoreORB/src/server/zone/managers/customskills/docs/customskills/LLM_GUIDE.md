# Custom Skills -- LLM Contributor Guide
Audience: LARGE LANGUAGE MODEL SESSIONS working on this repository -- and nobody else. Human players read `USER_GUIDE.md`; developers read `CODE_REFERENCE.md`; server administrators read `../installation/INSTALLATION.md`. If you are an LLM asked to do any work here, this document is your entry point.

## Identity and voice
You are one contributor among many concurrent LLM sessions plus the human owner (Cyvster). One entity, one voice: edit only within your claimed scope ([PROC R2]), never sign or complete content for another, and route disagreements through errata instead of editing over them ([PROC R7]). All workers commit under the shared machine identity, so document signatures are the ONLY attribution record -- sign everything per [rules/process.md](../rules/process.md) → Contributor Recording Conventions.

## Required reading order
1. [rules/project-design.md](../rules/project-design.md) -- master document; principles [CS-N]. Several intentional design choices look like bugs if unread (basis points, native fallback, parity).
2. [rules/process.md](../rules/process.md) -- collaboration, claiming, traceability tags, completeness ([R6.6]), discovery capture ([R6.9]).
3. [rules/errata.md](../rules/errata.md) -- dispute/correction protocol.
4. [../tracking/objectives.md](../tracking/objectives.md) -- current focus and open objectives.
5. [CODE_REFERENCE.md](CODE_REFERENCE.md) -- module technical reference: hooks H01-H16, services, menu.  
Deeper reference is indexed in [../README.md](../README.md).

## Session protocol
1. Read the OPEN entries in errata FIRST; attempt the oldest eligible one before other work ([ERR duty], errata.md → Worker quick start).
2. Check coordination state: briefs index for UNCLAIMED work, recent commits, and stagger scopes while another session is active ([PROC R8/E5] interim rule).
3. Claim before working: signed status line in the file, commit AND push at claim time ([PROC R6.5]; unpushed claims may be re-claimed).
4. Deliver completely: behavior changes carry their documentation in the same commit set ([R6.6]); newly learned toolchain/environment facts land in their owning reference doc same-session ([R6.9]).
5. Record where work landed; update trackers with reconciliation stamps.
6. **End-of-session discovery sweep (R6.9):** before committing, ask "Did this session teach me anything not yet in the owning reference doc?" -- enum scoping, which file owns a merge, restart-vs-reload behavior, build-host quirks, gotchas that bit us. If yes, add a `GOTCHA`-style note to CODE_REFERENCE / this guide / swgemu/CODE_REFERENCE (citing file:line per R6.10) in the SAME commit. Findings left in chat are lost findings.
7. **Pre-push privacy check (ERR-014):** grep your diff for the owner's real name (must be `Cyvster`) and local absolute path patterns (`[A-Z]:[\\/]`). Any hit blocks the push until scrubbed -- privacy rule in [rules/process.md](../rules/process.md).
8. If nothing is claimable or eligible: STOP and report back -- do not invent work ([briefs pickup protocol step 9]).

## Code tour (15 minutes)
Start inside the module dir (`MMOCoreORB/src/server/zone/managers/customskills/`):
`CustomSkillsModifierType.h` — modifier enum.  
`CustomSkillsConfig.h/.cpp` — config loading, defaults.  
`CustomSkillsModifiers.h/.cpp` — central modifier API.  
`MANIFEST.md` — every module file + integration point.  
Each hunk in `integration/core3-hooks.patch` is a small delegation call; read one to understand the pattern. Build/run/test procedures: [../../swgemu/CODE_REFERENCE.md](../swgemu/CODE_REFERENCE.md) (Part II). Server data-archive mechanics (TRE builder, stringfiles): same document → Client data archives.

## Common failure modes (learned the hard way)
Restating formats/rules instead of referencing their authority ([PROC R6.7] violation; grep-sweeps will catch it).  
Signing with the harness name instead of the model/persona identifier (slot semantics in process.md; see ERR-006 history).  
Editing another contributor's historical record (errata evidence, delivered briefs) to "fix" paths or formats -- records are protected; fix live documents only.  
Leaving ISO dates or stale paths after a convention change -- sweep your own delivery before committing.  
Pushing is part of committing: an unpushed commit does not exist for coordination purposes ([PROC R6.5/P1]).  
**Broken-LLM generation drops function bodies (not just declarations).** When reconciling generated C++, treat a declared-and-called symbol with no definition as a first-class defect. ERR-010 showed missing *declarations* (`addBonusItems`, `countOwnedBonuses`); ERR-011 showed a *declared-and-called-but-bodyless* function (`countModifier`) that compiled but failed at **link** with `undefined reference`. After any generated-code change, grep the header for every `static` declaration and confirm a matching `ClassName::symbol(` definition exists in the `.cpp` before declaring a build done. A single missing body surfaces only at link time, after all TUs compile.

## Badge & modifier integration rules
When working on badge assignments or modifier configuration (`config.lua`), follow these rules (moved here from the old badge reference; they are worker instructions, not reference data):

### For config authors (`config.lua`)
1. **Use stable keys only** -- `BadgeList::get(key)` resolves index; never hard-code indices (fragile across TRE changes).
2. **Never include excluded badges** in `badges[]` arrays (pilot/JTL + admin/event exclusions -- see CODE_REFERENCE Appendix B excluded list).
3. **Pilot badges**: only add if JTL is actually implemented on the target server.
4. **Basis points**: 100 = 1.00% for percentages; whole units for armor penetration / defense cap / SEA cap.
5. **Cap values**: 0 = uncapped; applied after badge aggregation.

### For developers
**BadgeDefinition** (metadata): key, resolved index, Core3 type, exploration subtype, source group, show flag.  
**BadgeModifierAssignment** (gameplay): badge key, modifier ID, amount, stacking rule.  
**Keep separate** -- prevents menu grouping from becoming gameplay logic; allows other servers to replace badge indices without rewriting modifier definitions.

## Build & toolchain facts (captured this session, R6.9)
**Config is pure Lua data, not a script.** `config.lua` is a single Lua table (`customSkillsConfig`) -- no `ipairs`/merge loop in Lua. The merge of `badges[]` + `badgeOverrides` + `badgeBonus` into the runtime map happens in **C++** (`CustomSkillsConfig::load()`, `CustomSkillsConfig.cpp:114` calls `lua.runFile("scripts/customskills/config.lua")`). A config edit needs a **server restart** to take effect (no `/reload` picks it up).  
**`CustomSkillsModifierType` is a class, not a namespace** (see CODE_REFERENCE gotcha, ERR-010). Enumerators MUST be `CustomSkillsModifierType::X`.  
**Build host is Linux (GCC 14 / Ninja).** SWGEmu Core3 compiles with `-std=gnu++14`; GCC 14 emits `-Wstringop-overflow` warnings on engine3's atomic code (auction/PlayerObject) -- those are benign warnings, not errors. Windows here has no C++ toolchain, so code changes are build-verified on the Linux host, not locally.

## Discovery capture is mandatory (R6.9)
Every fact learned while reading code or building -- enum scoping, which file owns a merge, restart-vs-reload behavior, build-host quirks -- goes into its **owning reference doc in the SAME session** (CODE_REFERENCE for architecture, this guide for worker gotchas, swgemu/CODE_REFERENCE for build/test). Do NOT leave findings in chat history. If you learn something and the owning doc doesn't cover it, add a subsection (or file errata proposing a home) -- do not drop it.

## Contributors
ox-alpha (opencode/x-preview-f-free), 08242026 -- initial guide.  
hy3-free (opencode/hy3-free), 08242026 -- BRIEF-020 dense-format pass.
*Last reconciled 08242026 by hy3-free (opencode/hy3-free) -- BRIEF-020.*


## Discovery capture ([R6.9])

Every delivery report lists discoveries made during the task, each with the
reference-doc cite where it was captured, or "no discoveries". Hotfixes that
require root-cause diagnosis must capture their lesson in a reference doc in
the same commit. See rules/process.md -> Discovery-capture enforcement.
