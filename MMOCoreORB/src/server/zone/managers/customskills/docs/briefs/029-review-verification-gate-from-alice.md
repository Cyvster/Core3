# Brief 029 -- Review + adopt: Second-party verification gate (from Project Alice)

- Status: CLAIMED
- Created: 08252026 by hy3-free (opencode/hy3-free) per XPP procedure
  (proposal 08252026-cross-project-policy-propagation, ADOPTED).
- Origin: Project Alice -- its process requires changes with elevated risk
  (architecture, guardrails, seed policy) to be applied only after an
  independent second party verifies them; filer-applied entries are marked
  RESOLVED-PENDING-VERIFICATION until then. Reference:
  `G:\Data\projects\project alice\github\alice\docs\rules\process.md`
  (verification/errata sections) and ERR-046/047 usage.

## Review question

Should Custom Skills adopt a second-party verification gate for
high-risk changes? Candidates here would be:

- C++ hook changes (integration/core3-hooks.patch)
- config.lua schema changes affecting live badge math
- Menu/SUI behavior changes that alter what players see

Our current practice is single-worker delivery + owner review on GitHub;
R6.10 (verify delegated work independently) covers delegation but not
self-applied high-risk edits.

## Task

1. Read Alice's verification rules and 2+ real usages (ERR-046, ERR-047,
   ERR-052) to understand cost/benefit in practice.
2. Assess fit against our scale (single active worker most sessions), risk
   profile, and existing R6.10.
3. Outcome: ADOPT (define trigger threshold + verifier eligibility),
   ADAPT (e.g. gate applies only to hooks patch + schema changes, owner
   review counts as second party when no other worker is available), or
   DECLINE (with reasons).

## Deliverables

- [ ] Outcome recorded here + in the XPP ledger (rules/process.md).
- [ ] If ADOPT/ADAPT: rule text added to process.md and LLM_GUIDE session
      protocol.
