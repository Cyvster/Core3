# BRIEF-010 -- Combat spam labels: enable/disable control, menu listing, and same-line append fix

- Status: CLAIMED by ox-alpha (opencode/x-preview-f-free), 08242026
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Owner report from last live test: the appended label
  (`(CRIT)` etc.) did not appear on the same line as the combat message
  -- it dropped to its own line, cluttering the screen and making combat
  output confusing. Owner also wants the label feature properly
  toggleable and visible in `/customskills`, with REMOVAL as the
  sanctioned fallback if the display defect cannot be fixed.

## Background

Each of these modifiers carries a `combatSpamLabel` in `config.lua`:
`(CRIT)`, `(DOUBLE)`, `(TRIPLE)`, `(QUAD)` (see `criticalChance`,
`doubleAttackChance`, `tripleAttackChance`, `quadAttackChance`). When a
proc fires, the label should be appended to that hit's existing combat
spam line.

## Deliverables

### Phase 1 -- Investigate (read-only; document findings in this brief)

1. **Trace the emission path**: locate where each label is consumed
   (expected: `CustomSkillsCombat.cpp` H01 pipeline and the attack-tier
   code) through to the actual client message API used. Identify exactly
   how the string is assembled and which Core3 send function delivers it.
2. **Root-cause the newline drop**: determine why the label renders on a
   separate line. Candidate causes to check and rule in/out with quoted
   evidence:
   - label sent as a SEPARATE system/combat-spam message instead of
     concatenated into the hit's message;
   - concatenation happening after a trailing newline in the base
     string;
   - wrong API (a variant that wraps lines / uses flytext vs chat);
   - server-side string fine but client font/width wrapping (if so,
     prove it: same payload via standard native spam stays on one line).
3. **Enable/disable audit**: establish current behavior for
   - modifier disabled -> no proc, hence no label (verify);
   - label set to empty string `""` -> does the code skip emission or
     send an empty suffix?
   - is there any owner-facing way to turn labels off WITHOUT disabling
     the mechanic? Document what exists vs what is missing.
4. **Menu audit**: confirm whether the Server Config page of
   `/customskills` lists combat spam labels/toggle state; record what is
   shown today.

### Phase 2 -- Fix (based on findings)

5. **Same-line append**: implement the fix so the label appends to the
   END of the same combat spam line (e.g. `Creature attacks X for N
   points of damage (CRIT)`). Minimal diff; reuse the native spam path
   rather than inventing a parallel one.
6. **Explicit enable/disable**: add an owner-facing control so labels
   can be turned off without disabling the underlying modifier --
   preferred shape: `combatSpamLabelsEnabled = true/false` global switch
   plus honoring empty-string label per-modifier as "off" (empty =
   suppress emission entirely, not blank suffix). Loader must accept and
   apply it; default = enabled (current behavior preserved).
7. **Menu listing**: ensure Server Config shows the combat spam state
   (e.g. `Combat Spam Labels: ON/OFF`) alongside existing toggle rows.
8. **Docs** ([PROC R6.6]): USER_GUIDE feature sections gain one line
   noting the label and its on/off control; INSTALLATION config
   reference documents the new switch; CODE_REFERENCE Appendix A rows
   mention it if they cite combatSpamLabel.

### Phase 3 -- Fallback (only if Phase 2 step 5 fails)

9. If the client provably cannot append same-line through any available
   API (must be demonstrated with evidence in this brief, not assumed):
   STOP. Do not remove anything unilaterally. Set this brief's status to
   AWAITING-OWNER with the findings summary; removal of the label
   feature (config keys + emission code + docs) happens only on an
   explicit owner directive after that report.

## Acceptance criteria

- Findings section in this brief answers all four Phase-1 questions with
  file/function anchors and quoted code.
- After fix: label appears on the SAME line as the hit's combat message
  (in-game verification steps written down; executed if a test server is
  available, otherwise marked pending live verification).
- Labels fully suppressible: global switch + per-modifier empty-string
  both work; disabled modifier emits nothing.
- Server Config page reflects the state.
- Docs updated in the same commit set ([PROC R6.6]).
- Commit(s) tagged `[BRIEF-010]`; compile verified or deferral documented
  (engine3 submodule caveat applies).

## Out of scope

- Changing proc rates, values, caps, or badge assignments.
- Removing the feature without explicit owner directive.
- Native (non-module) combat spam formatting.
