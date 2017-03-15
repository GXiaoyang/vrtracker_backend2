// OverlaysIndexer	
//
#pragma once
#include "openvr_serialization.h"
#include <unordered_map>
#include "vr_overlay_wrapper.h"
#include "vr_string_indexer.h"

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

	void update_presence(vr_result::OverlayWrapper &ow);

	int get_index_for_key(const char *key)
	{
		return m_string_indexer.get_index_for_string(key);
	}

	const char *get_overlay_key_for_index(const uint32_t overlay_index)
	{
		return m_string_indexer.get_string_for_index(overlay_index);
	}

	int get_num_overlays()
	{
		return m_string_indexer.size();
	}

	void read_lock_present_indexes()
	{
		m_string_indexer.read_lock_present_indexes();
	}
	const std::vector<int> &get_present_indexes()
	{
		return m_string_indexer.get_present_indexes();
	}
	void read_unlock_present_indexes()
	{
		m_string_indexer.read_unlock_present_indexes();
	}

private:
	StringIndexer m_string_indexer;
	std::vector<int> present_indexes_tmp;
};
