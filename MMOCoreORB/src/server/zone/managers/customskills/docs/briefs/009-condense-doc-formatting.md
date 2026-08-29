# BRIEF-009 -- Condense documentation; enforce clear visual hierarchy and distinct feature sections

- Status: CLAIMED
- Claimed: 08242026 by hy3-free (opencode/hy3-free) -- picked up per pickup
  protocol (first UNCLAIMED in index).
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Owner complaint (verbatim intent): reading a doc takes "10
  minutes" of scrolling for very little information; font-size
  differences and bold overuse make main categories hard to distinguish
  from subcategories; feature sections must be obviously delimited --
  where one starts and the next begins.

## Problem

1. **Low information density**: long prose blocks, repeated
   explanations, and generous tables spread little content across many
   screens (worst offenders: `customskills/CODE_REFERENCE.md` at ~1100
   lines after the appendix fold; `customskills/USER_GUIDE.md`).
2. **Broken hierarchy**: markdown heading levels are used
   inconsistently (multiple H1s per file in merged docs; H2/H3 mixed
   roles), so rendered output does not visually separate major sections
   from subsections.
3. **Fuzzy feature boundaries**: feature sections (e.g. each modifier in
   USER_GUIDE) run together with no visual delimiter.

## Formatting standard to apply (all files below)

- **Exactly one H1 per file** (the document title). H2 = major sections
  and EVERY feature/entry section. H3 = only sub-parts of an H2.
  Never skip levels.
- **`---` horizontal rule immediately before every H2** -- feature and
  section starts are unmissable when scrolling.
- **Feature sections use a fixed template** (USER_GUIDE modifiers,
  Appendix A entries): one-line purpose -> compact property table ->
  source list. No multi-paragraph essays.
- **Density rules**: no prose paragraph longer than 3 lines; prefer
  tables/bullets over sentences; no restating content defined elsewhere
  ([PROC R6.7] -- link instead); no decorative repetition of the same
  fact in multiple sections.
- **TOC**: every file longer than 150 lines gets an anchor-linked table
  of contents directly under the title.
- **Bold**: reserved for defined terms and table labels -- never for
  whole sentences or emphasis decoration.

## Files to process

1. `docs/customskills/CODE_REFERENCE.md` (largest; appendices A/B are
   mostly tables -- compress spec tables by merging property rows into
   single-line columns where possible)
2. `docs/customskills/USER_GUIDE.md`
3. `docs/swgemu/USER_GUIDE.md`
4. `docs/swgemu/CODE_REFERENCE.md` (Parts I/II)
5. `docs/customskills/LLM_GUIDE.md`
6. Workspace-root `readme.md` (already short -- verify standard)

## Deliverables

1. All six files reformatted per the standard above.
2. Content preservation rule: compression means removing redundancy and
   verbosity -- NOT facts. Every modifier, badge group, default value,
   cap, key name, hook reference, and procedure step that exists before
   must exist after (spot-checkable).
3. Line-count report appended to this brief (before -> after per file),
   signed.
4. Stamps updated on touched trackers ([PROC R6]); index wording in
   docs/README.md adjusted if it references structure that changed.
5. **Worked formula examples** added where a modifier has non-obvious
   math, placed in CODE_REFERENCE.md Appendix A entries (developer
   numbers) with a simplified echo in USER_GUIDE where players benefit.
   Every example is a factual claim: verify against code and cite the
   source per [PROC R6.10]. Required at minimum:
   - **Armor Penetration x weapon Armor Piercing ordering** (owner-
     required example, math pre-verified against source -- reproduce
     faithfully):
     Vanilla rule (`CombatManager::getArmorPiercing`, quoted):
     `AP > armor -> damage x 1.25^(AP - armor)` /
     `armor >= AP -> damage x 0.50^(armor - AP)`.
     The module's penetration lowers the defender's armor level first
     (`CustomSkillsCombat::getEffectiveArmorRating`:
     `max(0, nativeArmor - penetration)`), then the vanilla exponent
     evaluates. Example: Heavy armor (level 3) vs AP-1 weapon ->
     `0.50^(3-1)` = 0.25x damage; with +2 penetration -> armor 1 vs AP 1
     -> 1.00x; with +3 -> armor 0 vs AP 1 -> `1.25^(1-0)` = 1.25x. Both
     mechanics feed one exponent on opposite sides -- they compound;
     neither nullifies the other.
   - Critical hit chain: crit chance roll -> crit damage
     `(preArmorDamage x (150% base + badge bonus total))` -> repeat-tier
     check order Quad -> Triple -> Double (one tier only).
   - Experience composition: multiplicative
     `base x server x buff x custom` (100 x 2 x 5 = 1000, not 700).
   - Gathering Quantity: badge bonuses ADD, then multiply native yield
     (1 badge = 3x, 2 = 5x); rounded down; never below native.
   - Crafting Speed: `nativeDuration / multiplier` (min 1s); factory
     snapshot semantics noted.
6. Index wording in docs/README.md adjusted if it references structure
   that changed (already listed in item 4 -- keep both true).

## Acceptance criteria

- Single H1 per file; zero skipped heading levels (verifiable by grep
  of heading lines).
- Every H2 preceded by `---`.
- USER_GUIDE feature sections each fit within ~15 rendered lines and
  follow the identical template.
- Total line count across the six files reduced by >= 40% versus the
  pre-brief state (record actual numbers) with no fact loss.
- TOC present in all files > 150 lines.
- Single commit tagged `[BRIEF-009]`, pushed.

## Out of scope

- Changing any factual content, values, or procedures.
- Restructuring which facts live in which document (audience split is
  fixed by [DIRECTIVE 08242026]).
- The errata log and rules documents (protocol formatting already
  governed by their own sections; only apply the H1/rule/heading-level
  fixes there if trivially wrong).
