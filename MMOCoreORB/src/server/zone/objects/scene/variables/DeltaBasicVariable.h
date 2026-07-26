/*
 * DeltaVariable.h
 *
 *  Created on: 18/10/2013
 *      Author: victor
 */

#ifndef DELTABASICVARIABLE_H_
#define DELTABASICVARIABLE_H_

#include "engine/engine.h"
#include "engine/util/json_utils.h"

template<typename K>
class DeltaBasicVariable {
protected:
	K object;
public:
	bool toBinaryStream(ObjectOutputStream* stream) {
		return TypeInfo<K>::toBinaryStream(&object, stream);
	}

	bool parseFromBinaryStream(ObjectInputStream* stream) {
		return TypeInfo<K>::parseFromBinaryStream(&object, stream);
	}

	inline K operator|=(K obj) {
		return object |= obj;
	}

	inline K operator^=(K obj) {
		return object ^= obj;
	}

	inline K operator&=(K obj) {
		return object &= obj;
	}

	inline K operator>>=(K obj) {
		return object >>= obj;
	}

	inline K operator<<=(K obj) {
		return object <<= obj;
	}

	inline K operator%=(K obj) {
		return object %= obj;
	}

	inline K operator/=(K obj) {
		return object /= obj;
	}

	inline K operator*=(K obj) {
		return object *= obj;
	}

	inline K operator+=(K obj) {
		return object += obj;
	}

	inline K operator++(int) {
		return object++;
	}

	inline K operator--(int) {
		return object--;
	}

	inline K operator++() {
		return ++object;
	}

	inline K operator--() {
		return --object;
	}

	inline K operator-=(K obj) {
		return object -= obj;
	}

	inline void operator=(K obj) {
		object = obj;
	}

	inline void set(K obj) {
		object = obj;
	}

	inline K get() const {
		return object;
	}

	inline operator K() const {
		return object;
	}
};

template<typename K>
void to_json(nlohmann::json& j, const DeltaBasicVariable<K>& var) {
	j = var.get();
}

#endif /* DELTAVARIABLE_H_ */
