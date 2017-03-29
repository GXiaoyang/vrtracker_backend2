#pragma once
// StringIndexer
// * Utility class to index strings to monotonically increasing indexes
//   used by other classes (e.g. OverlayIndexer, ApplicationsIndexer)
//
#include <atomic>
#include "openvr_serialization.h"
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/spin_rw_mutex.h"
#include "string2int.h"

// knows how to map indexes to application keys
class StringIndexer
{
public:
	StringIndexer()
	{}

	bool operator == (const StringIndexer &rhs) const
	{
		if (this == &rhs)
			return true;
		if (updated_size != rhs.updated_size)
			return false;
		if (keys2index.size() != rhs.keys2index.size())
			return false;
		if (keys != rhs.keys)
		{
			for (int i = 0; i < keys.size(); i++)
			{
				std::string a = keys[i];
				std::string b = rhs.keys[i];
				if (a != b)
				{
					printf("%s %s\n", a.c_str(), b.c_str());
				}
			}
			return false;
		}
			
		if (live_indexes != rhs.live_indexes)
			return false;

		for (const auto & pair : keys2index)
		{
			const char *f = pair.first;
			auto iter = rhs.keys2index.find(f);
			if ( iter == rhs.keys2index.end() || iter->second != pair.second)
			{
				return false;
			}
		}
		return true;
	}

	bool operator !=(const StringIndexer &rhs) const
	{
		return !(*this == rhs);
	}

	void WriteToStream(EncodeStream &s) const;
	void ReadFromStream(EncodeStream &s);

	volatile int size()  { return updated_size; }

	const char *get_string_for_index(uint32_t app_index, int *count = nullptr)
	{
		auto &ref = keys[app_index];
		if (count)
		{
			*count = size_as_int(ref.size()) + 1;
		}
		return ref.c_str();
	}

	int get_index_for_string(const char *key)
	{
		int ret = -1;
		if (key)
		{
			auto iter = keys2index.find(key);
			if (iter != keys2index.end())
			{
				ret = iter->second;
			}
		}
		return ret;
	}

	// a read write lock protects the live indexes vector
	// multiple readers are able to read the live indexes while this 
	// vector is being updated by the updater threads
	// ... I am not sure if it would be better to just make this a concurrent vector
	//
	void read_lock_live_indexes()
	{
		live_index_lock.lock_read();
	}
	const std::vector<int> &get_live_indexes() 
	{ 
		return live_indexes; 
	}
	void read_unlock_live_indexes()
	{
		live_index_lock.unlock();
	}

	// maybe swap current_index_set
	// if v does not match the current set, lock and swap it
	void maybe_swap_live_indexes(std::vector<int> * v);

	// return an index.  if the key doesn't exist yet add it.
	int add_key_to_set(const char *key, bool *is_new_key)
	{
		int string_indexer_index;
		auto iter = keys2index.find(key);
		if (iter != keys2index.end())
		{
			string_indexer_index = iter->second;
			if (is_new_key)
				*is_new_key = false;
		}
		else
		{
			if (is_new_key)
				*is_new_key = true;
			string_indexer_index = keys.size();
			auto iter = keys.push_back(key);					// update caches
			const char *p = (*iter).c_str(); // p needs to point to the char in keys
			keys2index.insert({ p, string_indexer_index });		// spawn new value
			updated_size = string_indexer_index+1;
		}
		return string_indexer_index;
	}

	std::atomic<int> updated_size;
	tbb::concurrent_vector<std::string> keys;
	tbb::concurrent_unordered_map<
		const char *,
		uint32_t,
		hash_c_string,
		comp_c_string> keys2index;

	tbb::spin_rw_mutex live_index_lock;
	std::vector<int> live_indexes;
};
