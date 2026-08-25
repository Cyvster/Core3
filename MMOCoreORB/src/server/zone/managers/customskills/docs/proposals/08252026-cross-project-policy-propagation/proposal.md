# Proposal -- Cross-project policy propagation (customskills <-> Project Alice)

- Author: hy3-free (opencode/hy3-free)
- Date: 08252026
- Status: PROPOSED
- Type: Process/policy (affects both projects' process documents)

## Problem

Two sibling projects (SWGEmu Custom Skills, Project Alice) evolve their
policies and procedures independently. Improvements discovered in one --
discovery-capture requirements, archival lifecycle rules, privacy scrubbing,
signing conventions, pre-push verification checks -- do not automatically
reach the other. Recent history shows the lag:

- Discovery capture was mandated on customskills (BRIEF-019/R6.9) before
  Alice's briefs README gained its authoring-rules equivalent.
- The privacy scrub (real name -> Cyvster) was executed on both projects,
  but only because the owner noticed exposure manually; nothing required
  the second project to adopt the resulting rule.
- Alice invented bounded-ledger archives (brief 041); customskills only
  adopted it later via BRIEF-028, again by owner initiative.

Each lag is a governance gap: whichever project misses a good practice
operates with weaker guardrails indefinitely.

## Proposed rule: Cross-Project Propagation (XPP)

When a policy or procedure is **implemented** (adopted into a governing
document and delivered) on either project, its implementation MUST include:

1. **Mirror-review brief**: create a brief on the OTHER project titled
   "Review + adopt: <policy name> (from <origin project>)". The brief:
   - Links to the origin implementation (commit + file).
   - Asks the reviewer to evaluate applicability to this project's context.
   - Requires an outcome of ADOPT / ADAPT / DECLINE, recorded in the brief's
     delivery report.
2. **Tailoring is expected, not optional-strict**: the receiving project
   adapts wording, numbering, file paths, and scope to its own conventions
   and needs. Strict verbatim adoption is NOT expected. A DECLINE must state
   why the policy does not fit (different constraints, already covered by an
   equivalent local mechanism, etc.).
3. **Timing**: the mirror-review brief is created in the SAME session as the
   originating implementation (it is part of "implementation complete"),
   filed as UNCLAIMED unless the coordinator executes it immediately.
4. **Tracking**: each project keeps a short ledger section (in its process
   doc or docs/README) listing cross-propagated policies: name, origin,
   date adopted/adapted/declined. This prevents silent drift.

## Scope

- Applies to: process rules, documentation standards, quality gates,
  privacy/security practices, workflow procedures.
- Does NOT apply to: project-specific technical work (game mechanics vs
  memory architecture), bug fixes, feature briefs -- unless such work
  produces a reusable policy.

## Worked examples (would have triggered XPP)

| Policy | Origin | Mirror outcome expected |
|---|---|---|
| Discovery capture mandate | customskills R6.9 | Alice: ADOPTED (now done, briefs README authoring rules) |
| Bounded ledgers + archives | Alice brief 041 | customskills: ADOPTED (BRIEF-028) |
| Privacy rule (name -> handle, no local paths, pre-push grep) | customskills ERR-014 | Alice: ADAPTED (partial -- llm-guide grep check still pending) |
| Second-party verification gate | Alice | customskills: candidate for review |

## Implementation plan

1. customskills: add [PROC R#.#] "Cross-Project Propagation" section to
   docs/rules/process.md; add propagation ledger table to docs/README.md.
2. Alice: add equivalent section to docs/rules/process.md; ledger in
   docs/README.md (or tracking/objectives.md if process.md placement is
   disallowed by its design/process separation rule -- brief 031).
3. Seed both ledgers with the four worked examples above.
4. Each repo's brief-authoring rules gain one line: new policy/procedure
   briefs must include a "mirror-review brief created?" checkbox.

## Constraints

- Both repos are owned by the same operator; the mirror brief may be
  authored by the same coordinator session that implemented the origin
  policy (no second party needed for FILING; execution follows each
  project's normal pickup protocol).
- Alice's design-vs-process separation (its brief 031) governs WHERE the
  rule lives there; adapt placement accordingly.

## Decision requested

Adopt XPP as a standing procedure on BOTH projects, with tailoring as above?
