/*
 * PerformanceBuffImplementation.cpp
 *
 *  Created on: 04/03/2011
 *      Author: Itac
 */

#include "server/zone/objects/creature/buffs/PerformanceBuff.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "templates/params/creature/CreatureAttribute.h"
#include "server/zone/objects/creature/buffs/PerformanceBuffType.h"

void PerformanceBuffImplementation::activate(bool applyModifiers) {

	if(type == PerformanceBuffType::DANCE_MIND) {
		int mindStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::MIND)) +1000;
		int focusStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::FOCUS) +750);
		int willStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::WILLPOWER)) +750;
		setAttributeModifier(CreatureAttribute::MIND, mindStrength);
		setAttributeModifier(CreatureAttribute::FOCUS, focusStrength);
		setAttributeModifier(CreatureAttribute::WILLPOWER, willStrength);
		creature.get()->sendSystemMessage("@healing:performance_enhance_dance_mind_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_focus_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_willpower_d");

	}
	else if(type == PerformanceBuffType::MUSIC_FOCUS) {
		int mindStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::MIND)) +1000;
		int focusStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::FOCUS) +750);
		int willStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::WILLPOWER)) +750;
		setAttributeModifier(CreatureAttribute::MIND, mindStrength);
		setAttributeModifier(CreatureAttribute::FOCUS, focusStrength);
		setAttributeModifier(CreatureAttribute::WILLPOWER, willStrength);
		creature.get()->sendSystemMessage("@healing:performance_enhance_dance_mind_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_focus_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_willpower_d");

	}
	else if(type == PerformanceBuffType::MUSIC_WILLPOWER) {
		int mindStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::MIND)) +1000;
		int focusStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::FOCUS) +750);
		int willStrength = round(strength * (float)creature.get()->getBaseHAM(CreatureAttribute::WILLPOWER)) +750;
		setAttributeModifier(CreatureAttribute::MIND, mindStrength);
		setAttributeModifier(CreatureAttribute::FOCUS, focusStrength);
		setAttributeModifier(CreatureAttribute::WILLPOWER, willStrength);
		creature.get()->sendSystemMessage("@healing:performance_enhance_dance_mind_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_focus_d");
		creature.get()->sendSystemMessage("@healing:performance_enhance_music_willpower_d");
	}

	BuffImplementation::activate(true);

}

void PerformanceBuffImplementation::deactivate(bool removeModifiers) {
	BuffImplementation::deactivate(true);
}
