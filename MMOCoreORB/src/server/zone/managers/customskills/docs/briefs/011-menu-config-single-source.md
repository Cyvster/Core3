# BRIEF-011 -- Menu/config single-source: stop /customskills from duplicating badge assignments (resolves ERR-008)

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), produced by
  the BRIEF-008 deep audit; split from the original consolidated
  remediation brief so the behavioral fix gets an isolated delivery.
  Prerequisite reading: BRIEF-008 (AUDIT-1 evidence), ERR-008.

## Problem

`CustomSkillsMenu.cpp` hardcodes a second copy of every modifier's badge
assignments in 24 anonymous-namespace `badges*` arrays and computes
per-page badge lists, acquired counts, and bonus totals from that copy.
Assignments have ONE authority: `config.lua`. Drift is live: after
BRIEF-007 expanded Critical Chance to 60 badges, the menu's Critical
Chance page still lists the old 24-key subset and its category total
computes 24% while combat applies 60% ([CS-3] violated, ERR-008).

## Deliverables

1. Eliminate the duplicated assignment data. Choose one approach and
   document the choice in the commit message:
   - **Preferred**: derive menu lists at runtime by iterating each
     modifier's config map (`getBadgeBonuses(type)`) and grouping keys
     via a single static key -> menu-group table. The table carries
     GROUPING only -- per-badge bonus values are never restated.
   - **Alternative**: keep the arrays as presentation-only grouping but
     add boot-time validation in `CustomSkillsConfig::load()` warning
     loudly on any drift between union-of-arrays and each modifier's
     config keys.
2. Menu BONUS_* pages show every assigned badge; per-page totals equal
   gameplay totals (`CustomSkillsModifiers::getModifierTotal`) for all 18
   modifiers.
3. Resolve ERR-008 after verification. Solo sessions self-verify under
   [DIRECTIVE 08242026].
4. Docs ([PROC R6.6]): update any menu-structure references in
   CODE_REFERENCE.md Appendix B / ARCHITECTURE-derived text if grouping
   presentation changes.

## Constraints

- No changes to gameplay math -- the fix is presentational/source-of-
  truth only. Combat totals must be bit-identical before/after.
- Grouping tables may keep human-readable group names; they must not
  carry per-badge values.

## Acceptance criteria

- Repo-wide grep proves no second copy of per-badge assignment data
  outside config.lua (or validation warns on drift if alternative was
  chosen).
- For every modifier: menu page badge set == config badge set (spot-check
  Critical Chance 60, Gathering 13, Experience 5 documented in commit).
- ERR-008 RESOLVED with verification note.
- Compile verified or deferral documented (engine3 submodule caveat).
- Single commit tagged `[BRIEF-011]`, pushed.
