# BRIEF-024 -- Privacy scrub: remove personal name + local filesystem paths

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive. The repo
  owner is known publicly only by the handle **Cyvster** (git identity already
  correct: `Cyvster <41338761+Cyvster@users.noreply.github.com>`). Real name
  must not appear anywhere in public docs/code; local drive paths must not
  either.
- Priority: HIGH -- privacy exposure, treat like errata.

## Problem (audit findings, this session)

1. **Personal name**: ~49 occurrences of the owner's real name across the
   customskills docs tree (errata.md x11, briefs 001/005/006/013 +
   README, proposals/*, process.md, project-design.md, LLM_GUIDE.md,
   tracking/objectives.md, swgemu/CODE_REFERENCE.md, docs README).
   Upstream files (sql credits tables, doc/ConversationEditor) contain
   unrelated third-party names from SOE credits -- DO NOT touch those.
2. **Local paths**: references to the owner's Windows drive layout
   (`G:\Data\...` TRE-builder workspace path appears in swgemu/CODE_REFERENCE.md;
   audit all docs for similar `X:\`, `C:\Users\...`, home-dir strings).

## Goal

1. Replace every personal-name occurrence in tracked files with **Cyvster**
   (or "the owner" where prose reads better). Keep meaning intact.
2. Replace local absolute paths with repo-relative or generic descriptions
   (e.g. "the TRE builder workspace outside this repository").
3. Sweep ALL tracked files (docs, briefs, proposals, code comments,
   config examples) -- not just the docs tree.
4. Add a standing privacy rule to rules/process.md: no real names (use
   Cyvster), no local absolute paths, no machine-specific details in any
   tracked file; future contributors follow it.
5. Add a pre-push grep check to the delivery checklist in LLM_GUIDE session
   protocol (name + path patterns) so this never regresses.

## Constraints / cautions

- Do NOT edit upstream/third-party content (SOE credit dumps in sql/,
  engine3 vendor code) -- their names are public record and not the owner's.
- Git HISTORY will still contain old commits with the name; full removal
  requires history rewrite (filter-repo) + force push. That is disruptive and
  needs explicit owner approval as a separate decision -- this brief covers
  forward-fixing tracked files only, and documents the history-rewrite option
  in its delivery report.
- The brief itself must not contain the name; refer to "the owner's real
  name" generically.

## Acceptance criteria

- [ ] Zero occurrences of the owner's real name in tracked working-tree files
      (grep-verified, excluding upstream sql/credits).
- [ ] Zero local absolute paths (`[A-Z]:\` or `[A-Z]:/`) in tracked files.
- [ ] Privacy rule added to process.md + checklist line in LLM_GUIDE.
- [ ] Delivery report lists: files changed, count of replacements, and a
      note on the git-history-rewrite option requiring owner approval.
- [ ] Committed + pushed; brief marked DELIVERED.
