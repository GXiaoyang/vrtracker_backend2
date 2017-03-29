// setting up the tracker requires a few steps. the purpose of this header
// is to provide a central place for test programs to do that.

#pragma once
#include "slab_allocator.h"
#include "vr_tmp_vector.h"
#include "vr_tracker.h"
#include "openvr_broker.h"

struct tracker_test_context
{
public:
	static void reset_globals();

	tracker_test_context();
	~tracker_test_context();

	tracker_test_context(const tracker_test_context& rhs) = delete;
	tracker_test_context &operator =(const tracker_test_context& rhs) = delete;

	void ForceInitAll();

	TrackerConfig &get_config();
	vr_tracker& get_tracker();
	openvr_broker::open_vr_interfaces &raw_vr_interfaces();

private:
	TrackerConfig *m_config;
	vr_tracker *m_tracker;
	openvr_broker::open_vr_interfaces *m_interfaces;
};
