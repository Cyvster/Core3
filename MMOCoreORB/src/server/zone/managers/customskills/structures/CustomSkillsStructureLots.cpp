/*
 * BRIEF-050: Account-shared structure lots with a performance-first cache.
 * See the header for the design contract.
 */

#include "CustomSkillsStructureLots.h"

// BRIEF-050 (mod hook)
#include "server/zone/managers/customskills/CustomSkillsConfig.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/login/account/Account.h"
#include "server/login/account/AccountManager.h"
#include "server/ServerCore.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/Zone.h"

namespace {
	// One registry entry per account: total lots used plus a per-character
	// breakdown so transfers/removals stay exact even when characters drift
	// between accounts.
	class AccountLotEntry {
	public:
		int totalLotsUsed = 0;
		VectorMap<uint64, int> perCharLots; // charOID -> lots used

		void add(uint64 oid, int lots) {
			totalLotsUsed += lots;

			int idx = perCharLots.find(oid);
			if (idx >= 0)
				perCharLots.elementAt(idx).getValue() += lots;
			else
				perCharLots.put(oid, lots);
		}

		bool remove(uint64 oid, int lots) {
			int idx = perCharLots.find(oid);

			if (idx < 0)
				return false; // unknown delta: caller must invalidate

			VectorMapEntry<uint64, int>* e = &perCharLots.elementAt(idx);
			e->getValue() -= lots;
			totalLotsUsed -= lots;

			if (e->getValue() <= 0)
				perCharLots.remove(idx);

			if (totalLotsUsed < 0)
				totalLotsUsed = 0;

			if (perCharLots.isEmpty())
				return true; // entry now empty: caller may drop it

			return false;
		}
	};

	Mutex g_lotCacheMutex;
	VectorMap<uint32, AccountLotEntry*> g_lotCache; // accountID -> entry

	// Resolve a character OID to its accountID via PlayerObject::getAccount().
	uint32 resolveAccountID(uint64 playerOID) {
		auto zoneServer = ServerCore::getZoneServer();

		if (zoneServer == nullptr)
			return 0;

		auto scene = zoneServer->getObject(playerOID);

		if (scene == nullptr || !scene->isCreatureObject())
			return 0;

		auto creature = cast<CreatureObject*>(scene.get());

		if (creature == nullptr)
			return 0;

		auto ghost = creature->getPlayerObject();

		if (ghost == nullptr)
			return 0;

		return ghost->getAccountID();
	}

	// Lazy one-time build for an account: scan every character's
	// ownedStructures once, then keep it updated incrementally. Caller holds
	// no locks; we take what we need briefly.
	AccountLotEntry* getOrBuildEntry(uint32 accountID) {
		Locker locker(&g_lotCacheMutex);

		int idx = g_lotCache.find(accountID);

		if (idx >= 0)
			return g_lotCache.get(idx);

		auto entry = new AccountLotEntry();
		g_lotCache.put(accountID, entry);
		locker.release();

		// One-time scan. Account::getCharacterList() hits the DB on first
		// access and is cached on the Account object afterwards.
		Reference<Account*> account = AccountManager::getAccount(accountID);

		if (account == nullptr)
			return entry;

		Reference<CharacterList*> characters = account->getCharacterList();

		if (characters == nullptr)
			return entry;

		auto zoneServer = ServerCore::getZoneServer();

		if (zoneServer == nullptr)
			return entry;

		uint32 galaxyID = zoneServer->getGalaxyID();

		for (int i = 0; i < characters->size(); ++i) {
			CharacterListEntry& entryRef = characters->get(i);

			if (entryRef.getGalaxyID() != galaxyID)
				continue; // character lives on another galaxy

			uint64 charOID = entryRef.getObjectID();
			auto scene = zoneServer->getObject(charOID);

			if (scene == nullptr || !scene->isCreatureObject())
				continue; // offline / not loaded: nothing cached to scan

			auto creature = cast<CreatureObject*>(scene.get());

			if (creature == nullptr)
				continue;

			auto ghost = creature->getPlayerObject();

			if (ghost == nullptr)
				continue;

			// Sum this live character's owned structures by lot size.
			for (int j = 0; j < ghost->getTotalOwnedStructureCount(); ++j) {
				uint64 oid = ghost->getOwnedStructure(j);
				auto structure = zoneServer->getObject(oid).castTo<StructureObject*>();

				if (structure != nullptr) {
					Locker xlock(structure);
					entry->add(charOID, structure->getLotSize());
				}
			}
		}

		return entry;
	}
}

