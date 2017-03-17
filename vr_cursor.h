#pragma once
#include "vr_cursor_common.h"
#include "openvr_broker.h"

//
// VRcursor has:
//		pointers to parts of the state tracker
//      a set of iterators into that state tracker.
// There can be more than one cursor pointing into the same state tracker
//

struct vr_tracker;

struct VRCursor
{
	VRCursor() : pimpl(nullptr) {}

	void Init(vr_tracker *tracker);

	void SetFrame(time_index_t framenumber);
	time_index_t GetFrame() const;

	openvr_broker::open_vr_interfaces& interfaces() { return m_interfaces; }

private:
	struct VRCursorImpl;
	VRCursorImpl *pimpl;

	openvr_broker::open_vr_interfaces m_interfaces;
};
