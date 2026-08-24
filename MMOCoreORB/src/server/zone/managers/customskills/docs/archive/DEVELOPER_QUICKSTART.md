# Custom Skills — Developer Quickstart

> Subordinate to [../rules/project-design.md](../rules/project-design.md) -- the master document for project rules.

Minimal steps to set up, understand, and start contributing to the Custom Skills module.

## Contributors

- **Nemotron 3.5 Lightning Free (AI)** — Initial creation

---

## 5-Minute Setup

### 1. Get the Code
```bash
# This repo is the distributable module
cd customskills-mod

# Development happens in ../Core3 (separate checkout)
ls ../Core3   # Should exist
```

### 2. Install on Clean Core3 (Verification Build)
```bash
cd ../Core3

# Copy module files
cp -r ../customskills-mod/package/MMOCoreORB/ MMOCoreORB/

# Apply integration patch
git apply --check --ignore-space-change ../customskills-mod/integration/core3-hooks.patch
git apply --ignore-space-change ../customskills-mod/integration/core3-hooks.patch

# Build
cmake -B build && cmake --build build --target MMOCoreORB -j$(nproc)
```

### 3. Run & Test
```bash
# Deploy binary + scripts, start server
# In-game:
/customskills
```
Verify: Menu opens, shows "Accumulated Bonuses", "Badges" category, navigation works.

---

## 15-Minute Code Tour

### Start Here (Core Files)

| File | Purpose | Time to Read |
|------|---------|--------------|
| `CustomSkillsModifierType.h` | 17 modifier enum | 1 min |
| `CustomSkillsConfig.h/.cpp` | Config loading, defaults | 5 min |
| `CustomSkillsModifiers.h/.cpp` | Central modifier API | 5 min |
| `MANIFEST.md` | All files + integration points | 2 min |

### Key Services (Hook Targets)

| Service | File | Modifiers |
|---------|------|-----------|
| Combat | `combat/CustomSkillsCombat.h` | Crit, repeat-damage, armor pen, defense cap |
| Durability | `durability/CustomSkillsDurability.h` | Armor/weapon degrade reduction |
| Progression | `progression/CustomSkillsProgression.h` | XP multiplier |
| Crafting | `crafting/CustomSkillsCrafting.h` | Speed, amazing chance/results, practice XP |
| Buffs | `buffs/CustomSkillsBuffs.h` | Buff duration |
| Movement | `movement/CustomSkillsMovement.h` | Client + server speed |
| SkillMods | `skillmods/CustomSkillsSkillMods.h` | SEA cap, visible mods |
| Gathering | `gathering/CustomSkillsGathering.h` | Forage/milk quantity |

### Integration Patch
```
integration/core3-hooks.patch   # 19 files, 719 lines
```
Each hunk = small delegation call. Read one to understand the pattern.

---

## Common Tasks

### Enable a Disabled Modifier
```lua
-- MMOCoreORB/bin/scripts/customskills/config.lua
modifiers = {
    doubleAttackChance = { enabled = true, badgeBonus = 2000, cap = 10000,
                           combatSpamLabel = "(DOUBLE)", badges = {
                               "warren_compassion", "warren_hero",
                               "bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge",
                               "bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge"
                           }},
    -- ...
}
```
**Restart server** after config change.

### Add a New Badge to Existing Modifier
```lua
-- In config.lua, add to modifier's badges list:
badges = {
    "existing_badge_key",
    "new_badge_key",    -- Must exist in BadgeList (check badge_map.iff)
}
```

### Add a New Modifier (Full Checklist)
See `ARCHITECTURE.md` → "Adding a New Modifier" section.

---

## Documentation Map

| Need | Read |
|------|------|
| Install/configure | `../installation/INSTALLATION.md` |
| Player mechanics | `../user-guide/USER_GUIDE.md` |
| Project big picture | `PROJECT_OVERVIEW.md` |
| Design principles | `ARCHITECTURE.md` |
| All 17 modifiers | `MODIFIER_REFERENCE.md` |
| Hooks & build | `IMPLEMENTATION_GUIDE.md` |
| Menu/SUI details | `MENU_SYSTEM.md` |
| Badge catalog | `BADGE_REFERENCE.md` |

---

## Key Conventions

| Convention | Details |
|------------|---------|
| **Basis points** | 100 = 1.00% for percentages; 10000 = 1.00x for multipliers |
| **Whole units** | Armor Penetration (levels), Defense Cap/SEA Cap (points) |
| **Stable badge keys** | Never hard-code indices; use `BadgeList::get(key)` |
| **Single authority** | `CustomSkillsModifiers` serves gameplay + menu |
| **No new persistence** | Reads existing badge bitmasks on `PlayerObject` |
| **Config cached** | Restart required after `config.lua` changes |
| **Minimal Core3 edits** | Only generic delegation hooks at stable boundaries |

---

## Debugging Quick Reference

| Symptom | Check |
|---------|-------|
| Modifier not working | `isModifierEnabled(type)` in config; badge keys exact match |
| Menu ≠ combat values | Both use `CustomSkillsModifiers::getModifierTotal()` |
| Factory speed stuck | Activator snapshot at start; stop/restart factory |
| Movement desync | H13A (client) & H13B (server) use same `getSpeed()` |
| SEA cap not visible | H14B refreshes on badge change; `refreshVisibleSkillMods()` |
| Buff not extended | Eligibility classifier in `CustomSkillsBuffs` (CRC allowlist) |

---

## Testing Before Commit

```bash
# 1. Clean build verification
cd /tmp && git clone <core3-repo> Core3-test
# Install, patch, build, run

# 2. Run verification matrix (IMPLEMENTATION_GUIDE.md)
# - Disabled modifier = native behavior
# - Menu values = runtime values
# - Movement sync all travel modes
# - Factory survives restart
# - SEA server = client visible
# - Forage discrete loot not duplicated

# 3. Update docs if behavior changed
# - MODIFIER_REFERENCE.md status
# - MANIFEST.md if files added/removed
# - INSTALL.md if integration changed
```

---

## Git Workflow

- **Development branch**: `cyvster3`
- **Before deleting branch**: Tag completed module, install on clean integration branch, verify
- **Package** (`package/`) must remain installable without Git history
- **MANIFEST.md** + **INSTALL.md** updated with every material change

---

## Core3 Guide Errata Process

**If you find a factual error in `CORE3_CODE_REFERENCE.md` or
`CORE3_USER_GUIDE.md`:**

1. **Do not fix it directly** — these guides require two-party verification
2. **File an ERR entry in `../rules/errata.md`** using the entry template
3. **Wait for a different contributor** to verify independently
4. **Only after verification** → the verifier applies the fix and marks it RESOLVED

This prevents hallucinated APIs/constants from entering the guides.
Historical note: the retired `SWGEMU_CODE_GUIDE.md` used its own log
(`archive/CODE_GUIDE_ERRATA.md`); that procedure is superseded by
`../rules/errata.md`.

---

## Questions? Start Here

| Question | Answer Location |
|----------|-----------------|
| How does X modifier work? | `MODIFIER_REFERENCE.md` |
| Where is Y hook in Core3? | `IMPLEMENTATION_GUIDE.md` hook table |
| What badges grant Z? | `BADGE_REFERENCE.md` or `MENU_SYSTEM.md + BADGE_REFERENCE.md` |
| How to add menu page? | `MENU_SYSTEM.md` → "Adding a New Menu Page" |
| Config not reloading? | Restart server — config is cached |
| Patch won't apply? | Port hunks manually per `MANIFEST.md` |
| Found error in a Core3 guide? | File ERR in `../rules/errata.md` (two-party verification required) |
