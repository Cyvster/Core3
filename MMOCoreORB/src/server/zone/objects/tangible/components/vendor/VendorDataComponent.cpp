/*
 * VendorDataComponent.cpp
 *
 *  Created on: 5/29/2012
 *      Author: Kyle
 */

#include "VendorDataComponent.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/vendor/VendorManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/callbacks/VendorMaintSuiCallback.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/chat/ChatManager.h"
#include "server/zone/objects/auction/events/UpdateVendorTask.h"
#include "server/zone/managers/auction/AuctionManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/packets/object/SpatialChat.h"
#include "server/zone/objects/tangible/tasks/VendorReturnToPositionTask.h"
#include "server/zone/objects/transaction/TransactionLog.h"
#include "server/zone/CloseObjectsVector.h"
#include "server/zone/Zone.h"

VendorDataComponent::VendorDataComponent() : AuctionTerminalDataComponent(), adBarkingMutex() {
	ownerId = 0;
	auctionMan = nullptr;
	initialized = false;
	vendorSearchEnabled = false;
	disabled = false;
	registered = false;
	maintAmount = 0;
	awardUsageXP = 0;
	adBarking = false;
	mail1Sent = false;
	barkMessage = "";
	lastBark = 0;
	originalDirection = 1000;
	destroyStartedTime = Time((uint32)0);
	addSerializableVariables();
}

void VendorDataComponent::addSerializableVariables() {
	addSerializableVariable("ownerId", &ownerId);
	addSerializableVariable("initialized", &initialized);
	addSerializableVariable("vendorSearchEnabled", &vendorSearchEnabled);
	addSerializableVariable("disabled", &disabled);
	addSerializableVariable("registered", &registered);
	addSerializableVariable("maintAmount", &maintAmount);
	addSerializableVariable("lastXpAward", &lastXpAward);
	addSerializableVariable("awardUsageXP", &awardUsageXP);
	addSerializableVariable("lastSuccessfulUpdate", &lastSuccessfulUpdate);
	addSerializableVariable("adBarking", &adBarking);
	addSerializableVariable("mail1Sent", &mail1Sent);
	addSerializableVariable("emptyTimer", &emptyTimer);
	addSerializableVariable("barkMessage", &barkMessage);
	addSerializableVariable("barkMood", &barkMood);
	addSerializableVariable("barkAnimation", &barkAnimation);
	addSerializableVariable("originalDirection", &originalDirection);
}

void VendorDataComponent::writeJSON(nlohmann::json& j) const {
	AuctionTerminalDataComponent::writeJSON(j);

	SERIALIZE_JSON_MEMBER(ownerId);
	SERIALIZE_JSON_MEMBER(initialized);
	SERIALIZE_JSON_MEMBER(vendorSearchEnabled);
	SERIALIZE_JSON_MEMBER(disabled);
	SERIALIZE_JSON_MEMBER(registered);
	SERIALIZE_JSON_MEMBER(maintAmount);
	SERIALIZE_JSON_MEMBER(lastXpAward);
	SERIALIZE_JSON_MEMBER(awardUsageXP);
	SERIALIZE_JSON_MEMBER(lastSuccessfulUpdate);
	SERIALIZE_JSON_MEMBER(adBarking);
	SERIALIZE_JSON_MEMBER(mail1Sent);
	SERIALIZE_JSON_MEMBER(emptyTimer);
	SERIALIZE_JSON_MEMBER(barkMessage);
	SERIALIZE_JSON_MEMBER(barkMood);
	SERIALIZE_JSON_MEMBER(barkAnimation);
	SERIALIZE_JSON_MEMBER(originalDirection);
}

void VendorDataComponent::initializeTransientMembers() {

	AuctionTerminalDataComponent::initializeTransientMembers();

	lastBark = 0;
	ManagedReference<SceneObject*> strongParent = parent.get();

	if(strongParent != nullptr) {

		if (isInitialized()) {
			scheduleVendorCheckTask(VENDORCHECKDELAY + System::random(VENDORCHECKINTERVAL));

			if(originalDirection == 1000)
				originalDirection = strongParent->getDirectionAngle();

			if(isRegistered() && strongParent->getZone() != nullptr)
				strongParent->getZone()->registerObjectWithPlanetaryMap(strongParent);
		}
	}
}

