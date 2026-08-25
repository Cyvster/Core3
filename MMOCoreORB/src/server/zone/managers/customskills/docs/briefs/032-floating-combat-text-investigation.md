# Brief 032 -- Floating combat text investigation: hooking, sizing, coloring, positioning

- Status: CLAIMED
- Created: 08252026 by ox-alpha (opencode/x-preview-f-free), owner directive
- Priority: HIGH (replaces repeated failed attempts at chat combat-spam
  improvements, which are blocked by client modification requirements)

## Goal

Determine what is achievable server-side for modern-style floating combat
text (FCT) using the existing ShowFlyText mechanism and any client-side
options, WITHOUT client binary modification. Deliver a feasibility report
plus, if feasible, an implementation design.

## Owner's requirements to evaluate

1. Text size -- owner wants relative scaling only: adjust by percentage /
   multiplier so it scales with the player's own client slider setting;
   hard-setting size may fight the client.
2. Text content -- what the text says (e.g. damage numbers vs "hit_body").
3. Text color -- per-hit colors; unknown whether client hard-sets or
   respects server values.
4. Positioning -- some status effects render off-to-the-side of the head;
   if positional control exists, FCT could behave like a modern game.
5. Effects -- e.g. shake animation on big hits, if any mechanism exists.

## Known starting facts (verified this session)

- Server packet exists: `ShowFlyText` (packets/object/ShowFlyText.h),
  controller 0x1B/0x1BD, payload = targetID, stringId file+entry,
  float SCALE, RGB bytes, flags byte. **Scale is already a multiplier**
  sent from server; RGB is fully server-controlled in the packet.
- CombatManager.cpp:2916-2931 already sends hit-location flytexts
  (combat_effects hit_head/hit_body etc.) at scale 1.0f.
- Flags byte comment lists 0x0001 shown-on-target, 0x0002 show-in-chat?,
  0x0004 ??? -- flag semantics need mapping (positioning?).
- Client options exist for showing damage over self/others + a size slider
  (owner observation); their interaction with server scale is unverified.

## Investigation plan

1. Packet semantics: enumerate flags byte meanings via SWGEmu source
   history (client-side decomp references in engine3 / published docs);
   determine whether position offset is encoded anywhere.
2. Scale behavior: test matrix -- does final rendered size =
   client-slider x server-float? Find any existing server code varying
   scale (grep 1.0f alternates) and community findings.
3. Color: confirm client respects packet RGB for arbitrary strings/colors
   (evidence: existing red/green hits) and whether custom StringId text is
   honored (server can send arbitrary stf file/entry -- test with custom
   string table entry or raw numeric text).
4. Content limits: does the client require valid stf entries, or can
   arbitrary ASCII be embedded (some clients render the aux string raw)?
5. Position/effects: search client script/UI dumps for flytext placement
   logic; assess whether off-center rendering is flag-driven or
   hardcoded; whether shake/wobble is possible (likely NOT without client
   mod -- document honestly).
6. Hook design: where in our mod to intercept -- CombatManager sends
   directly; our hook options are (a) override/extend via custom
   CombatManager fork points, (b) post-process broadcast in our command
   layer, (c) Lua DirectorManager hooks. Recommend lowest-risk injection.

## Deliverables

- [ ] Feasibility report: per-requirement verdicts (YES/PARTIAL/NO) with
      evidence cites.
- [ ] Flag-byte map + scale/color/content findings captured into
      customskills/CODE_REFERENCE.md (R6.9 discovery capture).
- [ ] If feasible: implementation design brief (hook point, config knobs:
      enable toggle, size multiplier range, per-damage-type colors,
      big-hit threshold + scale boost).
- [ ] Honest statement of anything requiring client modification (out of
      scope by definition).
- [ ] Commit `[BRIEF-032]`, push.
