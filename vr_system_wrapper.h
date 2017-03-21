#pragma once
// vr_wrappers.h - map all vr api calls to and from the types in vr_result

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

struct SystemWrapper
{
	IVRSystem *sysi;

	SystemWrapper(IVRSystem *sysi_in)
		: sysi(sysi_in)
	{}

	inline Uint32Size<> GetRecommendedRenderTargetSize()
	{
		Uint32Size<> s;
		sysi->GetRecommendedRenderTargetSize(&s.val.width, &s.val.height);
		return s;
	}

	inline Bool<> GetIsDisplayOnDesktop()
	{
		return make_result(sysi->IsDisplayOnDesktop());
	}

	inline void GetTimeSinceLastVsync(
		Float<bool> *seconds_since_last_vsync,
		Uint64<bool> *frame_counter_since_last_vsync)
	{
		bool r = sysi->GetTimeSinceLastVsync(
			&seconds_since_last_vsync->val,
			&frame_counter_since_last_vsync->val);

		seconds_since_last_vsync->return_code = r;
		frame_counter_since_last_vsync->return_code = r;
	}

	inline Int32<> GetD3D9AdapterIndex()
	{
		return make_result(sysi->GetD3D9AdapterIndex());
	}

	inline Int32<> GetDXGIOutputInfo()
	{
		int32_t i = 0;
		sysi->GetDXGIOutputInfo(&i);
		return make_result(i);
	}

	inline TMPDeviceIndexes &GetSortedTrackedDeviceIndicesOfClass(
		ETrackedDeviceClass device_class,
		vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex,
		TMPDeviceIndexes *result)
	{
		result->val.resize(sysi->GetSortedTrackedDeviceIndicesOfClass(
			device_class, result->val.data(),
			result->val.max_size(),
			unRelativeToTrackedDeviceIndex));
		return *result;
	}

	inline Uint32<> CountDevicesOfClass(ETrackedDeviceClass device_class)
	{
		return make_result(sysi->GetSortedTrackedDeviceIndicesOfClass(device_class, nullptr, 0));
	}

	SCALAR_WRAP(IVRSystem, sysi, IsInputFocusCapturedByAnotherProcess);
	// c++14
#if 0
	auto IsInputFocusCapturedByAnotherProcess() 
	{
		return make_result(sysi->IsInputFocusCapturedByAnotherProcess());
	}
#endif
	
	SCALAR_WRAP(IVRSystem, sysi, GetSeatedZeroPoseToStandingAbsoluteTrackingPose);
	SCALAR_WRAP(IVRSystem, sysi, GetRawZeroPoseToStandingAbsoluteTrackingPose);

	inline HmdMatrix44<> GetProjectionMatrix(EVREye eEye, float fNearZ, float fFarZ)
	{
		return make_result(sysi->GetProjectionMatrix(eEye, fNearZ, fFarZ));
	}

	inline HmdMatrix34<> GetEyeToHeadTransform(EVREye eEye)
	{
		return make_result(sysi->GetEyeToHeadTransform(eEye));
	}

	inline HmdVector4<> GetProjectionRaw(EVREye eEye)
	{
		HmdVector4_t raw;
		sysi->GetProjectionRaw(eEye, &raw.v[0], &raw.v[1], &raw.v[2], &raw.v[3]);
		return make_result(raw);
	}

	inline DistortionCoord<bool> ComputeDistortion(EVREye eEye, float fU, float fV)
	{
		DistortionCoord<bool> rc;
		rc.return_code = sysi->ComputeDistortion(eEye, fU, fV, &rc.val);
		return rc;
	}

