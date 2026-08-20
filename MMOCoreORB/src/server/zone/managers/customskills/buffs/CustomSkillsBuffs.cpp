#include "CustomSkillsBuffs.h"

#include "server/zone/managers/customskills/CustomSkillsModifierType.h"
#include "server/zone/managers/customskills/CustomSkillsModifiers.h"
#include "server/zone/objects/creature/buffs/Buff.h"
#include "server/zone/objects/creature/buffs/BuffCRC.h"
#include "server/zone/objects/creature/buffs/BuffType.h"
#include "server/zone/objects/creature/buffs/SingleUseBuff.h"

bool CustomSkillsBuffs::isEligible(Buff* buff) {
	if (buff == nullptr)
		return false;
	if (dynamic_cast<SingleUseBuff*>(buff) != nullptr)
		return false;

	const int type = buff->getBuffType();

	if (type == BuffType::FOOD || type == BuffType::MEDICAL || type == BuffType::PERFORMANCE)
		return true;

	if (type == BuffType::SPICE) {
		switch (buff->getBuffCRC()) {
		case BuffCRC::SPICE_BOOSTER_BLUE_UP:
		case BuffCRC::SPICE_CRASH_N_BURN_UP:
		case BuffCRC::SPICE_DROID_LUBE_UP:
		case BuffCRC::SPICE_GIGGLEDUST_UP:
		case BuffCRC::SPICE_GREY_GABAKI_UP:
		case BuffCRC::SPICE_GUNJACK_UP:
		case BuffCRC::SPICE_MUON_GOLD_UP:
		case BuffCRC::SPICE_NEUTRON_PIXEY_UP:
		case BuffCRC::SPICE_PYREPENOL_UP:
		case BuffCRC::SPICE_SCRAMJET_UP:
		case BuffCRC::SPICE_SEDATIVE_H4B_UP:
		case BuffCRC::SPICE_SHADOWPAW_UP:
		case BuffCRC::SPICE_SWEETBLOSSOM_UP:
		case BuffCRC::SPICE_THRUSTER_HEAD_UP:
		case BuffCRC::SPICE_YARROCK_UP:
		case BuffCRC::SPICE_KLIKNIK_BOOST_UP:
		case BuffCRC::SPICE_KWI_BOOST_UP:
			return true;
		default:
			return false;
		}
	}

	if (type == BuffType::JEDI) {
		const uint32 crc = buff->getBuffCRC();
		return crc != STRING_HASHCODE("forceweaken1") &&
			crc != STRING_HASHCODE("forceweaken2") &&
			crc != STRING_HASHCODE("private_force_regen_debuff") &&
			crc != STRING_HASHCODE("private_groggy_debuff");
	}

	return false;
}

float CustomSkillsBuffs::getDuration(CreatureObject* recipient, Buff* buff, float nativeDuration) {
	if (recipient == nullptr || nativeDuration <= 0 || !isEligible(buff))
		return nativeDuration;

	const int bonus = CustomSkillsModifiers::getModifierTotal(recipient, CustomSkillsModifierType::BUFF_DURATION);
	if (bonus <= 0)
		return nativeDuration;
	return nativeDuration * (10000.f + bonus) / 10000.f;
}
