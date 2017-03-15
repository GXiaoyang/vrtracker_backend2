#pragma once
#include "vr_applications_wrapper.h"
#include "openvr_serialization.h"
#include <unordered_map>
#include <atomic>
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/spin_rw_mutex.h"

struct vr_result::ApplicationsWrapper;

// knows how to map indexes to application keys
class ApplicationsIndexer
{
public:
	ApplicationsIndexer()
	{
	}

	void WriteToStream(EncodeStream &s);
	void ReadFromStream(EncodeStream &s);

	void update(vr_result::ApplicationsWrapper &ow);
	
	// number of applications ever seen
	volatile int get_num_applications()  { return updated_size; }

	const char *get_key_for_index(uint32_t app_index, int *count = nullptr)
	{
		auto &ref = app_keys[app_index];
		if (count)
		{
			*count = (int)ref.size() + 1;
		}
		return ref.c_str();
	}

	int get_index_for_key(const char *key)
	{
		int ret = -1;
		if (key)
		{
			std::string skey(key);	// todo blargh, just use char *
			auto iter = app_keys2index.find(skey);
			if (iter != app_keys2index.end())
			{
				ret = iter->second;
			}
		}
		return ret;
	}

	void read_lock_present_indexes()
	{
		present_index_lock.lock_read();
	}
	const std::vector<int> &get_present_indexes() 
	{ 
		return present_indexes[0];  // 0th element is the one that's published (versus a temporary one)
	}
	void read_unlock_present_indexes()
	{
		present_index_lock.unlock();
	}


private:
	// return an index.  if the key doesn't exist yet add it.
	int get_index_for_key(const std::string &key)
	{
		int rc;
		auto iter = app_keys2index.find(key);
		if (iter != app_keys2index.end())
		{
			rc = iter->second;
		}
		else
		{
			app_keys.push_back(key);					// update caches
			rc = (int)app_keys.size() - 1;
			app_keys2index.insert(iter, { key, rc });
		}
		return rc;
	}

	std::atomic<int> updated_size;

	tbb::concurrent_vector<std::string> app_keys;
	tbb::concurrent_unordered_map<std::string, int> app_keys2index;

	tbb::spin_rw_mutex present_index_lock;
	std::vector<int> present_indexes[2];
};
