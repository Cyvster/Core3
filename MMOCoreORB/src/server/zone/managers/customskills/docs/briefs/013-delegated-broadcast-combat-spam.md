# BRIEF-013 -- Delegated broadcastCombatSpam: module-owned combat spam pipeline with same-line labels

- Status: UNCLAIMED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive resolving the BRIEF-010 AWAITING-OWNER decision (Option A --
  full-line replacement via delegated broadcast). Owner intent: full
  control of combat spam output enables future customization (color,
  font markup, per-label styling) impossible through the stock packet.

## Background

The native hit line is a stringId-mode CombatSpam packet whose text the
client composes from `file`/`stringName`/damage fields; the packet has no
free-text slot, so labels like `(CRIT)` cannot ride on it (BRIEF-010
Phase 1 evidence). All hit spam funnels through ONE function:
`CombatManager::broadcastCombatSpam(attacker, defender, item, damage,
file, stringName, color)`. Delegating that function to module code gives
us total authority over emission while every caller site stays untouched.

## Deliverables

1. **Delegation** (`CombatManager.cpp`, existing patched file): replace
   the body of `broadcastCombatSpam` with a single delegation call to
   `CustomSkillsCombat::broadcastCombatSpam(this, ...)` -- mirror the
   established `applyDamage` wrapper pattern. The vanilla implementation
   moves into module code.
2. **Module implementation** (`combat/CustomSkillsCombat.cpp/.h`):
   - No suffix pending -> replicate vanilla behavior exactly: same
     closeObjects iteration (`COMBAT_SPAM_RANGE`, PLAYERTYPE receivers),
     same stringId-mode packet construction, same color defaults.
     Cite upstream origin in a header comment so future audits track
     drift.
   - Suffix pending on the attacker -> build ONE raw-text packet
     containing the complete line: resolve `file:stringName` via
     `StringIdManager`, substitute `%DI` -> damage and `%TU`/`%TT` ->
     attacker/defender names, append the pending label, send to each
     in-range player (per-receiver loop preserved).
   - Clear the pending suffix after emission regardless of path.
3. **Suffix carrier** (`CreatureObject.idl`): add
   `public transient unicode pendingSpamSuffix;` (precedent:
   `tradeTargetID`). Requires idlc regeneration at build time.
4. **Producer swap** (`combat/CustomSkillsCombat.cpp::applyDamage`):
   replace the `sendCustomCombatSpam(label, 11)` call with setting the
   pending suffix; clear it in the wrapper after
   `applyVanillaDamage(...)` returns so nothing leaks to later hits.
5. **Remove `sendCustomCombatSpam`** from `CreatureObject.idl` +
   `CreatureObjectImplementation.cpp` -- it becomes unused, and its
   separate-packet behavior is exactly what we are eliminating.
6. **Toggle compliance**: label emission honors
   `isCombatSpamLabelsEnabled()` and empty-string labels (both shipped
   in BRIEF-010); suppressed runs take the vanilla-replication path.
7. **Docs + trackers** ([PROC R6.6]):
   - Regenerate `integration/core3-hooks.patch` (new/changed hunks:
     CombatManager delegation, CreatureObject.idl additions/removals,
     CreatureObjectImplementation.cpp).
   - MANIFEST.md integration-points list: add CreatureObject.idl /
     CreatureObjectImplementation.cpp entries.
   - ARCHITECTURE.md hook inventory: update H01 area to describe the
     broadcast delegation.
   - INSTALLATION.md / CODE_REFERENCE.md Appendix A: note that procced
     labels now appear inline on the damage line.

## Customization note (enabled capability, not implemented here)

Raw-text lines accept SWG markup (`\#RRGGBB ... \#.`), so per-label
colors/fonts become available once this lands. Keep the `color` byte
passthrough faithful to vanilla in this brief; styling extensions are
future work.

## Acceptance criteria

- Grep proves `broadcastCombatSpam` body is a pure delegation; no vanilla
  spam logic remains in CombatManager.cpp.
- Non-proc hits: implementation is a verbatim copy of the pre-change
  vanilla function (diff against upstream cited in commit message) --
  packets semantically identical.
- Procced crit hit: exactly ONE spam packet, containing damage number and
  label on the same line.
- Toggle off or empty label -> zero label packets, vanilla line only.
- Patch regenerated and applies cleanly to a vanilla checkout (reverse
  check documented).
- MANIFEST + ARCHITECTURE + INSTALLATION updated; ERR-009 untouched.
- Compile/idlc deferred to Docker build environment (documented), as
  with BRIEF-007.
- Single commit tagged `[BRIEF-013]`, pushed.

## Out of scope

- Implementing Double/Triple/Quad tier mechanics (ERR-009, owner
  decision pending).
- Per-label styling/font features beyond preserving the color byte.
- Any balance or value changes.
