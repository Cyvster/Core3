/*
 * CustomSkillsMissions.h -- BRIEF-043 mission terminal direction/difficulty
 * options (port of cyvster2 M01/M02/M03-titles, per _038_design).
 *
 * Delegation-only: MissionManager declares this class a friend and calls the
 * tiny static hooks below; all feature logic lives here, NOT in Core3.
 */

#ifndef CUSTOMSKILLSMISSIONS_H_
#define CUSTOMSKILLSMISSIONS_H_

#include "engine/engine.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
class CreatureObject;
}
}
}
}

namespace server {
namespace zone {
namespace objects {
namespace mission {
class MissionObject;
}
}
}
}

namespace server {
namespace templates {
namespace mobile {
class LairTemplate;
}
}
}

class CustomSkillsMissions {
public:
	// --- config gates -------------------------------------------------
	static bool isEnabled();
	static bool isDirectionOptionEnabled();
	static bool isDifficultyOptionEnabled();

	// --- Core3 hook values (each early-returns cheaply) ---------------

	// Difficulty tier override from ScreenPlayData
	// mission_level_choice/levelChoice; -1 when unset/reset (use vanilla).
	static int getDifficultyOverride(server::zone::objects::creature::CreatureObject* player);

	// Player-level override for getRandomLairSpawn's window filter;
	// -1 when unset/reset (use vanilla player/group level).
	static int getLevelOverride(server::zone::objects::creature::CreatureObject* player);

	// Heading override for destroy-mission placement. Returns the passed-in
	// fallback heading unchanged when no direction is chosen; otherwise the
	// chosen compass heading +/- deviation (config, default 5 degrees),
	// wrapped to 0-359. Feeds vanilla SceneObject::getWorldCoordinate.
	static float getMissionHeading(server::zone::objects::creature::CreatureObject* player, float fallbackHeading);

	// Mission-datapad cap (vanilla hard-coded 3).
	static int getMissionListSize();

	// Descriptive "CL<n> Destroy the <mobile>" titles on/off.
	static bool shouldUseDescriptiveTitles();

	// Builds and applies the descriptive title. No-op when the feature is
	// disabled or the lair template exposes no mobiles (falls back to the
	// caller's vanilla title).
	static void applyDescriptiveTitle(server::zone::objects::mission::MissionObject* mission,
			int diffDisplay, server::templates::mobile::LairTemplate* lairTemplate);

	// --- per-populate choice caching (the performance fix) ------------

	// Call ONCE at the top of populateMissionList: reads both ScreenPlayData
	// keys for this player into an in-memory cache so the per-mission hooks
	// never touch ScreenPlayData/OODB during generation.
	static void cacheChoices(server::zone::objects::creature::CreatureObject* player);

	// Call ONCE at the end of populateMissionList (RAII-safe: drops the
	// player's cache entry; stale entries cannot outlive the request).
	static void clearChoices(uint64 playerObjectID);

	// --- terminal option UI (pure C++ SUI, no Lua on this path) -------

	static void showDirectionMenu(server::zone::objects::creature::CreatureObject* player);
	static void showDifficultyMenu(server::zone::objects::creature::CreatureObject* player);

public:
	class Choices;

private:

	// Internal: cached-or-lazy read of both choices. Never throws on empty
	// ScreenPlayData (cyvster2 defect 1 fixed here).
	static const Choices& getChoices(server::zone::objects::creature::CreatureObject* player);

	static void persistChoice(server::zone::objects::creature::CreatureObject* player,
			bool directionMenu, int value);

	friend class MissionOptionsMenuSelectCallback;
};

#endif // CUSTOMSKILLSMISSIONS_H_
