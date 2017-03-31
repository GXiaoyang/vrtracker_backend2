#include "vr_system_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

using namespace vr;

VRSystemCursor::VRSystemCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->get_state()->system_node),
	iter_ref(m_context->get_iterators()->system_node),
	m_property_indexer(&m_context->get_keys()->GetDevicePropertiesIndexer())
{
	SynchronizeChildVectors();
}

void VRSystemCursor::SynchronizeChildVectors()
{
	if (iter_ref.structure_version == state_ref.structure_version)
		return;

	iter_ref.structure_version = state_ref.structure_version;

	iter_ref.eyes.resize(state_ref.eyes.size());
	for (int i = 0; i < size_as_int(state_ref.eyes.size()); i++)
	{
		iter_ref.eyes.back().hidden_meshes.resize(state_ref.eyes[i].hidden_meshes.size());
	}
	iter_ref.controllers.resize(state_ref.controllers.size());

	for (int i = 0; i < size_as_int(iter_ref.controllers.size()); i++)
	{
		iter_ref.controllers[i].components.resize(state_ref.controllers[i].components.size());
		iter_ref.controllers[i].bool_props.resize(state_ref.controllers[i].bool_props.size());
		iter_ref.controllers[i].float_props.resize(state_ref.controllers[i].float_props.size());
		iter_ref.controllers[i].int32_props.resize(state_ref.controllers[i].int32_props.size());
		iter_ref.controllers[i].uint64_props.resize(state_ref.controllers[i].uint64_props.size());
		iter_ref.controllers[i].mat34_props.resize(state_ref.controllers[i].mat34_props.size());
		iter_ref.controllers[i].string_props.resize(state_ref.controllers[i].string_props.size());
	}
	iter_ref.spatial_sorts.resize(state_ref.spatial_sorts.size());
}

bool VRSystemCursor::IsValidDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex)
{
	if (unDeviceIndex <= m_context->get_state()->system_node.controllers.size())
		return true;
	else
		return false;
}

void VRSystemCursor::GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetRecommendedRenderTargetSize");

	CURSOR_SYNC_STATE(size, recommended_target_size);

	*pnWidth = size->val.width;
	*pnHeight = size->val.height;

	LOG_EXIT("CppStubGetRecommendedRenderTargetSize");
}


struct vr::HmdMatrix44_t VRSystemCursor::GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ)
{
	LOG_ENTRY("CppStubGetProjectionMatrix");

	CURSOR_SYNC_STATE(proj, eyes[static_cast<int>(eEye)].projection);

	LOG_EXIT_RC(proj->val, "CppStubGetProjectionMatrix");
}

void VRSystemCursor::GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom)
{
	LOG_ENTRY("CppStubGetProjectionRaw");

	CURSOR_SYNC_STATE(v4, eyes[static_cast<int>(eEye)].projection_raw);

	*pfLeft = v4->val.v[0];
	*pfRight = v4->val.v[1];
	*pfTop = v4->val.v[2];
	*pfBottom = v4->val.v[3];

	LOG_EXIT("CppStubGetProjectionRaw");
}

bool VRSystemCursor::ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates)
{
	LOG_ENTRY("CppStubComputeDistortion");
	CURSOR_SYNC_STATE(distortion, eyes[static_cast<int>(eEye)].distortion);
	if (distortion->is_present() && pDistortionCoordinates)
	{
		int sample_width = m_context->get_keys()->GetDistortionSampleWidth();
		int sample_height = m_context->get_keys()->GetDistortionSampleHeight();
		int offset_x = CLAMP(0, sample_width - 1, (int)(fU * sample_width));
		int offset_y = CLAMP(0, sample_height - 1, (int)(fV * sample_height));

		*pDistortionCoordinates = distortion->val.at(offset_y * sample_width + offset_x);
	}

	LOG_EXIT_RC(distortion->return_code, "CppStubComputeDistortion");
}

struct vr::HmdMatrix34_t VRSystemCursor::GetEyeToHeadTransform(vr::EVREye eEye)
{
	LOG_ENTRY("CppStubGetEyeToHeadTransform");
	CURSOR_SYNC_STATE(tx, eyes[static_cast<int>(eEye)].eye2head);
	LOG_EXIT_RC(tx->val, "CppStubGetEyeToHeadTransform");
}

