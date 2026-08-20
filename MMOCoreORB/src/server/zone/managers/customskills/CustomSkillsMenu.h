/*
 * CustomSkillsMenu.h
 * Portable, server-side custom skills menu.
 */

#ifndef CUSTOMSKILLSMENU_H_
#define CUSTOMSKILLSMENU_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

class CustomSkillsMenu {
public:
	enum Page {
		MAIN = 0,
		COMBAT = 1,
		DEFENSE = 2,
		UTILITY = 3,
		CRAFTING = 4,
		ABOUT = 5
	};

	static void open(CreatureObject* player, Page page = MAIN);

private:
	static void addPageItems(SuiListBox* box, Page page);
};

#endif // CUSTOMSKILLSMENU_H_
