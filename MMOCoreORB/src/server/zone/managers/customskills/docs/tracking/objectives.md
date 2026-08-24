# Objectives

> Subordinate to [../rules/project-design.md](../rules/project-design.md) --
> the master document for project rules.

Working task list for the Custom Skills module. The milestone charters in
the master document record strategy; this file tracks concrete, current
work.

**Purpose:** any contributor (LLM or human) opening this repository should
see what is being worked on, what is done, and what comes next -- then
continue in the same direction.

## How to use this file

1. Read **Current Focus** first. Work on that unless the item is claimed.
2. Claim an item by appending your signature to its line
   (`-- <name/model (<origin>)>, <YYYY-MM-DD>`). One active claim per
   contributor.
3. Check off items only when the acceptance criterion actually holds.
4. Add newly discovered objectives under the right section instead of
   leaving them implicit.
5. Disputes about objectives go through [../rules/errata.md](../rules/errata.md),
   not silent edits.
6. Finished items move to **Archive** with completion date + signature.

---

## Current Focus

**M6 -- Challenge Tier skill trees (design phase)** -- data rows implemented
in skills.csv; remaining: tier count/bonus decision, display names, then
Core3-side wiring. See
[../feature-planning/CHALLENGE_TIER_SKILLS.md](../feature-planning/CHALLENGE_TIER_SKILLS.md).

---

## M5/M6 -- Challenge Tier system

- [ ] Settle open question: how many Challenge Tiers exist and what XP bonus
      each grants (planning assumes 1000% at high tiers) -- blocks final cost
      calibration. Decision by Daniel, recorded in feature-planning doc.
- [ ] Skill display names for all 57 boxes (currently TBD in tree tables).
- [ ] Verify resource-container stack limits handle creature_harvesting=1000
      yields during live testing.
- [ ] Implement Challenge Tier XP award gating (award only at Tier 1+,
      amount = creature CL x CT bonus x badge multiplier per agreed
      composition rule).
- [ ] Rebuild skills.iff via TRE builder and deploy to test server.

## Infrastructure & continuous

- [ ] **Mirror Core3-only work into package/ + MANIFEST** (discovered
      2026-08-23): rarity naming (H16), Server Config menu category,
      exceptional-color default, and related commits exist in `../Core3`
      (branch cyvster3, through 504bb6cb15) but are absent from `package/`
      and `MANIFEST.md`. Per [PROC R6.6] this is outstanding debt.
- [ ] Update `docs/reference/ARCHITECTURE.md` hook inventory with
      H16 placement details once mirrored.
- [ ] Decide whether module repository should gain its own git history
      (currently only `Core3/` is version-controlled; affects [PROC R6.5]
      record-keeping). Owner decision.
      **Update 2026-08-23:** superseded by owner directive to migrate all
      module artifacts into the Core3 repo (briefs 001-003) -- docs gain git
      versioning by moving under
      `MMOCoreORB/src/server/zone/managers/customskills/docs/`.

## Archive

*(completed objectives land here with date + signature)*

- M1 command-opened SUI menu -- complete (shell tested in-game)
- M2 first effective modifier (Critical Chance: 12 combat badges, 3% each,
  150% multiplier) -- complete
- M3 all 17 modifiers + hooks implemented -- complete
- M4 badge assignments & config, 111 badges mapped -- complete
- Documentation governance adoption (rules/, tracking/, briefs/,
  reference/ migration, START-HERE) -- ox-alpha
  (opencode/x-preview-f-free), 2026-08-23
- Core3 infrastructure research + two-guide split: CORE3_CODE_REFERENCE.md
  (patterns/systems, from retired SWGEMU_CODE_GUIDE.md) and
  CORE3_USER_GUIDE.md (build/run/test; verified unit-test suite findings:
  GoogleTest 1.13 compiled into core3 binary, ~264 cases, MariaDB required
  for all boot paths incl. tests, no CI) -- ox-alpha
  (opencode/x-preview-f-free), 2026-08-23
- Documentation compression: IMPLEMENTATION_GUIDE + MENU_SYSTEM merged into
  ARCHITECTURE.md (single module reference); PROJECT_OVERVIEW,
  DEVELOPER_QUICKSTART, integration/INSTALL archived as redundant (unique
  content salvaged to START-HERE / INSTALLATION / process.md); reference/
  reduced to 5 docs -- ox-alpha (opencode/x-preview-f-free), 2026-08-23
- Retired workspace-root SWGEMU_CUSTOMSKILLS_CONTEXT.md (the project's
  original mission-control doc) after salvaging its tech-stack summary into
  the root START-HERE.md; entry point relocated from customskills-mod/ to
  workspace root same day -- ox-alpha (opencode/x-preview-f-free), 2026-08-23

---

**Last reconciled:** 2026-08-23 by ox-alpha (opencode/x-preview-f-free).

## Contributors

- **Daniel** -- project owner
- ox-alpha (opencode/x-preview-f-free), 2026-08-23 -- initial tracker
