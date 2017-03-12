#pragma once
// vr_wrappers.h - map all vr api calls to and from the types in vr_result

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct OverlayWrapper
	{
		OverlayWrapper(IVROverlay *ovi_in)
			: ovi(ovi_in)
		{}

		SCALAR_WRAP(IVROverlay, ovi, GetPrimaryDashboardDevice);
		SCALAR_WRAP(IVROverlay, ovi, GetHighQualityOverlay);
		SCALAR_WRAP(IVROverlay, ovi, IsDashboardVisible);
		SCALAR_WRAP(IVROverlay, ovi, GetGamepadFocusOverlay);

		inline TMPString<> &GetKeyboardText(TMPString<> *result)
		{
			query_vector_rccount(result, ovi, &IVROverlay::GetKeyboardText);
			return *result;
		}

		inline TMPString<EVROverlayError> & GetOverlayKey(VROverlayHandle_t h, TMPString<EVROverlayError> *s)
		{
			query_vector_rccount(s, ovi, &IVROverlay::GetOverlayKey, h);
			return *s;
		}

		inline TMPString<EVROverlayError>& GetOverlayName(VROverlayHandle_t h, TMPString<EVROverlayError> *result)
		{
			query_vector_rccount(result, ovi, &IVROverlay::GetOverlayName, h);
			return *result;
		}

		inline OverlayHandle<EVROverlayError> GetOverlayHandle(const char *key)
		{
			OverlayHandle<EVROverlayError> rc;
			rc.return_code = ovi->FindOverlay(key, &rc.val);
			return rc;
		}

		vr::EVROverlayError GetImageData(VROverlayHandle_t ulOverlayHandle,
			Uint32<EVROverlayError> *width_out,
			Uint32<EVROverlayError> *height_out,
			uint8_t **ptr_out, uint32_t *size_out)
		{
			uint32_t width_query;
			uint32_t height_query;
			vr::EVROverlayError rc;
			vr::EVROverlayError err = ovi->GetOverlayImageData(ulOverlayHandle, nullptr, 0, &width_query, &height_query);

			if (err != vr::VROverlayError_ArrayTooSmall)
			{
				*width_out = make_result<uint32_t, EVROverlayError>(0, err);
				*height_out = make_result<uint32_t, EVROverlayError>(0, err);
				*ptr_out = nullptr;
				*size_out = 0;
				rc = err;
			}
			else
			{
				uint32_t size = width_query * height_query * 4;
				uint8_t *ptr = (uint8_t *)malloc(size);
				vr::EVROverlayError err = ovi->GetOverlayImageData(ulOverlayHandle, ptr, size, &width_query, &height_query);
				if (err != vr::VROverlayError_None)
				{
					free(ptr);
					*width_out = make_result<uint32_t, EVROverlayError>(0, err);
					*height_out = make_result<uint32_t, EVROverlayError>(0, err);
					*ptr_out = nullptr;
					*size_out = 0;
					rc = err;
				}
				else
				{
					*width_out = make_result<uint32_t, EVROverlayError>(width_query, err);
					*height_out = make_result<uint32_t, EVROverlayError>(height_query, err);
					*ptr_out = ptr;
					*size_out = size;
					rc = err;
				}
			}
			return rc;
		}

		void FreeImageData(void *ptr)
		{
			free(ptr);
		}

		SCALAR_WRAP_INDEXED(IVROverlay, ovi, uint32_t, GetOverlayRenderingPid, VROverlayHandle_t);

		SCALAR_WRAP_INDEXED(IVROverlay, ovi, bool, IsOverlayVisible, VROverlayHandle_t);
		SCALAR_WRAP_INDEXED(IVROverlay, ovi, bool, IsHoverTargetOverlay, VROverlayHandle_t);
		SCALAR_WRAP_INDEXED(IVROverlay, ovi, bool, IsActiveDashboardOverlay, VROverlayHandle_t);


		inline RGBColor<EVROverlayError> GetOverlayColor(VROverlayHandle_t h)
		{
			RGBColor<EVROverlayError> result;
			result.return_code = ovi->GetOverlayColor(h, &result.val.r, &result.val.g, &result.val.b);
			return result;
		}


		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayAlpha, VROverlayHandle_t, float);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayTexelAspect, VROverlayHandle_t, float);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlaySortOrder, VROverlayHandle_t, uint32_t);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayWidthInMeters, VROverlayHandle_t, float);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayTextureColorSpace, VROverlayHandle_t, EColorSpace);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayTextureBounds, VROverlayHandle_t, VRTextureBounds_t);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayTransformType, VROverlayHandle_t, vr::VROverlayTransformType);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayInputMethod, VROverlayHandle_t, vr::VROverlayInputMethod);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayMouseScale, VROverlayHandle_t, HmdVector2_t);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetDashboardOverlaySceneProcess, VROverlayHandle_t, uint32_t);
		SCALAR_RESULT_WRAP_INDEXED(IVROverlay, ovi, EVROverlayError, GetOverlayFlags, VROverlayHandle_t, uint32_t);

		Uint32Size<EVROverlayError> GetOverlayTextureSize(VROverlayHandle_t h)
		{
			Uint32Size<EVROverlayError> rc;
			rc.return_code = ovi->GetOverlayTextureSize(h, &rc.val.width, &rc.val.height);
			return rc;
		}

		FloatRange<EVROverlayError> GetOverlayAutoCurveDistanceRangeInMeters(VROverlayHandle_t h)
		{
			FloatRange<EVROverlayError> rc;
			rc.return_code = ovi->GetOverlayAutoCurveDistanceRangeInMeters(h, &rc.val.min, &rc.val.max);
			return rc;
		}

		AbsoluteTransform<EVROverlayError> GetOverlayTransformAbsolute(VROverlayHandle_t h)
		{
			AbsoluteTransform<EVROverlayError> rc;
			rc.return_code = ovi->GetOverlayTransformAbsolute(h, &rc.val.tracking_origin, &rc.val.origin2overlaytransform);
			return rc;
		}

		TrackedDeviceRelativeTransform<EVROverlayError> GetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t h)
		{
			TrackedDeviceRelativeTransform<EVROverlayError> rc;
			rc.return_code = ovi->GetOverlayTransformTrackedDeviceRelative(h, &rc.val.tracked_device, &rc.val.device2overlaytransform);
			return rc;
		}

		void GetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t h,
			DeviceIndex<EVROverlayError> *device_index,
			TMPString<EVROverlayError> *component)
		{
			// this function can 2/17/2017 this function can return uninitialized values
			device_index->val = k_unTrackedDeviceIndexInvalid;
			EVROverlayError rc = ovi->GetOverlayTransformTrackedDeviceComponent(h, &device_index->val,
				component->val.data(), component->val.size());
			device_index->return_code = rc;
			component->return_code = rc;
		}

		IVROverlay *ovi;
	};

}
