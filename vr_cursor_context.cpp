#include "vr_cursor_context.h"
#include "vr_tracker.h"

CursorContext::CursorContext(vr_tracker *tracker)
	:
	m_current_frame(tracker->get_last_updated_frame()),
	m_last_event_frame_returned(m_current_frame - 1),
	m_state(&tracker->m_state),
	m_vr_events(&tracker->m_vr_events),
	m_keys(&tracker->m_keys),
	m_tracker(tracker)
{
}

time_index_t CursorContext::ChangeFrame(time_index_t new_frame)
{
	if (new_frame == m_current_frame)
		return m_current_frame;

	if (new_frame > m_tracker->get_last_updated_frame())
		new_frame = m_tracker->get_last_updated_frame();

	m_current_frame = new_frame;
	m_last_event_frame_returned = new_frame - 1;
	return m_current_frame;
}

bool CursorContext::PollNextEvent(struct vr::VREvent_t * pEvent)
{
	assert(0);
#if 0
	if (m_context->last_event_frame_returned < m_context->current_frame)
	{
		auto prev = m_context->m_events->end();
		for (auto iter = m_context->m_events->begin(); iter != m_context->m_events->end(); iter++)
		{
			if (iter->get_time_index() < m_context->last_event_frame_returned)
				break;
			prev = iter;
		}
		// todo: this backwards search is stupid.  but alot of the cursor stuff has this sort of thing.
		//       once behaviour demonstrates value, then decide if it's worth using a vector or double ended queue
		if (prev != m_context->m_events->end() &&
			prev->get_time_index() > m_context->last_event_frame_returned &&
			prev->get_time_index() <= m_context->current_frame)
		{
			m_context->last_event_frame_returned = prev->get_time_index();
			if (pEvent)
			{
				*pEvent = prev->get_value();
			}
			rc = true;
		}
#endif
		return true;
}