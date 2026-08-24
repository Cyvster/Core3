# Installation and Removal

These instructions install the badge browser and the configuration-backed
modifier framework. All non-critical modifiers ship disabled with empty badge
lists, so installing this version preserves native behavior until configured.

## Install

1. Copy `package/MMOCoreORB/` over the target checkout's `MMOCoreORB/`
   directory. This adds the C++ module files, command script, and
   `scripts/customskills/config.lua`.
2. From the target Core3 repository root, validate and apply the supplied hook
   patch:

   ```bash
   git apply --check --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
   git apply --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
   ```

   The patch contains only the command-registration and generic delegation
   edits to existing Core3 files. If `--check` fails because the target Core3
   revision differs, stop and port those hunks manually; do not force the patch.
   `MANIFEST.md` lists every expected integration file, and
   `docs/MODIFIER_REFERENCE.md` describes the purpose and ordering of every hook.
3. Reconfigure CMake so its recursive source glob sees the new `.cpp` files,
   build Core3, deploy the binary and scripts, and restart the server.

## Configure

Edit `MMOCoreORB/bin/scripts/customskills/config.lua` before starting Core3.
Each modifier has an independent configuration table. Percentage values use
basis points, where `100 = 1.00%`.

Critical Chance defaults:

```lua
criticalChance = {
    enabled = true,
    badgeBonus = 300,
    multiplier = 15000,
    combatSpamLabel = "(CRIT)",
}
```

The root configuration sets `customSummaryColor = "00FF00"` for modifier text
in the menu. Native combat-spam colors are not overridden. The separate proc
labels are temporary development feedback.

Set `enabled = false` to remove the bonus from combat and display it as
disabled in the menu. Configuration is cached for combat performance; restart
Core3 after changing the file. Missing or invalid values use safe defaults and
produce a server-log warning.

Generic modifier tables accept `enabled`, `badgeBonus`, `cap`, and `badges`.
The three repeat-damage tables also accept temporary `combatSpamLabel` text.
Badge names must be BadgeList keys. Percentages are basis points; Armor
Penetration, Defense Cap Increase, and SEA Cap Increase use whole levels or
points. Do not assign bonuses to unobtainable, event-disabled, or JTL/pilot
badges unless those systems are actually available on the target server.

## Verify

1. Confirm startup reports no error while loading `customSkills.lua` or adding
   `CustomSkillsCommand`.
2. Log in with a normal player character and enter `/customskills`.
3. Open every category, use Back, use Cancel, reopen the command repeatedly,
   and test two characters at the same time.
4. Confirm each acquired ground combat mastery badge adds 3.00% Critical
   Chance in the menu, up to 36.00% for all twelve badges.
5. Compare repeated attacks with and without combat mastery badges and confirm
   critical attacks deal 150% of their otherwise identical pre-armor damage.
6. Before enabling another modifier, follow its focused tests in
   `docs/IMPLEMENTATION_GUIDE.md`. In particular, test movement on foot, mounted,
   and in vehicles; buff activation and renewal; factory stop/restart; both
   amazing-result phases; client-visible SEA values; forage; and milk.

## Remove

1. Remove the module files under
   `MMOCoreORB/src/server/zone/managers/customskills/`.
2. Remove `MMOCoreORB/bin/scripts/commands/customSkills.lua`.
3. Remove `MMOCoreORB/bin/scripts/customskills/config.lua`.
4. Reverse the supplied integration patch from the repository root:

   ```bash
   git apply --check --reverse --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
   git apply --reverse --ignore-space-change /path/to/customskills-mod/integration/core3-hooks.patch
   ```

   If the reverse check fails because the integration files were subsequently
   edited, remove the Custom Skills hunks manually using `MANIFEST.md`; do not
   force the reverse patch.
5. Reconfigure, rebuild, deploy, and restart Core3.
