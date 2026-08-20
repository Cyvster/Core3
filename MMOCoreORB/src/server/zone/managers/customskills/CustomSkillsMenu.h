/* Portable, server-side custom skills menu. */
#ifndef CUSTOMSKILLSMENU_H_
#define CUSTOMSKILLSMENU_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

class CustomSkillsMenu {
public:
	enum Page {
		MAIN, BADGES, MILESTONES, EXPLORATION, PROFESSION, QUEST, EVENT,
		EXPLORATION_MILESTONES, CORELLIA, DANTOOINE, DATHOMIR, ENDOR, LOK, NABOO, RORI, TALUS, TATOOINE, YAVIN4,
		PROFESSION_COMBAT, PROFESSION_CRAFTING, PROFESSION_OUTDOORS, PROFESSION_SCIENCE, PROFESSION_SOCIAL, PROFESSION_PILOT,
		QUEST_HERO, QUEST_WARREN, QUEST_THEME_PARKS, QUEST_CORVETTE,
		EVENT_COA, EVENT_ACCOLADES, EVENT_LIBRARIAN, EVENT_RACING, EVENT_DEATH_STAR
	};

	static void open(CreatureObject* player, Page page = MAIN);
	static Page getParent(Page page);
	static Page getChild(Page page, int selection);
	static bool hasChildPages(Page page);

private:
	static void addPageItems(SuiListBox* box, CreatureObject* player, Page page);
	static void addBadgeItems(SuiListBox* box, CreatureObject* player, const char* const* keys, int count);
	static String getTitle(Page page);
};
#endif
