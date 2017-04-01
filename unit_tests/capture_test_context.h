// setting up the capture requires a few steps. the purpose of this header
// is to provide a central place for test programs to do that.

#pragma once
#include "slab_allocator.h"
#include "vr_tmp_vector.h"
#include "capture.h"
#include "openvr_broker.h"

struct capture_test_context
{
public:
	static void reset_globals();

	capture_test_context();
	~capture_test_context();

	capture_test_context(const capture_test_context& rhs) = delete;
	capture_test_context &operator =(const capture_test_context& rhs) = delete;

	void ForceInitAll();

	CaptureConfig &get_config();
	capture& get_capture();
	openvr_broker::open_vr_interfaces &raw_vr_interfaces();

private:
	CaptureConfig *m_config;
	capture *m_capture;
	openvr_broker::open_vr_interfaces *m_interfaces;
};
