/*
 * SurveyToolSetRangeSuiCallback.h
 *
 *  Created on: Nov 3, 2010
 *      Author: crush
 */

#ifndef SURVEYTOOLSETRANGECALLBACK_H_
#define SURVEYTOOLSETRANGECALLBACK_H_


#include "server/zone/objects/tangible/tool/SurveyTool.h"
#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/managers/customskills/CustomSkillsConfig.h" // BRIEF-048 (mod hook)

// BRIEF-048 (mod hook): shared survey-range tier ladder (C06).
// Six tiers gated at the vanilla surveying skill levels; ranges run from
// the vanilla 64m base up to customSkillsConfig.surveying.maxRange
// (default 2624m; vanilla cap was 384m). With maxRange = 384 this ladder
// reproduces the exact vanilla tiers (64/128/192/256/320/384).
namespace SurveyRangeLadder {
const int TIER_COUNT = 6;
const int TIER_SKILLS[TIER_COUNT] = { 20, 35, 55, 75, 100, 120 };

inline int getMaxRange() {
	return CustomSkillsConfig::instance()->getSurveyMaxRange();
}

// Range for tier index (0-based). Interpolates 64m -> maxRange across the
// ladder, floored to a multiple of 64 so every tier lands on the vanilla
// granularity.
inline int tierRange(int index) {
	if (index < 0)
		return 0;
	if (index >= TIER_COUNT - 1)
		return getMaxRange();

	int maxRange = getMaxRange();
	int interpolated = 64 + (int)(((float)(maxRange - 64) * index) / (TIER_COUNT - 1));
	interpolated -= interpolated % 64;

	return Math::max(64, interpolated);
}

// Highest tier index the given surveying skill permits (-1 if none).
inline int tierForSkill(int skillLevel) {
	int best = -1;

	for (int i = 0; i < TIER_COUNT; ++i) {
		if (skillLevel >= TIER_SKILLS[i])
			best = i;
	}

	return best;
}

// Number of grid sample points per side for a given range (vanilla curve,
// extended: 6x6 above 1024m).
inline int pointsForRange(int range) {
	if (range >= 1024) {
		return 6;
	} else if (range >= 256) {
		return 5;
	} else if (range >= 128) {
		return 4;
	}

	return 3;
}
}

class SurveyToolSetRangeSuiCallback : public SuiCallback {
public:
	SurveyToolSetRangeSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed)
			return;

		if (args->size() < 1)
			return;

		ManagedReference<SurveyTool*> surveyTool = cast<SurveyTool*>(suiBox->getUsingObject().get().get());

		if(surveyTool == nullptr || player == nullptr)
			return;

		// BRIEF-048 (mod hook): resolve the selected row against the same
		// skill-gated ladder the menu was built from, instead of the old
		// 64 * index + 64 arithmetic that drifted from the option labels.
		int selectedIndex = Integer::valueOf(args->get(0).toString());
		int tier = SurveyRangeLadder::tierForSkill(player->getSkillMod("surveying"));

		if (selectedIndex < 0 || selectedIndex > tier)
			return;

		int range = SurveyRangeLadder::tierRange(selectedIndex);

		Locker _lock(surveyTool);
		surveyTool->setRange(range);
	}
};

#endif /* SURVEYTOOLSETRANGECALLBACK_H_ */
