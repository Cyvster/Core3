# BRIEF-039 integrity audit -- findings (08252026)

Auditor: ox-alpha (opencode/x-preview-f-free). READ-ONLY; branch cyvster3 @
201f797421. Severity ladder per briefs/039: CRITICAL / DEFECT / SMELL /
DOC-MISMATCH. Dispositions: ERR-OPEN (filed), REMEDIATION-BRIEF (cluster),
ACCEPTED (documented owner deviation), NO-ACTION.

## Findings table

| ID | Sev | Area | Summary | Cite | Disposition |
|----|-----|------|---------|------|-------------|
| F-01 | DEFECT | BRIEF-034 FCT | Tiered flytext sent to ATTACKER ONLY, spec says broadcast to attacker + defenders-of-record like vanilla hit flytext | combat/CustomSkillsCombat.cpp:105-106 (`creo->sendMessage(fly)`, creo = attacker); brief 034 item 5 | ERR OPEN |
| F-02 | DEFECT | BRIEF-036 pre-fill | Resource/component search scans only the player's TOP-LEVEL inventory container; resources/components in backpacks, containers, or the crafting satchel are invisible to repeat-craft, while vanilla crafting finds them | crafting/CustomSkillsCrafting.cpp:262-272 + :288-303 (single `getSlottedObject("inventory")` loop); contrast IngredientSlot::add paths accepting any sub-child of crafter (CraftingSessionImplementation.cpp:601-605) | ERR OPEN |
| F-03 | DEFECT | BRIEF-036 pre-fill | Partial quantity: a container holding LESS than required is consumed entirely and slot stays unfilled with NO "insufficient" message -- doRepeatCraft checks only `getQuantity() > 0`, then add() silently drains it; brief requires naming which resource is short | CustomSkillsCrafting.cpp:270 (`rc->getQuantity() > 0` guard, no `>= qtyNeeded` check); ResourceSlot.h:62-80 drain behavior vs brief 036 item 3 | ERR OPEN |
| F-04 | DEFECT | BRIEF-036 snapshot | Experimentation allocation captured (`cs36.exp`) but NEVER read back by doRepeatCraft; customization string never captured at all. Brief item 6 ("auto-apply snapshot allocations") undelivered in both halves | snapshot write: CraftingSessionImplementation.cpp:1097-1099 + CustomSkillsCrafting.cpp:172 (`cs36Put(tool, "exp", ...)`); zero reads of `"exp"` anywhere; no customization key exists | ERR OPEN |
| F-05 | SMELL | BRIEF-025/031 menu | SERVER_CONFIG prompt branch still renders dead text "--- SWGEMU Options --- / No options configured yet." superseded by the working SWGEMU_OPTIONS page from BRIEF-031 | CustomSkillsMenu.cpp:314-317 (else branch of getPromptText) vs appendSwgemuOptions (:237+) | REMEDIATION-BRIEF (menu cleanup cluster) |
| F-06 | SMELL | BRIEF-036 keys | Discard path deletes only cs36.schematicCrc, orphaning slot.* / exp keys on the tool until overwritten; storeRepeatRecipe never clears stale higher-indexed slot keys when new schematic has fewer slots | CustomSkillsCrafting.cpp:249 + :281 (`deleteLuaStringData(CS36_PREFIX + "schematicCrc")` only); write loop :139-166 | ERR OPEN |
| F-07 | DOC-MISMATCH | USER_GUIDE | Guide line still claims Double/Triple/Quad "not yet implemented server-side ([ERR-009])" while line 71 documents the delivered consolidated strike | docs/customskills/USER_GUIDE.md:109 vs :71; code combat/CustomSkillsCombat.cpp:85 | ERR OPEN |
| F-08 | DOC-MISMATCH | USER_GUIDE | "Config changes -- require server restart" now false for [dyn] mod-overrides.lua keys after BRIEF-027-impl hot reload | docs/customskills/USER_GUIDE.md:115 vs ConfigManager.cpp:49-60 overlay load + configVersion auto-reload (ConfigManager.h:180-195) | ERR OPEN |
| F-09 | SMELL | BRIEF-036 locking | Hand-rolled lock choreography in doRepeatCraft (Locker(tool) across session init/select/fill; Locker(oldSession)->cancelSession; re-lock of tool inside selectDraftSchematic) mixes lock orderings vs vanilla flow; no proven deadlock but fragile under worker maintenance | CustomSkillsCrafting.cpp:203-207, :226-228, :251-253; vanilla selectDraftSchematic locks tool internally (CraftingSessionImplementation.cpp:361-363) | REMEDIATION-BRIEF (repeat-craft hardening cluster w/ F-02/F-03/F-04/F-06) |
| F-10 | ACCEPTED | BRIEF-034 math | Single big hit vs N small hits armor interaction: identical because every mitigation stage is multiplicative percentage (owner-caught correction already recorded) | errata.md:429-432; CombatManager.cpp getArmorReduction call sites :1483/:1518/:1552 | ACCEPTED (recorded) |

## Verified-clean items (no finding)

- fctEnabled=false + chatTagEnabled=true: tag fires independently; both knobs
  documented as separate fields -> intentional. CustomSkillsCombat.cpp:95/:114.
- Snapshot path never touches ConfigManager state -> overlay hot-reload cannot
  corrupt an in-progress snapshot. CraftingSessionImplementation.cpp:1420.
- Repeat ordering vs Crafting Speed / Amazing Success: speed applies later in
  startCreationTasks (:1445); amazing results live in assembly/experiment
  phases repeat does not automate. Sane.
- SWGEMU options viewer reads live values each render -> display always equals
  effective behavior incl. overlay-forced values. CustomSkillsMenu.cpp:260-275.
- No /repeatcraft registration collision; both server-side registrations mirror
  the proven /customskills pattern; commands.lua entry present.
  CommandConfigManager.cpp:354-358; CommandConfigManager3.cpp:346;
  bin/scripts/commands/repeatCraft.lua.
- Crit-then-repeats order matches spec (crit multiplier first, repeats multiply
  on top). CustomSkillsCombat.cpp:55-61, :85.

## Counts

CRITICAL 0 | DEFECT 4 (F-01..F-04) | SMELL 3 (F-05, F-06, F-09) |
DOC-MISMATCH 2 (F-07, F-08) | ACCEPTED 1 (F-10). Total filed: 9 actionable +
1 accepted-deviation record.

## Proposed remediation clusters (do NOT fix inline)

1. **BRIEF-040 (proposed): repeat-craft hardening** -- F-02 + F-03 + F-04 +
   F-06 + F-09. One coherent pass over doRepeatCraft/storeRepeatRecipe:
   deep container traversal (reuse IngredientSlot parent-search semantics),
   require `quantity >= qtyNeeded` before consuming (or aggregate multiple
   containers), consume-or-skip decision for partial stacks, either wire
   cs36.exp into post-select experiment pre-fill or stop capturing it,
   prune full key family on discard, and replace hand-rolled lock sequence
   with the vanilla RequestCraftingSessionCommand-shaped path.
2. **BRIEF-041 (proposed): presentation fixes** -- F-01 alone is small:
   broadcast tier flytext to defender's observers (mirror showHitLocationFlyText
   audience, CombatManager.cpp:1614-1616) instead of attacker-only sendMessage.
3. **DOC pass (can ride any commit)** -- F-07, F-08, F-05: two USER_GUIDE
   corrections + delete the dead SERVER_CONFIG else-branch text.

Signed: ox-alpha (opencode/x-preview-f-free), 08252026.
