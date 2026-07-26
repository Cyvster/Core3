/*
 * DataObjectComponentReference.cpp
 *
 *  Created on: 18/03/2012
 *      Author: victor
 */

#include "DataObjectComponent.h"
#include "DataObjectComponentReference.h"

#ifdef ODB_SERIALIZATION
DataObjectComponentReference::ComponentResolver DataObjectComponentReference::componentResolver = nullptr;
#endif

bool DataObjectComponentReference::toBinaryStream(ObjectOutputStream* stream) {
	DataObjectComponent* object = Reference<DataObjectComponent*>::get();

	if (object != nullptr)
		object->toBinaryStream(stream);
	else
		stream->writeShort(0);

	return true;
}

bool DataObjectComponentReference::parseFromBinaryStream(ObjectInputStream* stream) {
	DataObjectComponent* object = Reference<DataObjectComponent*>::get();

#ifdef ODB_SERIALIZATION
	if (object == nullptr && componentResolver != nullptr) {
		object = componentResolver(stream);

		if (object != nullptr)
			updateObject(object);
	}
#endif

	if (object == nullptr) {
		stream->readShort();
	} else {
		// records saved before the class name was stamped carry an empty
		// _className; keep the factory-assigned name instead of the disk value
		String className = object->_getClassName();

		object->parseFromBinaryStream(stream);

		if (object->_getClassName().isEmpty())
			object->_setClassName(className);
	}

	return true;
}

DataObjectComponent* DataObjectComponentReference::operator= (DataObjectComponent* obj) {
	updateObject(obj);

	return obj;
}

void to_json(nlohmann::json& j, const DataObjectComponentReference& ref) {
	auto object = ref.get();

	if (object != nullptr)
		object->writeJSON(j);
	else
		j = {};

}
