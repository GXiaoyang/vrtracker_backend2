#include "vr_cursor_controller.h"
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
#include "vr_driver_manager_cursor.h"

#include "vr_cursor_context.h"
#include "vr_schema.h"
#include "capture.h"

struct vr_cursor_controller::VRCursorImpl
{
	explicit VRCursorImpl(capture *capture)
		:
		m_context(capture),
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
		m_resources_cursor(&m_context),
		m_driver_manager_cursor(&m_context)
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
	VRDriverManagerCursor   m_driver_manager_cursor;
};

//
// VRcursor has:
//		a pointer to the state capture
//      a set of iterators into that state capture.
//
//
// There can be more than one cursor pointing into the same state capture
//
void vr_cursor_controller::init(capture *capture)
{
	pimpl = new VRCursorImpl(capture);
	m_interfaces.sysi = 	&pimpl->m_system_cursor;
	m_interfaces.appi = 	&pimpl->m_applications_cursor;
	m_interfaces.seti = 	&pimpl->m_settings_cursor;
	m_interfaces.chapi = 	&pimpl->m_chaperone_cursor;
	m_interfaces.chapsi = 	&pimpl->m_chaperone_setup_cursor;
	m_interfaces.compi = 	&pimpl->m_compositor_cursor;
	m_interfaces.noti = 	&pimpl->m_notifications_cursor;
	m_interfaces.ovi = 		&pimpl->m_overlay_cursor;
	m_interfaces.remi = 	&pimpl->m_rendermodels_cursor;
	m_interfaces.exdi = 	&pimpl->m_extendeddisplay_cursor;
	m_interfaces.taci = 	&pimpl->m_trackedcamera_cursor;
	m_interfaces.screeni = 	&pimpl->m_screenshots_cursor;
	m_interfaces.resi = 	&pimpl->m_resources_cursor;
}

void vr_cursor_controller::SeekToFrame(time_index_t framenumber)
{
	pimpl->m_context.ChangeFrame(framenumber);
}

time_index_t vr_cursor_controller::GetFrame() const
{
	return pimpl->m_context.GetCurrentFrame();
}

void vr_cursor_controller::advance_one_frame()
{
	time_index_t a = pimpl->m_context.GetCurrentFrame();
	pimpl->m_context.ChangeFrame(a + 1);
}
