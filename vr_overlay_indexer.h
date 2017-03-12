// OverlaysIndexer	
//
#pragma once
#include "openvr_serialization.h"
#include <unordered_map>
#include "vr_overlay_wrapper.h"

// the idea of the overlay  is to make it possible to index this thing using integer indexes
// despite the fact that openvr interfaces index using both overlay handles AND string keys
//
class OverlayIndexer
{
public:
	OverlayIndexer();
	void Init(const char **initial_overlay_names, int num_names);
	void WriteToStream(EncodeStream &s);
	void ReadFromStream(EncodeStream &s);

	void update(vr_result::TMPInt32String<> *result, vr_result::OverlayWrapper &ow);

	int get_index_for_key(const char *key)
	{
		int ret = -1;
		std::string skey(key);	// todo blargh, just use char *
		auto iter = overlay_keys2index.find(skey);
		if (iter != overlay_keys2index.end())
		{
			ret = iter->second;
		}
		return ret;
	}
	int get_index_for_handle(vr::VROverlayHandle_t h)
	{
		int ret = -1;
		auto iter = overlay_handle2index.find(h);
		if (iter != overlay_handle2index.end())
		{
			ret = iter->second;
		}
		return ret;
	}

	const char *get_overlay_key_for_index(const uint32_t overlay_index)
	{
		return overlay_keys[overlay_index].c_str();
	}

	int get_num_overlays() const
	{
		return (int)overlay_keys.size();
	}

	void update_handle_for_index(int index, vr::VROverlayHandle_t h)
	{
		overlay_handle2index.insert({ h, index });
	}
private:
	int get_overlay_index_for_key(const std::string &key);

	std::vector<std::string> overlay_keys;
	std::unordered_map<std::string, int> overlay_keys2index;
	std::unordered_map<vr::VROverlayHandle_t, int> overlay_handle2index;
};
