/*
 * CustomSkillsSuiCallback.h
 */

#ifndef CUSTOMSKILLSSUICALLBACK_H_
#define CUSTOMSKILLSSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "CustomSkillsMenu.h"

class CustomSkillsSuiCallback : public SuiCallback {
private:
	CustomSkillsMenu::Page page;

public:
	CustomSkillsSuiCallback(ZoneServer* server, CustomSkillsMenu::Page menuPage)
		: SuiCallback(server), page(menuPage) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) override;
};

#endif // CUSTOMSKILLSSUICALLBACK_H_