bool VRSystemCursor::GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter)
{
	LOG_ENTRY("CppStubGetTimeSinceLastVsync");

	CURSOR_SYNC_STATE(seconds_since_last_vsync, seconds_since_last_vsync);
	CURSOR_SYNC_STATE(frame_counter_since_last_vsync, frame_counter_since_last_vsync);

	if (seconds_since_last_vsync->is_present())
	{
		if (pfSecondsSinceLastVsync)
		{
			*pfSecondsSinceLastVsync = seconds_since_last_vsync->val;
		}
		if (pulFrameCounter)
		{
			*pulFrameCounter = frame_counter_since_last_vsync->val;
		}
	}

	LOG_EXIT_RC(seconds_since_last_vsync->return_code, "CppStubGetTimeSinceLastVsync");
}

int32_t VRSystemCursor::GetD3D9AdapterIndex()
{
	LOG_ENTRY("CppStubGetD3D9AdapterIndex");
	CURSOR_SYNC_STATE(d3d9_adapter_index, d3d9_adapter_index);
	LOG_EXIT_RC(d3d9_adapter_index->val, "CppStubGetD3D9AdapterIndex");
}

void VRSystemCursor::GetDXGIOutputInfo(int32_t * pnAdapterIndex)
{
	LOG_ENTRY("CppStubGetDXGIOutputInfo");
	CURSOR_SYNC_STATE(dxgi_output_info, dxgi_output_info);
	if (pnAdapterIndex)
	{
		*pnAdapterIndex = dxgi_output_info->val;
	}
	LOG_EXIT("CppStubGetDXGIOutputInfo");
}

bool VRSystemCursor::IsDisplayOnDesktop()
{
	LOG_ENTRY("CppStubIsDisplayOnDesktop");
	CURSOR_SYNC_STATE(is_display_on_desktop, is_display_on_desktop);
	LOG_EXIT_RC(is_display_on_desktop->val, "CppStubIsDisplayOnDesktop");
}

void VRSystemCursor::GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin,
	float fPredictedSecondsToPhotonsFromNow,
	struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount)
{
	LOG_ENTRY("CppStubGetDeviceToAbsoluteTrackingPose");

	assert(fPredictedSecondsToPhotonsFromNow == 0);

	unTrackedDevicePoseArrayCount = std::max(unTrackedDevicePoseArrayCount, vr::k_unMaxTrackedDeviceCount);
	unTrackedDevicePoseArrayCount = std::max(unTrackedDevicePoseArrayCount, (uint32_t)m_context->get_state()->system_node.controllers.size());

	if (eOrigin == vr::TrackingUniverseSeated)
	{
		for (int i = 0; i < size_as_int(unTrackedDevicePoseArrayCount); i++)
		{
			CURSOR_SYNC_STATE(seated_tracking_pose, controllers[i].seated_tracking_pose);
			pTrackedDevicePoseArray[i] = seated_tracking_pose->val;
		}
	}
	else if (eOrigin == vr::TrackingUniverseStanding)
	{
		for (int i = 0; i < size_as_int(unTrackedDevicePoseArrayCount); i++)
		{
			CURSOR_SYNC_STATE(standing_tracking_pose, controllers[i].standing_tracking_pose);
			pTrackedDevicePoseArray[i] = standing_tracking_pose->val;
		}
	}
	else
	{
		for (int i = 0; i < size_as_int(unTrackedDevicePoseArrayCount); i++)
		{
			CURSOR_SYNC_STATE(raw_tracking_pose, controllers[i].raw_tracking_pose);
			pTrackedDevicePoseArray[i] = raw_tracking_pose->val;
		}
	}

	LOG_EXIT("CppStubGetDeviceToAbsoluteTrackingPose");
}

struct vr::HmdMatrix34_t VRSystemCursor::GetSeatedZeroPoseToStandingAbsoluteTrackingPose()
{
	LOG_ENTRY("CppStubGetSeatedZeroPoseToStandingAbsoluteTrackingPose");
	CURSOR_SYNC_STATE(seated2standing, seated2standing)
		LOG_EXIT_RC(seated2standing->val, "CppStubGetSeatedZeroPoseToStandingAbsoluteTrackingPose");
}

struct vr::HmdMatrix34_t VRSystemCursor::GetRawZeroPoseToStandingAbsoluteTrackingPose()
{
	LOG_ENTRY("CppStubGetRawZeroPoseToStandingAbsoluteTrackingPose");
	CURSOR_SYNC_STATE(raw2standing, raw2standing);
	LOG_EXIT_RC(raw2standing->val, "CppStubGetRawZeroPoseToStandingAbsoluteTrackingPose");
}

