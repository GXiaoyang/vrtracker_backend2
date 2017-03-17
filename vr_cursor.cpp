#pragma once

#include "vr_cursor.h"
#include "vr_system_cursor.h"
#include "vr_applications_cursor.h"
#include "vr_settings_cursor.h"
#include "vr_chaperone_cursor.h"
#include "vr_chaperone_setup_cursor.h"
#include "vr_compositor_cursor.h"
#include "vr_notifications_cursor.h"
#include "vr_overlay_cursor.h"
#include "vr_render_models_cursor.h"
#include "vr_extended_display_cursor.h"
#include "vr_tracked_camera_cursor.h"
#include "vr_screenshots_cursor.h"
#include "vr_resources_cursor.h"

#include "vr_cursor_context.h"
#include "vr_schema.h"
#include "vr_tracker.h"

struct VRCursor::VRCursorImpl
{
	VRCursorImpl(vr_tracker *tracker)
		:
		m_context(tracker->m_frame_number, &iterators, &tracker->m_state,
			&tracker->m_events, &tracker->keys),
		m_system_cursor(&m_context),
		m_applications_cursor(&m_context),
		m_settings_cursor(&m_context),
		m_chaperone_cursor(&m_context),
		m_chaperone_setup_cursor(&m_context),
		m_compositor_cursor(&m_context),
		m_notifications_cursor(&m_context),
		m_overlay_cursor(&m_context),
		m_rendermodels_cursor(&m_context),
		m_extendeddisplay_cursor(&m_context),
		m_trackedcamera_cursor(&m_context),
		m_screenshots_cursor(&m_context),
		m_resources_cursor(&m_context)
	{}

	vr_result::vr_iterator iterators;
	CursorContext m_context;

	VRSystemCursor			m_system_cursor;
	VRApplicationsCursor	m_applications_cursor;
	VRSettingsCursor		m_settings_cursor;
	VRChaperoneCursor		m_chaperone_cursor;
	VRChaperoneSetupCursor	m_chaperone_setup_cursor;
	VRCompositorCursor		m_compositor_cursor;
	VRNotificationsCursor	m_notifications_cursor;
	VROverlayCursor			m_overlay_cursor;
	VRRenderModelsCursor	m_rendermodels_cursor;
	VRExtendedDisplayCursor m_extendeddisplay_cursor;
	VRTrackedCameraCursor	m_trackedcamera_cursor;
	VRScreenshotsCursor		m_screenshots_cursor;
	VRResourcesCursor       m_resources_cursor;
};

//
// VRcursor has:
//		a pointer to the state tracker
//      a set of iterators into that state tracker.
//
//
// There can be more than one cursor pointing into the same state tracker
//
void VRCursor::Init(vr_tracker *tracker)
{
	pimpl = new VRCursor::VRCursorImpl(tracker);
}

void VRCursor::SetFrame(time_index_t framenumber)
{
	pimpl->m_context.current_frame = framenumber;
	pimpl->m_context.last_event_frame_returned = framenumber - 1;
}

time_index_t VRCursor::GetFrame() const
{
	return pimpl->m_context.current_frame;
}
