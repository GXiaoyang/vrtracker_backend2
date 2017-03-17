#include "vr_extended_display_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

using namespace vr;

VRExtendedDisplayCursor::VRExtendedDisplayCursor(CursorContext *context)
	:
	m_context(context),
	iter_ref(context->iterators->extended_display_node),
	state_ref(context->state->extended_display_node)
{}


void VRExtendedDisplayCursor::SynchronizeChildVectors()
{
}

void VRExtendedDisplayCursor::GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetWindowBounds");

	CURSOR_SYNC_STATE(window_bounds, window_bounds);

	if (pnX)
	{
		*pnX = window_bounds->val.i[0];
	}

	if (pnY)
	{
		*pnY = window_bounds->val.i[1];
	}

	if (pnWidth)
	{
		*pnWidth = window_bounds->val.u[0];
	}

	if (pnHeight)
	{
		*pnHeight = window_bounds->val.u[1];
	}

	LOG_EXIT("CppStubGetWindowBounds");
}

void VRExtendedDisplayCursor::GetEyeOutputViewport(
				vr::EVREye eEye, 
				uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetEyeOutputViewport");

	const ViewPort_t *vp;
	if (eEye == vr::Eye_Left)
	{
		CURSOR_SYNC_STATE(left_output_viewport, left_output_viewport);
		vp = &left_output_viewport->val;
	}
	else
	{
		CURSOR_SYNC_STATE(right_output_viewport, right_output_viewport);
		vp = &right_output_viewport->val;
	}

	if (pnX)
	{
		*pnX = vp->u[0];
	}

	if (pnY)
	{
		*pnY = vp->u[1];
	}

	if (pnWidth)
	{
		*pnWidth = vp->u[2];
	}

	if (pnHeight)
	{
		*pnHeight = vp->u[3];
	}

	LOG_EXIT("CppStubGetEyeOutputViewport");
}
