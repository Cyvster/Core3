# BRIEF-020 -- Apply approved dense doc format to reference/lookup docs

- Status: CLAIMED
- Created: 08242026 by hy3-free (opencode/hy3-free), owner directive: "update
  all documents with similar formatting" -- scoped by owner to reference/
  lookup docs only (this message).
- Priority: ERRATA > BRIEFS > anything else. No live errata/brief blocking;
  this is a formatting pass.

## House style (the approved format, demonstrated by swgemu/ADMIN_COMMANDS.md)

The reference format was iterated with the owner on ADMIN_COMMANDS.md and
approved at commit 203b42fa71. It is:

1. **Tight blocks** -- minimal blank lines between paragraphs; no `---`
   horizontal rules separating sections.
2. **Stacked single-line lists, NO bullet glyph** -- list-like content (e.g.
   command entries, scenario steps) is one item per line, separated by GitHub
   Flavored Markdown **trailing-space hard line breaks** (two spaces at end of
   line) so they render on separate lines WITHOUT a `-`/`*` marker. This is the
   "list format without bullets" the owner asked for.
3. **Bullets only where genuinely nested** -- keep `-` bullets solely for true
   sub-lists; the owner removed them from the Commands and Worked scenarios
   sections.
4. **Inline source citations** -- file:line references stay inline in the line
   they support, not padded into a separate "Sources" block.
5. **No table where a stacked list reads denser** -- tables add GitHub cell
   padding (the puffiest rendered element); prefer stacked lines.
6. **Headers kept minimal** (`##`/`###` only, no rules); prose collapsed to
   scannable single lines.

Net goal (owner's words): "a lot less spacing," "more information on a single
page," "designed to maximize search capability." GitHub's own CSS heading
margins/line-height are accepted as unchangeable from the .md source.

## Targets (reference/lookup docs only)

| Doc | Notes |
|-----|-------|
| `customskills/CODE_REFERENCE.md` | Largest ref doc; flatten section rules, convert enumerations/tables to stacked no-bullet lines where it aids density. Preserve all technical content + file:line citations. |
| `customskills/LLM_GUIDE.md` | Worker guide; apply tight blocks + stacked lists. Keep the "Common failure modes" as stacked lines (no bullets). |
| `customskills/USER_GUIDE.md` | Player guide; same treatment. |
| `swgemu/CODE_REFERENCE.md` | Emulator dev ref; same treatment. This is large -- convert its enumerations/tables to stacked no-bullet lines where density improves; keep the Gating chain section intact. |
| `swgemu/USER_GUIDE.md` | Player/emulator guide; same treatment. |

## Out of scope (owner decision: leave as historical/audit records)

- `rules/errata.md` -- audit trail; section structure is load-bearing.
- `rules/process.md`, `rules/project-design.md` -- governing rules; keep.
- `briefs/*.md` (including this one) -- task records; keep.
- `docs/README.md`, `briefs/README.md` -- indices; keep (minor: may trim
  folder descriptions only if trivial).

## Acceptance criteria

- [ ] All 5 target docs reformatted to the house style above.
- [ ] No `---` rules remain in the 5 targets (unless a rule is required by an
      existing convention the owner wants kept -- none known).
- [ ] No `-`/`*` bullet glyphs remain in list-like content of the 5 targets;
      stacked hard-break lines used instead.
- [ ] All technical content + file:line citations preserved (verify with a
      before/after grep of key tokens: e.g. `CustomSkillsModifierType`,
      `getBadgeBonuses`, `GrantBadgeCommand`, gating terms).
- [ ] Each doc committed separately (reviewable), pushed to origin/cyvster3.
- [ ] Brief marked DELIVERED with a per-doc commit list.

## Notes

- This is a documentation-formatting pass; it changes no code or config.
- Mirror the ADMIN_COMMANDS.md end state exactly when in doubt about a
  specific construct.
- Do NOT over-flatten narrative explanations into unreadable walls; the goal is
  density + searchability, not destruction of readability.
