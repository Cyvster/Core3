#include "CustomSkillsSuiCallback.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

void CustomSkillsSuiCallback::run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
	if (player == nullptr || suiBox == nullptr || !suiBox->isListBox() || eventIndex == 1)
		return;

	// Script.listBox closes after every submitted action. Reopen the current
	// page for malformed or empty submissions so only Cancel exits the menu.
	if (args == nullptr || args->size() == 0) {
		CustomSkillsMenu::open(player, page);
		return;
	}

	int selectionArgument = 0;
	if (page != CustomSkillsMenu::MAIN) {
		if (args->size() < 2) {
			CustomSkillsMenu::open(player, page);
			return;
		}
		if (Bool::valueOf(args->get(0).toString())) {
			CustomSkillsMenu::open(player, CustomSkillsMenu::getParent(page));
			return;
		}
		selectionArgument = 1;
	}

	int selection = Integer::valueOf(args->get(selectionArgument).toString());
	SuiListBox* listBox = cast<SuiListBox*>(suiBox);
	if (selection < 0 || selection >= listBox->getMenuSize()) {
		CustomSkillsMenu::open(player, page);
		return;
	}

	CustomSkillsMenu::open(player, CustomSkillsMenu::getChild(page, selection));
}
