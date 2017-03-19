#include "vr_compositor_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"


VRCompositorCursor::VRCompositorCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->get_state()->compositor_node),
	iter_ref(m_context->get_iterators()->compositor_node)
{
}

void VRCompositorCursor::SynchronizeChildVectors()
{
	iter_ref.controllers.resize(state_ref.controllers.size());
}

vr::ETrackingUniverseOrigin VRCompositorCursor::GetTrackingSpace()
{
	LOG_ENTRY("CppStubGetTrackingSpace");
	CURSOR_SYNC_STATE(tracking_space, tracking_space);
	vr::ETrackingUniverseOrigin rc = tracking_space->val;
	LOG_EXIT_RC(rc, "CppStubGetTrackingSpace");
}

vr::EVRCompositorError VRCompositorCursor::WaitGetPoses(
	struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount,
	struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
	return GetLastPoses(pRenderPoseArray, unRenderPoseArrayCount, pGamePoseArray, unGamePoseArrayCount);
}

vr::EVRCompositorError VRCompositorCursor::GetLastPoses(
	struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount,
	struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
	LOG_ENTRY("CppStubGetLastPoses");

	vr::EVRCompositorError rc = vr::VRCompositorError_None;
	SynchronizeChildVectors();
	// rebuild the arrays from the controllers
	for (int i = 0;
		(i < size_as_int(unRenderPoseArrayCount)) || (i < size_as_int(unGamePoseArrayCount)) &&
		i < size_as_int(m_context->get_iterators()->compositor_node.controllers.size());
		i++)
	{
		if (i < size_as_int(unRenderPoseArrayCount))
		{
			CURSOR_SYNC_STATE(last_render_pose, controllers[i].last_render_pose);
			if (last_render_pose->is_present())
			{
				pRenderPoseArray[i] = last_render_pose->val;
			}
			else
			{
				rc = last_render_pose->return_code;
				break;
			}
		}
		if (i < size_as_int(unGamePoseArrayCount))
		{
			CURSOR_SYNC_STATE(last_game_pose, controllers[i].last_game_pose);
			if (last_game_pose->is_present())
			{
				pGamePoseArray[i] = last_game_pose->val;
			}
			else
			{
				rc = last_game_pose->return_code;
				break;
			}
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetLastPoses");
}

vr::EVRCompositorError VRCompositorCursor::GetLastPoseForTrackedDeviceIndex(
	vr::TrackedDeviceIndex_t unDeviceIndex,
	struct vr::TrackedDevicePose_t * pOutputPose,
	struct vr::TrackedDevicePose_t * pOutputGamePose)
{
	LOG_ENTRY("CppStubGetLastPoseForTrackedDeviceIndex");

	vr::EVRCompositorError rc = vr::VRCompositorError_None;
	SynchronizeChildVectors();
	if (unDeviceIndex >= m_context->get_iterators()->compositor_node.controllers.size())
	{
		rc = vr::VRCompositorError_IndexOutOfRange;
	}
	else
	{
		if (pOutputPose)
		{
			CURSOR_SYNC_STATE(last_render_pose, controllers[unDeviceIndex].last_render_pose);
			if (last_render_pose->is_present())
			{
				*pOutputPose = last_render_pose->val;
			}
			else
			{
				rc = last_render_pose->return_code;
			}
		}
		if (pOutputGamePose && rc == vr::VRCompositorError_None)
		{
			CURSOR_SYNC_STATE(last_game_pose, controllers[unDeviceIndex].last_game_pose);
			if (last_game_pose->is_present())
			{
				*pOutputGamePose = last_game_pose->val;
			}
			else
			{
				rc = last_game_pose->return_code;
			}
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetLastPoseForTrackedDeviceIndex");
}

bool VRCompositorCursor::GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo)
{
	LOG_ENTRY("CppStubGetFrameTiming");
	bool rc = false;
	CURSOR_SYNC_STATE(frame_timing, frame_timing);	// TODO : I can actually walk backwards and figure unFramesAgo out
	if (pTiming && frame_timing->is_present())		//        OR - I could cache the frame timing table out of
													//        band - e.g. like the events and timestamps
	{
		*pTiming = frame_timing->val;
		rc = true;
	}
	LOG_EXIT_RC(rc, "CppStubGetFrameTiming");
}

uint32_t VRCompositorCursor::GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames)
{
	LOG_ENTRY("CppStubGetFrameTimings");				// TODO/Notes: See GetFrameTiming comment above
	uint32_t rc = 0;
	CURSOR_SYNC_STATE(frame_timings, frame_timings);
	if (pTiming && frame_timings->is_present() && frame_timings->val.size() > 0)
	{
		nFrames = std::min((uint32_t)frame_timings->val.size(), nFrames);
		memcpy(pTiming, &frame_timings->val.at(0), sizeof(vr::Compositor_FrameTiming)*nFrames);
		rc = nFrames;
	}
	LOG_EXIT_RC(rc, "CppStubGetFrameTimings");
}

float VRCompositorCursor::GetFrameTimeRemaining()
{
	LOG_ENTRY("CppStubGetFrameTimeRemaining");
	CURSOR_SYNC_STATE(frame_time_remaining, frame_time_remaining);
	float rc = frame_time_remaining->val;
	LOG_EXIT_RC(rc, "CppStubGetFrameTimeRemaining");
}

float VRCompositorCursor::GetFrameTimeRemaining2()
{
	LOG_ENTRY("CppStubGetFrameTimeRemaining");
	CURSOR_SYNC_STATE(frame_time_remaining_seg, frame_time_remaining_seg);
	float rc = frame_time_remaining_seg->val;
	LOG_EXIT_RC(rc, "CppStubGetFrameTimeRemaining");
}

float VRCompositorCursor::GetFrameTimeRemaining3()
{
	LOG_ENTRY("CppStubGetFrameTimeRemaining");
	CURSOR_SYNC_STATE(frame_time_remaining_conc, frame_time_remaining_conc);
	float rc = frame_time_remaining_conc->val;
	LOG_EXIT_RC(rc, "CppStubGetFrameTimeRemaining");
}

void VRCompositorCursor::GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes)
{
	LOG_ENTRY("CppStubGetCumulativeStats");

	CURSOR_SYNC_STATE(cumulative_stats, cumulative_stats);
	if (pStats && nStatsSizeInBytes == sizeof(vr::Compositor_CumulativeStats))
	{
		*pStats = cumulative_stats->val;
	}

	LOG_EXIT("CppStubGetCumulativeStats");
}

struct vr::HmdColor_t VRCompositorCursor::GetCurrentFadeColor(bool bBackground)
{
	LOG_ENTRY("CppStubGetCurrentFadeColor");

	struct vr::HmdColor_t rc;
	if (bBackground == false)
	{
		CURSOR_SYNC_STATE(foreground_fade_color, foreground_fade_color);
		rc = foreground_fade_color->val;
	}
	else
	{
		CURSOR_SYNC_STATE(background_fade_color, background_fade_color);
		rc = background_fade_color->val;
	}

	LOG_EXIT_RC(rc, "CppStubGetCurrentFadeColor");
}

float VRCompositorCursor::GetCurrentGridAlpha()
{
	LOG_ENTRY("CppStubGetCurrentGridAlpha");
	CURSOR_SYNC_STATE(grid_alpha, grid_alpha);
	float rc = grid_alpha->val;
	LOG_EXIT_RC(rc, "CppStubGetCurrentGridAlpha");
}

bool VRCompositorCursor::IsFullscreen()
{
	LOG_ENTRY("CppStubIsFullscreen");
	CURSOR_SYNC_STATE(is_fullscreen, is_fullscreen);
	bool rc = is_fullscreen->val;
	LOG_EXIT_RC(rc, "CppStubIsFullscreen");
}

uint32_t VRCompositorCursor::GetCurrentSceneFocusProcess()
{
	LOG_ENTRY("CppStubGetCurrentSceneFocusProcess");
	CURSOR_SYNC_STATE(current_scene_focus_process, current_scene_focus_process);
	uint32_t rc = current_scene_focus_process->val;
	LOG_EXIT_RC(rc, "CppStubGetCurrentSceneFocusProcess");
}

uint32_t VRCompositorCursor::GetLastFrameRenderer()
{
	LOG_ENTRY("CppStubGetLastFrameRenderer");
	CURSOR_SYNC_STATE(last_frame_renderer, last_frame_renderer);
	uint32_t rc = last_frame_renderer->val;
	LOG_EXIT_RC(rc, "CppStubGetLastFrameRenderer");
}

bool VRCompositorCursor::CanRenderScene()
{
	LOG_ENTRY("CppStubCanRenderScene");
	CURSOR_SYNC_STATE(can_render_scene, can_render_scene);
	bool rc = can_render_scene->val;
	LOG_EXIT_RC(rc, "CppStubCanRenderScene");
}

bool VRCompositorCursor::IsMirrorWindowVisible()
{
	LOG_ENTRY("CppStubIsMirrorWindowVisible");
	CURSOR_SYNC_STATE(is_mirror_visible, is_mirror_visible);
	bool rc = is_mirror_visible->val;
	LOG_EXIT_RC(rc, "CppStubIsMirrorWindowVisible");
}

bool VRCompositorCursor::ShouldAppRenderWithLowResources()
{
	LOG_ENTRY("CppStubShouldAppRenderWithLowResources");
	CURSOR_SYNC_STATE(should_app_render_with_low_resource, should_app_render_with_low_resource);
	bool rc = should_app_render_with_low_resource->val;
	LOG_EXIT_RC(rc, "CppStubShouldAppRenderWithLowResources");
}

uint32_t VRCompositorCursor::GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CppStubGetVulkanInstanceExtensionsRequired");
	uint32_t rc = 0;
	CURSOR_SYNC_STATE(instance_extensions_required, instance_extensions_required);
	if (instance_extensions_required->is_present())
	{
		util_vector_to_return_buf_rc(&instance_extensions_required->val, pchValue, unBufferSize, &rc);
	}
	LOG_EXIT_RC(rc, "CppStubGetVulkanInstanceExtensionsRequired");
}

uint32_t VRCompositorCursor::GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CppStubGetVulkanDeviceExtensionsRequired");
	static uint32_t rc = 0; // todo
	LOG_EXIT_RC(rc, "CppStubGetVulkanDeviceExtensionsRequired");
}

