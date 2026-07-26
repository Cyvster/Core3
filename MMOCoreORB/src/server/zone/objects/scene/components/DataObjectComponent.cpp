/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#include "DataObjectComponent.h"
#include "server/zone/objects/scene/SceneObject.h"

DataObjectComponent::DataObjectComponent() {

}

DataObjectComponent::~DataObjectComponent() {

}

void DataObjectComponent::initializeTransientMembers() {

}

void DataObjectComponent::notifyObjectDestroyingFromDatabase() {

}

int DataObjectComponent::writeClassNameMember(ObjectOutputStream* stream) {
	String name = "_className";
	name.toBinaryStream(stream);

	int offset = stream->getOffset();
	stream->writeInt(0);

	_getClassName().toBinaryStream(stream);

	stream->writeInt(offset, (uint32) (stream->getOffset() - (offset + 4)));

	return 1;
}

bool DataObjectComponent::readClassNameMember(ObjectInputStream* stream, const String& name) {
	if (name != "_className")
		return false;

	String className;
	className.parseFromBinaryStream(stream);

	if (!className.isEmpty())
		_setClassName(className);

	return true;
}

void DataObjectComponent::setParent(SceneObject* object) {
	parent = object;
}

SceneObject* DataObjectComponent::getParent() {
	return parent.get().get();
}

void to_json(nlohmann::json& j, const DataObjectComponent& comp) {
	comp.writeJSON(j);
}
