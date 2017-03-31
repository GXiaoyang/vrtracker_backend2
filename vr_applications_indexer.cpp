#include "vr_applications_indexer.h"
#include "vr_applications_wrapper.h"
#include "vr_tmp_vector.h"

void ApplicationsIndexer::WriteToStream(BaseStream &s) const
{
	m_string_indexer.WriteToStream(s);
}

void ApplicationsIndexer::ReadFromStream(BaseStream &s)
{
	m_string_indexer.ReadFromStream(s);
}

void ApplicationsIndexer::update_presence_and_size(vr_result::ApplicationsWrapper *wrap)
{
	auto count = wrap->GetApplicationCount();
	int my_buf = 1;
	working_indexes.clear();
	// go through each application
	for (int i = 0; i < size_as_int(count.val); i++)
	{
		vr_result::TMPString<vr::EVRApplicationError> key;
		wrap->GetApplicationKeyByIndex(i, &key); 

		// if openvr thinks the key is ok
		if (key.is_present())
		{
			// add it to the set
			bool is_new_key;
			int indexer_index = m_string_indexer.add_key_to_set(key.val.data(), &is_new_key); // can increase size of the 'global dictionary'<--- 
			if (is_new_key)
			{
				VRKeysUpdate new_key_event;
				new_key_event.update_type = VRKeysUpdate::NEW_APP_KEY;
				new_key_event.sparam1 = key.val.data();
				this->NotifyObservers(new_key_event);
			}
			working_indexes.push_back(indexer_index);
		}
	}
	m_string_indexer.maybe_swap_live_indexes(&working_indexes);
}