uint32_t VRSystemCursor::GetSortedTrackedDeviceIndicesOfClass(
	vr::ETrackedDeviceClass eTrackedDeviceClass,
	vr::TrackedDeviceIndex_t * punTrackedDeviceIndexArray,
	uint32_t unTrackedDeviceIndexArrayCount,
	vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex)
{
	LOG_ENTRY("CppStubGetSortedTrackedDeviceIndicesOfClass");

	unsigned int spatial_sort_index = unRelativeToTrackedDeviceIndex + 1;
	uint32_t count = 0;

	switch (eTrackedDeviceClass)
	{
	case TrackedDeviceClass_Invalid:
		return 0;
		break;
	case TrackedDeviceClass_HMD:
	{
		CURSOR_SYNC_STATE(hmds_sorted, spatial_sorts[spatial_sort_index].hmds_sorted);
		util_vector_to_return_buf_rc(&hmds_sorted->val, punTrackedDeviceIndexArray, unTrackedDeviceIndexArrayCount, &count);
	}
	break;

	case TrackedDeviceClass_Controller:
	{
		CURSOR_SYNC_STATE(controllers_sorted, spatial_sorts[spatial_sort_index].controllers_sorted);
		util_vector_to_return_buf_rc(&controllers_sorted->val, punTrackedDeviceIndexArray, unTrackedDeviceIndexArrayCount, &count);
	}
	break;

	case TrackedDeviceClass_GenericTracker:
	{
		CURSOR_SYNC_STATE(trackers_sorted, spatial_sorts[spatial_sort_index].trackers_sorted);
		util_vector_to_return_buf_rc(&trackers_sorted->val, punTrackedDeviceIndexArray, unTrackedDeviceIndexArrayCount, &count);
	}
	break;

	case TrackedDeviceClass_TrackingReference:
	{
		CURSOR_SYNC_STATE(reference_sorted, spatial_sorts[spatial_sort_index].reference_sorted);
		util_vector_to_return_buf_rc(&reference_sorted->val, punTrackedDeviceIndexArray, unTrackedDeviceIndexArrayCount, &count);
	}
	break;
	default:
		assert(0);
	}

	LOG_EXIT_RC(count, "CppStubGetSortedTrackedDeviceIndicesOfClass");
}

vr::EDeviceActivityLevel VRSystemCursor::GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId)
{
	LOG_ENTRY("CppStubGetTrackedDeviceActivityLevel");
	CURSOR_SYNC_STATE(activity_level, controllers[unDeviceId].activity_level);
	LOG_EXIT_RC(activity_level->val, "CppStubGetTrackedDeviceActivityLevel");
}

void VRSystemCursor::ApplyTransform(
	vr::TrackedDevicePose_t *B,
	const vr::TrackedDevicePose_t *A,
	const vr::HmdMatrix34_t *transform)
{
	LOG_ENTRY("CppStubApplyTransform");

	B->vAngularVelocity = A->vAngularVelocity;
	B->eTrackingResult = A->eTrackingResult;
	B->bPoseIsValid = A->bPoseIsValid;
	B->bDeviceIsConnected = A->bDeviceIsConnected;

	// apply transform to pose
	for (int i = 0; i < 4; i++)
	{
		float a = transform->m[0][i];
		float b = transform->m[1][i];
		float c = transform->m[2][i];
		B->mDeviceToAbsoluteTracking.m[0][i] = A->mDeviceToAbsoluteTracking.m[0][0] * a +
			A->mDeviceToAbsoluteTracking.m[0][1] * b +
			A->mDeviceToAbsoluteTracking.m[0][2] * c +
			A->mDeviceToAbsoluteTracking.m[0][3];

		B->mDeviceToAbsoluteTracking.m[1][i] = A->mDeviceToAbsoluteTracking.m[1][0] * a +
			A->mDeviceToAbsoluteTracking.m[1][1] * b +
			A->mDeviceToAbsoluteTracking.m[1][2] * c +
			A->mDeviceToAbsoluteTracking.m[1][3];

		B->mDeviceToAbsoluteTracking.m[2][i] = A->mDeviceToAbsoluteTracking.m[2][0] * a +
			A->mDeviceToAbsoluteTracking.m[2][1] * b +
			A->mDeviceToAbsoluteTracking.m[2][2] * c +
			A->mDeviceToAbsoluteTracking.m[2][3];
	}

	//
	//  update linear velocity
	// 

	// 1. apply rotation part of A to the translation component of the transform ([R][T])
	float tx = transform->m[0][3];
	float ty = transform->m[1][3];
	float tz = transform->m[2][3];
	float rx = A->mDeviceToAbsoluteTracking.m[0][0] * tx +
		A->mDeviceToAbsoluteTracking.m[0][1] * ty +
		A->mDeviceToAbsoluteTracking.m[0][2] * tz;
	float ry = A->mDeviceToAbsoluteTracking.m[1][0] * tx +
		A->mDeviceToAbsoluteTracking.m[1][1] * ty +
		A->mDeviceToAbsoluteTracking.m[1][2] * tz;
	float rz = A->mDeviceToAbsoluteTracking.m[2][0] * tx +
		A->mDeviceToAbsoluteTracking.m[2][1] * ty +
		A->mDeviceToAbsoluteTracking.m[2][2] * tz;

	// final linear velocity is angular cross-prod step-1
	B->vVelocity.v[0] = A->vVelocity.v[0] + A->vAngularVelocity.v[1] * rz - A->vAngularVelocity.v[2] * ry;
	B->vVelocity.v[1] = A->vVelocity.v[1] - (A->vAngularVelocity.v[0] * rz - A->vAngularVelocity.v[2] * rx);
	B->vVelocity.v[2] = A->vVelocity.v[2] + A->vAngularVelocity.v[0] * ry - A->vAngularVelocity.v[1] * rx;

	LOG_EXIT("CppStubApplyTransform");
}

