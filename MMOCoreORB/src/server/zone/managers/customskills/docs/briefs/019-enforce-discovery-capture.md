# BRIEF-019 -- Institutionalize R6.9 discovery capture (codify the standing procedure)

- Status: UNCLAIMED
- Created: 08242026 by hy3-free (opencode/hy3-free), owner directive:
  "we have been doing a lot of code research; updating docs with findings
  should be process/procedure, not ad hoc."
- Priority: ERRATA > BRIEFS > anything else. This brief is a process brief
  (not a code/doc defect); queue it behind any live errata/briefs.

## Problem

During the BRIEF-014 / ERR-010 work we read deep into the module (enum
scoping, where config.lua merge happens, restart-vs-reload, build-host
warnings) but left most findings in chat instead of recording them. [R6.9]
discovery capture and [R6.6] behavior-changes-carry-docs already mandate this;
the gap was discipline, not missing rules. ERR-010 itself was partly caused by
a doc snippet that showed a bare `enum Type` (omitting the enclosing `class`),
so an LLM "simplified" to `using namespace` and broke the build.

## Objective

Make discovery capture a default behavior, not a reminder we forget:

1. Every non-trivial fact learned from reading code or building MUST land in
   its owning reference doc in the SAME session (per R6.9):
   - architecture / class shape / API -> `customskills/CODE_REFERENCE.md`
   - worker gotchas / build-host quirks -> `customskills/LLM_GUIDE.md`
   - build / run / test mechanics -> `swgemu/CODE_REFERENCE.md`
2. Each captured fact carries a `GOTCHA` / note block citing the verifying
   source (file:line) per [R6.10], so a later session can trust it.
3. Misleading snippets in reference docs (like the old bare-enum snippet) are
   corrected when found, because they cause the next worker to repeat the
   mistake.

## Acceptance criteria

- [ ] LLM_GUIDE has a standing "Discovery capture is mandatory (R6.9)" section
      (DONE this session -- verify it persists).
- [ ] CODE_REFERENCE's `CustomSkillsModifierType.h` section shows the real
      `class { enum Type {…} };` shape + the class-not-namespace gotcha
      (DONE this session -- verify it persists).
- [ ] A short checklist exists that workers run at session end: "Did I learn
      anything not yet in the owning doc? If yes, add it before committing."
- [ ] This brief is closed only after the checklist is appended to LLM_GUIDE's
      session-protocol (step 4/5 area) so it is seen at claim and delivery time.

## Notes

The backfill for THIS session's findings (config.lua is data not script;
class-not-namespace; Linux/GCC14 build warnings benign; menu count helper) was
applied directly to CODE_REFERENCE + LLM_GUIDE in commit accompanying this
brief. This brief's remaining work is to make the *procedure* unmissable.
