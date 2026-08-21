#include "CustomSkillsModifiers.h"
#include "CustomSkillsConfig.h"

#include "server/zone/managers/player/BadgeList.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/customskills/skillmods/CustomSkillsSkillMods.h"

namespace {
	const char* const combatProfessionBadges[] = {
		"combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master",
		"combat_brawler_master", "combat_carbine_master", "combat_commando_master",
		"combat_marksman_master", "combat_pistol_master", "combat_polearm_master",
		"combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master"
	};

	static_assert(sizeof(combatProfessionBadges) / sizeof(combatProfessionBadges[0]) == 12,
		"Critical Chance must be assigned to exactly 12 combat profession badges");
}

bool CustomSkillsModifiers::isCombatProfessionBadge(const String& badgeKey) {
	for (const char* key : combatProfessionBadges) {
		if (badgeKey == key)
			return true;
	}

	return false;
}

int CustomSkillsModifiers::getBadgeCriticalChance(const String& badgeKey) {
	CustomSkillsConfig* config = CustomSkillsConfig::instance();
	return config->isCriticalChanceEnabled() && isCombatProfessionBadge(badgeKey) ?
		config->getCriticalChancePerCombatBadge() : 0;
}

bool CustomSkillsModifiers::isCriticalChanceEnabled() {
	return CustomSkillsConfig::instance()->isCriticalChanceEnabled();
}

int CustomSkillsModifiers::getCriticalChance(PlayerObject* ghost) {
	return getCriticalChance(ghost, combatProfessionBadges, sizeof(combatProfessionBadges) / sizeof(combatProfessionBadges[0]));
}

int CustomSkillsModifiers::getCriticalChance(PlayerObject* ghost, const char* const* badgeKeys, int count) {
	if (ghost == nullptr || badgeKeys == nullptr || count <= 0)
		return 0;

	const BadgeList* badgeList = BadgeList::instance();
	int chance = 0;

	for (int i = 0; i < count; ++i) {
		const Badge* badge = badgeList->get(badgeKeys[i]);
		if (badge != nullptr && ghost->hasBadge(badge->getIndex()))
			chance += getBadgeCriticalChance(badge->getKey());
	}

	return Math::min(chance, 10000);
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
		"Critical Chance", "Critical Multiplier",
		"Double Attack Chance", "Triple Attack Chance", "Quad Attack Chance",
		"Armor Penetration", "Defense Cap", "Armor Degrade Reduction", "Weapon Degrade Reduction",
		"SEA Cap", "Movement Speed", "Buff Duration", "Experience Bonus", "Practice Experience Bonus",
		"Crafting Speed", "Amazing Success Chance", "Amazing Results", "Gathering Quantity"
	};
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

	while (valueStr.length() < 14)
		valueStr += " ";

	return valueStr + getModifierName(type);
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
