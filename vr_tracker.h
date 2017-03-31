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
	save_summary()
	{
		memset(&start_date_string, 0, sizeof(start_date_string));
		last_encoded_frame = -1; 
	}
	char start_date_string[64];
	time_index_t last_encoded_frame;

	void encode(EncodeStream &s) const
	{
		s.memcpy_out_to_stream(start_date_string, sizeof(start_date_string));
		s.memcpy_out_to_stream(&last_encoded_frame, sizeof(last_encoded_frame));
	}
	void decode(EncodeStream &s) 
	{
		s.memcpy_from_stream(start_date_string, sizeof(start_date_string));
		s.memcpy_from_stream(&last_encoded_frame, sizeof(last_encoded_frame));
	}
	bool operator ==(const save_summary &rhs) const
	{
		if (&rhs == this)
			return true;
		return strcmp(start_date_string, rhs.start_date_string) == 0 && (last_encoded_frame == rhs.last_encoded_frame);
	}
	bool operator !=(const save_summary &rhs) const
	{
		return !(*this == rhs);
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
	std::chrono::time_point<std::chrono::steady_clock> m_start;	
	
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

	vr_tracker()
		:
		m_last_updated_frame_number(-1),
		m_state(base::URL("vr", "/vr"), &m_state_registry),
		m_time_stamps(slab_allocator<time_stamp_t>())
	{
	}


	vr_tracker(const vr_tracker &rhs)
		: 
			m_last_updated_frame_number(rhs.m_last_updated_frame_number),
			m_state_registry(rhs.m_state_registry),
			m_start(rhs.m_start),
			m_save_summary(rhs.m_save_summary),
			m_keys(rhs.m_keys),
			m_state(rhs.m_state),
			m_vr_events(rhs.m_vr_events),
			m_time_stamps(rhs.m_time_stamps),
			m_keys_updates(rhs.m_keys_updates),
			m_state_update_bits(rhs.m_state_update_bits)
	{}

	vr_tracker &operator =(const vr_tracker &rhs)
	{
		m_last_updated_frame_number = rhs.m_last_updated_frame_number;
		m_state_registry = rhs.m_state_registry;
		m_start = rhs.m_start;
		m_save_summary = rhs.m_save_summary;
		m_keys = rhs.m_keys;
		m_state = rhs.m_state;
		m_vr_events = rhs.m_vr_events;
		m_time_stamps = rhs.m_time_stamps;
		m_keys_updates = rhs.m_keys_updates;
		m_state_update_bits = rhs.m_state_update_bits;
		return *this;
	}
		

};

inline bool operator ==(const vr_tracker &a, const vr_tracker &b)
{
	if (&a == &b)
		return true;
	if (a.m_save_summary != b.m_save_summary)
		return false;
	if (a.m_state_registry != b.m_state_registry)
		return false;
	if (a.get_last_updated_frame() != b.get_last_updated_frame())
		return false;
	if (a.m_keys != b.m_keys)
		return false;
	if (a.m_time_stamps != b.m_time_stamps)
		return false;
	if (a.m_keys_updates != b.m_keys_updates)
		return false;
	if (a.m_state_update_bits != b.m_state_update_bits)
		return false;

	return true;
}

inline bool operator !=(const vr_tracker &a, const vr_tracker &b)
{
	return !(a == b);
}