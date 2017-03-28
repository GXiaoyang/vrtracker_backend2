#pragma once
// vr_tracker_traverse
//
//  * knows how to take the vr_tracker object and then initiate a traversal across it using
//    the visitors
//  * it is a friend of the vr_tracker so that it is able to modify the
//    m_last_updated_frame_number
//

#include "openvr_broker.h"
struct vr_tracker;

struct vr_tracker_traverse
{
	vr_tracker_traverse();
	~vr_tracker_traverse();

	void update_tracker_sequential(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces);
	void update_tracker_parallel(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces);

	bool save_tracker_to_binary_file(vr_tracker *tracker, const char *filename);
	bool load_tracker_from_binary_file(vr_tracker *tracker, const char *filename);

private:
	struct impl;
	impl* m_pimpl;
};
