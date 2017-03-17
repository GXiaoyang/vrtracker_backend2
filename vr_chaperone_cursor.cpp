#include "vr_chaperone_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"


VRChaperoneCursor::VRChaperoneCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->state->chaperone_node),
	iter_ref(m_context->iterators->chaperone_node)
{
}

void VRChaperoneCursor::SynchronizeChildVectors()
{
}

vr::ChaperoneCalibrationState VRChaperoneCursor::GetCalibrationState()
{
	LOG_ENTRY("CppStubGetCalibrationState");
	vr::ChaperoneCalibrationState rc;
	CURSOR_SYNC_STATE(calibration_state, calibration_state);
	rc = calibration_state->val;
	LOG_EXIT_RC(rc, "CppStubGetCalibrationState");
}

bool VRChaperoneCursor::GetPlayAreaSize(float * pSizeX, float * pSizeZ)
{
	LOG_ENTRY("CppStubGetPlayAreaSize");
	CURSOR_SYNC_STATE(play_area_size, play_area_size);
	if (play_area_size->is_present())
	{
		if (pSizeX)
			*pSizeX = play_area_size->val.v[0];
		if (pSizeZ)
			*pSizeZ = play_area_size->val.v[1];
	}

	bool rc = play_area_size->is_present();
	LOG_EXIT_RC(rc, "CppStubGetPlayAreaSize");
}

bool VRChaperoneCursor::GetPlayAreaRect(struct vr::HmdQuad_t * rect)
{
	LOG_ENTRY("CppStubGetPlayAreaRect");
	CURSOR_SYNC_STATE(play_area_rect, play_area_rect);
	if (play_area_rect->is_present() && rect)
	{
		*rect = play_area_rect->val;
	}
	bool rc = play_area_rect->is_present();
	LOG_EXIT_RC(rc, "CppStubGetPlayAreaRect");
}

bool VRChaperoneCursor::AreBoundsVisible()
{
	LOG_ENTRY("CppStubAreBoundsVisible");
	CURSOR_SYNC_STATE(bounds_visible, bounds_visible);
	bool rc = bounds_visible->is_present() && bounds_visible->val;
	LOG_EXIT_RC(rc, "CppStubAreBoundsVisible");
}

void VRChaperoneCursor::GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors,
	float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor)
{
	LOG_ENTRY("CppStubGetBoundsColor");
	if (pOutputColorArray)
	{
		CURSOR_SYNC_STATE(bounds_colors, bounds_colors);
		if (bounds_colors->val.size() == 0)
		{
			memset(pOutputColorArray, 0, sizeof(vr::HmdColor_t) * nNumOutputColors);
		}
		else
		{
			int elements_to_copy = std::min(nNumOutputColors, (int)bounds_colors->val.size());
			memcpy(pOutputColorArray, &bounds_colors->val.at(0), sizeof(vr::HmdColor_t) * elements_to_copy);
			while (elements_to_copy < nNumOutputColors)
			{
				pOutputColorArray[elements_to_copy++] = bounds_colors->val.back();
			}
		}
	}

	if (pOutputCameraColor)
	{
		CURSOR_SYNC_STATE(camera_color, camera_color);
		*pOutputCameraColor = camera_color->val;
	}

	LOG_EXIT("CppStubGetBoundsColor");
}
