# BRIEF-013 -- Delegated broadcastCombatSpam: module-owned combat spam pipeline with same-line labels

- Status: WITHDRAWN by ox-alpha (opencode/x-preview-f-free), 08242026 -- owner directive: combat spam labels are being REMOVED entirely rather than enhanced. Superseded by BRIEF-014. Number not reused ([PROC R3]).
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

## Phase 0 -- Client-settings research (GATING: do before any code)

Players customize combat spam client-side. Documented options (SWG
Options -> Combat / Chat color; sources: swg.fandom.com/wiki/Options,
swglegends.com/wiki/Options):

- **Brief vs Verbose combat messages**: Brief = "Korren hits a bolma male
  528 points"; Verbose = prose format naming the special attack used.
- **Verbose sub-toggles**: Show Attacker's Weapon, Show Damage Detail
  (energy/kinetic + elemental split), Show Armor Absorption.
- **Chat color customization**: players set per-channel/message colors in
  the Chat color tab.

The client composes these variants FROM THE PACKET FIELDS. A raw-text
replacement line is fixed server-side and CANNOT honor Brief/Verbose,
the verbose sub-toggles, or player chat colors. Therefore:

- The naive raw-text full-line replacement from the original draft is a
  LAST RESORT and violates this gate as a primary mechanism.
- **Preferred compliant mechanism**: keep stringId-mode packets and swap
  WHICH stf key they reference when a label fires -- e.g. broadcast
  `cbt_spam:attack_hit_crit` instead of `attack_hit`. The `(CRIT)` text
  lives in custom stf entries shipped via the operator's TRE builder workspace (client-
  side), so the CLIENT composes the line itself: every Brief/Verbose/
  weapon/detail/color setting keeps working, because the packet path is
  unchanged. Server picks the key; client renders.
- If research shows the variant-key approach cannot preserve some
  setting, STOP and report AWAITING-OWNER with specifics.

## Deliverables

0. **Client-settings research record** appended to this brief: confirm
   each listed option's interaction with both packet modes (stringId vs
   raw-text), citing code paths (`broadcastCombatSpam` construction) and
   client documentation. Conclude explicitly whether the variant-stf-key
   approach preserves every player option.
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
   - Suffix pending on the attacker -> emit the label via the compliant
     mechanism chosen in Phase 0 (preferred: swap stringName to the
     `_crit` stf variant key so the CLIENT still composes Brief/Verbose/
     colored output itself; raw-text fallback only if Phase 0 proves
     variant keys insufficient, and then AWAITING-OWNER before shipping).
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
- Player-settings preservation verified: Brief mode, Verbose mode (+weapon/detail/absorption sub-toggles), and chat colors all behave with labels ON and OFF.
- Compile/idlc deferred to Docker build environment (documented), as
  with BRIEF-007.
- Single commit tagged `[BRIEF-013]`, pushed.

## Out of scope

- Implementing Double/Triple/Quad tier mechanics (ERR-009, owner
  decision pending).
- Per-label styling/font features beyond preserving the color byte.
- Any balance or value changes.
