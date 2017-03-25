// 
// serialization to memory routines.  A block of memory and a counter.
//
#pragma once
#include <cstdint>
#include <vector>
#include <memory.h>
#include <assert.h>
#include "EncodeStream.h"

// has an index supports virtual serialization
using serialization_id = uint32_t;

class RegisteredSerializable
{
	serialization_id m_id;
public:
	RegisteredSerializable() {}
	explicit RegisteredSerializable(serialization_id id)
		: m_id(id)
	{}
	void set_serialization_index(uint32_t id) { m_id = id; }
	serialization_id get_serialization_index() const { return m_id; }
	virtual void encode(EncodeStream &e) const = 0;
	virtual void decode(EncodeStream &e) = 0;
};

class SerializableRegistry
{
public:
	SerializableRegistry() {}
	uint32_t Register(RegisteredSerializable *p) 
	{ 
		registered.push_back(p); 
		return registered.size() - 1;
	}
	uint32_t GetNumRegistered() { return registered.size(); }
	std::vector <RegisteredSerializable *> registered;
};