/*
 * CustomSkillsSuiCallback.cpp
 */

#include "CustomSkillsSuiCallback.h"

#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

void CustomSkillsSuiCallback::run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
	if (player == nullptr || suiBox == nullptr || !suiBox->isListBox() || eventIndex == 1 || args == nullptr || args->size() == 0)
		return;

	int selectionArgument = 0;

	if (page != CustomSkillsMenu::MAIN) {
		if (args->size() < 2)
			return;

		bool backPressed = Bool::valueOf(args->get(0).toString());

		if (backPressed) {
			CustomSkillsMenu::open(player, CustomSkillsMenu::MAIN);
			return;
		}

		selectionArgument = 1;
	}

	int selection = Integer::valueOf(args->get(selectionArgument).toString());
	SuiListBox* listBox = cast<SuiListBox*>(suiBox);

	if (selection < 0 || selection >= listBox->getMenuSize())
		return;

	if (page == CustomSkillsMenu::MAIN) {
		if (selection <= CustomSkillsMenu::ABOUT)
			CustomSkillsMenu::open(player, static_cast<CustomSkillsMenu::Page>(selection + 1));

		return;
	}

	// Category rows are informational in the menu-shell milestone.
	CustomSkillsMenu::open(player, page);
}
