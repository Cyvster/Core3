# BRIEF-014 -- Remove the combat spam label feature from code and documentation

- Status: DELIVERED
- Created: 08242026 by ox-alpha (opencode/x-preview-f-free), owner
  directive. Supersedes BRIEF-010/013 enhancement plans: same-line labels
  are not achievable without breaking player client settings (BRIEF-010
  Phase 1 evidence), and the owner has elected full removal.

## Remove from module C++

1. `combat/CustomSkillsCombat.cpp::applyDamage`: delete the entire label
   emission block (`isCombatSpamLabelsEnabled` check,
   `getCriticalCombatSpamLabel`, `sendCustomCombatSpam` call). The crit
   damage multiplier logic above it stays untouched.
2. `CustomSkillsConfig.h/.cpp`: remove
   - `combatSpamLabelsEnabled` member, getter, default, and load line
   - `criticalCombatSpamLabel` member + getter and its load line
   - the `modifierCombatSpamLabels[]` array, its per-modifier load in
     `loadModifier`, `setDefaults` initialization, and
     `getModifierCombatSpamLabel()`
3. **SWGEmu patch files**: remove `sendCustomCombatSpam` from
   `CreatureObject.idl` + `CreatureObjectImplementation.cpp` (module-
   added via hooks; no other callers). Regenerate
   `integration/core3-hooks.patch`.

## Remove from config.lua

- Root field `combatSpamLabelsEnabled`
- Every `combatSpamLabel = "(...)"` entry (criticalChance +
  doubleAttackChance/tripleAttackChance/quadAttackChance)

## Remove from documentation

1. `docs/customskills/CODE_REFERENCE.md` Appendix A: drop the combat-spam
   property row from every modifier section; note in the Critical Chance
   history paragraph that the label feature was removed (owner decision
   after BRIEF-010).
2. `docs/customskills/USER_GUIDE.md`: remove all four "Combat spam"
   bullet lines.
3. `docs/installation/INSTALLATION.md`: remove
   `combatSpamLabelsEnabled` from the root-settings example and the
   separate-message clause in the verification step.

## Acceptance criteria

- Repo-wide grep clean: `combatSpamLabel`, `CombatSpamLabel`,
  `sendCustomCombatSpam`, `combatSpamLabelsEnabled` return zero hits in
  module code/config/docs (errata/archive historical records excepted).
- Crit damage math unchanged -- only the label emission disappears.
- Patch regenerated; MANIFEST integration list updated if file set
  changed.
- Single commit tagged `[BRIEF-014]`, pushed. Compile deferred to Docker
  build env (documented).

## Out of scope

- ERR-009 (Double/Triple/Quad implementation-vs-removal) -- separate
  owner decision in progress; this brief does not touch their config
  entries or menu rows beyond the spam-label lines named above.

---

## Execution addendum -- 08242026 by ox-alpha (opencode/x-preview-f-free): precise removal map

Verified inventory at time of writing. Execute top to bottom; every item
is a deletion or simplification, no new functionality.

### 1. Module C++

**`combat/CustomSkillsCombat.cpp`** -- inside `applyDamage`, delete the
entire label block from the crit branch (keep the crit chance roll and
the `damage` multiplier lines above it):

```cpp
			CustomSkillsConfig* config = CustomSkillsConfig::instance();
			if (config->isCombatSpamLabelsEnabled()) {
				const String& label = config->getCriticalCombatSpamLabel();
				if (!label.isEmpty())
					creo->sendCustomCombatSpam(UnicodeString(label), 11);
			}
```

**`CustomSkillsConfig.h`** -- remove:
- `bool combatSpamLabelsEnabled;` member + `isCombatSpamLabelsEnabled()` getter
- `int criticalChanceFallbackBonus` stays; remove
  `String criticalCombatSpamLabel;` member and
  `getCriticalCombatSpamLabel()` getter
- `String modifierCombatSpamLabels[...]` array member and
  `getModifierCombatSpamLabel()` declaration

**`CustomSkillsConfig.cpp`** -- remove:
- `setDefaults()`: `modifierCombatSpamLabels[i] = "";`,
  `criticalCombatSpamLabel = "(CRIT)";`
- `loadModifier()`: the
  `modifierCombatSpamLabels[type] = modifier.getStringField("combatSpamLabel", ...)` line
- `load()`: the critical-section spamLabel read block
  (`String spamLabel = ...` through the isEmpty check)
- `getModifierCombatSpamLabel()` implementation

**`CustomSkillsMenu.cpp`** -- remove:
- `countEnabledOptions()`: the
  `if (config->isCombatSpamLabelsEnabled()) ++count;` line
- SERVER_CONFIG prompt: the `String spamState = ...` line and the
  `summary << spamState << "\\#. Combat Spam Labels" << endl;` line
- MOD_OPTIONS page items: the
  `box->addMenuItem("Combat Spam Labels " + spamState + "\\#.");` line

