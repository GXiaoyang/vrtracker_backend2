#pragma once

#include "slab_allocator.h"
#include "segmented_list.h"
#include "time_containers.h"
#include "vr_schema.h"
#include "vr_keys.h"
#include <chrono>
#include <mutex>

struct save_summary
{
	char start_date_string[64];
	void encode(EncodeStream &s) const
	{
		s.memcpy_out_to_stream(start_date_string, sizeof(start_date_string));
	}
};


struct vr_tracker
{
private:
	time_index_t m_last_updated_frame_number;
	friend struct vr_tracker_traverse;
public:
	time_index_t get_last_updated_frame() const { return m_last_updated_frame_number; }

	

	//
	// data that is not saved and why
	// 
	
	// it's an id to object pointer map so needs to be rebuilt on load
	SerializableRegistry m_state_registry;  

	// only used to base timestamps at zero. so only used when generating deltas (and not after reload)
	std::chrono::time_point<std::chrono::steady_clock> start;	

	//
	// data that is saved
	// 
	save_summary m_save_summary;

	// state
	vr_keys m_keys;						// indexes of things to track
	vr_result::vr_state m_state;		// tree of vr nodes
	VREventList m_vr_events;			// sparse vector of VREvents

	// updates
	VRTimestampVector   m_time_stamps; 
	VRKeysUpdateVector	m_keys_updates;			// sparse vector of strings showing new configuration events (updates keys)
	VRUpdateVector		m_state_update_bits;	// sparse vector of bitfields of updates (updates m_state)
		

	// search for an index given a time_stamp
	time_index_t get_closest_time_index(time_stamp_t val)
	{
		auto iter = last_item_less_than_or_equal_to(m_time_stamps.begin(), m_time_stamps.end(), val);
		return std::distance(m_time_stamps.begin(), m_time_stamps.end());
	}

	// lookup a timestamp using an index
	time_stamp_t get_time_stamp(time_index_t i) 
	{
		assert(i < size_as_int(m_time_stamps.size()));
		return m_time_stamps[i]; 
	}

	vr_tracker(slab *slab)
		:
		m_last_updated_frame_number(-1),
		m_state(base::URL("vr", "/vr"), &m_state_registry),
		m_time_stamps(slab_allocator<time_stamp_t>())
	{
	}
};