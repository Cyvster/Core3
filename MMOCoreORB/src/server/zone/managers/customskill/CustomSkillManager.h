/*
 * CustomSkillManager.h
 *
 * Self-contained custom skill system. This system is intentionally separate
 * from Core3's existing SkillManager and the normal SWG skill tree.
 */
#ifndef CUSTOMSKILLMANAGER_H_
#define CUSTOMSKILLMANAGER_H_

#include "engine/engine.h"
#include <vector>

namespace server {
namespace zone {
namespace objects {
namespace creature {
class CreatureObject;
}
}
}
}

using namespace server::zone::objects::creature;

namespace server {
namespace zone {
namespace managers {
namespace customskill {

struct CustomSkillDefinition {
	String id;
	String name;
	String description;
	int maxLevel;

	CustomSkillDefinition() : maxLevel(1) {
	}

	CustomSkillDefinition(const String& skillId, const String& skillName, const String& skillDescription, int maximumLevel)
		: id(skillId), name(skillName), description(skillDescription), maxLevel(maximumLevel) {
	}
};

class CustomSkillManager : public Singleton<CustomSkillManager>, public Logger, public Object {
	std::vector<CustomSkillDefinition> definitions;

	static const String SCREENPLAY;

	void loadDefaultDefinitions();

public:
	CustomSkillManager();

	void openMenu(CreatureObject* player);

	int getSkillLevel(CreatureObject* player, const String& skillId) const;
	void setSkillLevel(CreatureObject* player, const String& skillId, int level) const;

	const CustomSkillDefinition* getDefinition(int index) const;
	int getDefinitionCount() const;
};

}
}
}
}

using namespace server::zone::managers::customskill;

#endif /* CUSTOMSKILLMANAGER_H_ */
