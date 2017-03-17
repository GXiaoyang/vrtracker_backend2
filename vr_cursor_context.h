#pragma once
#include "platform.h"
#include "vr_schema.h"
#include "vr_keys.h"
#include "vr_constants.h"

//
// CursorContext: hold the shared internal state required by the vr_xxxx_cursor objects.
//

struct vr_tracker;

struct CursorContext
{
	CursorContext();
	~CursorContext();

	void Init(vr_tracker *);

	time_index_t get_current_frame() const { return m_current_frame;  }

	vr_result::vr_iterator *get_iterators() { return m_iterators;  }

private:
	time_index_t m_current_frame;
	time_index_t m_last_event_frame_returned;
	vr_result::vr_iterator *m_iterators;
	vr_result::vr_state *m_state;
	VRForwardList<FrameNumberedEvent> *m_events;
	vr_keys *m_resource_keys;
};
