# Brief 030 -- Align customskills process/procedure to Project Alice (XPP alignment pass 1)

- Status: DELIVERED -- hy3-free (opencode/hy3-free), 08252026. Gap table: all 8 audited, outcomes in commit 0bb1cc6495 (verification convention adopted, doc separation added, audit cadence adopted, errata compliant, proposals OK, MILESTONES+CONTRIBUTORS created, signing conform). Mirror brief on Alice: next.
- Created: 08252026 by hy3-free (opencode/hy3-free), owner directive:
  "we are probably behind alice" -- bring this repo's policy/procedure up to
  Alice's standard. Part 1 of a two-part alignment (part 2 = mirror brief on
  Alice to adopt anything unique to us).
- Priority: HIGH -- people work across both projects; divergent procedure
  causes real friction.
- XPP note: this brief is itself the customskills-side half of alignment;
  its completion triggers the Alice-side mirror brief.

## Goal

Close every gap where Alice's governance is stricter or more mature than
ours, so that after this brief + its mirror, both projects' policy/
procedure are nearly identical (tailored wording/paths only).

## Known gaps to review (audit each; adopt unless reason not to)

1. **Second-party verification gate** -- Alice applies high-risk changes
   only after independent verification; filer-applied entries sit at
   RESOLVED-PENDING-VERIFICATION. We have R6.10 for delegated work but no
   gate on self-applied high-risk edits. (Detail already scoped in
   BRIEF-029 -- fold its outcome here rather than running separately.)
2. **Design-vs-process separation** (Alice brief 031) -- strict separation
   of design content from process documents with defined placement rules
   and reconciliation gates. Our docs mix freely; evaluate adopting the
   separation principle at our scale.
3. **Integrity audits** -- Alice runs periodic integrity audits
   (`docs/audits/integrity/<date>/checklist.md`) with adjudication and
   resolution records. We have ad-hoc errata but no audit cadence. Evaluate
   a lightweight audit cycle (e.g. after each milestone/brief batch).
4. **Errata archival discipline** -- Alice's errata has an archive split,
   self-index, tombstone rules, and a formal verification-block convention
   (verification never separates from entry). We adopted L1-L6 in BRIEF-028
   on paper; verify our errata.md actually complies today (entry format,
   Status values RESOLVED-PENDING-VERIFICATION etc.) and conform it.
5. **Proposal framework parity** -- compare author/reviewer quick starts,
   decision.md requirements, feedback file conventions, status vocabulary.
   Conform gaps.
6. **MILESTONES.md ledger** -- Alice keeps a milestone ledger; we track
   milestones nowhere central. Evaluate adoption (even minimal).
7. **CONTRIBUTORS.md roster** -- Alice maintains one; we don't. Adopt.
8. **Signing/attribution conventions** -- compare long/short formats and
   slot semantics; conform any drift.

## Method

For each numbered gap: read Alice's authoritative text, read ours, decide
ADOPT / ADAPT / DECLINE (with reasons), implement conforming edits in the
same pass. BRIEF-029's verification-gate question resolves here too.

## Deliverables

- [ ] Gap table completed in delivery report (gap | Alice practice | ours |
      outcome | edit made).
- [ ] process.md / LLM_GUIDE / supporting docs updated per outcomes.
- [ ] ERR/registry updates if conventions change existing entries' shape.
- [ ] Mirror brief created ON ALICE: "Align Alice to customskills"
      (part 2), covering anything we have that Alice lacks (e.g. XPP check
      in session protocol step numbering, pre-push privacy grep in session
      steps, admin-command doc standards) -- UNCLAIMED.
- [ ] Commit tagged `[BRIEF-030]`; pushed.

## Out of scope

- Player-facing docs (USER_GUIDE/ADMIN_COMMANDS content) -- governance only.
- Anything requiring Alice-side changes (goes in the mirror brief instead).
