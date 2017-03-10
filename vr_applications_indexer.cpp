#pragma once

#include "vr_applications_indexer.h"
#include "vr_applications_wrapper.h"
#include "vr_tmp_vector.h"

void ApplicationsIndexer::WriteToStream(EncodeStream &s)
{
	int x = 88;
	encode(x, s);
	write_string_vector_to_stream(s, app_keys);
}


void ApplicationsIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 88);
	app_keys.clear();
	app_keys2index.clear();
	read_string_vector_from_stream(s, app_keys);
	for (int i = 0; i < (int)app_keys.size(); i++)
	{
		app_keys2index.insert({ app_keys[i], i });
	}
}

// tmp_vector_pool<FixedSizeBytes> *pool, const FinalAllocatorType &final_allocator

void ApplicationsIndexer::update(std::vector<int> *active_indexes, vr_result::ApplicationsWrapper &ow)
{
	auto count = ow.GetApplicationCount();
	active_indexes->reserve(count.val);
	for (int i = 0; i < (int)count.val; i++)
	{
		vr_tmp_vector<vr_result::TMPString<vr::EVRApplicationError>> result;
#if 0
		result = make_temporary<
		vr_result::TMPString<vr::EVRApplicationError> result;
		ow.GetApplicationKeyByIndex(i, &result); // todo: why am I allocating and copying here
		std::string key(result.val.data());	// todo get rid of stupid string - yet again
		int index = get_index_for_key(key);	// implicitly automatically populates keys
		active_indexes->push_back(index);
#endif
	}
}

