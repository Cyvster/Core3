# Per-worker topology: isolated clones, integration by gate (policy)

**Proposed by:** ox-alpha (opencode/x-preview-f-free), 08232026
**Last updated:** 08232026 by ox-alpha (opencode/x-preview-f-free)
**Status:** POLICY ADOPTED -- Cyvster (directive), 08232026; provisioning
pending
**Trigger:** The shared working tree is the ROOT cause of tonight's
collisions (errata clobber; duplicate brief delivery). Push/staging/sync
rules are guards on a risky topology. Framework source: Project Alice
proposal `08232026-per-entity-clones` (POLICY ADOPTED there, provisioning
pending).

---

## 1. Governing conventions

- [../../rules/process.md](../../rules/process.md): R5, R6.5, R6.8.
- Disputes: [../rules/errata.md](../rules/errata.md).
- Companions: the three guard proposals adopted alongside.

## 2. Why this exists

Two sessions writing one checkout can always race; discipline only lowers
the rate. Isolated clones remove shared state by construction: each worker
edits its own tree, publishes to a personal branch, and integrates through
an explicit gate.

## 3. Scope

| Covers | Excludes |
|--------|----------|
| Policy: per-worker clone/worktree + personal branch | Provisioning mechanics (Cyvster's environment) |
| Integration via explicit rebase/merge gate | CI automation |
| Interim single-scope-staggering rule until provisioned | Retiring the guard proposals |

## 4. The proposal

E1. Each worker operates in an isolated clone or dedicated git worktree of
    `Core3/` -- never directly in another session's checkout, once
    provisioned.
E2. Workers push their personal branch (named `work/<short-id>/<topic>`);
    [PROC R6.5] push language applies to that branch.
E3. Integration into the shared base branch happens through an explicit
    gate: pull --ff-only/rebase by the integrating session after C1 checks,
    with Original-author trailers preserved.
E4. Guard proposals (push/staging/sync) relax to style preferences when
    E1-E3 are OPERATIONAL: at least two workers provisioned; gate used for
    at least one integration; no direct shared-tree commits for 7
    consecutive days.
E5. INTERIM RULE (effective now, until provisioning): concurrent sessions
    on the shared tree must STAGGER SCOPES -- before starting work, check
    the briefs index and recent commits (`git log --oneline -5`); do not
    touch files outside your declared scope while another session is active
    in the tree; claims are still committed AND pushed immediately per the
    remote-push proposal.

## 5. Integration plan

Phase 1 (this adoption): process.md gains "Repository topology" section
stating policy + interim rule. Phase 2: Cyvster provisions clones/worktrees
and declares the gate branch. Phase 3: E4 sunset evaluation.

## 6. Relationship matrix

| Interacts with | Nature |
|----------------|--------|
| All three guard proposals | Sunset dependency (their S4/C-relaxation/E4) |
| [PROC R6.5] | Push target becomes personal branch post-provisioning |
| Briefs pickup protocol | Claim/deliver push targets personal branch |

## 7. Implementation notes

Open question for Cyvster: preferred mechanism -- separate clones vs
`git worktree` on one machine. Either satisfies E1.

## 8. Revision log

| Date | Change | Source |
|------|--------|--------|
| 08232026 | Initial draft, policy adopted same day | Owner directive |
