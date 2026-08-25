# Brief 032 Findings -- Floating Combat Text (ShowFlyText) feasibility

Researched 08252026 by ox-alpha. Research-only; no code modified.
All cites are to this tree (MMOCoreORB) unless marked [web].

## Mechanism summary

- Packet class: src/server/zone/packets/object/ShowFlyText.h
  - ObjectControllerMessage(targetID, 0x1B, 0x1BD).
  - Payload: long targetID, ascii stf FILE, int spacer, ascii stf ENTRY/AUX,
    float SCALE ("1.0 for broadcasted, 0 for none" -- comment line 20),
    byte R, byte G, byte B, byte FLAGS (hardcoded 5, line 31).
  - Flags comment (lines 25-30): 0x0001 only-shown-on-target,
    0x0002 show-in-chat?, 0x0004 unknown. Value 5 = 0b101 (1|4).
- Server wrapper: SceneObjectImplementation.cpp:1862 showFlyText(file, aux,
  r, g, b, isPrivate=false): public -> broadcastMessage with scale 1.0;
  private -> sendMessage directly with scale **0** (client-local render).
- IDL: SceneObject.idl:904 exposes it to Lua too.

## R1. Catalog of all usages in src/

Every call site uses default scale via SceneObject::showFlyText (implicit
1.0 public / 0 private). The ONLY explicit scale in the entire tree is
CombatManager::showHitLocationFlyText (CombatManager.cpp:2909-2943):
hit_head blue 1.0f, hit_body red 1.0f, hit_larm/hit_rarm red 1.0f,
hit_lleg/hit_rleg green 1.0f -- all sent ONLY to attacker
(attacker->sendMessage(fly), line 2942).
=> No non-1.0 scale and no flag variation exists anywhere in src/.
Flags are hardcoded 5 in the packet ctor; no caller can change them today.

Other notable users: doMiss/doCounterAttack/doBlock/doDodge white/green
(CombatManager.cpp:2865-2906); warcry/intimidate miss red (:3165,:3170);
npc_reaction/flytext (?, !, afraid, low_power...) across AI/pets/droids;
buff start/end flytexts (BuffImplementation.cpp:121,:168 -- file/aux come
from Buff template, proving dynamic stf strings work); fishing markers.

## R2. Other 0x1BD mechanisms

Only one packet builder for 0x1BD (ShowFlyText.h). SWGANH controller list
[web] also documents a separate opcode 0x45A "ShowCombatText" (NGE-era
numeric damage text) -- NOT present in this tree; our target is the preCU
14.1 client which uses ShowFlyText + CombatSpam instead.

## R3. Client-side option interaction (web)

- SWG Legends Options wiki [web] confirms client prefs: "Show Damage
  Numbers over Others / over Self" toggle visibility of pool-damage
  flytext; there is no documented per-server-size slider override --
  the numeric damage flytext on modern clients is generated CLIENT-SIDE
  from CombatSpam data, not from ShowFlyText.
- Community thread (modthegalaxy "Combat Damage numbers") [web]:
  requesters asking for larger/repositioned damage numbers were pointed
  at CombatSpam/client mods; no known server-side size or position
  control. Consistent with packet layout: no x/y/z offset field exists.

## Verdicts per owner requirement

1. SIZE scaling -- YES (server-controlled float).
   Scale is an IEEE float in the packet (ShowFlyText.h:20). Nothing in
   tree varies it, but nothing blocks it: build ShowFlyText directly with
   e.g. 2.5f. Whether final render = clientSlider x serverFloat needs a
   live test (owner matrix from brief plan #2), but the knob exists and
   is per-message. Caveat: applies to ShowFlyText stf strings; the NUMERIC
   damage flytext many clients draw from CombatSpam has NO server size knob.
2. TEXT CONTENT -- PARTIAL.
   Content must be an stf file+entry pair; no raw-text field exists in
   the packet. We CAN ship custom string tables: add entries to a custom
   .stf (e.g. customskills/fct.stf) client-side via patch distribution, or
   reuse existing tables. Numeric damage values CANNOT be sent as literal
   digits through this packet without a table entry per number (impractical)
   -- damage numbers as such are not feasible via ShowFlyText.
3. COLOR -- YES.
   RGB bytes are fully server-controlled per message; existing code sends
   distinct colors per event (blue head hits, red body, green legs, white
   miss). Per-damage-type color mapping is trivial.
4. POSITIONING -- NO.
   Packet carries no positional/offset data; flags byte has no documented
   placement bit (comment ShowFlyText.h:25-30; swganh controller docs
   [web]). Flytext anchors above the target's head, hardcoded client-side.
5. EFFECTS (shake/wobble on big hits) -- NO.
   No animation/effect field in the packet. Nearest server-side tool is
   doCombatAnimation (e.g. dodge, CombatManager.cpp:2905) which plays a
   clienteffect animation on the defender -- could fake feedback but is not
   text shake. True FCT effects need client mod: out of scope.

## Hook point recommendation

Lowest-risk injection mirrors our existing pattern: CustomSkillsCombat is
already a friend of CombatManager (CombatManager.h:21,24) and
CombatManager::applyDamage delegates to CustomSkillsCombat::applyDamage
(CombatManager.cpp:1419-1421). Recommended:

(a) Damage FCT: inside CustomSkillsCombat::applyDamage (we already own the
    post-mitigation total there), construct ShowFlyText directly (not the
    SceneObject wrapper) so we control scale/color, and send via
    attacker->sendMessage / defender broadcast as desired. Zero edits to
    core combat logic beyond what already exists.
(b) Hit-location FCT variant: showHitLocationFlyText is non-virtual const
    on CombatManager (CombatManager.h:273); since friend access already
    exists, either replicate its logic in CustomSkillsCombat or wrap calls
    -- do NOT fork CombatManager methods wholesale.
(c) Config knobs belong in CustomSkillsConfig (enable toggle, size range,
    per-type color map, big-hit threshold/scale boost).

## Requires client modification => NOT FEASIBLE server-side

- Arbitrary literal text (raw ASCII/digits) in flytext.
- Positional offsets / off-to-the-side rendering.
- Shake/wobble/any motion effect on flytext.
- Resizing the client-generated numeric damage flytext (that path is
  CombatSpam-driven client-side).

## Open items needing live test

- Final rendered size formula: client slider x server float vs. max().
- Flag bit 0x0002 (show-in-chat?) real semantics; try 0/1/4/5 variants.
- Whether scale 0 truly means "private/self-only render" vs "invisible".
