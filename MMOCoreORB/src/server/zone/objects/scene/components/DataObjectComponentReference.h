/*
 * DataObjectReference.h
 *
 *  Created on: 18/03/2012
 *      Author: victor
 */

#ifndef DATAOBJECCOMPONENTTREFERENCE_H_
#define DATAOBJECCOMPONENTTREFERENCE_H_

#include "engine/engine.h"

#include "engine/util/json_utils.h"

#include "DataObjectComponent.h"

class DataObjectComponentReference : public Reference<DataObjectComponent*> {
public:
	bool toBinaryStream(ObjectOutputStream* stream);
	bool parseFromBinaryStream(ObjectInputStream* stream);
	DataObjectComponent* operator=(DataObjectComponent* obj);

#ifdef ODB_SERIALIZATION
	// may inspect the stream to pick a component type but must restore the
	// offset before returning; returns nullptr when the type cannot be resolved
	typedef DataObjectComponent* (*ComponentResolver)(ObjectInputStream* stream);

	static void setComponentResolver(ComponentResolver resolver) {
		componentResolver = resolver;
	}

private:
	static ComponentResolver componentResolver;
#endif
};

void to_json(nlohmann::json& j, const DataObjectComponentReference& ref);

#endif /* DATAOBJECTREFERENCE_H_ */
