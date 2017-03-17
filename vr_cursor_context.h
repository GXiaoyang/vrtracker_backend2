#pragma once
#include "platform.h"
#include "vr_schema.h"
#include "vr_keys.h"
#include "vr_constants.h"

struct CursorContext
{
	CursorContext(
		time_index_t my_current_frame,
		vr_result::vr_iterator *my_iterators,
		vr_result::vr_state *my_state,
		VRForwardList<FrameNumberedEvent>  *my_events,
		vr_keys *resource_keys)
		:
		last_event_frame_returned(my_current_frame - 1),
		current_frame(my_current_frame),
		iterators(my_iterators),
		state(my_state),
		m_events(my_events),
		m_resource_keys(resource_keys)
	{}

	int last_event_frame_returned;
	time_index_t current_frame;
	vr_result::vr_iterator *iterators;

	vr_result::vr_state *state;
	VRForwardList<FrameNumberedEvent> *m_events;	// reference into the tracker event. used so iterators
																// for it can be created

	vr_keys *m_resource_keys;
};
