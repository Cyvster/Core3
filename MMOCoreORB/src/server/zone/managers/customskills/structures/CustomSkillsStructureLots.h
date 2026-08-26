/*
 * BRIEF-050: Account-shared structure lots with a performance-first cache.
 *
 * Feature (Q02): the lot pool becomes maximumLots * (live characters on the
 * account), shared across ALL characters on that account. When any account
 * character places a structure, every other account character also receives
 * ADMIN permission on it.
 *
 * Performance design (the cyvster2 stutter lesson): getLotsRemaining() is
 * called from hot paths (CharacterSheetResponseMessage, placement checks).
 * We NEVER rescan per query. A per-account registry is maintained:
 *   - Lazy one-time build per account after server boot (single scan of that
 *     account's characters' ownedStructures), guarded by a mutex.
 *   - Incremental updates at each ownership-change point in Core3
 *     (place / destroy / transfer / camp adoption) via recordPlace() /
 *     recordRemove().
 *
 * All methods are static; no Singleton wiring or engine task dependencies.
 */

#ifndef CUSTOMSKILLSSTRUCTURELOTS_H_
#define CUSTOMSKILLSSTRUCTURELOTS_H_

#include "engine/engine.h"

namespace server {
namespace zone {
namespace objects {
namespace structure {
class StructureObject;
}
}
}
}

class CustomSkillsStructureLots {
public:
	// Returns true when the feature is enabled via config
	// (structures.accountSharedLots). Safe to call anywhere.
	static bool isEnabled();

	// Account-wide lots remaining for a character's account:
	// maximumLots * liveCharacterCount - totalLotsUsedByAccount.
	// Falls back to vanilla semantics if anything is unavailable.
	static int getAccountLotsRemaining(uint64 playerOID);

	// Incremental cache maintenance. Call exactly once per ownership change,
	// immediately after the vanilla add/removeOwnedStructure call.
	//   recordPlace(ownerOID, lotSize)  -- structure gained by ownerOID
	//   recordRemove(oldOwnerOID, lotSize) -- structure lost by oldOwnerOID
	static void recordPlace(uint64 ownerOID, int lotSize);
	static void recordRemove(uint64 oldOwnerOID, int lotSize);

	// Grants ADMIN permission on the structure to every OTHER character OID
	// on the placing player's account (placing player already has ADMIN).
	static void grantAccountAdmin(server::zone::objects::structure::StructureObject* structure, uint64 placerOID);
};

#endif /* CUSTOMSKILLSSTRUCTURELOTS_H_ */