bool CustomSkillsStructureLots::isEnabled() {
	return CustomSkillsConfig::instance()->isAccountSharedLotsEnabled();
}

int CustomSkillsStructureLots::getAccountLotsRemaining(uint64 playerOID) {
	uint32 accountID = resolveAccountID(playerOID);

	if (accountID == 0)
		return -1; // signal caller to fall back to vanilla math

	// Character count: prefer the account's persistent character list so the
	// pool includes OFFLINE characters too (they still own structures).
	int charCount = 0;
	int maxPerChar = 10;
	{
		auto zoneServer = ServerCore::getZoneServer();

		if (zoneServer == nullptr)
			return -1;

		// Per-character cap from the querying character's ghost so admin
		// /adjustLotCount grants are honored for the whole pool.
		auto ownerScene = zoneServer->getObject(playerOID);

		if (ownerScene != nullptr && ownerScene->isCreatureObject()) {
			auto ownerCreo = cast<CreatureObject*>(ownerScene.get());

			if (ownerCreo != nullptr) {
				auto ownerGhost = ownerCreo->getPlayerObject();

				if (ownerGhost != nullptr)
					maxPerChar = ownerGhost->getMaximumLots();
			}
		}

		Reference<Account*> account = AccountManager::getAccount(accountID);
		Reference<CharacterList*> characters = (account != nullptr) ? account->getCharacterList() : nullptr;

		if (characters != nullptr) {
			uint32 galaxyID = zoneServer->getGalaxyID();

			for (int i = 0; i < characters->size(); ++i) {
				if (characters->get(i).getGalaxyID() == galaxyID)
					++charCount;
			}
		}
	}

	if (charCount <= 0)
		return -1; // fall back rather than report an empty pool

	AccountLotEntry* entry = getOrBuildEntry(accountID);

	if (entry == nullptr)
		return -1;

	int maxLots = static_cast<int>(maxPerChar) * charCount; // BRIEF-050: shared pool = maxLots * characters on account
	int remaining = maxLots - entry->totalLotsUsed;

	return (remaining < 0) ? 0 : remaining;
}

void CustomSkillsStructureLots::recordPlace(uint64 ownerOID, int lotSize) {
	if (!isEnabled() || lotSize <= 0)
		return;

	uint32 accountID = resolveAccountID(ownerOID);

	if (accountID == 0)
		return; // lazy rebuild will pick it up if the account resolves later

	Locker locker(&g_lotCacheMutex);

	int idx = g_lotCache.find(accountID);

	if (idx < 0)
		return; // not built yet: lazy build will include this structure

	g_lotCache.get(idx)->add(ownerOID, lotSize);
}

void CustomSkillsStructureLots::recordRemove(uint64 oldOwnerOID, int lotSize) {
	if (!isEnabled() || lotSize <= 0)
		return;

	uint32 accountID = resolveAccountID(oldOwnerOID);

	if (accountID == 0)
		return;

	Locker locker(&g_lotCacheMutex);

	int idx = g_lotCache.find(accountID);

	if (idx < 0)
		return;

	AccountLotEntry* entry = g_lotCache.get(idx);

	if (entry->remove(oldOwnerOID, lotSize)) {
		g_lotCache.remove(idx);
		delete entry;
	}
}

void CustomSkillsStructureLots::grantAccountAdmin(server::zone::objects::structure::StructureObject* structure, uint64 placerOID) {
	if (structure == nullptr || !isEnabled())
		return;

	uint32 accountID = resolveAccountID(placerOID);

	if (accountID == 0)
		return;

	Reference<Account*> account = AccountManager::getAccount(accountID);

	if (account == nullptr)
		return;

	Reference<CharacterList*> characters = account->getCharacterList();

	if (characters == nullptr)
		return;

	uint32 galaxyID = ServerCore::getZoneServer()->getGalaxyID();
	uint64 structureOID = structure->getObjectID();

	for (int i = 0; i < characters->size(); ++i) {
		CharacterListEntry& entry = characters->get(i);

		if (entry.getGalaxyID() != galaxyID)
			continue;

		uint64 charOID = entry.getObjectID();

		if (charOID == placerOID)
			continue; // placer already got ADMIN from vanilla

		structure->grantPermission("ADMIN", charOID);
	}
}
