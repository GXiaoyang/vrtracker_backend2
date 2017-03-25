#pragma once
#include "vr_applications_wrapper.h"
#include "vr_observable_indexer.h"
#include "vr_string_indexer.h"

//
// assigns and tracks monotonically increasing indexes<-> application keys
//
class ApplicationsIndexer : public BasicObservableIndexer
{
public:
	ApplicationsIndexer()
	{}

	void WriteToStream(EncodeStream &s);
	void ReadFromStream(EncodeStream &s);

	void update_presence_and_size(vr_result::ApplicationsWrapper *ow);
	
	// number of applications ever seen
	volatile int get_num_applications() { return m_string_indexer.size(); }

	const char *get_key_for_index(uint32_t app_index, int *count = nullptr)
	{
		return m_string_indexer.get_string_for_index(app_index, count);
	}

	int get_index_for_key(const char *key)
	{
		return m_string_indexer.get_index_for_string(key);
	}

	// access number of currently 'live' indexes
	void read_lock_live_indexes()
	{
		m_string_indexer.read_lock_live_indexes();
	}
	const std::vector<int> &get_live_indexes() 
	{ 
		return m_string_indexer.get_live_indexes();
	}
	void read_unlock_live_indexes()
	{
		m_string_indexer.read_unlock_live_indexes();
	}

private:
	StringIndexer m_string_indexer;
	std::vector<int> working_indexes;	// the published vector is in m_string_indexer. 
										// this vector is just a working buffer
};
