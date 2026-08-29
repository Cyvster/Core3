---
type: format-reference
version: 1
updated: 08262026
---

# Documentation Format Reference

Token-optimized formatting rules for project documentation. Apply these
when creating or updating any `.md` file in this repository.

## Rules

### Blank lines
- **No blank lines between tightly related content** (bullet items, consecutive descriptions, list entries)
- **One blank line between major sections** (## headers)
- **No blank lines inside tables**
- Each blank line = 1 token. Every one must earn its place.

### Headers
- Use `##` for major sections, `###` for subsections
- **Compact headers:** `## Feature Name` not `## Feature Name\n`
- Remove header trailing whitespace

### Lists
- **Bullet lists:** one line per item, no blank lines between items
- **Compact bullets:** `- **Label:** description` not `- **Label:**\n  description`
- Avoid nested lists when a flat list works

### Tables
- Use tables for structured data (they're token-efficient)
- **Compact tables:** no blank lines before/after
- Single-line rows: `| col1 | col2 | col3 |`

### Code blocks
- **Avoid code blocks for short examples** — use inline code instead
- `menu > submenu` beats a 5-line code block showing the same menu
- Reserve code blocks for actual code or multi-line output

### Inline formatting
- **Bold** for labels/keys: `**Key:** value`
- `backticks` for file paths, commands, code references
- Avoid bold+italic, strikethrough, or other heavy formatting

### Sections
- **Merge small sections** that are 1-3 lines each
- **Remove redundant headers** if the content is obvious from context
- **Compact contributor metadata** to one line at the end

### Redundancy
- **Never restate what's in a linked document** — link to it instead
- **Remove introductory phrases** ("In order to", "It is important to note that")
- **Remove qualifying text** that restates the obvious

## Examples

### Before (verbose)
```markdown
## Understanding the Menu

The menu is opened by typing the following command:

```
/customskills
```

Once opened, you will see the following options:

- **Badges** — This option allows you to browse badges by category
  to see which ones contribute to your totals.
- **Bonuses** — This option shows your current accumulated totals,
  grouped by Combat, Utility, and Crafting. Only non-zero values
  are displayed.
- **Server Config** — This option shows the server-side toggle
  state (e.g., Rarity Naming).

**Navigation:** OK (select row) opens subpage; Back returns one
level; Cancel closes menu; Refresh (on badge lists) reopens
current page.
```

### After (optimized)
```markdown
## Menu Navigation
`/customskills` opens: Badges (browse by category), Bonuses (your totals: Combat/Utility/Crafting, non-zero only), Server Config (toggle state). Navigation: OK=subpage, Back=one level, Cancel=close, Refresh=reopen.
```

### Token comparison
- Before: ~120 tokens
- After: ~40 tokens
- Savings: 67%

## Quick Checklist

Before committing any documentation:
1. Count blank lines — remove unnecessary ones
2. Check for code blocks used where inline would work
3. Check for nested lists that could be flat
4. Check for redundant introductory phrases
5. Check for content that restates a linked document
6. Verify all information is preserved (format changes only)