void VendorDataComponent::notifyObjectDestroyingFromDatabase() {
	logUnmanagedRemoval("destroyObjectFromDatabase");

	ManagedReference<SceneObject*> strong = parent.get();

	if(strong == nullptr)
		return;

	ManagedReference<CreatureObject*> player = strong->getZoneServer()->getObject(ownerId).castTo<CreatureObject*>();
	if(player == nullptr)
		return;

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost != nullptr)
		ghost->removeVendor(strong);
}

void VendorDataComponent::notifyObjectDestroyingFromWorld() {
	logUnmanagedRemoval("destroyObjectFromWorld");
}

// Mirrors PlayerManagerCommand::dumpCOV, minus the object Locker, the caller of the destroy
// hooks already holds the vendor lock and safeCopyTo does its own locking on the vector
String VendorDataComponent::getCloseObjectsDump(SceneObject* vendor, int& playerCount, String& playerList) {
	StringBuffer dump;
	StringBuffer players;

	playerCount = 0;
	playerList = "";

	if (vendor == nullptr)
		return "no vendor";

	SortedVector<uint64> seenPlayers;
	seenPlayers.setNoDuplicateInsertPlan();

	auto ourPosition = vendor->getWorldPosition();

	auto notePlayer = [&](SceneObject* obj, float distance) {
		if (obj == nullptr || !obj->isPlayerCreature() || seenPlayers.contains(obj->getObjectID()))
			return;

		seenPlayers.put(obj->getObjectID());

		if (playerCount > 0)
			players << "; ";

		players << obj->getDisplayedName() << " (" << obj->getObjectID() << ") " << (int)distance << "m";

		playerCount++;
	};

	CloseObjectsVector* closeVector = (CloseObjectsVector*)vendor->getCloseObjects();

	if (closeVector == nullptr) {
		dump << "vendor has no close objects vector";
	} else {
		SortedVector<TreeEntry*> closeObjects;
		closeVector->safeCopyTo(closeObjects);

		VectorMap<float, SceneObject*> sortedObjects;
		sortedObjects.setAllowDuplicateInsertPlan();

		for (int i = 0; i < closeObjects.size(); ++i) {
			auto obj = static_cast<SceneObject*>(closeObjects.getUnsafe(i));

			if (obj == nullptr)
				continue;

			sortedObjects.put(ourPosition.distanceTo(obj->getWorldPosition()), obj);
		}

		dump << sortedObjects.size() << " objects:";

		for (int i = 0; i < sortedObjects.size(); ++i) {
			auto obj = sortedObjects.get(i);

			if (obj == nullptr) {
				dump << endl << "  " << i << ": nullptr object";
				continue;
			}

			auto objParent = obj->getParent().get();
			auto objTemplate = obj->getObjectTemplate();
			float distance = ourPosition.distanceTo(obj->getWorldPosition());

			dump << endl << "  " << i << ": " << obj->getObjectID() << ": " << obj->getDisplayedName()
				<< " (" << (objTemplate == nullptr ? "no template" : objTemplate->getTemplateFileName()) << ")"
				<< " parent: " << (objParent == nullptr ? String("<none>") : String::valueOf(objParent->getObjectID()))
				<< " receivers: " << CloseObjectsVector::receiverFlagsToString(obj->getReceiverFlags())
				<< " @ " << obj->getWorldPosition().toString() << " " << (int)distance << "m";

			if (obj->isPlayerCreature()) {
				dump << " <== PLAYER";
				notePlayer(obj, distance);
			}
		}
	}

	// A vendor indoors may not carry the cell occupants in its own vector
	ManagedReference<SceneObject*> vendorParent = vendor->getParent().get();

	if (vendorParent != nullptr && vendorParent->isCellObject()) {
		for (int i = 0; i < vendorParent->getContainerObjectsSize(); ++i) {
			auto obj = vendorParent->getContainerObject(i);

			if (obj == nullptr || !obj->isPlayerCreature() || seenPlayers.contains(obj->getObjectID()))
				continue;

			float distance = ourPosition.distanceTo(obj->getWorldPosition());

			dump << endl << "  in same cell: " << obj->getObjectID() << ": " << obj->getDisplayedName() << " " << (int)distance << "m <== PLAYER";

			notePlayer(obj, distance);
		}
	}

	playerList = players.toString();

	return dump.toString();
}

