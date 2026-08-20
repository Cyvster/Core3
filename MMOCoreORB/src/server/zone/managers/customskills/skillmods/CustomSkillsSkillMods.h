#ifndef CUSTOMSKILLSSKILLMODS_H_
#define CUSTOMSKILLSSKILLMODS_H_

#include "server/zone/objects/creature/variables/SkillModEntry.h"
#include "server/zone/objects/creature/variables/SkillModList.h"

class CustomSkillsSkillMods {
public:
	static int getSkillMod(CreatureObject* player, const SkillModList& list, const String& name, int nativeValue);
	static SkillModEntry getVisibleSkillMod(CreatureObject* player, const SkillModList& list, const String& name, const SkillModEntry& nativeEntry);
	static SkillModList getVisibleSkillModList(CreatureObject* player, const SkillModList& list);
	static void refreshVisibleSkillMods(CreatureObject* player);

private:
	static int getWearableAdjustment(CreatureObject* player, const SkillModList& list, const String& name);
};

#endif // CUSTOMSKILLSSKILLMODS_H_
