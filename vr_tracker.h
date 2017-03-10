#pragma once

#include "slab_allocator.h"
#include "segmented_list.h"
#include "time_containers.h"
#include "vr_schema.h"
#include "vr_keys.h"
#include <chrono>
#include <mutex>

// data recorded at time of save.  
// it's useful when loading to have an idea what was loaded and when it was captured
struct tracker_save_summary
{
	int num_frames;
	char date_string[64];
};

template <typename Allocator>
struct vr_tracker
{
	static const int LARGE_SEGMENT_SIZE = 5400;		// segment size for per/frame data.  e.g. 1minute at 90 fps - 5400

	slab *m_slab;
	slab_allocator<char> m_allocator;
	tmp_vector_pool<VRTMPSize> m_string_pool;
	time_index_t m_frame_number;
	std::chrono::time_point<std::chrono::steady_clock> start_time;

	std::mutex update_mutex;
	int blocking_update_calls;
	int non_blocking_update_calls;
	tracker_save_summary save_info;
	vr_keys additional_resource_keys;

	vr_result::vr_state m_state;
	// cursors are responsible for detecting changes in m_state on their own
	// should TreeNodeIF do the same?
	// the principle is that updates to m_state are not blocked so they should be kept 
	// up to date in the history walk.  though it should be made cheap to check
	// when the structure does change.

	std::forward_list<FrameNumberedEvent, Allocator>  m_events;
	segmented_list<time_stamp_t, LARGE_SEGMENT_SIZE, slab_allocator<time_stamp_t>>  m_time_stamps;

	time_index_t get_closest_time_index(time_stamp_t val)
	{
		auto iter = last_item_less_than_or_equal_to(m_time_stamps.begin(), m_time_stamps.end(), val);
		return std::distance(m_time_stamps.begin(), m_time_stamps.end());
	}

	time_stamp_t get_time_stamp(time_index_t i) { return m_time_stamps[i]; }

	vr_tracker(slab *slab)
		:
		m_slab(slab),
		m_allocator(slab),
		m_frame_number(0),
		blocking_update_calls(0),
		non_blocking_update_calls(0),
		m_state(URL("vr", "/vr"), m_allocator),
		m_events(m_allocator),
		m_time_stamps(slab_allocator<time_stamp_t>())
	{
		memset(&save_info, 0, sizeof(save_info));
	}
};