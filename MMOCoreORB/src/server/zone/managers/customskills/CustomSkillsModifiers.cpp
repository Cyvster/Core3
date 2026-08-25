#include "CustomSkillsModifiers.h"
#include "CustomSkillsConfig.h"

#include "server/zone/managers/player/BadgeList.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.h"

bool CustomSkillsModifiers::isCriticalChanceEnabled() {
	return CustomSkillsConfig::instance()->isCriticalChanceEnabled();
}

int CustomSkillsModifiers::getCriticalChance(PlayerObject* ghost) {
	if (ghost == nullptr)
		return 0;

	CustomSkillsConfig* config = CustomSkillsConfig::instance();
	if (!config->isCriticalChanceEnabled())
		return 0;

	const BadgeList* badgeList = BadgeList::instance();
	const VectorMap<String, int>& bonuses = config->getBadgeBonuses(CustomSkillsModifierType::CRITICAL_CHANCE);
	int total = 0;
	for (int i = 0; i < bonuses.size(); ++i) {
		const Badge* badge = badgeList->get(bonuses.elementAt(i).getKey());
		if (badge != nullptr && ghost->hasBadge(badge->getIndex()))
			total += bonuses.elementAt(i).getValue();
	}

	int cap = config->getModifierCap(CustomSkillsModifierType::CRITICAL_CHANCE);
	return cap > 0 ? Math::min(total, cap) : total;
}

int CustomSkillsModifiers::getCriticalMultiplier(CreatureObject* player) {
	if (player == nullptr)
		return 10000;

	int base = CustomSkillsConfig::instance()->getCriticalMultiplier();
	int bonus = getModifierTotal(player, CustomSkillsModifierType::CRITICAL_MULTIPLIER);
	return base + bonus;
}

String CustomSkillsModifiers::formatPercent(int basisPoints) {
	int absolute = basisPoints < 0 ? -basisPoints : basisPoints;
	String fraction = String::valueOf(absolute % 100);
	if (fraction.length() < 2)
		fraction = "0" + fraction;

	return (basisPoints < 0 ? "-" : "") + String::valueOf(absolute / 100) + "." + fraction + "%";
}

String CustomSkillsModifiers::colorizeCriticalText(const String& text) {
	return "\\#" + CustomSkillsConfig::instance()->getCustomSummaryColor() + text + "\\#.";
}

int CustomSkillsModifiers::getBadgeModifier(const String& badgeKey, CustomSkillsModifierType::Type type) {
	return CustomSkillsConfig::instance()->getBadgeBonus(type, badgeKey);
}

int CustomSkillsModifiers::getModifierTotal(CreatureObject* player, CustomSkillsModifierType::Type type) {
	if (player == nullptr || !player->isPlayerCreature())
		return 0;

	PlayerObject* ghost = player->getPlayerObject();
	CustomSkillsConfig* config = CustomSkillsConfig::instance();
	if (ghost == nullptr || !config->isModifierEnabled(type))
		return 0;

	const BadgeList* badgeList = BadgeList::instance();
	const VectorMap<String, int>& bonuses = config->getBadgeBonuses(type);
	int total = 0;
	for (int i = 0; i < bonuses.size(); ++i) {
		const Badge* badge = badgeList->get(bonuses.elementAt(i).getKey());
		if (badge != nullptr && ghost->hasBadge(badge->getIndex()))
			total += bonuses.elementAt(i).getValue();
	}

	int cap = config->getModifierCap(type);
	return cap > 0 ? Math::min(total, cap) : total;
}

bool CustomSkillsModifiers::isModifierEnabled(CustomSkillsModifierType::Type type) {
	return CustomSkillsConfig::instance()->isModifierEnabled(type);
}

int CustomSkillsModifiers::applyModifierCap(CustomSkillsModifierType::Type type, int value) {
	int cap = CustomSkillsConfig::instance()->getModifierCap(type);
	return cap > 0 ? Math::min(value, cap) : value;
}

String CustomSkillsModifiers::getModifierName(CustomSkillsModifierType::Type type) {
	static const char* names[] = {
		"Cri", "Crit Multi",
		"Double", "Triple", "Quad",
		"Armor Penetration", "Defense Cap", "Armor Degrade Reduction", "Weapon Degrade Reduction",
		"SEA Cap", "Movement Speed", "Buff Duration", "Experience Bonus", "Practice Experience Bonus",
		"Crafting Speed", "Amazing Success Chance", "Amazing Results", "Gathering Quantity"
	};
	static_assert(sizeof(names) / sizeof(names[0]) == static_cast<size_t>(CustomSkillsModifierType::COUNT), "names[] out of sync with CustomSkillsModifierType enum");
	return type >= 0 && type < CustomSkillsModifierType::COUNT ? names[type] : "Unknown Modifier";
}

String CustomSkillsModifiers::formatModifierBonus(CustomSkillsModifierType::Type type, int value) {
	String valueStr;
	if (type == CustomSkillsModifierType::ARMOR_PENETRATION ||
			type == CustomSkillsModifierType::DEFENSE_CAP_INCREASE ||
			type == CustomSkillsModifierType::SEA_CAP_INCREASE)
		valueStr = "+" + String::valueOf(value);
	else
		valueStr = "+" + formatPercent(value);

	return valueStr;
}

void CustomSkillsModifiers::notifyBadgeAwarded(CreatureObject* player) {
	if (player == nullptr)
		return;

	CustomSkillsSkillMods::refreshVisibleSkillMods(player);
	player->updateRunSpeed();
}

bool CustomSkillsModifiers::applyRarityNaming(TangibleObject* object, float excMod, float legendaryThreshold, float exceptionalThreshold) {
	CustomSkillsConfig* config = CustomSkillsConfig::instance();
	if (!config->isRarityNamingEnabled())
		return false;

	String color;
	if (excMod >= legendaryThreshold)
		color = config->getLegendaryColor();
	else if (excMod >= exceptionalThreshold)
		color = config->getExceptionalColor();
	else
		return false;

	String coloredName = "\\#" + color + object->getDisplayedName() + "\\#.";
	object->setCustomObjectName(coloredName, false);
	object->addMagicBit(false);
	return true;
}