vr::TrackedDeviceIndex_t VRSystemCursor::GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType)
{
	LOG_ENTRY("CppStubGetTrackedDeviceIndexForControllerRole");

	vr::TrackedDeviceIndex_t rc = k_unTrackedDeviceIndexInvalid;
	for (unsigned int i = 0; i < m_context->get_state()->system_node.controllers.size(); i++)
	{
		CURSOR_SYNC_STATE(controller_role, controllers[i].controller_role);
		if (controller_role->val == unDeviceType)
		{
			rc = i;
			break;
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetTrackedDeviceIndexForControllerRole");
}

vr::ETrackedControllerRole VRSystemCursor::GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex)
{
	LOG_ENTRY("CppStubGetControllerRoleForTrackedDeviceIndex");
	vr::ETrackedControllerRole rc = TrackedControllerRole_Invalid;
	if (IsValidDeviceIndex(unDeviceIndex))
	{
		CURSOR_SYNC_STATE(controller_role, controllers[unDeviceIndex].controller_role);
		rc = controller_role->val;
	}
	LOG_EXIT_RC(rc, "CppStubGetControllerRoleForTrackedDeviceIndex");
}

vr::ETrackedDeviceClass VRSystemCursor::GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex)
{
	LOG_ENTRY("CppStubGetTrackedDeviceClass");
	vr::ETrackedDeviceClass rc = TrackedDeviceClass_Invalid;
	if (IsValidDeviceIndex(unDeviceIndex))
	{
		CURSOR_SYNC_STATE(device_class, controllers[unDeviceIndex].device_class);
		rc = device_class->val;
	}
	LOG_EXIT_RC(rc, "CppStubGetTrackedDeviceClass");
}

bool VRSystemCursor::IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex)
{
	LOG_ENTRY("CppStubIsTrackedDeviceConnected");
	bool rc = false;
	if (IsValidDeviceIndex(unDeviceIndex))
	{
		CURSOR_SYNC_STATE(connected, controllers[unDeviceIndex].connected);
		rc = connected->val;
	}
	LOG_EXIT_RC(rc, "CppStubIsTrackedDeviceConnected");
}

bool VRSystemCursor::lookup_property_index(vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum,
	PropertiesIndexer::PropertySettingType setting_type,
	int *property_index, vr::ETrackedPropertyError * pError)
{
	SynchronizeChildVectors();
	bool rc = false;
	vr::ETrackedPropertyError error;
	if (IsValidDeviceIndex(unDeviceIndex))
	{
		if (m_property_indexer->GetIndexForEnum(setting_type, prop_enum, property_index))
		{
			rc = true;
			error = TrackedProp_Success;
		}
		else
		{
			error = TrackedProp_UnknownProperty;
		}
	}
	else
	{
		error = TrackedProp_InvalidDevice;
	}
	if (pError)
	{
		*pError = error;
	}
	return rc;
}

