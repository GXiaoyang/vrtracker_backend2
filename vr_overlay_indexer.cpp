#include "vr_overlay_indexer.h"

OverlayIndexer::OverlayIndexer()
{}

void OverlayIndexer::Init(const char **initial_overlay_names, int num_names)
{
	for (int i = 0; i < num_names; i++)
	{
		m_string_indexer.add_key_to_set(initial_overlay_names[i], nullptr); // adds and assigns
	}
}

void OverlayIndexer::WriteToStream(EncodeStream &s) const
{
	m_string_indexer.WriteToStream(s);
}

void OverlayIndexer::ReadFromStream(EncodeStream &s)
{
	m_string_indexer.ReadFromStream(s);
}

void OverlayIndexer::update_presence(vr_result::OverlayWrapper *ow)
{
	live_indexes_tmp.clear();
	for (int i = 0; i < m_string_indexer.size(); i++)
	{
		vr::VROverlayHandle_t handle;
		const char *overlay_name = m_string_indexer.get_string_for_index(i);
		vr::EVROverlayError e = ow->ovi->FindOverlay(overlay_name, &handle);
		if (e == vr::VROverlayError_None)
		{
			live_indexes_tmp.push_back(i);
		}
	}

	m_string_indexer.maybe_swap_live_indexes(&live_indexes_tmp);
}

