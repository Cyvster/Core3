/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef NEWBIESELECTSTARTINGLOCATIONCOMMAND_H_
#define NEWBIESELECTSTARTINGLOCATIONCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/player/StartingLocation.h"
#include "server/zone/objects/player/PlayerObject.h"

class NewbieSelectStartingLocationCommand : public QueueCommand {
public:
	NewbieSelectStartingLocationCommand(const String& name, ZoneProcessServer* server) : QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* player, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(player))
			return INVALIDSTATE;

		if (!player->isPlayerCreature())
			return GENERALERROR;

		if (!checkInvalidLocomotions(player))
			return INVALIDLOCOMOTION;

		auto zone = player->getZone();

		if (zone == nullptr || zone->getZoneName() != "tutorial")
			return GENERALERROR;

		auto zoneServer = player->getZoneServer();

		if (zoneServer == nullptr) {
			return GENERALERROR;
		}

		ManagedReference<SceneObject*> rootTutorial = player->getRootParent();

		if (rootTutorial == nullptr) {
			return GENERALERROR;
		}

		auto playerManager = zoneServer->getPlayerManager();

		if (playerManager == nullptr) {
			return GENERALERROR;
		}

		String city = arguments.toString();

		StartingLocation* startingLocation = playerManager->getStartingLocation(city);

		if (startingLocation == nullptr) {
			player->info(true) << "Attempted to start at invalid starting location: " << city << ".";
			return GENERALERROR;
		}

		zone = zoneServer->getZone(startingLocation->getZoneName());

		if (zone == nullptr) {
			player->sendSystemMessage("This starting location is disabled, please select a different one");
			return GENERALERROR;
		}

		player->switchZone(startingLocation->getZoneName(), startingLocation->getX(), startingLocation->getZ(), startingLocation->getY(), startingLocation->getCell());
		player->setDirection(startingLocation->getHeading());

		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		if (ghost != nullptr) {
			ghost->setCloningFacility(nullptr);

			if (ghost->getBankLocation() != "") {
				ghost->setBankLocation(startingLocation->getZoneName());
			}
		}

		if (rootTutorial != nullptr && rootTutorial->getGameObjectType() == SceneObjectType::TUTORIALBUILDING) {
			auto tutorial = rootTutorial->asBuildingObject();

			if (tutorial != nullptr) {
				StructureManager::instance()->destroyStructure(tutorial, false);
			} else {
				error() << "Failed to destroy tutorial building for Player: " << player->getFirstName() << " ID: " << player->getObjectID() << " Building ID: " << rootTutorial->getObjectID();
			}
		}

		return SUCCESS;
	}
};

#endif // NEWBIESELECTSTARTINGLOCATIONCOMMAND_H_
