# BRIEF-025 -- Menu fixes: show all modifiers per badge, shorten labels, fix bottom-pane spacing, retry column alignment

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner-reported from live
  menu screenshot.
- Priority: HIGH (player-facing regression + UX polish).

## Problem 1 -- Bonus pages show only the current modifier's bonus per badge

`CustomSkillsMenu::addBonusItems` (CustomSkillsMenu.cpp:152) lists badges for
ONE modifier type and appends only that type's bonus. Previously the menu
displayed EVERY modifier affecting a badge. Example: Bounty Hunter badges
grant crit chance + triple + quad, but inside the Crit Chance page only the
crit chance value shows.

**Fix**: in `addBonusItems`, after computing the current `type` bonus,
append ALL other non-zero bonuses for that badge (loop
`CustomSkillsModifierType::COUNT` like `addBadgeItems` at :82-86 does),
marking/distinguishing the current page's modifier first. Reference
behavior: `addBadgeItems` (:66-90) already renders all-modifier suffixes.

## Problem 2 -- Shorten display names

Owner-approved short names (apply consistently across category rows,
titles, prompt text, and badge-bonus suffixes):

| Current | New |
|---|---|
| Critical Chance | Cri |
| Critical Multiplier | Crit Multi |
| Double Attack | Double |
| Triple Attack | Triple |
| Quad Attack | Quad |

Sites to update: `CustomSkillsMenu.cpp` addPageItems case BONUS_COMBAT
(:311) labels; `getTitle` (:389); any prompt text naming modifiers;
`CustomSkillsModifiers.cpp:93` name table (used by formatModifierBonus).

## Problem 3 -- Bottom pane: huge gap between bonus name and stat

The Bonuses summary pane renders "name ........ stat" with a huge gap.
Likely cause: SUI listbox rows use the game's default variable-font tab /
long padding between label and value. Investigate how the row text is built
(likely `addCategoryItem` with showBonuses=true, or a prompt-text table);
replace whitespace/tab padding with a compact single-space or fixed two-space
separator and verify in-game that spacing collapses.

## Problem 4 -- Top pane column alignment (retry)

Top-pane columns don't align (owner notes prior attempts failed). Likely
because the client listbox font is proportional and space-padding cannot
align. Options to try, in order:
1. Use the game's color codes + monospaced-friendly markers already in use
   (`O`/`X`) and pad with a fixed-width separator instead of spaces.
2. If SUI supports it, switch the top pane to a two-column layout via
   `\t` tab stops or SuiListBox data columns (check
   `SuiListBox::addMenuItem` overloads / DataTableListBox variants).
3. If neither works, document the limitation in MENU_DESIGN.md and leave as-is
   (owner accepts).

## Deliverables

- [ ] addBonusItems shows every modifier's bonus per badge (current page's
      modifier listed first).
- [ ] Short names applied everywhere (Cri, Crit Multi, Double, Triple, Quad).
- [ ] Bottom-pane gap fixed or root-caused with findings documented.
- [ ] Column alignment retried; outcome (fix or documented limitation) noted
      in MENU_DESIGN.md.
- [ ] Build verified on Linux host; committed tagged `[BRIEF-025]`; pushed.

## Notes

- Menu code lives in CustomSkillsMenu.{h,cpp}; formatting helpers in
  CustomSkillsModifiers.cpp. Keep changes surgical; this is UI-only.
- R6.9: any new SUI layout facts go to customskills/CODE_REFERENCE.md.
