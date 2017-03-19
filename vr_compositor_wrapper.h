#pragma once


#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct CompositorWrapper
	{
		CompositorWrapper(IVRCompositor *compi_in)
			: compi(compi_in)
		{}

		inline CompositorFrameTiming<bool> GetFrameTiming(uint32_t frames_ago)
		{
			CompositorFrameTiming<bool> result;
			vr::Compositor_FrameTiming *p = &result.val;
			p->m_nSize = sizeof(vr::Compositor_FrameTiming);
			result.return_code = compi->GetFrameTiming(&result.val, frames_ago);
			return result;
		}

		inline TMPCompositorFrameTimingString<> &GetFrameTimings(uint32_t num_frames, TMPCompositorFrameTimingString<> *timings)
		{
			assert(timings->val.max_size() >= num_frames);
			num_frames = std::min(num_frames, timings->val.max_size());
			vr::Compositor_FrameTiming *p = timings->val.data();
			for (int i = 0; i < size_as_int(num_frames); i++)
			{
				p->m_nSize = sizeof(vr::Compositor_FrameTiming);
				p++;
			}
			timings->val.resize(compi->GetFrameTimings(timings->val.data(), num_frames));
			return *timings;
		}

		inline Float<> GetFrameTimeRemaining()
		{
			return make_result(compi->GetFrameTimeRemaining());
		}

		inline CompositorCumulativeStats<> GetCumulativeStats()
		{
			CompositorCumulativeStats<> result;
			compi->GetCumulativeStats(&result.val, sizeof(result.val));
			return result;
		}

		inline HmdColor<> GetForegroundFadeColor()
		{
			return make_result(compi->GetCurrentFadeColor(false));
		}

		inline HmdColor<> GetBackgroundFadeColor()
		{
			return make_result(compi->GetCurrentFadeColor(true));
		}

		inline TMPString<> &GetVulkanInstanceExtensionsRequired(TMPString<> *result)
		{
			result->val.data()[0] = 0;
			compi->GetVulkanInstanceExtensionsRequired(result->val.data(), result->val.max_size());
			result->val.resize(strlen(result->val.data()) + 1);
			return *result;
		}

		SCALAR_WRAP(IVRCompositor, compi, GetTrackingSpace);
		SCALAR_WRAP(IVRCompositor, compi, GetCurrentGridAlpha);
		SCALAR_WRAP(IVRCompositor, compi, IsFullscreen);
		SCALAR_WRAP(IVRCompositor, compi, GetCurrentSceneFocusProcess);
		SCALAR_WRAP(IVRCompositor, compi, GetLastFrameRenderer);
		SCALAR_WRAP(IVRCompositor, compi, CanRenderScene);
		SCALAR_WRAP(IVRCompositor, compi, IsMirrorWindowVisible);
		SCALAR_WRAP(IVRCompositor, compi, ShouldAppRenderWithLowResources);

		inline void GetLastPoseForTrackedDeviceIndex(
			TrackedDeviceIndex_t unDeviceIndex,
			DevicePose<EVRCompositorError> *render_pose,
			DevicePose<EVRCompositorError> *game_pose)
		{
			EVRCompositorError e = compi->GetLastPoseForTrackedDeviceIndex(unDeviceIndex,
				&render_pose->val,
				&game_pose->val);
			render_pose->return_code = e;
			game_pose->return_code = e;
		}

		IVRCompositor *compi;
	};
	
}
