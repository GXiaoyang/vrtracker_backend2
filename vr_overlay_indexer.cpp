#include "vr_overlay_indexer.h"

OverlayIndexer::OverlayIndexer()
{}

void OverlayIndexer::Init(const char **initial_overlay_names, int num_names)
{
	for (int i = 0; i < num_names; i++)
	{
		std::string s(initial_overlay_names[i]);
		get_overlay_index_for_key(s); // adds and assigns
	}
}

void OverlayIndexer::WriteToStream(EncodeStream &s)
{
	int x = 11;
	encode(x, s);
	write_string_vector_to_stream(s, overlay_keys);
}

void OverlayIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 11);
	overlay_keys.clear();
	overlay_keys2index.clear();
	overlay_handle2index.clear();
	read_string_vector_from_stream(s, overlay_keys);
	for (int i = 0; i < (int)overlay_keys.size(); i++)
	{
		overlay_keys2index.insert({ overlay_keys[i], i });
	}
}

// walks through known overlays and updates index set
void OverlayIndexer::update(std::vector<int> *active_indexes, vr_result::OverlayWrapper &ow)
{
	for (int i = 0; i < (int)overlay_keys.size(); i++)
	{
		vr::VROverlayHandle_t handle;
		vr::EVROverlayError e = ow.ovi->FindOverlay(overlay_keys[i].c_str(), &handle);
		if (e == vr::VROverlayError_None)
		{
			// this overlay is active.
			// lookup it's index
			int index = get_overlay_index_for_key(overlay_keys[i]);
			active_indexes->push_back(index);
			overlay_handle2index.insert({ handle, index });	// cache it's handle
		}
	}
}

	
// return an index.  if the key doesn't exist yet add it.
int OverlayIndexer::get_overlay_index_for_key(const std::string &key)
	{
		int rc;
		auto iter = overlay_keys2index.find(key);
		if (iter != overlay_keys2index.end())
		{
			rc = iter->second;
		}
		else
		{
			overlay_keys.push_back(key);					// update caches
			rc = (int)overlay_keys.size() - 1;
			overlay_keys2index.insert(iter, { key, rc });
			//
			// todo this should also look up the handle?
			//
		}
		return rc;
}

