# Brief 042 -- Remediation pass: FCT visibility, repeat-craft hardening, subcommand + radial

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free)
- Consolidates: ERR-016..019 remediation (BRIEF-039 findings) + owner
  live-test feedback + BRIEF-041 subcommand/radial conversion.

## A. FCT visibility fix (owner live-test: escalation invisible; vanilla pool colors dominate)

Root cause verified in code: vanilla `showHitLocationFlyText`
(CombatManager.cpp:2936, called from :1614) renders at scale 1.0 with pool
colors on EVERY hit; our escalated ShowFlyText (CustomSkillsCombat.cpp:106)
renders at the same anchor immediately after. Client stacks/z-fights them;
vanilla wins visually.

Fix approach:
1. Add a suppressible path: e.g. a member flag or parameter on
   `showHitLocationFlyText` (or a CombatManager bool set around the escalated
   call) so that when our escalation fires, vanilla's text is SKIPPED and ours
   is the only hit-location text rendered.
2. Broadcast ours to fight observers like vanilla does (defender's observers,
   not attacker-only -- fixes audit F-01).
3. Keep suppression strictly scoped: only when repeats>1 or critical AND
   fctEnabled. Base hits must still show vanilla text exactly as before.
4. Choose implementation minimizing Core3 surface: prefer a default-parameter
   or small wrapper over changing call sites broadly; document whatever touch
   is made to CombatManager.cpp/.h.

### B.1 Stun-vs-lair documentation note (owner question)

Investigated: no vanilla stun immunity mechanism exists for lairs in this
tree. LairObject has no armor/resist fields; the TANO damage path
(:1630) applies only armor-piercing + turret reduction. What players
perceive as "stun doesn't affect lairs" is that stun weapons' STATE effect
cannot apply to a TangibleObject -- raw stun-type damage still lands.
Consolidated strike multiplies whatever the path produces regardless of
weapon type, so no special replication is needed. Document in USER_GUIDE:
tier hits on lairs work with all weapon types; state effects (stun/blind)
remain creature-only per vanilla.

## B. Lair / TANO-defender support (owner live-test: no double/crit vs lairs)

Lair/TANO defenders take `applyDamage(TangibleObject* attacker ...)` via
CombatManager.cpp:377 -> :1417 -- which DOES delegate to us, BUT our gate
`attacker->isPlayerCreature()` passes while tier rolls run... verify why
double/crit did not apply vs lair (suspect: lair damage path uses the OTHER
overload at :1630 with different signature that bypasses our hook entirely).
Trace precisely which overload "a pile of rocks" hits take; wire tier logic +
FCT into whichever path is missing it. Document the call graph in delivery.

## C. Repeat-craft hardening (ERR-017/018/019)

1. Recursive inventory scan: pre-fill resource search must include containers
   inside inventory (backpacks, satchels) -- recurse containers, depth-limited.
2. Partial-stack handling: if total available < needed across stacks, fill
   what exists, leave remainder empty, and send the short-resource message
   naming the resource and shortfall (brief 036 requirement).
3. Experiment restore: either honor cs36.exp on pre-fill (apply stored
   row/point allocation) or stop writing it -- per original brief intent,
   implement the restore.
4. Customization capture was also spec'd: assess feasibility (customization
   string on schematic); if impractical, document honestly and remove from
   snapshot scope.

## D. Subcommand + radial conversion (BRIEF-041)

1. `/customskills repeatcraft [tool]`: argument parsing on existing
   customskills command; remove RepeatCraftCommand.h + repeatCraft.lua
   registration.
2. Radial "Repeat Craft" on crafting tools with a stored snapshot
   (pattern: CraftingToolImplementation.cpp:48-52 server radial). Route to
   doRepeatCraft. Verify no TRE requirement.

## E. Doc sync

CODE_REFERENCE (flytext suppression note, TANO path notes, subcommand form),
USER_GUIDE (/customskills repeatcraft), errata statuses -> RESOLVED with
verification notes where fixes land.

VERIFY: brace balance zero on all touched files; grep no real names/paths;
commit `[BRIEF-042]`, push origin cyvster3.
Final response: per-item disposition list, files touched, any item deferred
with reason.
