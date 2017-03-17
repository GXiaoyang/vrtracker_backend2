#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRCompositorCursor : public VRCompositorCppStub
{
public:
	CursorContext *m_context;
	vr_result::compositor_state &state_ref;
	vr_result::compositor_iterator &iter_ref;

	VRCompositorCursor(CursorContext *context);
	void SynchronizeChildVectors();

	vr::ETrackingUniverseOrigin GetTrackingSpace() override;
	vr::EVRCompositorError WaitGetPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) override;
	vr::EVRCompositorError GetLastPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) override;
	vr::EVRCompositorError GetLastPoseForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex, struct vr::TrackedDevicePose_t * pOutputPose, struct vr::TrackedDevicePose_t * pOutputGamePose) override;
	bool GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo) override;
	uint32_t GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames) override;
	float GetFrameTimeRemaining() override;
	void GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes) override;
	struct vr::HmdColor_t GetCurrentFadeColor(bool bBackground) override;
	float GetCurrentGridAlpha() override;
	bool IsFullscreen() override;
	uint32_t GetCurrentSceneFocusProcess() override;
	uint32_t GetLastFrameRenderer() override;
	bool CanRenderScene() override;
	bool IsMirrorWindowVisible() override;
	bool ShouldAppRenderWithLowResources() override;
	uint32_t GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize) override;
	uint32_t GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize) override;
};
