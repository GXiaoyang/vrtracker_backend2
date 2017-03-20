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

struct vr_tracker
{
private:
	time_index_t m_last_updated_frame_number;
	friend struct vr_tracker_traverse;
public:
	time_index_t get_last_updated_frame() const { return m_last_updated_frame_number; }

	static const int LARGE_SEGMENT_SIZE = 5400;		// segment size for per/frame data.  e.g. 1minute at 90 fps - 5400

	//slab *m_slab;
	//slab_allocator<char> m_allocator;
	//tmp_vector_pool<VRTMPSize> m_string_pool;

	std::chrono::time_point<std::chrono::steady_clock> start_time;

	std::mutex update_mutex;
	int blocking_update_calls;
	int non_blocking_update_calls;
	tracker_save_summary save_info;
	vr_keys keys;

	vr_result::vr_state m_state;

	VRForwardList<FrameNumberedEvent>  m_events;
	segmented_list<time_stamp_t, LARGE_SEGMENT_SIZE, slab_allocator<time_stamp_t>>  m_time_stamps;

	time_index_t get_closest_time_index(time_stamp_t val)
	{
		auto iter = last_item_less_than_or_equal_to(m_time_stamps.begin(), m_time_stamps.end(), val);
		return std::distance(m_time_stamps.begin(), m_time_stamps.end());
	}

	time_stamp_t get_time_stamp(time_index_t i) { return m_time_stamps[i]; }

	vr_tracker(slab *slab)
		:
		//m_slab(slab),
		//m_allocator(slab),
		m_last_updated_frame_number(-1),
		blocking_update_calls(0),
		non_blocking_update_calls(0),
		m_state(base::URL("vr", "/vr")),
		m_time_stamps(slab_allocator<time_stamp_t>())
	{
		memset(&save_info, 0, sizeof(save_info));
	}
};