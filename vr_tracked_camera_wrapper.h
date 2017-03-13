#pragma once
// vr_wrappers.h - map all vr api calls to and from the types in vr_result

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct TrackedCameraWrapper
	{
		TrackedCameraWrapper(IVRTrackedCamera *taci_in)
			: taci(taci_in)
		{}

		inline Bool<EVRTrackedCameraError> HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex)
		{
			Bool<EVRTrackedCameraError> result;
			result.return_code = taci->HasCamera(nDeviceIndex, &result.val);
			return result;
		}

		inline CameraFrameSize<EVRTrackedCameraError> &GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType frame_type,
			CameraFrameSize<EVRTrackedCameraError> *frame_size)
		{
			EVRTrackedCameraError rc = taci->GetCameraFrameSize(
				nDeviceIndex,
				frame_type,
				&frame_size->val.width,
				&frame_size->val.height,
				&frame_size->val.size);

			frame_size->return_code = rc;
			return *frame_size;
		}

		inline
			CameraFrameIntrinsics<EVRTrackedCameraError> &
			GetCameraIntrinsics(
				vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType frame_type,
				CameraFrameIntrinsics<EVRTrackedCameraError> *intrinsics)
		{
			intrinsics->return_code = taci->GetCameraIntrinsics(
				nDeviceIndex,
				frame_type,
				&intrinsics->val.focal_length,
				&intrinsics->val.center);
			return *intrinsics;
		}

		inline HmdMatrix44<EVRTrackedCameraError> &GetCameraProjection(
			TrackedDeviceIndex_t device_index,
			EVRTrackedCameraFrameType frame_type,
			float flZNear,
			float flZFar,
			HmdMatrix44<EVRTrackedCameraError> *projection)
		{
			projection->return_code = taci->GetCameraProjection(device_index, frame_type, flZNear, flZFar, &projection->val);
			return *projection;
		}

		inline  VideoStreamTextureSize<EVRTrackedCameraError>&
			GetVideoStreamTextureSize(
				TrackedDeviceIndex_t device_index,
				EVRTrackedCameraFrameType frame_type,
				VideoStreamTextureSize<EVRTrackedCameraError> *size
			)
		{
			size->return_code = taci->GetVideoStreamTextureSize(device_index, frame_type,
				&size->val.texture_bounds,
				&size->val.width,
				&size->val.height);
			return *size;
		}

		IVRTrackedCamera *taci;
	};
}
