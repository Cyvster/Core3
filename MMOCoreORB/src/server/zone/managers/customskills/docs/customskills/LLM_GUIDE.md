# Custom Skills -- LLM Contributor Guide

**Start here:** read `.context/AGENTS.md` at the start of every session.
This document is reference material -- consult it when you need detail on
a specific topic, not as an onboarding requirement.

## Quick Reference

| Topic | Where to look |
|-------|--------------|
| Architecture and principles | `.context/AGENTS.md` -> Architecture, or `rules/project-design.md` sections 2-3 |
| Module code reference | `CODE_REFERENCE.md` |
| Collaboration process | `rules/process.md` |
| Claiming and completing work | `.context/AGENTS.md` -> Context Protocol |
| Filing disputes or corrections | `rules/errata.md` |
| Current objectives | `.context/AGENTS.md` -> Current Focus, or `tracking/objectives.md` |
| Build and toolchain | `swgemu/CODE_REFERENCE.md` (Part II) |

## Identity and voice
You are one contributor among many concurrent LLM sessions plus the human owner (Cyvster). One entity, one voice: edit only within your claimed scope, never sign or complete content for another, and route disagreements through errata instead of editing over them. Sign everything per [rules/process.md](../rules/process.md) -> Contributor Recording Conventions.

## Session protocol
1. Read `.context/AGENTS.md` for current state and conventions.
2. Scan `rules/errata.md` for OPEN entries; attempt the oldest eligible one.
3. Claim before working: signed status line, commit AND push at claim time.
4. Deliver completely: behavior changes carry their documentation in the same commit.
5. Write discoveries to `.context/GOTCHAS.md` and owning reference doc same session.
6. Pre-push privacy check: grep diff for real name + local paths.
7. If nothing is claimable: STOP and report back.

## Common failure modes
- Restating formats/rules instead of referencing their authority.
- Signing with the harness name instead of model/persona identifier.
- Editing another contributor's historical records.
- Pushing is part of committing: unpushed claims don't exist for coordination.
- **Broken-LLM generation drops function bodies.** After any generated-code change, grep for every `static` declaration and confirm a matching definition exists in the `.cpp`.

## Badge & modifier rules
- Use stable keys only (`BadgeList::get(key)`); never hard-code indices.
- Never include excluded badges in `badges[]` arrays.
- Basis points: 100 = 1.00%. Cap values: 0 = uncapped.
- `BadgeDefinition` (metadata) and `BadgeModifierAssignment` (gameplay) stay separate.

## Build facts
- Config is pure Lua data, not a script. Merge happens in C++.
- Config edits require server restart.
- `CustomSkillsModifierType` is a class -- enumerators MUST be `CustomSkillsModifierType::X`.
- Build host is Linux (GCC 14 / Ninja); Windows has no C++ toolchain.

## Contributors
ox-alpha (opencode/x-preview-f-free), 08242026 -- initial guide.
hy3-free (opencode/hy3-free), 08242026 -- BRIEF-020 dense-format pass.
*Last reconciled 08242026 by hy3-free (opencode/hy3-free) -- BRIEF-020.*
