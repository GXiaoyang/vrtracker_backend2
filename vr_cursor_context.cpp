#pragma once
#include "vr_cursor_context.h"
#include "vr_tracker.h"

CursorContext::CursorContext()
	: m_iterators(nullptr)
{
}

CursorContext::~CursorContext()
{
	delete m_iterators;
}

void CursorContext::Init(vr_tracker *tracker)
{
	m_current_frame = tracker->get_last_updated_frame();
	m_last_event_frame_returned = m_current_frame - 1; // 
	m_iterators = new vr_result::vr_iterator();
	m_state = &tracker->m_state;
	m_events = &tracker->m_events;
	m_keys = &tracker->keys;
}

	time_index_t m_current_frame;
	time_index_t m_last_event_frame_returned;

	
	vr_result::vr_iterator *iterators;

	vr_result::vr_state *state;
	VRForwardList<FrameNumberedEvent> *m_events;	// reference into the tracker event. used so iterators
													// for it can be created

	vr_keys *m_resource_keys;
};
