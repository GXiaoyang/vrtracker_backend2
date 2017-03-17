#include "vr_overlay_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"
#include "gsl.h"

using namespace vr;

VROverlayCursor::VROverlayCursor(CursorContext *context)
	:
	m_context(context),
	iter_ref(context->get_iterators()->overlay_node),
	state_ref(context->get_state()->overlay_node)
{}

void VROverlayCursor::SynchronizeChildVectors()
{
	iter_ref.overlays.resize(state_ref.overlays.size());
}

vr::TrackedDeviceIndex_t VROverlayCursor::GetPrimaryDashboardDevice()
{
	LOG_ENTRY("CppStubGetPrimaryDashboardDevice");

	vr::TrackedDeviceIndex_t rc;
	CURSOR_SYNC_STATE(primary_dashboard_device, primary_dashboard_device);
	rc = primary_dashboard_device->val;
	LOG_EXIT_RC(rc, "CppStubGetPrimaryDashboardDevice");
}

bool VROverlayCursor::IsDashboardVisible()
{
	LOG_ENTRY("CppStubIsDashboardVisible");
	bool rc;
	CURSOR_SYNC_STATE(dashboard_visible, is_dashboard_visible);
	rc = dashboard_visible->val;
	LOG_EXIT_RC(rc, "CppStubIsDashboardVisible");
}

vr::EVROverlayError VROverlayCursor::FindOverlay(const char *pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle)
{
	vr::EVROverlayError rc = vr::VROverlayError_UnknownOverlay;
	OverlayIndexer *h = &m_context->get_keys()->GetOverlayIndexer();

	// regardless of result, pOverlayHandle return value is set 2/6/2017
	if (pOverlayHandle)
	{
		*pOverlayHandle = 0;
	}

	int index = h->get_index_for_key(pchOverlayKey);			// find index
	if (index >= 0)
	{
		CURSOR_SYNC_STATE(handle, overlays[index].overlay_handle); // synchronize time.  overlay_handles will change
		if (handle->is_present())
		{
			*pOverlayHandle = handle->val;
			assert(0);

			//h->update_handle_for_index(index, handle->val);	// todo I am confused how these handles should be recovered
		}
		else
		{
			*pOverlayHandle = vr::k_ulOverlayHandleInvalid;
		}
		rc = handle->return_code;
	}
	return rc;
}


vr::EVROverlayError
VROverlayCursor::GetOverlayIndexForHandle(vr::VROverlayHandle_t ulOverlayHandle, int *index_ret)
{
	bool found_it = false;
	CURSOR_SYNC_STATE(active_overlay_indexes, active_overlay_indexes);	// what overlays are currently present

	for (auto iter = active_overlay_indexes->val.begin(); iter != active_overlay_indexes->val.end(); iter++)
	{
		int index = *iter;
		CURSOR_SYNC_STATE(handle, overlays[index].overlay_handle);
		if (handle->is_present())
		{
			if (handle->val == ulOverlayHandle)
			{
				*index_ret = index;
				found_it = true;
			}
		}
	}
	vr::EVROverlayError rc;
	if (found_it)
	{
		rc = vr::VROverlayError_None;
	}
	else
	{
		rc = vr::VROverlayError_InvalidHandle;	// 2/6/2017: this is what's returned for non existant handles
	}
	return rc;
}

#define LOOKUP_OVERLAY_STRING(VAR_NAME)\
int index;\
vr::EVROverlayError found_it = GetOverlayIndexForHandle(ulOverlayHandle, &index);\
uint32_t rc = 0;\
\
if (found_it == vr::VROverlayError_None)\
{\
	CURSOR_SYNC_STATE(key, overlays[index].VAR_NAME);\
	util_vector_to_return_buf_rc(&key->val, pchValue, unBufferSize, &rc);\
}\
\
if (pError)\
{\
	*pError = found_it;\
}

uint32_t VROverlayCursor::GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
	LOG_ENTRY("CppStubGetOverlayKey");

	int index;
	vr::EVROverlayError found_it = GetOverlayIndexForHandle(ulOverlayHandle, &index);
	uint32_t rc = 0;
	if (found_it == vr::VROverlayError_None)
	{
		const char *name = state_ref.overlays[index].get_name().c_str();
		util_vector_to_return_buf_rc(&gsl::make_span(name, state_ref.overlays[index].get_name().size() + 1),
				pchValue, unBufferSize, &rc);
	}

	if (pError)
	{
		*pError = found_it;
	}
	LOG_EXIT_RC(rc, "CppStubGetOverlayKey");
}

