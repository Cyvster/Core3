# BRIEF-039 integrity audit -- checklist (08252026)

Auditor: ox-alpha (opencode/x-preview-f-free). READ-ONLY audit; method per
briefs/039-integrity-audit-post008.md. All cites against branch cyvster3 @
201f797421. Findings detail: findings.md.

## Scope commits examined

| Commit | What |
|---|---|
| 087cf01aae | BRIEF-034 consolidated strike + tiered FCT + chat tag |
| b273b193ae | BRIEF-027-impl mod-overrides.lua overlay hook |
| 0ddf57c51d | BRIEF-036 repeat-craft assisted pre-fill |
| 08716104cd | hotfix: global-ns fwd decls -> real includes |
| 201f797421 | hotfix: qualify GENERALERROR/SUCCESS as QueueCommand:: |
| (context)   | CustomSkillsMenu.cpp viewer pages, CustomSkillsConfig.cpp/.h, config.lua |

## Checks performed

1. **Spec conformance**
   - [x] BRIEF-034 re-read vs CustomSkillsCombat.cpp delivery
     - multiplier placement verified: crit roll first (combat .cpp:55-61),
       then `damage *= repeats` (:85) BEFORE the single applyVanillaDamage
       (:87-88) whose body performs armor mitigation (CombatManager.cpp:1483+
       getArmorReduction). Matches spec items 1-2. ERR-009 follow-up recorded
       (errata.md:425-432).
     - FCT scale math matches bp spec (:97-99); colors config-driven; flags
       byte 5 confirmed in ShowFlyText.h.
     - DELTA: flytext audience -- spec item 5 says broadcast like vanilla;
       delivery sends to ATTACKER ONLY. -> F-01 (DEFECT).
     - DELTA: chat tag delivered as second spam line (brief's preferred
       option) using the verified unicode CombatSpam ctor (CombatSpam.h:44).
       Conformant.
     - NOT DELIVERED, undocumented deviation: two-size flytext pair was
       conditional ("if stf shipping accepted") -- acceptable omission.
   - [x] BRIEF-036 re-read vs RepeatCraftCommand.h + CustomSkillsCrafting.cpp +
       CraftingSession.idl/-Impl.cpp delivery
     - snapshot on tool via luaStringData cs36.* (TangibleObject.idl:49,
       :1011-1023) -- lowest-risk storage documented. Conformant w/ item 1
       except: customization string never captured; experiment string
       captured but NEVER CONSUMED. -> F-04 (DEFECT).
     - /repeatcraft command + registration both sides (special-command
       creation CommandConfigManager.cpp:354-358; factory
       CommandConfigManager3.cpp:346) mirrors proven /customskills pattern.
       No collision: name absent from client command tables; commands.lua
       entry matches customSkills.lua precedent.
     - re-validation + discard-with-notice implemented (:239-289 area).
     - practice gating matches owner directive (Impl.cpp:1420-1422).
     - repeatEnabled default false in code defaults + config.lua. Conformant.
     - DELTAS: nested-container scan gap (F-02), partial-quantity silent
       underfill (F-03) -- see below.
   - [x] BRIEF-027-impl vs ConfigManager.cpp hook: loads mod-overrides.lua
       into the SAME Lua state after config-local.lua, single parse pass --
       matches example-file header claims. Sparse/no-op when file absent.
       Secrets excluded per example header. Conformant.

2. **Cross-feature interactions**
   - [x] fctEnabled=false + chatTagEnabled=true: chat tag DOES fire; the two
     knobs gate independently (CustomSkillsCombat.cpp:95 vs :114). Config
     comments document them as separate fields -> judged INTENTIONAL, not a
     defect. Recorded as verified behavior.
   - [x] repeat-craft snapshot vs overlay hot-reload: snapshot path never
     writes ConfigManager state; only reads
     CustomSkillsConfig::isRepeatPracticeAllowed() (Impl.cpp:1420). Tool
     luaStringData is orthogonal to ConfigManager reloads. SAFE.
   - [x] repeat-craft ordering vs Crafting Speed / Amazing Success: speed
     applied downstream in startCreationTasks
     (CraftingSessionImplementation.cpp:1445); Amazing Results apply during
     assembly/experimentation which repeat does not automate (stops at
     state 2). Ordering sane; no double-application.
   - [x] menu SWGEMU options viewer vs overlay-forced values: viewer does
     live ConfigManager::getBool/getInt/getString per render
     (CustomSkillsMenu.cpp:260-275), so displayed value == effective value
     including overlays. Restart-tagged entries carry "(restart required)"
     suffix. Conformant. (Stale sibling text: F-05.)
   - [x] command registration collisions: none (see 1). Client lua file
     present alongside customSkills.lua precedent.

