#pragma once
// capture_traverser
//
//  * knows how to take the capture object and then initiate a traversal across it using
//    the visitors
//  * it is a friend of the capture so that it is able to modify the
//    m_last_updated_frame_number
//

#include "platform.h"
#include "openvr_broker.h"
struct capture;

struct capture_traverser
{
	capture_traverser();
	~capture_traverser();

	void update_capture(capture *capture, openvr_broker::open_vr_interfaces *interfaces, time_stamp_t update_time, bool parallel);

	void update_capture_parallel(capture *capture, openvr_broker::open_vr_interfaces *interfaces, time_stamp_t update_time)
	{
		update_capture(capture, interfaces, update_time, true);
	}
	void update_capture_sequential(capture *capture, openvr_broker::open_vr_interfaces *interfaces, time_stamp_t update_time)
	{
		update_capture(capture, interfaces, update_time, false);
	}


	bool save_capture_to_binary_file(capture *capture, const char *filename);
	bool load_capture_from_binary_file(capture *capture, const char *filename);

private:
	

	struct impl;
	impl* m_pimpl;
};
