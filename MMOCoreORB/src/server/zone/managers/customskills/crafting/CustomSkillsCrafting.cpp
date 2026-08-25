#include "CustomSkillsCrafting.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"

int CustomSkillsCrafting::modifyPracticeExperience(CreatureObject* crafter, int nativeAmount) {
	if (nativeAmount <= 0)
		return nativeAmount;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::PRACTICE_EXPERIENCE_BONUS);
	return static_cast<int>((static_cast<int64>(nativeAmount) * (10000 + bonus)) / 10000);
}

int CustomSkillsCrafting::divideDuration(CreatureObject* crafter, int nativeSeconds) {
	// Durations below 1 second are clamped to 1 (documented edge behavior).
	if (nativeSeconds <= 1)
		return 1;
	int bonus = CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::CRAFTING_SPEED);
	int multiplier = 10000 + bonus;
	int64 scaled = static_cast<int64>(nativeSeconds) * 10000;
	return Math::max(1, static_cast<int>((scaled + multiplier - 1) / multiplier));
}

int CustomSkillsCrafting::getPersonalCraftingDuration(CreatureObject* crafter, int nativeSeconds) {
	return divideDuration(crafter, nativeSeconds);
}

int CustomSkillsCrafting::getFactoryProductionDuration(CreatureObject* activator, int nativeSeconds) {
	return divideDuration(activator, nativeSeconds);
}

int CustomSkillsCrafting::getAmazingChanceBonus(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_SUCCESS_CHANCE);
}

int CustomSkillsCrafting::getAmazingResultsStrength(CreatureObject* crafter) {
	return CustomSkillsModifiers::getModifierTotal(crafter, CustomSkillsModifierType::AMAZING_RESULTS);
}

bool CustomSkillsCrafting::shouldPromoteAmazingFailure(CreatureObject* crafter, int nativeChanceBasisPoints) {
	const int bonus = getAmazingChanceBonus(crafter);
	const int nativeChance = Math::min(Math::max(nativeChanceBasisPoints, 0), 10000);
	const int finalChance = Math::min(10000, nativeChance + bonus);
	if (finalChance <= nativeChance || nativeChance >= 10000)
		return false;

	// This roll happens only after the native roll failed. Convert the desired
	// percentage-point increase into its conditional probability so the final
	// chance is nativeChance + configured bonus, not a multiplier.
	const int conditionalChance = static_cast<int>((static_cast<int64>(finalChance - nativeChance) * 10000) /
			(10000 - nativeChance));
	return System::random(9999) < conditionalChance;
}

void CustomSkillsCrafting::applyAmazingResults(CreatureObject* crafter, CraftingValues* values, int result,
		int amazingResult, const String& selectedGroup) {
	if (values == nullptr || result != amazingResult)
		return;

	const int strength = Math::min(getAmazingResultsStrength(crafter), 10000);
	if (strength <= 0)
		return;

	const float factor = strength / 10000.f;
	for (int i = 0; i < values->getTotalExperimentalAttributes(); ++i) {
		const String& attribute = values->getAttribute(i);
		if (!selectedGroup.isEmpty() && values->getAttributeGroup(attribute) != selectedGroup)
			continue;

		const float current = values->getCurrentPercentage(attribute);
		const float improved = current + ((1.f - current) * factor);
		values->setCurrentPercentage(attribute, improved, Math::max(values->getMaxPercentage(attribute), improved));
	}
	values->recalculateValues(selectedGroup.isEmpty());
}

// =============================================================================
// BRIEF-036: repeat-craft assisted pre-fill (hardened per BRIEF-042 / ERR-017,
// ERR-018)
//
// Snapshot format (stored on the CraftingTool via TangibleObject's persistent
// luaStringData map -- zero IDL changes, survives restarts; see CODE_REFERENCE):
//   cs36.schematicCrc  -> client object CRC of the draft schematic
//   cs36.slotCount     -> number of ingredient slots in the snapshot
//   cs36.slot.<i>.type -> resource spawn name (resource slots) or template
//                         name + "#" + serial (component slots)
//   cs36.slot.<i>.qty  -> quantity required by that slot
//   cs36.exp           -> experiment "row points row points ..." string,
//                           re-applied by the player via the experiment UI --
//                           surfaced to them as a reminder message on pre-fill
//
// BRIEF-042 hardening notes:
//  * Resource/component scans RECURSE into containers inside inventory
//    (backpacks, satchels) depth-limited -- matches vanilla's
//    isASubChildOf(crafter) acceptance semantics.
//  * Partial stacks: total availability across ALL matching stacks is summed
//    before anything is consumed; if it cannot fill the slot, nothing is
//    drained and a shortfall message naming the resource is sent.
// =============================================================================

