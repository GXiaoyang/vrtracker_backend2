#pragma once
#include "platform.h"
#include "vr_schema.h"
#include "vr_keys.h"
#include "vr_constants.h"

//
// CursorContext: hold the shared internal state required by the vr_xxxx_cursor objects.
//                simulates the event queue for the current frame

struct vr_tracker;

struct CursorContext
{
	CursorContext(vr_tracker *);

	void ChangeFrame(time_index_t new_frame);
	time_index_t GetCurrentFrame() const { return m_current_frame;  }

	vr_result::vr_iterator *get_iterators() { return &m_iterators;  }
	vr_result::vr_state    *get_state()     { return m_state; }
	vr_keys* get_keys() { return m_keys; }


	bool PollNextEvent(struct vr::VREvent_t * pEvent);

private:
	time_index_t m_current_frame;
	time_index_t m_last_event_frame_returned;
	vr_result::vr_iterator m_iterators;
	vr_result::vr_state *m_state;
	VRForwardList<FrameNumberedEvent> *m_events;
	vr_keys *m_keys;
	vr_tracker *m_tracker;
};
