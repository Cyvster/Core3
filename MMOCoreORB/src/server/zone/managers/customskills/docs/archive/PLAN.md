# Implementation Plan

## Milestone 1: command-opened menu

The first test commit implements only the command, placeholder summary/category
pages, and navigation. Modifier definitions and badge calculations follow after
the menu shell passes an in-game test.

The menu shell has passed its initial in-game test. Badge and menu organization
is specified in `MENU_AND_BADGE_ORGANIZATION.md`; the next implementation step
is a typed registry plus one display-only badge assignment.

1. Define typed modifier IDs, categories, units, stacking rules, and badge-source
   definitions in the C++ module.
2. Implement a read-only modifier service that calculates a requested modifier
   from the invoking character's badges.
3. Implement a C++ SUI list menu with a summary page and safe submenu routing.
4. Implement `CustomSkillsCommand` and register `/customskills` through one
   special-command hook in `CommandConfigManager.cpp`.
5. Add at least one non-gameplay placeholder modifier definition to prove the
   summary and source-breakdown paths without changing combat behavior.
6. Build Core3 and inspect startup and command-registration logs.
7. Test `/customskills`, selection, cancellation, invalid selections, repeated
   opening, and simultaneous use by multiple characters.
8. Verify the standalone package matches the installed module files.
9. Validate installation and removal against a clean Core3 checkout.


## Milestone 2: first effective badge modifier

1. Specify exact semantics for the first modifier, including eligible attacks,
   stacking, cap, rounding, and presentation.
2. Add its badge-to-modifier definitions to the registry.
3. Add the smallest gameplay integration hook that queries the module's
   modifier service.
4. Make the summary and detail menu query that same service.
5. Test no-badge, one-badge, multiple-badge, award, revoke, relog, and restart
   cases.
6. Benchmark the gameplay hook. Introduce a cache only if measurements justify
   it; cache invalidation must cover every badge mutation path before release.

## Later milestones

Future menu entries, progression, non-badge sources, permissions, and
administration features require separate designs and tests. They must not be
added implicitly as part of the initial menu milestone.

## Branch lifecycle

Development may occur on `cyvster3`. Before deleting that branch, preserve the
completed module with a durable tag or commit reference, install it onto a new
clean integration branch, and verify that branch successfully.
