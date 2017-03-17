#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VROverlayCursor : public VROverlayCppStub
{
public:
	CursorContext *m_context;
	vr_result::overlay_state &state_ref;
	vr_result::overlay_iterator &iter_ref;

	VROverlayCursor(CursorContext *context);
	void SynchronizeChildVectors();

	vr::EVROverlayError GetOverlayIndexForHandle(vr::VROverlayHandle_t ulOverlayHandle, int *index);

	vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override;
	bool IsDashboardVisible() override;

	vr::EVROverlayError FindOverlay(const char *pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle) override;
	uint32_t GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) override;
	uint32_t GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) override;
	vr::EVROverlayError GetOverlayImageData(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight) override;

	uint32_t GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle) override;
	bool IsOverlayVisible(vr::VROverlayHandle_t ulOverlayHandle) override;
	bool IsHoverTargetOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
	vr::EVROverlayError GetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool *pbEnabled) override;

	vr::EVROverlayError GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue) override;
	vr::EVROverlayError GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha) override;
	vr::EVROverlayError GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect) override;
	vr::EVROverlayError GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder) override;
	vr::EVROverlayError GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters) override;
	vr::EVROverlayError GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters) override;
	vr::EVROverlayError GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace) override;
	vr::EVROverlayError GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VRTextureBounds_t * pOverlayTextureBounds) override;
	vr::EVROverlayError GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType) override;
	vr::EVROverlayError GetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin * peTrackingOrigin, struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform) override;
	vr::EVROverlayError GetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punTrackedDevice, struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform) override;
	vr::EVROverlayError GetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punDeviceIndex, char * pchComponentName, uint32_t unComponentNameSize) override;
	vr::EVROverlayError GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod) override;
	vr::EVROverlayError GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale) override;
	vr::VROverlayHandle_t GetGamepadFocusOverlay() override;
	vr::EVROverlayError GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight) override;

	bool IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
	vr::EVROverlayError GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId) override;
	uint32_t GetKeyboardText(char * pchText, uint32_t cchText) override;
	vr::EVROverlayError GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags) override;
};
