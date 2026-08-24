# Challenge Tier System

A server-side difficulty scaling system where players select a personal
challenge tier that scales their experience, loot, and combat encounters.

## Agreed framework

- **Name**: Challenge Tier
- **Scope**: Player-specific. Each player chooses their own tier.
- **Visibility**: A player's tier only affects them. Players can group and
  play alongside others who are not using the system or at different tiers.
- **PVP exclusion**: Challenge tiers must have no impact on PVP. Damage
  to/from other players is never affected. Checks must be placed throughout
  the system to ensure PVP is excluded.

### Tier change rules
- **Increasing tier**: Requires the player to be in a cantina and not in
  combat. This prevents exploits such as weakening a monster then raising
  the tier for a large experience bonus on the final hit.
- **Decreasing tier**: Can be done anywhere, including during combat. Players
  may travel far and realize their tier is too high.

### Storage
- Player state stored via a single transient C++ class in PlayerObject.idl,
  referenced from customskills code. Manages all temporary settings as
  key-value pairs. Resets on logout/restart automatically.
- No database or client modifications required.

### Experience
- Challenge tier XP bonus applied as a multiplier to existing XP awards
  through the existing CustomSkillsProgression hook. No new XP type needed.
- Client sees the bonus in its existing XP window.

### Code checks available
- **Combat check**: `player->isInCombat()` -- single method call
- **Cantina check**: `cast<BuildingObject*>(player->getRootParent())` then
  `building->getPlanetMapCategory()->getName() == "cantina"` -- same pattern
  used in `RegisterWithLocationCommand.h`

## Design open questions

- What will the challenge tiers affect?
- How many tiers will there be?
- What are the tier labels?