uint32_t VROverlayCursor::GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
	LOG_ENTRY("CppStubGetOverlayName");
	LOOKUP_OVERLAY_STRING(overlay_name);
	LOG_EXIT_RC(rc, "CppStubGetOverlayName");
}

uint32_t VROverlayCursor::GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle)
{
	uint32_t rc = 0;
	int index;
	vr::EVROverlayError found_it = GetOverlayIndexForHandle(ulOverlayHandle, &index);
	if (found_it == vr::VROverlayError_None)
	{
		CURSOR_SYNC_STATE(key, overlays[index].overlay_rendering_pid);
		rc = key->val;
	}
	return rc;
}

#define OVERLAY_BOOL_LOOKUP(var_name)\
bool rc = 0;\
int index;\
vr::EVROverlayError found_it = GetOverlayIndexForHandle(ulOverlayHandle, &index);\
if (found_it == vr::VROverlayError_None)\
{\
	CURSOR_SYNC_STATE(key, overlays[index].var_name);\
	rc = key->val;\
}\
return rc;

bool VROverlayCursor::IsOverlayVisible(VROverlayHandle_t ulOverlayHandle)
{
	OVERLAY_BOOL_LOOKUP(overlay_is_visible);
}

bool VROverlayCursor::IsHoverTargetOverlay(VROverlayHandle_t ulOverlayHandle)
{
	OVERLAY_BOOL_LOOKUP(overlay_is_hover_target);
}

bool VROverlayCursor::IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubIsActiveDashboardOverlay");
	OVERLAY_BOOL_LOOKUP(overlay_is_active_dashboard);
	LOG_EXIT_RC(rc, "CppStubIsActiveDashboardOverlay");
}

EVROverlayError
VROverlayCursor::GetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled)
{
	EVROverlayError err = vr::VROverlayError_UnknownOverlay;
	int index;
	vr::EVROverlayError found_it = GetOverlayIndexForHandle(ulOverlayHandle, &index);
	if (found_it == vr::VROverlayError_None)
	{
		CURSOR_SYNC_STATE(key, overlays[index].overlay_flags);
		err = key->return_code;
		if (key->is_present())
		{
			int bit_pos = (int)eOverlayFlag;
			uint32_t mask = 0x1 << (bit_pos);	// weird in the lowest bit isn't used
			uint32_t flags = key->val;
			if ((mask & flags) != 0)
			{
				if (pbEnabled)
					*pbEnabled = true;
			}
			else
			{
				if (pbEnabled)
					*pbEnabled = false;
			}
		}
	}
	return err;
}

