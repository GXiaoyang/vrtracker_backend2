#pragma once

#include "openvr_broker.h"
struct vr_tracker;

struct vr_tracker_updater
{
	vr_tracker_updater();
	~vr_tracker_updater();
	void update_tracker_sequential(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces);
	void update_tracker_parallel(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces);
private:
	struct impl;
	impl* m_pimpl;
};