	bool ComputeDistortion(EVREye eEye,
		int sample_width,
		int sample_height,
		DistortionCoordinates_t **result,
		int *count_return)
	{
		bool rc;
		int count = 0;
		DistortionCoordinates_t *buf = (DistortionCoordinates_t*)malloc(sample_width*sample_height * sizeof(DistortionCoordinates_t));
		if (buf)
		{
			rc = true;
			float w = (float)(1.0 / float(sample_width - 1));
			float h = (float)(1.0 / float(sample_height - 1));

			vr::DistortionCoordinates_t *coords = buf;
			for (int y = 0; y < sample_height && rc; y++)
			{
				for (int x = 0; x < sample_width && rc; x++)
				{
					float u = x*w;
					float v = y*h;
					rc = sysi->ComputeDistortion(eEye, u, v, &coords[y*sample_width + x]);
					if (rc)
					{
						count++;
					}
				}
			}
		}
		else
		{
			rc = false; // malloc failed
		}

		// write return values
		if (rc == false)
		{
			if (buf)
			{
				free(buf);
				buf = nullptr;
			}
			count = 0;
		}
		if (result)
		{
			*result = buf;
		}
		else
		{
			free(buf);
		}
		if (count_return)
		{
			*count_return = count;
		}
		return rc;
	}

	void FreeDistortion(DistortionCoordinates_t *result)
	{
		free(result);
	}

	inline void GetControllerStateWithPose(
		ETrackingUniverseOrigin origin,
		TrackedDeviceIndex_t unDeviceIndex,
		ControllerState<bool> *controller_state,
		DevicePose<bool> *synced_pose)
	{
		bool rc = sysi->GetControllerStateWithPose(
			origin, unDeviceIndex, &controller_state->val, sizeof(controller_state->val),
			&synced_pose->val);
		controller_state->return_code = rc;
		synced_pose->return_code = rc;
	}

	inline ActivityLevel<> GetTrackedDeviceActivityLevel(TrackedDeviceIndex_t unDeviceIndex)
	{
		return make_result(sysi->GetTrackedDeviceActivityLevel(unDeviceIndex));
	}

	inline ControllerRole<> GetControllerRoleForTrackedDeviceIndex(TrackedDeviceIndex_t unDeviceIndex)
	{
		return make_result(sysi->GetControllerRoleForTrackedDeviceIndex(unDeviceIndex));
	}

	inline DeviceClass<> GetTrackedDeviceClass(TrackedDeviceIndex_t unDeviceIndex)
	{
		return make_result(sysi->GetTrackedDeviceClass(unDeviceIndex));
	}

	inline Bool<> IsTrackedDeviceConnected(TrackedDeviceIndex_t unDeviceIndex)
	{
		return make_result(sysi->IsTrackedDeviceConnected(unDeviceIndex));
	}

	inline void GetTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, 
		Bool<ETrackedPropertyError> *result)
	{
		result->val = sysi->GetBoolTrackedDeviceProperty(unDeviceIndex, prop, &result->return_code);
	}

	inline void GetTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop,
		Float<ETrackedPropertyError> *result)
	{
		result->val = sysi->GetFloatTrackedDeviceProperty(unDeviceIndex, prop, &result->return_code);
	}

	inline void GetTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop,
		Int32<ETrackedPropertyError> *result)
	{
		result->val = sysi->GetInt32TrackedDeviceProperty(unDeviceIndex, prop, &result->return_code);
	}

	inline void GetTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop,
		Uint64<ETrackedPropertyError> *result)
	{
		result->val = sysi->GetUint64TrackedDeviceProperty(unDeviceIndex, prop, &result->return_code);
	}

	inline void GetTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop,
		HmdMatrix34<ETrackedPropertyError> *result)
	{
		result->val = sysi->GetMatrix34TrackedDeviceProperty(unDeviceIndex, prop, &result->return_code);
	}

	template <typename A>
	inline void GetStringTrackedDeviceProperty(TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop,
		TMPStringProperty<A> *result)
	{
		query_vector_rccount(result, sysi, &IVRSystem::GetStringTrackedDeviceProperty, unDeviceIndex, prop);
	}
};

}
