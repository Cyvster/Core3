/* Portable, server-side custom skills menu. */
#ifndef CUSTOMSKILLSMENU_H_
#define CUSTOMSKILLSMENU_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "CustomSkillsModifierType.h"

class CustomSkillsMenu {
public:
	enum Page {
		MAIN, BADGES, MILESTONES, EXPLORATION, PROFESSION, QUEST, EVENT,
		EXPLORATION_MILESTONES, CORELLIA, DANTOOINE, DATHOMIR, ENDOR, LOK, NABOO, RORI, TALUS, TATOOINE, YAVIN4,
		PROFESSION_COMBAT, PROFESSION_CRAFTING, PROFESSION_OUTDOORS, PROFESSION_SCIENCE, PROFESSION_SOCIAL, PROFESSION_PILOT,
		QUEST_HERO, QUEST_WARREN, QUEST_THEME_PARKS, QUEST_CORVETTE,
		EVENT_COA, EVENT_ACCOLADES, EVENT_LIBRARIAN, EVENT_RACING, EVENT_DEATH_STAR,
		BONUSES, BONUS_COMBAT, BONUS_UTILITY, BONUS_CRAFTING,
		BONUS_CRIT_CHANCE, BONUS_CRIT_MULTI, BONUS_DOUBLE, BONUS_TRIPLE, BONUS_QUAD, BONUS_ARMOR_PEN, BONUS_DEF_CAP,
		BONUS_ARMOR_DEGRADE, BONUS_WEAPON_DEGRADE, BONUS_SEA_CAP, BONUS_MOVE_SPEED, BONUS_BUFF_DUR, BONUS_EXP_BONUS, BONUS_GATHERING,
		BONUS_PRACTICE_XP, BONUS_CRAFT_SPEED, BONUS_AMAZING_SUCCESS, BONUS_AMAZING_RESULTS,
		SERVER_CONFIG, MOD_OPTIONS, RARITY_NAMING, SWGEMU_OPTIONS
	};

	static void open(CreatureObject* player, Page page = MAIN);
	static Page getParent(Page page);
	static Page getChild(Page page, int selection);
	static bool hasChildPages(Page page);

private:
	static void addPageItems(SuiListBox* box, CreatureObject* player, Page page);
	static void addBadgeItems(SuiListBox* box, CreatureObject* player, const char* const* keys, int count);
	static void addBonusItems(SuiListBox* box, CreatureObject* player, CustomSkillsModifierType::Type type);
	static void addCategoryItem(SuiListBox* box, CreatureObject* player, const String& name, Page category, bool showBonuses = true);
	static int getAcquiredCount(CreatureObject* player, Page page);
	static int countAcquired(CreatureObject* player, const char* const* keys, int count);
	static int countOwnedBonuses(CreatureObject* player, CustomSkillsModifierType::Type type);
	static int getModifierTotal(CreatureObject* player, Page page, CustomSkillsModifierType::Type type);
	static int countModifier(CreatureObject* player, const char* const* keys, int count, CustomSkillsModifierType::Type type);
	static String getPromptText(CreatureObject* player, Page page);
	static void addSwgemuOptionItems(SuiListBox* box);
	static void addModOptionItems(SuiListBox* box); // BRIEF-051 (mod hook)
	static String getTitle(Page page);
	static int countEnabledOptions();
};
#endif