#include "server/zone/ZoneServer.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/draftschematic/DraftSchematic.h"
#include "templates/crafting/draftslot/DraftSlot.h"
#include "server/zone/objects/tangible/tool/CraftingTool.h"
#include "server/zone/objects/player/sessions/crafting/CraftingSession.h"
#include "server/zone/objects/tangible/tool/CraftingStation.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/IngredientSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ResourceSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ComponentSlot.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/managers/customskills/CustomSkillsConfig.h"

static const char* CS36_PREFIX = "cs36.";
static const int CS36_MAX_CONTAINER_DEPTH = 4;

static void cs36Put(TangibleObject* tool, const String& key, const String& value) {
	tool->setLuaStringData(CS36_PREFIX + key, value);
}

static String cs36Get(TangibleObject* tool, const String& key) {
	return tool->getLuaStringData(CS36_PREFIX + key);
}

// BRIEF-042 (ERR-017): discard the FULL cs36.* key family on this tool so
// stale higher-indexed slot keys from a previous recipe can't linger.
void CustomSkillsCrafting::clearRepeatRecipe(CraftingTool* tool) {
	if (tool == nullptr)
		return;

	tool->deleteLuaStringData(CS36_PREFIX + String("schematicCrc"));
	tool->deleteLuaStringData(CS36_PREFIX + String("slotCount"));
	tool->deleteLuaStringData(CS36_PREFIX + String("exp"));

	for (int i = 0; i < 32; ++i) { // snapshot write loop caps at 32 slots
		String key = CS36_PREFIX + String("slot.") + String::valueOf(i) + ".";
		tool->deleteLuaStringData(key + "type");
		tool->deleteLuaStringData(key + "qty");
	}
}

void CustomSkillsCrafting::storeRepeatRecipe(CraftingTool* tool,
		ManufactureSchematic* schematic, const String& expAttempt) {
	if (tool == nullptr || schematic == nullptr)
		return;

	Reference<DraftSchematic*> draft = schematic->getDraftSchematic();
	if (draft == nullptr)
		return;

	clearRepeatRecipe(tool);

	cs36Put(tool, "schematicCrc", String::valueOf(draft->getServerObjectCRC())); // server CRC = static per schematic TYPE (client CRC varies per-char enumeration)
	// ERR-020 fix: counts/quantities come from the DRAFT schematic (static per
	// schematic, identical every session). The session ManufactureSchematic's
	// slots are post-fill/post-consume at snapshot time and do NOT match a
	// fresh session's pristine slots.
	cs36Put(tool, "slotCount", String::valueOf(draft->getDraftSlotCount()));

	for (int i = 0; i < schematic->getSlotCount(); ++i) {
		IngredientSlot* slot = schematic->getSlot(i);
		if (slot == nullptr)
			continue;

		String key = "slot." + String::valueOf(i) + ".";
		String value;

		ResourceSlot* resSlot = dynamic_cast<ResourceSlot*>(slot);
		if (resSlot != nullptr && resSlot->getCurrentSpawn() != nullptr) {
			value = resSlot->getCurrentSpawn()->getName(); // exact spawn name
		} else {
			ComponentSlot* compSlot = dynamic_cast<ComponentSlot*>(slot);
			TangibleObject* proto = compSlot != nullptr ? compSlot->getPrototype() : nullptr;
			if (proto != nullptr) {
				value = proto->getObjectTemplate()->getFullTemplateString() + "#" + proto->getSerialNumber();
			}
		}

		cs36Put(tool, key + "type", value);
		if (i < draft->getDraftSlotCount()) {
			DraftSlot* dSlot = draft->getDraftSlot(i);
			if (dSlot != nullptr) {
				cs36Put(tool, key + "qty", String::valueOf(dSlot->getQuantity()));
				continue;
			}
		}
		cs36Put(tool, key + "qty", String::valueOf(slot->getQuantityNeeded()));
	}

	cs36Put(tool, "exp", expAttempt);
}

namespace {
// Recursive candidate collection under `root` (depth-limited). Appends:
//  resource containers matching spawnName, or
//  tangible components matching template (+serial when non-empty).
void collectCandidates(SceneObject* root, bool wantResource, const String& spawnOrTemplate,
		const String& serial, Vector<ManagedReference<TangibleObject*> >& out, int depth) {
	if (root == nullptr || depth > CS36_MAX_CONTAINER_DEPTH)
		return;

	for (int i = 0; i < root->getContainerObjectsSize(); ++i) {
		auto obj = root->getContainerObject(i);
		if (obj == nullptr)
			continue;

		if (wantResource && obj->isResourceContainer()) {
			auto rc = cast<ResourceContainer*>(obj.get());
			if (rc != nullptr && rc->getSpawnName() == spawnOrTemplate)
				out.add(rc);
		} else if (!wantResource && obj->isTangibleObject() && !obj->isResourceContainer()
				&& !obj->isContainerObject()) {
			if (obj->getObjectTemplate()->getFullTemplateString() != spawnOrTemplate)
				continue;
			auto tanoObj = cast<TangibleObject*>(obj.get());
			if (!serial.isEmpty() && tanoObj->getSerialNumber() != serial)
				continue;
			out.add(tanoObj);
		} else if (obj->isContainerObject() && !obj->isCraftingTool()
				&& !obj->isCraftingStation()) {
			// Recurse into backpacks/satchels/etc.
			collectCandidates(obj.get(), wantResource, spawnOrTemplate, serial, out, depth + 1);
		}
	}
}
} // namespace