vr::EVROverlayError VROverlayCursor::GetOverlayImageData(
	vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight)
{
	LOG_ENTRY("CppStubGetOverlayImageData");

	int index;
	vr::EVROverlayError rc = GetOverlayIndexForHandle(ulOverlayHandle, &index);

	if (rc == vr::VROverlayError_None)
	{
		CURSOR_SYNC_STATE(height, overlays[index].overlay_image_height);
		CURSOR_SYNC_STATE(width, overlays[index].overlay_image_width);
		CURSOR_SYNC_STATE(data, overlays[index].overlay_image_data);

		if (data->is_present())
		{
			if (punWidth)
			{
				*punWidth = width->val;
			}
			if (punHeight)
			{
				*punHeight = height->val;
			}

			uint8_t *ptr = (uint8_t*)pvBuffer;
			if (!util_vector_to_return_buf_rc(&data->val, ptr, unBufferSize, nullptr))
			{
				rc = VROverlayError_ArrayTooSmall;
			}
		}
		else
		{
			rc = data->return_code;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetOverlayImageData");
}

vr::EVROverlayError VROverlayCursor::GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue)
{
	LOG_ENTRY("CppStubGetOverlayColor");
	int index;
	vr::EVROverlayError rc = GetOverlayIndexForHandle(ulOverlayHandle, &index);
	if (rc == vr::VROverlayError_None)
	{
		CURSOR_SYNC_STATE(color, overlays[index].overlay_color);
		if (color->is_present())
		{
			if (pfRed)
			{
				*pfRed = color->val.r;		// 2/7/2017: openvr writes to pfRed only if valid
			}
			if (pfGreen)
			{
				*pfGreen = color->val.g;
			}
			if (pfBlue)
			{
				*pfBlue = color->val.b;
			}
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetOverlayColor");
}

#define OVERLAY_VAL_LOOKUP(schema_name, param_name) \
int index;\
vr::EVROverlayError rc = GetOverlayIndexForHandle(ulOverlayHandle, &index);\
if (rc == vr::VROverlayError_None)\
{\
	CURSOR_SYNC_STATE(state_val, overlays[index].schema_name);\
	if (state_val->is_present()) \
	{\
		*param_name = state_val->val;\
	}\
	rc = state_val->return_code;\
}

vr::EVROverlayError VROverlayCursor::GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha)
{
	LOG_ENTRY("CppStubGetOverlayAlpha");

	OVERLAY_VAL_LOOKUP(overlay_alpha, pfAlpha);

	LOG_EXIT_RC(rc, "CppStubGetOverlayAlpha");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect)
{
	LOG_ENTRY("CppStubGetOverlayTexelAspect");
	OVERLAY_VAL_LOOKUP(overlay_texel_aspect, pfTexelAspect);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTexelAspect");
}

vr::EVROverlayError VROverlayCursor::GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder)
{
	LOG_ENTRY("CppStubGetOverlaySortOrder");
	OVERLAY_VAL_LOOKUP(overlay_sort_order, punSortOrder);
	LOG_EXIT_RC(rc, "CppStubGetOverlaySortOrder");
}

vr::EVROverlayError VROverlayCursor::GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters)
{
	LOG_ENTRY("CppStubGetOverlayWidthInMeters");
	OVERLAY_VAL_LOOKUP(overlay_width_in_meters, pfWidthInMeters);
	LOG_EXIT_RC(rc, "CppStubGetOverlayWidthInMeters");
}

// take a struct and split it into two outgoing params
#define OVERLAY_2_STRUCT_VAL_LOOKUP(schema_name, schema_field1, schema_field2, param_name1, param_name2) \
int index;\
vr::EVROverlayError rc = GetOverlayIndexForHandle(ulOverlayHandle, &index);\
if (rc == vr::VROverlayError_None)\
{\
	CURSOR_SYNC_STATE(state_val, overlays[index].schema_name);\
	if (state_val->is_present())\
	{\
		if (param_name1)\
		{\
			*param_name1 = state_val->val.schema_field1;\
		}\
		if (param_name2)\
		{\
			*param_name2 = state_val->val.schema_field2;\
		}\
	}\
	rc = state_val->return_code;\
}