void VendorDataComponent::logUnmanagedRemoval(const String& context) {
	bool destroyStarted = destroyStartedTime.getTime() != 0;
	int64 msSinceDestroyStarted = destroyStarted ? destroyStartedTime.miliDifference() : -1;

	if (destroyStarted && msSinceDestroyStarted <= DESTROYWINDOWMS)
		return;

	ManagedReference<SceneObject*> strongParent = parent.get();

	if (strongParent == nullptr)
		return;

	auto zoneServer = strongParent->getZoneServer();
	ManagedReference<CreatureObject*> owner = zoneServer == nullptr ? nullptr : zoneServer->getObject(ownerId).castTo<CreatureObject*>();

	auto zone = strongParent->getZone();
	String zoneName = zone == nullptr ? "none" : zone->getZoneName();
	Vector3 worldPosition = strongParent->getWorldPosition();

	ManagedReference<SceneObject*> vendorParent = strongParent->getParent().get();
	uint64 parentID = vendorParent == nullptr ? 0 : vendorParent->getObjectID();

	// The immediate parent of an indoor vendor is a cell and cells carry no name, the structure
	// above it is what actually identifies where the vendor stood
	ManagedReference<SceneObject*> rootParent = strongParent->getRootParent();
	uint64 buildingID = rootParent == nullptr ? 0 : rootParent->getObjectID();
	String buildingName = rootParent == nullptr ? "none" : rootParent->getDisplayedName();

	int playerCount = 0;
	String nearbyPlayers;
	String closeObjectsDump = getCloseObjectsDump(strongParent, playerCount, nearbyPlayers);

	StackTrace callStack;
	String callStackData = callStack.toStringData();

	// Console logger so this always reaches the global log, an AiAgent vendor has global
	// logging disabled and would otherwise bury this in its own per-object file
	auto msg = Logger::console.error();

	msg << "Vendor removed outside VendorManager via " << context
		<< " -- vendor: " << strongParent->getObjectID()
		<< " name: " << strongParent->getDisplayedName()
		<< " ownerId: " << ownerId
		<< " ownerName: " << (owner == nullptr ? "unknown" : owner->getFirstName())
		<< " uid: " << getUID()
		<< " zone: " << zoneName
		<< " worldPosition: " << worldPosition.getX() << "," << worldPosition.getY() << "," << worldPosition.getZ()
		<< " parent: " << parentID
		<< " building: " << buildingID << " (" << buildingName << ")"
		<< " maintAmount: " << maintAmount
		<< " registered: " << registered;

	if (destroyStarted)
		msg << " sanctionedDestroyStarted: " << msSinceDestroyStarted << "ms ago, past the " << (int)DESTROYWINDOWMS << "ms window";
	else
		msg << " sanctionedDestroyStarted: never";

	if (playerCount > 0)
		msg << " -- players nearby: " << nearbyPlayers;
	else
		msg << " -- players nearby: none";

	msg << endl << "COV was: " << closeObjectsDump;
	msg << endl << "callStack: " << callStackData;

	msg.flush();

	TransactionLog trx(owner, strongParent, TrxCode::VENDORLIFECYCLE);
	trx.addState("subjectAction", "unmanaged_removal");
	trx.addState("subjectDestroyReason", "removed outside VendorManager via " + context);
	trx.addState("vendorName", strongParent->getDisplayedName());
	trx.addState("vendorOwnerId", ownerId);
	trx.addState("vendorOwnerName", owner == nullptr ? "unknown" : owner->getFirstName());
	trx.addState("vendorMaintAmount", maintAmount);
	trx.addState("vendorRegistered", registered);
	trx.addState("vendorUID", getUID());
	trx.addState("vendorDestroyStarted", destroyStarted);
	trx.addState("vendorMsSinceDestroyStarted", msSinceDestroyStarted);
	trx.addState("vendorParentId", parentID);
	trx.addState("vendorBuildingId", buildingID);
	trx.addState("vendorBuildingName", buildingName);
	trx.addState("vendorNearbyPlayerCount", playerCount);
	trx.addState("vendorNearbyPlayers", nearbyPlayers);
	trx.addState("vendorCloseObjects", closeObjectsDump);
	trx.addState("vendorCallStack", callStackData);

	trx.addRelatedObject(strongParent, true);
	trx.setExportRelatedObjects(true);
	trx.exportRelated();
}

