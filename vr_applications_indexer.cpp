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

// Go through each application in application count
// * if I don't have an index for this app, make one.
// * return in active_indexes, the list of indexes that were found
void ApplicationsIndexer::update(vr_result::ApplicationsWrapper &ow)
{
	auto count = ow.GetApplicationCount();
	present_indexes.clear();
	for (int i = 0; i < (int)count.val; i++)
	{
		vr_result::TMPString<vr::EVRApplicationError> key;
		ow.GetApplicationKeyByIndex(i, &key); 
		if (key.is_present())
		{
			std::string key(key.val.data());	// todo get rid of stupid string - yet again
			int index = get_index_for_key(key);	// implicitly automatically populates keys
			present_indexes.push_back(i);
		}
	}
}

