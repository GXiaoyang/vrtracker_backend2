#pragma once
#include "vr_applications_wrapper.h"
#include "openvr_serialization.h"
#include <unordered_map>

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
	void update(std::vector<int> *active_indexes, vr_result::ApplicationsWrapper &ow);
	
	// number of applications ever seen
	int get_num_applications() { return (int)app_keys.size(); }

	// index to key mapping
	// say
	// To
	// appa,appb,appc
	// T1
	// appa,appc
	// 
	// the appb disappeared - the key is gone from OpenVR - so the AppHelper will hold it
	const char *get_key_for_index(uint32_t app_index, int *count)
	{
		auto &ref = app_keys[app_index];
		*count = (int)ref.size() + 1;
		return ref.c_str();
	}

	// return a valid index for a key (ie has to be in the active_indexes_set)
	// blargh - overlay doesn't check it versus the active_indexes - and relies on
	// the presence state to indicate to the caller if the value is dead.
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

	std::vector<std::string> app_keys;
	std::unordered_map<std::string, int> app_keys2index;
};
