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

// Go through each application in application count
// * if I don't have an index for this app, make one.
// * return in active_indexes, the list of indexes that were found
vr_result::TMPInt32String<> &ApplicationsIndexer::update(vr_result::TMPInt32String<> *result, vr_result::ApplicationsWrapper &ow)
{
	auto count = ow.GetApplicationCount();
	assert(count.val < result->val.max_size());
	vr_result::TMPString<vr::EVRApplicationError> key;
	for (int i = 0; i < (int)count.val; i++)
	{
		ow.GetApplicationKeyByIndex(i, &key); 
		if (key.is_present())
		{
			std::string key(key.val.data());	// todo get rid of stupid string - yet again
			int index = get_index_for_key(key);	// implicitly automatically populates keys
			result->val.push_back(index);
		}
	}
	return *result;
}