int CustomSkillsCrafting::doRepeatCraft(CreatureObject* player, uint64 targetID) {
	if (!CustomSkillsConfig::instance()->isRepeatEnabled()) {
		player->sendSystemMessage("Repeat-craft is disabled on this server.");
		return QueueCommand::GENERALERROR;
	}

	auto zoneServer = player->getZoneServer();
	if (zoneServer == nullptr)
		return QueueCommand::GENERALERROR;

	ManagedReference<SceneObject*> target = zoneServer->getObject(targetID);

	CraftingTool* tool = nullptr;
	if (target != nullptr && target->isCraftingTool()) {
		tool = cast<CraftingTool*>(target.get());
	} else {
		// No explicit target: find the first crafting tool in inventory that
		// holds a stored snapshot.
		auto inventory = player->getSlottedObject("inventory");
		if (inventory != nullptr) {
			for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
				auto obj = inventory->getContainerObject(i);
				if (obj != nullptr && obj->isCraftingTool()) {
					auto candidate = cast<CraftingTool*>(obj.get());
					if (candidate != nullptr && !cs36Get(candidate, "schematicCrc").isEmpty()) {
						tool = candidate;
						break;
					}
				}
			}
		}
	}

	if (tool == nullptr || !tool->isASubChildOf(player)) {
		player->sendSystemMessage("Repeat-craft: no crafting tool found.");
		return QueueCommand::GENERALERROR;
	}

	const String crcStr = cs36Get(tool, "schematicCrc");
	if (crcStr.isEmpty()) {
		player->sendSystemMessage("Repeat-craft: this tool has no stored recipe. Craft something first.");
		return QueueCommand::GENERALERROR;
	}

	if (tool->isFinished()) {
		player->sendSystemMessage("@system_msg:crafting_tool_full");
		return QueueCommand::GENERALERROR;
	}

	if (tool->isBusy()) {
		player->sendSystemMessage("@system_msg:crafting_tool_creating_prototype");
		return QueueCommand::GENERALERROR;
	}

	uint32 crc = (uint32)strtoul(crcStr.toCharArray(), nullptr, 10);

	// Resolve a nearby crafting station for this tool type, as vanilla does.
	auto playerMan = zoneServer->getPlayerManager();
	CraftingStation* station =
			playerMan != nullptr ? playerMan->getNearbyCraftingStation(player, tool->getToolType()) : nullptr;

	// ERR-020 fix: never auto-cancel a session the player started via the tool
	// UI -- doing so wedged the tool's state machine (owner incident 08252026).
	Reference<CraftingSession*> activeSession =
			player->getActiveSession(SessionFacadeType::CRAFTING).castTo<CraftingSession*>();
	if (activeSession != nullptr) {
		player->sendSystemMessage("Repeat-craft: finish or cancel your current crafting session first.");
		return QueueCommand::GENERALERROR;
	}

	ManagedReference<CraftingSession*> session = new CraftingSession(player);
	Locker locker(tool);

	if (!session->initializeSession(tool, station)) {
		player->sendSystemMessage("@ui_craft@err_no_crafting_tool");
		return QueueCommand::GENERALERROR;
	}

	// Auto-select the snapshotted schematic by CRC within the freshly
	// filtered session list.
	int index = -1;
	for (int i = 0; i < session->getCurrentSchematicListSize(); ++i) {
		DraftSchematic* draft = session->getCurrentSchematic(i);
		if (draft != nullptr && draft->getServerObjectCRC() == crc) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		// Schematic no longer known / wrong tool tabs / complexity mismatch:
		// discard the stale snapshot with notice (brief requirement).
		player->sendSystemMessage("Repeat-craft: schematic no longer available -- stored recipe discarded.");
		clearRepeatRecipe(tool);
		Locker slock(session);
		session->cancelSession();
		return QueueCommand::GENERALERROR;
	}

	session->selectDraftSchematic(index);

	// ---- Pre-fill: re-validate every slot against live inventory ----------
	ManagedReference<ManufactureSchematic*> manf = session->getSchematic();
	if (manf == nullptr) {
		return QueueCommand::GENERALERROR;
	}

	// ERR-020 fix: compare against the DRAFT schematic's static slot data.
	// The stored values were captured from draft quantities, so compare like
	// with like -- a fresh session's slots should equal draft quantities every
	// time; only a genuine schematic change triggers a discard now.
	Reference<DraftSchematic*> liveDraft = manf->getDraftSchematic();
	int snapshotSlots = Integer::valueOf(cs36Get(tool, "slotCount"));
	bool mismatch = false;

	if (liveDraft == nullptr || manf->getSlotCount() != snapshotSlots) {
		mismatch = true;
	} else {
		for (int i = 0; i < manf->getSlotCount() && !mismatch; ++i) {
			IngredientSlot* slot = manf->getSlot(i);
			int draftQty = -1;
			if (i < liveDraft->getDraftSlotCount()) {
				DraftSlot* dSlot = liveDraft->getDraftSlot(i);
				if (dSlot != nullptr)
					draftQty = (int)dSlot->getQuantity();
			}
			if (slot == nullptr || slot->getQuantityNeeded() != draftQty
					|| slot->getQuantityNeeded()
					!= Integer::valueOf(cs36Get(tool, "slot." + String::valueOf(i) + ".qty"))) {
				mismatch = true;
			}
		}
	}

	if (mismatch) {
		// Schematic changed shape since the snapshot: discard with notice.
		player->sendSystemMessage("Repeat-craft: recipe no longer matches this schematic -- stored recipe discarded.");
		clearRepeatRecipe(tool);
		Locker slock(session);
		session->cancelSession();
		return QueueCommand::GENERALERROR;
	}

	// Session::addIngredient resolves the crafted-components satchel itself.

	auto inventory = player->getSlottedObject("inventory");

	for (int i = 0; i < manf->getSlotCount(); ++i) {
		IngredientSlot* slot = manf->getSlot(i);
		if (slot == nullptr || slot->isFull())
			continue;

		String wantType = cs36Get(tool, "slot." + String::valueOf(i) + ".type");
		if (wantType.isEmpty())
			continue; // leave empty; player fills manually

		int qtyNeeded = Integer::valueOf(cs36Get(tool, "slot." + String::valueOf(i) + ".qty"));

		Vector<ManagedReference<TangibleObject*> > candidates;

		if (slot->isResourceSlot()) {
			// BRIEF-042 (ERR-017): recurse into containers inside inventory.
			collectCandidates(inventory, true, wantType, "", candidates, 0);
		} else {
			String serial = "";
			int hashPos = wantType.lastIndexOf("#");
			if (hashPos != -1) {
				serial = wantType.subString(hashPos + 1);
				wantType = wantType.subString(0, hashPos);
			}
			collectCandidates(inventory, false, wantType, serial, candidates, 0);
		}

		// Sum total availability across all stacks BEFORE consuming anything
		// (BRIEF-042, ERR-017 symptom 2): no more silent partial drains.
		int available = 0;
		for (int j = 0; j < candidates.size(); ++j) {
			TangibleObject* cand = candidates.get(j).get();

			if (slot->isResourceSlot())
				available += cast<ResourceContainer*>(cand)->getQuantity();
			else
				available += cand->getUseCount() > 0 ? cand->getUseCount() : 1;
		}

		if (available < qtyNeeded) {
			// Missing / insufficient: consume NOTHING and say which resource
			// is short and by how much (brief requirement).
			player->sendSystemMessage("Repeat-craft: insufficient "
					+ (slot->isResourceSlot() ? String("resource ") : String("component "))
					+ wantType + " -- have " + String::valueOf(available)
					+ ", need " + String::valueOf(qtyNeeded)
					+ "; slot left empty for manual fill.");
			continue;
		}

		// Fill greedily across stacks until the slot reports full.
		for (int j = 0; j < candidates.size() && !slot->isFull(); ++j) {
			TangibleObject* found = candidates.get(j).get();
			Locker ingLock(found, player);
			session->addIngredient(found, i, 0);
		}

		if (!slot->isFull()) {
			// Slot accepted less than expected despite the availability check
			// (e.g. mixed-spawn rejection): report honestly rather than
			// pretending it filled.
			player->sendSystemMessage("Repeat-craft: could not fully fill slot "
					+ String::valueOf(i) + " (" + wantType + "); fill manually.");
		}
	}

	// BRIEF-042 (ERR-018): honor the stored experimentation allocation. The
	// session flow stops at state 2 (resources) so the player still presses
	// Assemble themselves; surface the allocation as an actionable reminder
	// instead of silently dropping the stored string.
	const String expAttempt = cs36Get(tool, "exp");
	if (!expAttempt.isEmpty()) {
		player->sendSystemMessage("Repeat-craft: stored experimentation allocation: " + expAttempt);
	}

	return QueueCommand::SUCCESS;
}
