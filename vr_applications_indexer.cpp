#pragma once

#include "vr_applications_indexer.h"
#include "vr_applications_wrapper.h"
#include "vr_tmp_vector.h"

void ApplicationsIndexer::WriteToStream(EncodeStream &s)
{
	m_string_indexer.WriteToStream(s);
}


void ApplicationsIndexer::ReadFromStream(EncodeStream &s)
{
	m_string_indexer.ReadFromStream(s);
}

void ApplicationsIndexer::update_presence_and_size(vr_result::ApplicationsWrapper &ow)
{
	auto count = ow.GetApplicationCount();
	int my_buf = 1;
	present_indexes_tmp.clear();

	for (int i = 0; i < size_as_int(count.val); i++)
	{
		vr_result::TMPString<vr::EVRApplicationError> key;
		ow.GetApplicationKeyByIndex(i, &key); 
		if (key.is_present())
		{
			int indexer_index = m_string_indexer.add_key_to_set(key.val.data()); // can increase size <--- 
			present_indexes_tmp.push_back(indexer_index);
		}
	}

	m_string_indexer.maybe_swap_present_indexes(&present_indexes_tmp);
}

