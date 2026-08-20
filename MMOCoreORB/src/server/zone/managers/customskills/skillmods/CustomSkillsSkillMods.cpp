#include "CustomSkillsSkillMods.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/managers/skill/SkillModManager.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/variables/SkillModList.h"

int CustomSkillsSkillMods::getWearableAdjustment(CreatureObject* player, const SkillModList& list, const String& name) {
	const int increase = CustomSkillsModifiers::getModifierTotal(player, CustomSkillsModifierType::SEA_CAP_INCREASE);
	if (increase <= 0)
		return 0;

	const int raw = list.getRawSkillModOfType(name, SkillModManager::WEARABLE);
	if (raw <= 25)
		return 0;

	return Math::min(raw, 25 + increase) - 25;
}

int CustomSkillsSkillMods::getSkillMod(CreatureObject* player, const SkillModList& list, const String& name, int nativeValue) {
	return nativeValue + getWearableAdjustment(player, list, name);
}

SkillModEntry CustomSkillsSkillMods::getVisibleSkillMod(CreatureObject* player, const SkillModList& list,
		const String& name, const SkillModEntry& nativeEntry) {
	SkillModEntry result(nativeEntry);
	result.setSkillBonus(result.getSkillBonus() + getWearableAdjustment(player, list, name));
	return result;
}

SkillModList CustomSkillsSkillMods::getVisibleSkillModList(CreatureObject* player, const SkillModList& list) {
	SkillModList result(list);
	const SkillModGroup* wearable = list.getSkillModGroupIfPresent(SkillModManager::WEARABLE);
	if (wearable == nullptr)
		return result;

	for (int i = 0; i < wearable->size(); ++i) {
		const String& name = wearable->elementAt(i).getKey();
		SkillModEntry entry = getVisibleSkillMod(player, list, name, list.getVisibleSkillMod(name));
		if (entry.getTotalSkill() != 0)
			result.set(name, entry);
		else
			result.drop(name);
	}
	return result;
}

void CustomSkillsSkillMods::refreshVisibleSkillMods(CreatureObject* player) {
	if (player == nullptr)
		return;

	SkillModList* list = player->getSkillModList();
	SkillModGroup* wearable = list->getSkillModGroup(SkillModManager::WEARABLE);
	Vector<String> names;
	for (int i = 0; i < wearable->size(); ++i)
		names.add(wearable->elementAt(i).getKey());

	for (int i = 0; i < names.size(); ++i)
		player->addSkillMod(SkillModManager::WEARABLE, names.get(i), 0, true);
}
