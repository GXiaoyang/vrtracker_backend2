// 
// serialization to memory routines.  A block of memory and a counter.
//
#pragma once
#include <cstdint>
#include <vector>
#include <memory.h>
#include <assert.h>
#include "url_named.h"
#include "tbb/concurrent_vector.h"
#include "EncodeStream.h"


// has an index supports virtual serialization
using serialization_id = uint16_t;

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
	virtual const base::URL& get_serialization_url(void) const = 0;
	virtual void encode(EncodeStream &e) const = 0;
	virtual void decode(EncodeStream &e) = 0;
};

// register objects by an id so they can be found for serialization and deserialization
class SerializableRegistry
{
public:
	SerializableRegistry() {}

	// Register is multiple writer safe since the update pass can be multi-threaded
	serialization_id Register(RegisteredSerializable *p)
	{ 
		auto iter = registered.push_back(p); 
		serialization_id id = std::distance(registered.begin(), iter);
		return id;
	}
	void dump() const;
	
	serialization_id GetNumRegistered() { return static_cast<serialization_id>(registered.size()); }
	tbb::concurrent_vector <RegisteredSerializable *> registered;
};
