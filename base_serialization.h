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
#include "tbb/spin_mutex.h"
#include "EncodeStream.h"
#include <map>

// has an index supports virtual serialization
using serialization_id = uint16_t;

class RegisteredSerializable
{
protected:
	void encode_id(EncodeStream &e) const
	{
		e.memcpy_out_to_stream(&m_id, sizeof(m_id));
	}
	void decode_id(EncodeStream &e)
	{
		e.memcpy_from_stream(&m_id, sizeof(m_id));
	}

	serialization_id m_id;
public:
	RegisteredSerializable() {}
	explicit RegisteredSerializable(serialization_id id)
		: m_id(id)
	{}

	bool operator==(const RegisteredSerializable &rhs) const
	{
		if (&rhs == this)
			return true;
		return m_id == rhs.m_id;
	}
	
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
	SerializableRegistry(const SerializableRegistry&) = delete;
	SerializableRegistry &operator=(const SerializableRegistry&) = delete;

	bool operator==(const SerializableRegistry&) const;
	bool operator!=(const SerializableRegistry& rhs) const
	{
		return !(*this == rhs);
	}

	// Register is multiple writer safe since the update pass can be multi-threaded
	serialization_id Register(RegisteredSerializable *p)
	{
#ifdef _DEBUG
		map_lock.lock();
		map.find(p->get_serialization_url());
		assert(map.find(p->get_serialization_url()) == map.end());
		map.insert({ p->get_serialization_url(), p });
		map_lock.unlock(); 
#endif
		auto iter = registered.push_back(p); 
		serialization_id id = std::distance(registered.begin(), iter);
		p->set_serialization_index(id);
		return id;
	}

	void dump() const;
#ifdef _DEBUG
	tbb::spin_mutex map_lock;
	std::map<base::URL, RegisteredSerializable*> map;
#endif
	serialization_id GetNumRegistered() { return static_cast<serialization_id>(registered.size()); }
	tbb::concurrent_vector <RegisteredSerializable *> registered;
};