bool VRSystemCursor::GetBoolTrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum,
	vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetBoolTrackedDeviceProperty");
	bool rc = false;
	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_BOOL, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].bool_props[property_index]);
		rc = prop->val;
		if (pError)
			*pError = prop->return_code;
	}
	LOG_EXIT_RC(rc, "CppStubGetBoolTrackedDeviceProperty");
}

float VRSystemCursor::GetFloatTrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum,
	vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetFloatTrackedDeviceProperty");

	float rc = 0.0f;

	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_FLOAT, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].float_props[property_index]);
		rc = prop->val;
		if (pError)
			*pError = prop->return_code;
	}

	LOG_EXIT_RC(rc, "CppStubGetFloatTrackedDeviceProperty");
}

int32_t VRSystemCursor::GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum, vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetInt32TrackedDeviceProperty");

	int32_t rc = 0;
	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_INT32, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].int32_props[property_index]);
		rc = prop->val;
		if (pError)
			*pError = prop->return_code;
	}


	LOG_EXIT_RC(rc, "CppStubGetInt32TrackedDeviceProperty");
}

uint64_t VRSystemCursor::GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum, vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetUint64TrackedDeviceProperty");

	uint64_t rc = 0;
	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_FLOAT, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].uint64_props[property_index]);
		rc = prop->val;
		if (pError)
			*pError = prop->return_code;
	}

	LOG_EXIT_RC(rc, "CppStubGetUint64TrackedDeviceProperty");
}

struct vr::HmdMatrix34_t VRSystemCursor::GetMatrix34TrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum, vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetMatrix34TrackedDeviceProperty");

	vr::HmdMatrix34_t rc;
	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_MAT34, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].mat34_props[property_index]);
		rc = prop->val;
		if (pError)
			*pError = prop->return_code;
	}
	else
	{
		memset(&rc, 0, sizeof(rc));
	}

	LOG_EXIT_RC(rc, "CppStubGetMatrix34TrackedDeviceProperty");
}