vr::EVROverlayError VROverlayCursor::GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle,
	float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters)
{
	LOG_ENTRY("CppStubGetOverlayAutoCurveDistanceRangeInMeters");
	OVERLAY_2_STRUCT_VAL_LOOKUP(overlay_auto_curve_range_in_meters, min, max, pfMinDistanceInMeters, pfMaxDistanceInMeters);
	LOG_EXIT_RC(rc, "CppStubGetOverlayAutoCurveDistanceRangeInMeters");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace)
{
	LOG_ENTRY("CppStubGetOverlayTextureColorSpace");
	OVERLAY_VAL_LOOKUP(overlay_texture_color_space, peTextureColorSpace);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureColorSpace");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle,
	struct vr::VRTextureBounds_t * pOverlayTextureBounds)
{
	LOG_ENTRY("CppStubGetOverlayTextureBounds");
	OVERLAY_VAL_LOOKUP(overlay_texture_bounds, pOverlayTextureBounds);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureBounds");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType)
{
	LOG_ENTRY("CppStubGetOverlayTransformType");
	OVERLAY_VAL_LOOKUP(overlay_transform_type, peTransformType);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformType");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTransformAbsolute(
	vr::VROverlayHandle_t ulOverlayHandle,
	vr::ETrackingUniverseOrigin * peTrackingOrigin,
	struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform)
{
	LOG_ENTRY("CppStubGetOverlayTransformAbsolute");
	OVERLAY_2_STRUCT_VAL_LOOKUP(overlay_transform_absolute, tracking_origin, origin2overlaytransform, peTrackingOrigin, pmatTrackingOriginToOverlayTransform);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformAbsolute");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTransformTrackedDeviceRelative(
	vr::VROverlayHandle_t ulOverlayHandle,
	vr::TrackedDeviceIndex_t * punTrackedDevice,
	struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform)
{
	LOG_ENTRY("CppStubGetOverlayTransformTrackedDeviceRelative");
	OVERLAY_2_STRUCT_VAL_LOOKUP(overlay_transform_device_relative,
		tracked_device, device2overlaytransform,
		punTrackedDevice, pmatTrackedDeviceToOverlayTransform);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformTrackedDeviceRelative");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTransformTrackedDeviceComponent(
	vr::VROverlayHandle_t ulOverlayHandle,
	vr::TrackedDeviceIndex_t * punDeviceIndex,
	char * pchComponentName, uint32_t unComponentNameSize)
{
	LOG_ENTRY("CppStubGetOverlayTransformTrackedDeviceComponent");

	int index;
	vr::EVROverlayError rc = GetOverlayIndexForHandle(ulOverlayHandle, &index);
	if (rc == vr::VROverlayError_None)
	{
		CURSOR_SYNC_STATE(device_index, overlays[index].overlay_transform_component_relative_device_index);
		CURSOR_SYNC_STATE(name, overlays[index].overlay_transform_component_relative_name);

		if (device_index->is_present() && name->is_present())
		{
			if (punDeviceIndex)
			{
				*punDeviceIndex = device_index->val;
			}
			if (!util_vector_to_return_buf_rc(&name->val, pchComponentName, unComponentNameSize, nullptr))
			{
				rc = VROverlayError_ArrayTooSmall; // TODO: verify this is what happens if the buffer is too small
			}
		}
		else
		{
			if (!device_index->is_present())
				rc = device_index->return_code;
			else
				rc = name->return_code;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformTrackedDeviceComponent");
}


vr::EVROverlayError VROverlayCursor::GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod)
{
	LOG_ENTRY("CppStubGetOverlayInputMethod");
	OVERLAY_VAL_LOOKUP(overlay_input_method, peInputMethod);
	LOG_EXIT_RC(rc, "CppStubGetOverlayInputMethod");
}

vr::EVROverlayError VROverlayCursor::GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale)
{
	LOG_ENTRY("CppStubGetOverlayMouseScale");
	OVERLAY_VAL_LOOKUP(overlay_mouse_scale, pvecMouseScale);
	LOG_EXIT_RC(rc, "CppStubGetOverlayMouseScale");
}


vr::VROverlayHandle_t VROverlayCursor::GetGamepadFocusOverlay()
{
	LOG_ENTRY("CppStubGetGamepadFocusOverlay");
	vr::VROverlayHandle_t rc;
	CURSOR_SYNC_STATE(gamepad_focus_overlay, gamepad_focus_overlay);
	rc = gamepad_focus_overlay->val;
	LOG_EXIT_RC(rc, "CppStubGetGamepadFocusOverlay");
}

vr::EVROverlayError VROverlayCursor::GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight)
{
	LOG_ENTRY("CppStubGetOverlayTextureSize");
	OVERLAY_2_STRUCT_VAL_LOOKUP(overlay_texture_size,
		width, height,
		pWidth, pHeight);
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureSize");
}

vr::EVROverlayError VROverlayCursor::GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId)
{
	LOG_ENTRY("CppStubGetDashboardOverlaySceneProcess");
	OVERLAY_VAL_LOOKUP(overlay_dashboard_scene_process, punProcessId);
	LOG_EXIT_RC(rc, "CppStubGetDashboardOverlaySceneProcess");
}

uint32_t VROverlayCursor::GetKeyboardText(char * pchText, uint32_t cchText)
{
	LOG_ENTRY("CppStubGetKeyboardText");
	uint32_t rc;
	CURSOR_SYNC_STATE(keyboard_text, keyboard_text);
	util_vector_to_return_buf_rc(&keyboard_text->val, pchText, cchText, &rc);
	LOG_EXIT_RC(rc, "CppStubGetKeyboardText");
}

vr::EVROverlayError VROverlayCursor::GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags)
{
	LOG_ENTRY("CppStubGetOverlayFlags");
	OVERLAY_VAL_LOOKUP(overlay_flags, pFlags);
	LOG_EXIT_RC(rc, "CppStubGetOverlayFlags");
}

