#include "vr_tracked_camera_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

using namespace vr;

VRTrackedCameraCursor::VRTrackedCameraCursor(CursorContext *context)
	:
	m_context(context),
	iter_ref(context->iterators->tracked_camera_node),
	state_ref(context->state->tracked_camera_node)
{}

void VRTrackedCameraCursor::SynchronizeChildVectors()
{
	iter_ref.controllers.resize(state_ref.controllers.size());
	for (int i = 0; i < size_as_int(iter_ref.controllers.size()); i++)
	{
		iter_ref.controllers[i].cameraframetypes.resize(state_ref.controllers[i].cameraframetypes.size());
	}
}

const char * VRTrackedCameraCursor::GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError)
{
	LOG_ENTRY("CursorGetCameraErrorNameFromEnum");

	const char * rc = openvr_string::EVRTrackedCameraErrorToString(eCameraError);

	LOG_EXIT_RC(rc, "CursorGetCameraErrorNameFromEnum");
}

vr::EVRTrackedCameraError VRTrackedCameraCursor::HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera)
{
	LOG_ENTRY("CursorHasCamera");

	vr::EVRTrackedCameraError rc;
	bool has_camera_ret = false;
	SynchronizeChildVectors();
	if (nDeviceIndex < iter_ref.controllers.size())
	{
		CURSOR_SYNC_STATE(has_camera, controllers[nDeviceIndex].has_camera);
		if (has_camera->is_present())
		{
			has_camera_ret = has_camera->val;
		}
		rc = has_camera->return_code;
	}
	else
	{
		rc = VRTrackedCameraError_NotSupportedForThisDevice; // 1/26/2017 - invalid device index returns this and sets pHasCamera to false
	}

	if (pHasCamera)
	{
		*pHasCamera = has_camera_ret;
	}

	LOG_EXIT_RC(rc, "CursorHasCamera");
}

vr::EVRTrackedCameraError VRTrackedCameraCursor::GetCameraFrameSize(
	vr::TrackedDeviceIndex_t nDeviceIndex,
	vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize)
{
	LOG_ENTRY("CursorGetCameraFrameSize");

	vr::EVRTrackedCameraError rc;
	SynchronizeChildVectors();
	if (nDeviceIndex < iter_ref.controllers.size())
	{
		CURSOR_SYNC_STATE(size, controllers[nDeviceIndex].cameraframetypes[(int)eFrameType].frame_size);

		// 2/10/2017: regardless of presence, write the stuffed value back to the caller
		//if (size->is_present())
		{
			if (pnWidth)
				*pnWidth = size->val.width;
			if (pnHeight)
				*pnHeight = size->val.height;
			if (pnFrameBufferSize)
				*pnFrameBufferSize = size->val.size;
		}
		rc = size->return_code;
	}
	else
	{
		rc = VRTrackedCameraError_NotSupportedForThisDevice;
	}

	LOG_EXIT_RC(rc, "CursorGetCameraFrameSize");
}

vr::EVRTrackedCameraError VRTrackedCameraCursor::GetCameraIntrinsics(
	vr::TrackedDeviceIndex_t nDeviceIndex,
	vr::EVRTrackedCameraFrameType eFrameType,
	vr::HmdVector2_t * pFocalLength,
	vr::HmdVector2_t * pCenter)
{
	LOG_ENTRY("CursorGetCameraIntrinsics");
	vr::EVRTrackedCameraError rc;
	SynchronizeChildVectors();
	if (nDeviceIndex < iter_ref.controllers.size())
	{
		CURSOR_SYNC_STATE(intrinsics, controllers[nDeviceIndex].cameraframetypes[(int)eFrameType].intrinsics);
		if (intrinsics->is_present())
		{
			if (pFocalLength)
				*pFocalLength = intrinsics->val.focal_length;
			if (pCenter)
				*pCenter = intrinsics->val.center;
		}
		rc = intrinsics->return_code;
	}
	else
	{
		rc = VRTrackedCameraError_NotSupportedForThisDevice;
	}
	LOG_EXIT_RC(rc, "CursorGetCameraIntrinsics");
}

vr::EVRTrackedCameraError VRTrackedCameraCursor::GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex,
	vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection)
{
	LOG_ENTRY("CursorGetCameraProjection");
	vr::EVRTrackedCameraError rc;
	SynchronizeChildVectors();
	if (nDeviceIndex < iter_ref.controllers.size())
	{
		CURSOR_SYNC_STATE(projection, controllers[nDeviceIndex].cameraframetypes[(int)eFrameType].projection);
		if (projection->is_present())
		{
			if (pProjection)
				*pProjection = projection->val;
		}
		rc = projection->return_code;
	}
	else
	{
		rc = VRTrackedCameraError_NotSupportedForThisDevice;
	}
	LOG_EXIT_RC(rc, "CursorGetCameraProjection");
}