void VendorDataComponent::runVendorUpdate() {
	ManagedReference<SceneObject*> strongParent = parent.get();

	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return;

	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	ManagedReference<PlayerManager*> playerManager = strongParent->getZoneServer()->getPlayerManager();
	ManagedReference<TangibleObject*> vendor = cast<TangibleObject*>(strongParent.get());

	if (owner == nullptr || !owner->isPlayerCreature() || playerManager == nullptr || vendor == nullptr) {
		return;
	}

	scheduleVendorCheckTask(VENDORCHECKINTERVAL);

	removeAllVendorBarks();

	int now = time(0);
	int last = lastSuccessfulUpdate.getTime();
	float hoursSinceLastUpdate = now - last;
	hoursSinceLastUpdate /= 3600.f;

	if (maintAmount > 0)
		inactiveTimer.updateToCurrentTime();

	/// parent salaries
	Locker vlocker(owner, vendor);
	maintAmount -= getMaintenanceRate() * hoursSinceLastUpdate;

	if (maintAmount < 0) {
		vendor->setConditionDamage(-maintAmount, true);
	} else {
		vendor->setConditionDamage(0, true);
		vendor->setMaxCondition(1000, true);
	}

	if (isEmpty()) {
		ManagedReference<ChatManager*> cman = strongParent->getZoneServer()->getChatManager();

		String sender = strongParent->getDisplayedName();
		UnicodeString subject("@auction:vendor_status_subject");

		if (!mail1Sent && time(0) - emptyTimer.getTime() > EMPTYWARNING) {
			StringIdChatParameter body("@auction:vendor_status_endangered");
			body.setTO(strongParent->getDisplayedName());
			if (cman != nullptr)
				cman->sendMail(sender, subject, body, owner->getFirstName());
			mail1Sent = true;

			TransactionLog trx(owner, vendor, TrxCode::VENDORLIFECYCLE);
			trx.addState("subjectAction", "endangered");
			trx.addState("vendorName", strongParent->getDisplayedName());
			trx.addState("vendorOwnerId", ownerId);
			trx.addState("vendorEmptyDays", getEmptyDays());
			trx.addState("vendorMaintAmount", maintAmount);
			trx.addState("vendorRegistered", registered);
			trx.addState("vendorUID", getUID());
			trx.addState("vendorMailSent", cman != nullptr);
		}

		else if (time(0) - emptyTimer.getTime() > EMPTYDELETE) {
			StringIdChatParameter body("@auction:vendor_status_deleted");
			if (cman != nullptr)
				cman->sendMail(sender, subject, body, owner->getFirstName());
			VendorManager::instance()->destroyVendor(vendor, "nothing listed for sale for 28 days");
			return;
		}

	} else {
		mail1Sent = false;
		emptyTimer.updateToCurrentTime();
	}

	if (isOnStrike()) {
		if (isRegistered())
			VendorManager::instance()->handleUnregisterVendor(owner, vendor);

		if (isVendorSearchEnabled())
			setVendorSearchEnabled(false);

		if (time(0) - inactiveTimer.getTime() > DELETEWARNING) {

			ManagedReference<ChatManager*> cman = strongParent->getZoneServer()->getChatManager();

			String sender = strongParent->getDisplayedName();
			UnicodeString subject("@auction:vendor_status_subject");

			StringIdChatParameter body("@auction:vendor_status_deleted");
			if (cman != nullptr)
				cman->sendMail(sender, subject, body, owner->getFirstName());
			VendorManager::instance()->destroyVendor(vendor, "maintenance pool empty for 100 days");
		}

	} else {

		/// Award hourly XP
		E3_ASSERT(vendor->isLockedByCurrentThread());

		Locker locker(owner, vendor);
		playerManager->awardExperience(owner, "merchant", 150 * hoursSinceLastUpdate, false);

		playerManager->awardExperience(owner, "merchant", awardUsageXP * 50, false);

	}

	awardUsageXP = 0;
	lastSuccessfulUpdate.updateToCurrentTime();
}

