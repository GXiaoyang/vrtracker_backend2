#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRTrackedCameraCursor : public VRTrackedCameraCppStub
{
public:
	CursorContext *m_context;
	vr_result::tracked_camera_state &state_ref;
	vr_result::tracked_camera_iterator &iter_ref;

	explicit VRTrackedCameraCursor(CursorContext *context);
	void SynchronizeChildVectors();

	const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) override;
	vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera) override;
	vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize) override;
	vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t * pFocalLength, vr::HmdVector2_t * pCenter) override;
	vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection) override;
};