### 2. SWGEmu patch files (minimal-touch additions being rolled back)

**`CreatureObject.idl`**: delete the
`public native void sendCustomCombatSpam(final unicode customString, byte color);`
declaration (module-added; nothing else uses it).
**`CreatureObjectImplementation.cpp`**: delete the
`sendCustomCombatSpam` native implementation body.
Regenerate `integration/core3-hooks.patch` afterwards so both files
drop out of it.

### 3. config.lua

- Root field: `combatSpamLabelsEnabled = true,` (+ its explanatory
  comment lines)
- Per-modifier: all four `combatSpamLabel = "(...)"` lines
  (criticalChance, doubleAttackChance, tripleAttackChance,
  quadAttackChance)

### 4. Documentation

- `docs/installation/INSTALLATION.md`: remove the
  `combatSpamLabelsEnabled = true` root-settings example lines, the
  `combatSpamLabel = "(CRIT)",` example line, the four per-modifier
  one-liner examples that include `combatSpamLabel = "(DOUBLE)/(TRIPLE)/(QUAD)"`,
  and the verification-step clause "With combatSpamLabelsEnabled = true,
  crits emit a separate (CRIT) combat message".
- `docs/customskills/USER_GUIDE.md`: delete the four
  `- **Combat spam**: ...` bullet lines (Crit/Double/Triple/Quad cards).
- `docs/customskills/CODE_REFERENCE.md`: remove Appendix A property rows
  `| **Combat spam label** | (DOUBLE) |` / `(TRIPLE)` / `(QUAD)`, the
  API mention `getModifierCombatSpamLabel()`, the
  "`combatSpamLabel` (repeat-damage only)." fragment, and the
  "| Combat spam labels | Native colors untouched |" row.
- LEAVE UNTOUCHED (historical records): docs/archive/*, delivered briefs
  008/010/013/015 content, errata.md entries.
- CHALLENGE_TIER_SKILLS.md mention of mitigate_damage combat-spam chat
  line describes NATIVE engine behavior for an excluded mod -- leave.

### 5. Verification

1. Repo-wide grep returns zero for: `combatSpamLabel`,
   `CombatSpamLabel`, `sendCustomCombatSpam`, `combatSpamLabelsEnabled`
   -- excluding archive/, delivered briefs 010/013/015 text, and this
   brief's own instructions.
2. Brace balance intact in every touched .cpp/.h (count { vs }).
3. Compile deferred to Docker build environment (engine3 submodule
   caveat) -- document in delivery note.
4. Commit tagged `[BRIEF-014]`, pushed ([PROC R6.5]).

---

## Delivery Report

- Executor: hy3-free (opencode/x-preview-f-free), 08242026. C++/config
  removals executed via OpenCode CLI delegation (ox-alpha free model) with
  independent verification; docs sweep delegated the same way.
- Module C++: label emission block removed from
  `combat/CustomSkillsCombat.cpp::applyDamage` (incl. orphaned-brace repair
  after an interrupted first pass); all members/getters/load lines for
  `combatSpamLabelsEnabled`, `criticalCombatSpamLabel`,
  `modifierCombatSpamLabels[]` removed from `CustomSkillsConfig.h/.cpp`;
  menu rows + countEnabledOptions reference removed from
  `CustomSkillsMenu.cpp`.
- SWGEmu patch files: `sendCustomCombatSpam` declaration removed from
  `CreatureObject.idl`; native implementation body removed from
  `CreatureObjectImplementation.cpp`. Patch regeneration NOT required:
  grep of `integration/core3-hooks.patch` shows it never contained a
  `sendCustomCombatSpam` hunk or a `CreatureObject.idl` entry (the
  module-added method predated patch capture; MANIFEST integration list
  unchanged).
- config.lua: root `combatSpamLabelsEnabled` (+ comments) and all four
  per-modifier `combatSpamLabel` entries removed. Verified with Lua 5.1:
  `assert(loadfile(...))` parses clean.
- Docs: CODE_REFERENCE.md (Appendix A spam-label property rows,
  getModifierCombatSpamLabel mention, generic-fields fragment, color-scheme
  row, dev-labels sentence, signed History note, stamp + Contributors),
  USER_GUIDE.md (four Combat spam bullets, stamp + Contributors),
  INSTALLATION.md (root-settings examples, per-modifier one-liners,
  verification clause). Zero `combatSpam` matches remain in all three.
- Acceptance verified [PROC R6.10]: repo-wide grep for
  `combatSpamLabel|CombatSpamLabel|sendCustomCombatSpam|combatSpamLabelsEnabled`
  returns ZERO hits outside archive/, delivered briefs 008/010/013/015,
  errata records, and this brief. Brace balance confirmed on every touched
  .cpp/.h. Compile DEFERRED to Docker build environment (engine3 submodule
  caveat) per brief; deletions only, no new code paths.
- Commit tagged `[BRIEF-014]`, pushed ([PROC R6.5]).