float VendorDataComponent::getMaintenanceRate() {
	ManagedReference<SceneObject*> strongParent = parent.get();
	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return 15.f;

	// 15 credits base maintenance
	float maintRate = 15.f;

	// Apply reduction for merchant skills
	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	if (owner != nullptr && owner->isPlayerCreature() ) {
		if(owner->hasSkill("crafting_merchant_master"))
			maintRate *= .60f;
		else if(owner->hasSkill("crafting_merchant_sales_02"))
			maintRate *= .80f;
	}

	// Additional 6 credits per hour to be registered on the map
	if (registered)
		maintRate += 6.f;

	return maintRate;
}

void VendorDataComponent::payMaintanence() {
	ManagedReference<SceneObject*> strongParent = parent.get();
	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return;

	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	if(owner == nullptr)
		return;

	ManagedReference<SuiInputBox*> input = new SuiInputBox(owner, SuiWindowType::STRUCTURE_VENDOR_PAY);
	input->setPromptTitle("@player_structure:pay_vendor_t"); //Add Militia Member
	input->setPromptText("@player_structure:pay_vendor_d");
	input->setUsingObject(strongParent);
	input->setForceCloseDistance(5.f);
	input->setCallback(new VendorMaintSuiCallback(strongParent->getZoneServer()));

	owner->getPlayerObject()->addSuiBox(input);
	owner->sendMessage(input->generateMessage());

}

void VendorDataComponent::handlePayMaintanence(int value) {
	ManagedReference<SceneObject*> strongParent = parent.get();
	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return;

	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	if(owner == nullptr)
		return;

	if(value > 100000) {
		owner->sendSystemMessage("@player_structure:vendor_maint_invalid");
		return;
	}

	if(value <= 0) {
		owner->sendSystemMessage("@player_structure:amt_greater_than_zero");
		return;
	}

	if(owner->getBankCredits() + owner->getCashCredits() >= value) {
		if(owner->getBankCredits() > value) {
			TransactionLog trx(owner, strongParent, TrxCode::VENDORMAINTANENCE, value, false);
			maintAmount += value;
			owner->subtractBankCredits(value);
		} else {
			TransactionLog trxCash(owner, strongParent, TrxCode::VENDORMAINTANENCE, value - owner->getBankCredits(), true);
			owner->subtractCashCredits(value - owner->getBankCredits());
			maintAmount += value - owner->getBankCredits();

			TransactionLog trxBank(owner, strongParent, TrxCode::VENDORMAINTANENCE, owner->getBankCredits(), false);
			trxBank.groupWith(trxCash);
			owner->subtractBankCredits(owner->getBankCredits());
			maintAmount += owner->getBankCredits();
		}

		StringIdChatParameter message("@player_structure:vendor_maint_accepted");
		message.setDI(maintAmount);
		owner->sendSystemMessage(message);

	} else {
		owner->sendSystemMessage("@player_structure:vendor_maint_denied");
	}
}

void VendorDataComponent::withdrawMaintanence() {
	ManagedReference<SceneObject*> strongParent = parent.get();
	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return;

	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	if(owner == nullptr)
		return;

	ManagedReference<SuiInputBox*> input = new SuiInputBox(owner, SuiWindowType::STRUCTURE_VENDOR_WITHDRAW);
	input->setPromptTitle("@player_structure:withdraw_vendor_t"); //Add Militia Member
	input->setPromptText("@player_structure:withdraw_vendor_d");
	input->setUsingObject(strongParent);
	input->setForceCloseDistance(5.f);
	input->setCallback(new VendorMaintSuiCallback(strongParent->getZoneServer()));

	owner->getPlayerObject()->addSuiBox(input);
	owner->sendMessage(input->generateMessage());

}

void VendorDataComponent::handleWithdrawMaintanence(int value) {
	ManagedReference<SceneObject*> strongParent = parent.get();
	if (strongParent == nullptr || strongParent->getZoneServer() == nullptr)
		return;

	ManagedReference<CreatureObject*> owner = strongParent->getZoneServer()->getObject(getOwnerId()).castTo<CreatureObject*>();
	if(owner == nullptr)
		return;

	if(value > maintAmount) {
		StringIdChatParameter message("@player_structure:vendor_withdraw_fail"); // The vendor maintenance pool doesn't have %DI credits!
		message.setDI(value);
		owner->sendSystemMessage(message);
		return;
	}

	if(value <= 0) {
		owner->sendSystemMessage("@player_structure:amt_greater_than_zero"); // The amount must be greater than zero.
		return;
	}

	{
		TransactionLog trx(strongParent, owner, TrxCode::VENDORMAINTANENCE, value, true);
		maintAmount -= value;
		owner->addBankCredits(value, true);
	}

	StringIdChatParameter message("@player_structure:vendor_withdraw"); // You successfully withdraw %DI credits from the maintenance pool.
	message.setDI(value);
	owner->sendSystemMessage(message);
}

