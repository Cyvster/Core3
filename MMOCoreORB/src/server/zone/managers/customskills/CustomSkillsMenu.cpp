/*
 * CustomSkillsMenu.cpp
 */

#include "CustomSkillsMenu.h"
#include "CustomSkillsSuiCallback.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

void CustomSkillsMenu::open(CreatureObject* player, Page page) {
	if (player == nullptr)
		return;

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr)
		return;

	int boxType = page == MAIN ? SuiListBox::HANDLETWOBUTTON : SuiListBox::HANDLETHREEBUTTON;
	ManagedReference<SuiListBox*> box = new SuiListBox(player, SuiWindowType::NONE, boxType);
	box->setCallback(new CustomSkillsSuiCallback(player->getZoneServer(), page));
	box->setCancelButton(true, "@cancel");
	box->setOkButton(true, "@ok");

	if (page == MAIN) {
		box->setPromptTitle("Custom Skills - Overall Summary");
		box->setPromptText("Select a category to view its character bonuses. Values are placeholders for menu testing.");
	} else {
		box->setOtherButton(true, "@back");

		switch (page) {
		case COMBAT:
			box->setPromptTitle("Custom Skills > Combat");
			break;
		case DEFENSE:
			box->setPromptTitle("Custom Skills > Defense");
			break;
		case UTILITY:
			box->setPromptTitle("Custom Skills > Utility");
			break;
		case CRAFTING:
			box->setPromptTitle("Custom Skills > Crafting");
			break;
		case ABOUT:
			box->setPromptTitle("Custom Skills > About");
			break;
		default:
			return;
		}

		box->setPromptText("Placeholder values only. Badge-derived modifiers will be connected in a later milestone.");
	}

	addPageItems(box, page);
	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());
}

void CustomSkillsMenu::addPageItems(SuiListBox* box, Page page) {
	switch (page) {
	case MAIN:
		box->addMenuItem("Combat Bonuses");
		box->addMenuItem("Defense Bonuses");
		box->addMenuItem("Utility Bonuses");
		box->addMenuItem("Crafting Bonuses");
		box->addMenuItem("About This System");
		break;
	case COMBAT:
		box->addMenuItem("Critical Chance                         +0.00%");
		box->addMenuItem("Critical Damage                         +0.00%");
		box->addMenuItem("General Damage                          +0.00%");
		break;
	case DEFENSE:
		box->addMenuItem("General Defense                         +0");
		box->addMenuItem("Melee Defense                           +0");
		box->addMenuItem("Ranged Defense                          +0");
		break;
	case UTILITY:
		box->addMenuItem("Movement Speed                          +0.00%");
		box->addMenuItem("Experience Gain                         +0.00%");
		box->addMenuItem("Resource Yield                          +0.00%");
		break;
	case CRAFTING:
		box->addMenuItem("Assembly                                +0");
		box->addMenuItem("Experimentation                         +0");
		box->addMenuItem("Crafting Success                        +0.00%");
		break;
	case ABOUT:
		box->addMenuItem("Character-specific bonuses granted by custom badges.");
		break;
	}
}