uint32_t VRSystemCursor::GetStringTrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex,
	vr::ETrackedDeviceProperty prop_enum,
	char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError)
{
	LOG_ENTRY("CppStubGetStringTrackedDeviceProperty");

	uint32_t rc = 0;
	int property_index;
	if (lookup_property_index(unDeviceIndex, prop_enum, PropertiesIndexer::PROP_STRING, &property_index, pError))
	{
		CURSOR_SYNC_STATE(prop, controllers[unDeviceIndex].string_props[property_index]);
		util_vector_to_return_buf_rc(&prop->val, pchValue, unBufferSize, &rc);
		if (pError)
		{
			*pError = prop->return_code;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetStringTrackedDeviceProperty");
}

const char * VRSystemCursor::GetPropErrorNameFromEnum(vr::ETrackedPropertyError error)
{
	LOG_ENTRY("CppStubGetPropErrorNameFromEnum");
	const char * rc = openvr_string::ETrackedPropertyErrorToString(error);
	LOG_EXIT_RC(rc, "CppStubGetPropErrorNameFromEnum");
}

// caller is going to advance time in 
// getwaitposes
// therefore
// the rule is that PollNextEvent
// returns the next un returned event
//
// which is part of the context
// envison rewinding time to 10.
// and advancing via wait get poses to 15
// then pollnextevent should return events from 10
// up to current time: 15
//
// 

bool VRSystemCursor::PollNextEvent(struct vr::VREvent_t * pEvent, uint32_t uncbVREvent)
{
	LOG_ENTRY("CppStubPollNextEvent");
	bool rc = false;
	rc = m_context->PollNextEvent(pEvent);
	LOG_EXIT_RC(rc, "CppStubPollNextEvent");
}

bool VRSystemCursor::PollNextEventWithPose(
	vr::ETrackingUniverseOrigin eOrigin,
	struct vr::VREvent_t * pEvent,
	uint32_t uncbVREvent,
	vr::TrackedDevicePose_t * pTrackedDevicePose)
{
	LOG_ENTRY("CppStubPollNextEventWithPose");
	assert(0); // todo to do after I've got PollNextEvent working
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubPollNextEventWithPose");
}

const char * VRSystemCursor::GetEventTypeNameFromEnum(vr::EVREventType eType)
{
	LOG_ENTRY("CppStubGetEventTypeNameFromEnum");
	const char * rc = openvr_string::EVREventTypeToString(eType);
	LOG_EXIT_RC(rc, "CppStubGetEventTypeNameFromEnum");
}

struct vr::HiddenAreaMesh_t VRSystemCursor::GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type)
{
	LOG_ENTRY("CppStubGetHiddenAreaMesh");

	CURSOR_SYNC_STATE(vertices, eyes[static_cast<int>(eEye)].hidden_meshes[(int)type].hidden_mesh_vertices);
	CURSOR_SYNC_STATE(triangle_count, eyes[static_cast<int>(eEye)].hidden_meshes[(int)type].hidden_mesh_triangle_count);

	vr::HiddenAreaMesh_t ret;
	ret.pVertexData = &vertices->val[0];
	ret.unTriangleCount = triangle_count->val;

	LOG_EXIT_RC(ret, "CppStubGetHiddenAreaMesh");
}

bool VRSystemCursor::GetControllerState(
	vr::TrackedDeviceIndex_t unControllerDeviceIndex,
	vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize)
{
	LOG_ENTRY("CppStubGetControllerState");

	bool rc = false;
	if (IsValidDeviceIndex(unControllerDeviceIndex) && pControllerState)
	{
		CURSOR_SYNC_STATE(controller_state, controllers[unControllerDeviceIndex].controller_state);
		if (controller_state->is_present())
		{
			*pControllerState = controller_state->val;
			rc = true;
		}
		else
		{
			// 2/12/2017 - even in the false case, he stomps it with zeros
			memset(pControllerState, 0, sizeof(VRControllerState_t));
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetControllerState");
}

bool VRSystemCursor::GetControllerStateWithPose(
	vr::ETrackingUniverseOrigin eOrigin,
	vr::TrackedDeviceIndex_t unControllerDeviceIndex,
	vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize,
	struct vr::TrackedDevicePose_t * pTrackedDevicePose)
{
	LOG_ENTRY("CppStubGetControllerStateWithPose");

	bool rc = false;
	if (IsValidDeviceIndex(unControllerDeviceIndex) && pControllerState)
	{
		CURSOR_SYNC_STATE(controller_state, controllers[unControllerDeviceIndex].controller_state);
		if (controller_state->is_present())
		{
			rc = true;
			if (pControllerState)
			{
				*pControllerState = controller_state->val;
			}
		}
		else
		{
			// 2/12/2017
			// even it it's not present openvr stomps it:
			memset(pControllerState, 0, sizeof(*pControllerState));
		}

		if (rc == true && pTrackedDevicePose)
		{
			switch (eOrigin)
			{
			case TrackingUniverseSeated:
			{
				CURSOR_SYNC_STATE(pose, controllers[unControllerDeviceIndex].synced_seated_pose);
				if (pose->is_present())
				{
					*pTrackedDevicePose = pose->val;
				}
				else
				{
					rc = false;
				}
				break;
			}
			case TrackingUniverseStanding:
			{
				CURSOR_SYNC_STATE(pose, controllers[unControllerDeviceIndex].synced_standing_pose);
				if (pose->is_present())
				{
					*pTrackedDevicePose = pose->val;
				}
				else
				{
					rc = false;
				}
				break;
			}
			case TrackingUniverseRawAndUncalibrated:
			{
				CURSOR_SYNC_STATE(pose, controllers[unControllerDeviceIndex].synced_raw_pose);
				if (pose->is_present())
				{
					*pTrackedDevicePose = pose->val;
				}
				else
				{
					rc = false;
				}

				break;
			}
			}
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetControllerStateWithPose");
}

const char * VRSystemCursor::GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId)
{
	LOG_ENTRY("CppStubGetButtonIdNameFromEnum");
	const char * rc = openvr_string::EVRButtonIdToString(eButtonId);
	LOG_EXIT_RC(rc, "CppStubGetButtonIdNameFromEnum");
}

const char * VRSystemCursor::GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType)
{
	LOG_ENTRY("CppStubGetControllerAxisTypeNameFromEnum");
	const char * rc = openvr_string::EVRControllerAxisTypeToString(eAxisType);
	LOG_EXIT_RC(rc, "CppStubGetControllerAxisTypeNameFromEnum");
}

bool VRSystemCursor::IsInputFocusCapturedByAnotherProcess()
{
	LOG_ENTRY("CppStubIsInputFocusCapturedByAnotherProcess");
	CURSOR_SYNC_STATE(input_focus_captured_by_other, input_focus_captured_by_other);
	LOG_EXIT_RC(input_focus_captured_by_other->val, "CppStubIsInputFocusCapturedByAnotherProcess");
}
