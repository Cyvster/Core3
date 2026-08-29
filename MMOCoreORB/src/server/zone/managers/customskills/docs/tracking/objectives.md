# Objectives

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Working task list for the Custom Skills module. The milestone charters in the master document record strategy; this file tracks concrete, current work.

**Purpose:** any contributor (LLM or human) opening this repository should see what is being worked on, what is done, and what comes next -- then continue in the same direction.

## How to use this file

1. Read **Current Focus** first. Work on that unless the item is claimed.
2. Claim an item by appending your name to its line (format: [../rules/process.md](../rules/process.md) -> Contributor Recording Conventions). One active claim per contributor.
3. Check off items only when the acceptance criterion actually holds.
4. Add newly discovered objectives under the right section instead of leaving them implicit.
5. Disputes about objectives go through [../rules/errata.md](../rules/errata.md), not silent edits.
6. Finished items move to **Archive** with completion date.

---

## Current Focus

**M6 -- Challenge Tier skill trees (design phase)** -- data rows implemented in skills.csv; remaining: tier count/bonus decision, display names, then Core3-side wiring. See [../feature-planning/CHALLENGE_TIER_SKILLS.md](../feature-planning/CHALLENGE_TIER_SKILLS.md).

---

## M5/M6 -- Challenge Tier system

- [x] ~~Settle open question: how many Challenge Tiers exist and what XP bonus each grants (planning assumes 1000% at high tiers)~~ -- DECIDED 08242026: ten tiers, +25/+50/+100/+175/+275/+400/+550/+700/+850/+1000%. Recorded in feature-planning doc (Challenge Tier ladder).
- [x] ~~Skill display names for all 57 boxes (currently TBD in tree tables)~~ -- DECIDED 08242026: scheme A (Novice/Master + Branch I-IV); filled into the tree tables. Stringfile entries pending deployment.
- [ ] Verify resource-container stack limits handle creature_harvesting=1000 yields during live testing.
- [ ] Implement Challenge Tier XP award gating (award only at Tier 1+, amount = creature CL x CT bonus x badge multiplier per agreed composition rule).
- [ ] Rebuild skills.iff via TRE builder and deploy to test server.

## Infrastructure & continuous

- [ ] Verify regenerated core3-hooks.patch with a full reverse-check on a clean checkout (deferred from BRIEF-003 delivery; needs a vanilla-base build session).
- [x] ~~Mirror Core3-only work into package/ + MANIFEST~~ -- OBSOLETE: package mirror dissolved by BRIEF-001 (08232026); remaining content accuracy work (H16 inventory, patch coverage, modifier count) moved to [../briefs/003-reconcile-hooks-patch-modifier-count.md](../briefs/003-reconcile-hooks-patch-modifier-count.md).
- [x] ~~Decide whether module repository should gain its own git history~~ -- RESOLVED 08232026: owner directive migrated all module artifacts INTO the Core3 repo; docs now version-controlled at `MMOCoreORB/src/server/zone/managers/customskills/docs/`.

## Archive



- M1 command-opened SUI menu -- complete (shell tested in-game)
- M2 first effective modifier (Critical Chance: 12 combat badges, 3% each, 150% multiplier) -- complete
- M3 all modifiers + hooks implemented -- complete
- M4 badge assignments & config, 111 badges mapped -- complete
- Documentation governance adoption (rules/, tracking/, briefs/, reference/ migration, readme.md) -- ox-alpha (opencode/x-preview-f-free), 08232026
- Core3 infrastructure research + two-guide split: CORE3_CODE_REFERENCE.md (patterns/systems, from retired SWGEMU_CODE_GUIDE.md) and CORE3_USER_GUIDE.md (build/run/test; verified unit-test suite findings: GoogleTest 1.13 compiled into core3 binary, ~264 cases, MariaDB required for all boot paths incl. tests, no CI) -- ox-alpha (opencode/x-preview-f-free), 08232026
- Documentation compression: IMPLEMENTATION_GUIDE + MENU_SYSTEM merged into ARCHITECTURE.md (single module reference); PROJECT_OVERVIEW, DEVELOPER_QUICKSTART, integration/INSTALL archived as redundant (unique content salvaged to readme.md / INSTALLATION / process.md); reference/ reduced to 5 docs -- ox-alpha (opencode/x-preview-f-free), 08232026
- Retired workspace-root SWGEMU_CUSTOMSKILLS_CONTEXT.md (the project's original mission-control doc) after salvaging its tech-stack summary into the root readme.md.md; entry point relocated from customskills-mod/ to workspace root same day -- ox-alpha (opencode/x-preview-f-free), 08232026

---

**Last reconciled:** 08272026

## Contributors

- **Cyvster** -- project owner
- ox-alpha (opencode/x-preview-f-free), 08232026 -- initial tracker