3. **Copy-paste/artifact scan**
   - [x] cs36.* prefix consistent everywhere (single CS36_PREFIX const);
     key format documented in code header block + CODE_REFERENCE.md:1632.
   - [x] error messages: plain-sentence style for repeat-craft vs @stf for
     tool-state cases -- inconsistent register but functional; noted in
     findings table (SMELL, folded into F-06 notes) -- not filed separately.
   - [x] STALE ARTIFACT FOUND: SERVER_CONFIG prompt branch still prints
     "--- SWGEMU Options ---" + "No options configured yet."
     (CustomSkillsMenu.cpp ~:314-317) although the real SWGEMU_OPTIONS page
     works -- leftover from pre-BRIEF-031 design. -> F-05 (SMELL).
   - [x] stale fwd-decl class from the escaped compile break: swept; no
     global-namespace forward decls of IDL classes remain in mod headers.
   - [x] STALE DOC FOUND: USER_GUIDE.md:109 still says Double/Triple/Quad
     "not yet implemented server-side ([ERR-009])" while the same file's
     line 71 documents the delivered mechanic. -> F-07 (DOC-MISMATCH).
   - [x] STALE DOC: USER_GUIDE.md:115 "Config changes -- require server
     restart" is now false for [dyn] mod-overrides keys after BRIEF-027-impl.
     -> F-08 (DOC-MISMATCH).
   - [x] snapshot discard deletes only cs36.schematicCrc, orphaning
     slot.*/exp keys; storeRepeatRecipe never prunes keys beyond the new
     slotCount. -> F-06 (SMELL).

4. **Quality pass**
   - [x] null guards: defender guarded for FCT; tool/session/inventory
     guarded in doRepeatCraft; parseRgb length-guarded; config loader
     validates ranges + warns. OK.
   - [x] locks: doRepeatCraft hand-rolls lock choreography (Locker(tool)
     held across initializeSession/selectDraftSchematic/addIngredient;
     explicit Locker(oldSession)->cancelSession; Locker(session) before
     cancel). Re-lock of tool inside selectDraftSchematic is recursive-safe,
     but the pattern diverges from the vanilla session-task flow and mixes
     lock orderings (tool -> session, ingredient -> player). No proven
     deadlock; flagged. -> F-09 (SMELL).
   - [x] hot-path smells: consolidated-strike path adds 3 modifier-table
     lookups + config reads per landed player hit -- same order as the
     pre-034 repeat-chain code (which also did these rolls), no regression
     vs BRIEF-033 numbers. Viewer/menu code unchanged cost profile.
   - [x] partial-fill semantics of ResourceSlot::add (ResourceSlot.h:62-80)
     interact badly with the single-call pre-fill loop -> F-03 (DEFECT).

5. **Docs-vs-code spot checks**
   - [x] CODE_REFERENCE consolidatedStrike table (:736-753) matches config
     keys, defaults, and load() field names exactly.
   - [x] CODE_REFERENCE repeat-craft section (:1617-1671) matches code incl.
     IDL accessor names and window-flow caveat (honest limitation note).
   - [x] config.lua self-documenting blocks present for consolidatedStrike
     + repeatCraft; REAL EXAMPLE blocks commented out per house standard.
   - [x] CONFIG_OPTIONS.md: all 19 viewer keys traced to real consumers
     (spot-verified GalaxyWideGrouping/WipeFillingOnClone/
     DisableGroupVisibility/AdvancedWaypoints/EnableTutorial et al.);
     accountVictimList naming gotcha retained.
   - [x] MIGRATION_LEDGER.md status snapshot internally consistent
     (IN-MOD 4 / TODO 30 / WONT-PORT 14); missions quality column present
     per BRIEF-037/038.
   - [x] errata.md: ERR-009 BRIEF-034 follow-up appended with owner-caught
     armor-mitigation correction. Accurate.

## Verdict

No CRITICAL findings. Server-breaking risk contained to the two hotfixes'
root causes (declaration hygiene, out-of-class enum scope), both closed.
Remaining defects are behavioral, concentrated in the repeat-craft pre-fill
loop and FCT audience; docs drifted in three places. Batch quality: solid
structure/config discipline, but delegation-worker scope-tracking slips are
visible (unused snapshot field, stale menu text, stale guide lines).