void VendorDataComponent::setVendorSearchEnabled(bool enabled) {
	ManagedReference<SceneObject*> strongParent = parent.get();
	ManagedReference<AuctionManager*> auctionManager = auctionMan.get();

	if (auctionManager == nullptr || strongParent == nullptr || strongParent->getZoneServer() == nullptr || strongParent->getZone() == nullptr)
		return;

	vendorSearchEnabled = enabled;
	auctionManager->updateVendorSearch(strongParent, vendorSearchEnabled);
}


void VendorDataComponent::performVendorBark(SceneObject* target) {
	ManagedReference<CreatureObject*> vendor = cast<CreatureObject*>(parent.get().get());

	if (vendor == nullptr) {
		return;
	}

	if ((vendor->getOptionsBitmask() & OptionBitmask::VENDOR) && isOnStrike()) {
		return;
	}

	ManagedReference<CreatureObject*> player = cast<CreatureObject*>(target);

	if (player == nullptr || !player->isPlayerCreature() || player->isInvisible())
		return;

	resetLastBark();
	addBarkTarget(target->getObjectID());

	Reference<CreatureObject*> vendorRef = vendor;
	Reference<CreatureObject*> playerRef = player;

	Core::getTaskManager()->executeTask([vendorRef, playerRef] () {
		if (vendorRef == nullptr || playerRef == nullptr) {
			return;
		}

		Locker locker(vendorRef);

		VendorDataComponent* data = cast<VendorDataComponent*>(vendorRef->getDataObjectComponent()->get());

		if (data == nullptr)
			return;

		vendorRef->faceObject(playerRef);
		vendorRef->updateDirection(Math::deg2rad(vendorRef->getDirectionAngle()));

		auto zoneServer = vendorRef->getZoneServer();

		if (zoneServer == nullptr) {
			return;
		}

		auto chatManager = zoneServer->getChatManager();

		if (chatManager == nullptr) {
			return;
		}

		SpatialChat* chatMessage = nullptr;
		String barkMessage = data->getAdPhrase();

		if (barkMessage.beginsWith("@")) {
			StringIdChatParameter message;
			message.setStringId(barkMessage);
			message.setTT(playerRef->getObjectID());

			chatMessage = new SpatialChat(vendorRef->getObjectID(), playerRef->getObjectID(), playerRef->getObjectID(), message, 50, 0, chatManager->getMoodID(data->getAdMood()), 0, 0);
		} else {
			UnicodeString uniMessage(barkMessage);

			chatMessage = new SpatialChat(vendorRef->getObjectID(), playerRef->getObjectID(), playerRef->getObjectID(), uniMessage, 50, 0, chatManager->getMoodID(data->getAdMood()), 0, 0);
		}

		vendorRef->broadcastMessage(chatMessage, true);
		vendorRef->doAnimation(data->getAdAnimation());

		Reference<VendorReturnToPositionTask*> returnTask = new VendorReturnToPositionTask(vendorRef, data->getOriginalDirection());

		if (returnTask != nullptr) {
			vendorRef->addPendingTask("vendorreturn", returnTask, 3000);
		}

	}, "VendorBarkLambda");
}

void VendorDataComponent::scheduleVendorCheckTask(int delay) {
	ManagedReference<SceneObject*> strongParent = parent.get();

	if (strongParent == nullptr)
		return;

	if (vendorCheckTask == nullptr)
		vendorCheckTask = new UpdateVendorTask(strongParent);

	vendorCheckTask->reschedule(1000 * 60 * delay);
}

void VendorDataComponent::cancelVendorCheckTask() {
	if (vendorCheckTask != nullptr)
		vendorCheckTask->cancel();
}
