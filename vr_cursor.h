#pragma once
#include "vr_cursor_common.h"
#include "openvr_broker.h"

//
// VRcursor has:
//		pointers to parts of the state capture
//      a set of iterators into that state capture.
// There can be more than one cursor pointing into the same state capture
//

struct capture;

struct VRCursor
{
	VRCursor() : pimpl(nullptr) {}

	void Init(capture *capture);

	void SeekToFrame(time_index_t framenumber);
	time_index_t GetFrame() const;

	openvr_broker::open_vr_interfaces& interfaces() { return m_interfaces; }

private:
	struct VRCursorImpl;
	VRCursorImpl *pimpl;

	openvr_broker::open_vr_interfaces m_interfaces;
};
