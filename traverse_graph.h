#pragma once

#include "vr_schema.h"

#include "vr_system_wrapper.h"
#include "vr_render_model_wrapper.h"
#include "vr_applications_wrapper.h"
#include "vr_settings_wrapper.h"
#include "vr_chaperone_wrapper.h"
#include "vr_chaperone_setup_wrapper.h"
#include "vr_compositor_wrapper.h"
#include "vr_overlay_wrapper.h"
#include "vr_extended_display_wrapper.h"
#include "vr_tracked_camera_wrapper.h"
#include "vr_resources_wrapper.h"

#include "vr_keys.h"

namespace vr_result
{

#define START_VECTOR(vector_name) \
visitor.start_vector(ss->vector_name.get_url(), ss->vector_name)

#define END_VECTOR(vector_name) \
visitor.end_vector(ss->vector_name.get_url(), ss->vector_name)


#define LEAF_SCALAR(member_name, wrapper_call)\
if (visitor.visit_source_interfaces())\
{\
	decltype(wrapper_call) member_name_temporaryX(wrapper_call); \
	visitor.visit_node(ss->member_name, member_name_temporaryX);\
}\
else\
{\
	visitor.visit_node(ss->member_name);\
}


// leaf 0 because there is no presence/error code type
#define LEAF_VECTOR0(member_name, wrapper_call)\
if (visitor.visit_source_interfaces())\
{\
	decltype(wrapper_call) member_name(wrapper_call); \
	visitor.visit_node(ss->member_name, member_name.val.data(), member_name.count);\
}\
else\
{\
	visitor.visit_node(ss->member_name);\
}

// for when there IS a presence type
// LEAF_VECTOR1(supported_mime_types, appw.GetApplicationSupportedMimeTypes(app_key));
// the first case creates member_name from invoking the wrapper.
//
// the second case is broken - to keep the same visit_node() call it creates
// a fake object - even though that side should never be read (since visit_source_interfaces() is false)
//
#define LEAF_VECTOR1(member_name, wrapper_call)\
if (visitor.visit_source_interfaces())\
{\
	decltype(wrapper_call) member_name(wrapper_call); \
	visitor.visit_node(ss->member_name, member_name.val.data(), member_name.return_code, member_name.count);\
}\
else\
{\
	visitor.visit_node(ss->member_name); \
}

template <typename visitor_fn>
static void visit_hidden_mesh(visitor_fn &visitor,
	vr_state::hidden_mesh_schema *ss,
	vr::EVREye eEye,
	EHiddenAreaMeshType mesh_type,
	IVRSystem *sysi, SystemWrapper wrap)
{
	if (visitor.visit_source_interfaces())
	{
		Uint32<> hidden_mesh_triangle_count;
		const HmdVector2_t *vertex_data = nullptr;
		uint32_t vertex_data_count = 0;

		vr::HiddenAreaMesh_t mesh = sysi->GetHiddenAreaMesh(eEye, mesh_type);
		vertex_data = mesh.pVertexData;
		hidden_mesh_triangle_count.val = mesh.unTriangleCount;
		if (mesh_type == vr::k_eHiddenAreaMesh_LineLoop)
		{
			vertex_data_count = mesh.unTriangleCount;  // from openvr.h LineLoop->"triangle count" is vertex count
		}
		else
		{
			vertex_data_count = mesh.unTriangleCount * 3;
		}


		visitor.visit_node(ss->hidden_mesh_triangle_count, hidden_mesh_triangle_count);
#if 0
		// TODO: another case of the old invoke with array and count instead of a result
		visitor.visit_node(ss->hidden_mesh_vertices, vertex_data, vertex_data_count);
#endif
	}
	else
	{
		visitor.visit_node(ss->hidden_mesh_triangle_count);
		visitor.visit_node(ss->hidden_mesh_vertices);
	}
}

template <typename visitor_fn>
static void visit_eye_state(visitor_fn &visitor,
	vr_state::eye_schema *ss,
	vr_state::system_schema *system_ss,
	vr::EVREye eEye,
	IVRSystem *sysi, SystemWrapper wrap,
	const vr_keys &c,
	VRAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), eEye);

	LEAF_SCALAR(projection, wrap.GetProjectionMatrix(eEye, c.GetNearZ(), c.GetFarZ()));
	LEAF_SCALAR(eye2head, wrap.GetEyeToHeadTransform(eEye));
	LEAF_SCALAR(projection_raw, wrap.GetProjectionRaw(eEye));

	if (visitor.visit_source_interfaces())
	{
#if 0
		DistortionCoordinates_t *coords;
		int count;


		

		// this is a todo - here the results are not in a result and are relying on the old
		// array and count mechanism.
		//todo:
		// build a result object that can convert down to the final allocator vector type
		// test it.

		bool rc = wrap.ComputeDistortion(eEye, c.GetDistortionSampleWidth(), c.GetDistortionSampleHeight(), &coords, &count);
		visitor.visit_node(ss->distortion, coords, rc, count);
		wrap.FreeDistortion(coords);
#endif
	}
	else
	{
#if 0
		visitor.visit_node(ss->distortion);
#endif
	}

	if (ss->hidden_meshes.size() < 3)
	{
		ss->hidden_meshes.emplace_back(URL(), allocator);
		ss->hidden_meshes.emplace_back(URL(), allocator);
		ss->hidden_meshes.emplace_back(URL(), allocator);
		system_ss->structure_version++;
	}
	START_VECTOR(hidden_meshes);
	for (int i = 0; i < 3; i++)
	{
		visit_hidden_mesh(visitor, &ss->hidden_meshes[i], eEye, EHiddenAreaMeshType(i), sysi, wrap);
	}
	END_VECTOR(hidden_meshes);

	visitor.end_group_node(ss->get_url(), eEye);
}

template <typename visitor_fn>
static void visit_component_on_controller_schema(
	visitor_fn &visitor, vr_state::component_on_controller_schema *ss, RenderModelWrapper wrap,
	const char *render_model_name,
	ControllerState<bool> &controller_state,
	uint32_t component_index)
{
	visitor.start_group_node(ss->get_url(), component_index);

	RenderModelComponentState<bool> transforms;
	RenderModelComponentState<bool> transforms_scroll_wheel;

	if (visitor.visit_source_interfaces() && controller_state.is_present())
	{
		TMPString<> component_name;
		wrap.GetComponentModelName(render_model_name, component_index, &component_name);
		if (component_name.val.size() > 0)
		{
			wrap.GetComponentState(
				render_model_name,
				component_name.val.data(),
				controller_state.val,
				false,
				&transforms);

			wrap.GetComponentState(
				render_model_name,
				component_name.val.data(),
				controller_state.val,
				true,							// scroll_wheel set to true
				&transforms_scroll_wheel);
		}
		else
		{
			transforms.return_code = false;
			transforms_scroll_wheel.return_code = false;
		}
	}
	if (visitor.visit_source_interfaces())
	{
		visitor.visit_node(ss->transforms, transforms);
		visitor.visit_node(ss->transforms_scroll_wheel, transforms_scroll_wheel);
	}
	else
	{
		visitor.visit_node(ss->transforms);
		visitor.visit_node(ss->transforms_scroll_wheel);
	}
	visitor.end_group_node(ss->get_url(), component_index);
}


// visits a property vector (non-string type) 
template <typename visitor_fn, typename ResultType>
void visit_vec(visitor_fn &visitor,
	TrackedDeviceIndex_t unDeviceIndex,
	vr_state::VECTOR_OF_NODES<ResultType> &vec,		// vector of the "ResultTypes" ie bool, float etc etc
	SystemWrapper &wrap,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	visitor.start_vector(vec.get_url(), vec);
	for (int index = 0; index < num_props; index++)
	{
		ResultType result;
		if (visitor.visit_source_interfaces())
		{
			vr::ETrackedDeviceProperty prop = (vr::ETrackedDeviceProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetTrackedDeviceProperty(unDeviceIndex, prop, &result);
			visitor.visit_node(vec[index], result);
		}
		else
		{
			visitor.visit_node(vec[index]);
		}
	}
	visitor.end_vector(vec.get_url(), vec);
}

template <typename visitor_fn, typename ResultType>
void visit_string_vec(visitor_fn &visitor,
	TrackedDeviceIndex_t unDeviceIndex,
	vr_state::VECTOR_OF_NODES<ResultType> &vec,
	SystemWrapper &wrap,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{

	visitor.start_vector(vec.get_url(), vec);

	int num_props = indexer->GetNumPropertiesOfType(prop_type);

	if (visitor.visit_source_interfaces())
	{
		TMPString<ETrackedPropertyError> result;
		for (int index = 0; index < num_props; index++)
		{
			vr::ETrackedDeviceProperty prop = (vr::ETrackedDeviceProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetStringTrackedDeviceProperty(unDeviceIndex, prop, &result);
			visitor.visit_node(vec[index], result);
		}
	}
	else
	{
		for (int index = 0; index < num_props; index++)
		{
			visitor.visit_node(vec[index]);
		}
	}

	visitor.end_vector(vec.get_url(), vec);
}


template <typename visitor_fn>
static void visit_controller_state(visitor_fn &visitor,
	vr_state::system_controller_schema *ss,
	vr_state::system_schema *system_ss,
	SystemWrapper &wrap, RenderModelWrapper &rmw,
	int controller_index,
	PropertiesIndexer *indexer,
	VRAllocator &allocator)
{
	{
		LEAF_SCALAR(activity_level, wrap.GetTrackedDeviceActivityLevel(controller_index));
		LEAF_SCALAR(controller_role, wrap.GetControllerRoleForTrackedDeviceIndex(controller_index));
		LEAF_SCALAR(device_class, wrap.GetTrackedDeviceClass(controller_index));
		LEAF_SCALAR(connected, wrap.IsTrackedDeviceConnected(controller_index));
	}

	// used in two places:
	ControllerState<bool> controller_state;
	{
		if (visitor.visit_source_interfaces())
		{
			DevicePose<bool> synced_pose;

			wrap.GetControllerStateWithPose(
				vr::TrackingUniverseSeated, controller_index, &controller_state, &synced_pose);
			visitor.visit_node(ss->synced_seated_pose, synced_pose);

			wrap.GetControllerStateWithPose(
				vr::TrackingUniverseStanding, controller_index, &controller_state, &synced_pose);
			visitor.visit_node(ss->synced_standing_pose, synced_pose);

			wrap.GetControllerStateWithPose(
				vr::TrackingUniverseRawAndUncalibrated, controller_index, &controller_state, &synced_pose);
			visitor.visit_node(ss->synced_raw_pose, synced_pose);

			visitor.visit_node(ss->controller_state, controller_state);
		}
		else
		{
			visitor.visit_node(ss->synced_seated_pose);
			visitor.visit_node(ss->synced_standing_pose);
			visitor.visit_node(ss->synced_raw_pose);
			visitor.visit_node(ss->controller_state);
		}
	}

	{
		structure_check(&system_ss->structure_version, ss->string_props, indexer, PropertiesIndexer::PROP_STRING, allocator);
		visit_string_vec(visitor, controller_index, ss->string_props, wrap, indexer, PropertiesIndexer::PROP_STRING, "string props");

		structure_check(&system_ss->structure_version, ss->bool_props, indexer, PropertiesIndexer::PROP_BOOL, allocator);
		visit_vec(visitor, controller_index, ss->bool_props, wrap, indexer, PropertiesIndexer::PROP_BOOL, "bool props");

		structure_check(&system_ss->structure_version, ss->float_props, indexer, PropertiesIndexer::PROP_FLOAT, allocator);
		visit_vec(visitor, controller_index, ss->float_props, wrap, indexer, PropertiesIndexer::PROP_FLOAT, "float props");

		structure_check(&system_ss->structure_version, ss->mat34_props, indexer, PropertiesIndexer::PROP_MAT34, allocator);
		visit_vec(visitor, controller_index, ss->mat34_props, wrap, indexer, PropertiesIndexer::PROP_MAT34, "mat34 props");

		structure_check(&system_ss->structure_version, ss->int32_props, indexer, PropertiesIndexer::PROP_INT32, allocator);
		visit_vec(visitor, controller_index, ss->int32_props, wrap, indexer, PropertiesIndexer::PROP_INT32, "int32 props");

		structure_check(&system_ss->structure_version, ss->uint64_props, indexer, PropertiesIndexer::PROP_UINT64, allocator);
		visit_vec(visitor, controller_index, ss->uint64_props, wrap, indexer, PropertiesIndexer::PROP_UINT64, "uint64 props");
	}
	// update the component states on this controller
	// based on: component name
	//           render model name
	//           controller state

	// render model name comes from a property.  to avoid coupling to visit_string_properties, 
	// just look it up again

	{
		TMPString<ETrackedPropertyError> render_model;
		int component_count = 0;
		if (visitor.visit_source_interfaces())
		{
			wrap.GetStringTrackedDeviceProperty(controller_index, vr::Prop_RenderModelName_String, &render_model);
			if (render_model.is_present())
			{
				component_count = rmw.GetComponentCount(render_model.val.data());
				ss->components.reserve(component_count);
				while ((int)ss->components.size() <component_count)
				{
					ss->components.emplace_back(URL(), allocator);
					system_ss->structure_version++;
				}
			}
		}
		else
		{
			component_count = (int)ss->components.size();
		}

		START_VECTOR(components);
		for (int i = 0; i < (int)ss->components.size(); i++)
		{
			visit_component_on_controller_schema(visitor, &ss->components[i], rmw, render_model.val.data(), controller_state, i);
		}
		END_VECTOR(components);
	}
}

template <typename visitor_fn>
static void visit_system_node(
	visitor_fn &visitor,
	vr_state::system_schema *ss,
	IVRSystem *sysi, SystemWrapper sysw,
	RenderModelWrapper rmw,
	vr_keys &resource_keys,
	VRAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);
	{
		LEAF_SCALAR(recommended_target_size, sysw.GetRecommendedRenderTargetSize());
		LEAF_SCALAR(is_display_on_desktop, sysw.GetIsDisplayOnDesktop());
		LEAF_SCALAR(seated2standing, sysw.GetSeatedZeroPoseToStandingAbsoluteTrackingPose());
		LEAF_SCALAR(raw2standing, sysw.GetRawZeroPoseToStandingAbsoluteTrackingPose());

		LEAF_SCALAR(num_hmd, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_HMD));
		LEAF_SCALAR(num_controller, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_Controller));
		LEAF_SCALAR(num_tracking, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_GenericTracker));
		LEAF_SCALAR(num_reference, sysw.CountDevicesOfClass(TrackedDeviceClass_TrackingReference));

		LEAF_SCALAR(input_focus_captured_by_other, sysw.IsInputFocusCapturedByAnotherProcess());


		if (visitor.visit_source_interfaces())
		{
			Float<bool>		seconds_since_last_vsync;
			Uint64<bool>	frame_counter_since_last_vsync;
			sysw.GetTimeSinceLastVsync(&seconds_since_last_vsync,
				&frame_counter_since_last_vsync);
			visitor.visit_node(ss->seconds_since_last_vsync, seconds_since_last_vsync);
			visitor.visit_node(ss->frame_counter_since_last_vsync, frame_counter_since_last_vsync);
		}
		else
		{
			visitor.visit_node(ss->seconds_since_last_vsync);
			visitor.visit_node(ss->frame_counter_since_last_vsync);
		}

		LEAF_SCALAR(d3d9_adapter_index, sysw.GetD3D9AdapterIndex());
		LEAF_SCALAR(dxgi_output_info, sysw.GetDXGIOutputInfo());
	}

	//
	// eyes
	//
	{
		if (ss->eyes.size() < 2)
		{
			ss->eyes.emplace_back(ss->eyes.make_name("left"), allocator);
			ss->eyes.emplace_back(ss->eyes.make_name("right"), allocator);
			ss->structure_version++;
		}
		START_VECTOR(eyes);
		for (int i = 0; i < 2; i++)
		{
			vr::EVREye eEye = (i == 0) ? vr::Eye_Left : vr::Eye_Right;
			visit_eye_state(visitor, &ss->eyes[i], ss, eEye, sysi, sysw, resource_keys, allocator);
		}
		END_VECTOR(eyes);
	}

	//
	// controllers
	//
	{
		ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			std::string name(std::to_string(ss->controllers.size()));
			ss->controllers.emplace_back(ss->make_name(name.c_str()), allocator);
			ss->structure_version++;
		}
		START_VECTOR(controllers);
		{
			TrackedDevicePose_t raw_pose_array[vr::k_unMaxTrackedDeviceCount];
			TrackedDevicePose_t standing_pose_array[vr::k_unMaxTrackedDeviceCount];
			TrackedDevicePose_t seated_pose_array[vr::k_unMaxTrackedDeviceCount];

			if (visitor.visit_source_interfaces())
			{
				memset(raw_pose_array, 0, sizeof(raw_pose_array));	// 2/6/2017 - on error this stuff should be zero
				memset(standing_pose_array, 0, sizeof(standing_pose_array));
				memset(seated_pose_array, 0, sizeof(seated_pose_array));
				sysi->GetDeviceToAbsoluteTrackingPose(TrackingUniverseRawAndUncalibrated,
					resource_keys.GetPredictedSecondsToPhoton(), raw_pose_array, vr::k_unMaxTrackedDeviceCount);

				sysi->GetDeviceToAbsoluteTrackingPose(TrackingUniverseStanding,
					resource_keys.GetPredictedSecondsToPhoton(), standing_pose_array, vr::k_unMaxTrackedDeviceCount);

				sysi->GetDeviceToAbsoluteTrackingPose(TrackingUniverseSeated,
					resource_keys.GetPredictedSecondsToPhoton(), seated_pose_array, vr::k_unMaxTrackedDeviceCount);
			}

			PropertiesIndexer *indexer = &resource_keys.GetDevicePropertiesIndexer();

			for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
			{
				// peek for a meaningful name
				const char *group_name = "controllers";
				visitor.start_group_node(ss->controllers.get_url(), i);

				if (visitor.visit_source_interfaces())
				{
					visitor.visit_node(ss->controllers[i].raw_tracking_pose, make_result(raw_pose_array[i]));
					visitor.visit_node(ss->controllers[i].standing_tracking_pose, make_result(standing_pose_array[i]));
					visitor.visit_node(ss->controllers[i].seated_tracking_pose, make_result(seated_pose_array[i]));
				}
				else
				{
					visitor.visit_node(ss->controllers[i].raw_tracking_pose);
					visitor.visit_node(ss->controllers[i].standing_tracking_pose);
					visitor.visit_node(ss->controllers[i].seated_tracking_pose);
				}
				visit_controller_state(visitor, &ss->controllers[i], ss, sysw, rmw, i, indexer, allocator);
				visitor.end_group_node(ss->controllers.get_url(), i);
			}
		}
		END_VECTOR(controllers);
	}

	//
	// spatial sorts
	//
	ss->spatial_sorts.reserve(vr::k_unMaxTrackedDeviceCount + 1);
	while (ss->spatial_sorts.size() < vr::k_unMaxTrackedDeviceCount + 1)
	{
		std::string name(std::to_string(ss->spatial_sorts.size()));
		ss->spatial_sorts.emplace_back(ss->make_name(name.c_str()), allocator);
	}

	START_VECTOR(spatial_sorts);
	for (unsigned i = 0; i < k_unMaxTrackedDeviceCount + 1; i++)
	{
		unsigned unRelativeToTrackedDeviceIndex = -1 + i;
		if (visitor.visit_source_interfaces())
		{
			TMPDeviceIndexes result;			

			sysw.GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass_HMD, unRelativeToTrackedDeviceIndex, &result);
			visitor.visit_node(ss->spatial_sorts[i].hmds_sorted, result);

			sysw.GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass_Controller, unRelativeToTrackedDeviceIndex, &result);
			visitor.visit_node(ss->spatial_sorts[i].controllers_sorted, result);

			sysw.GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass_GenericTracker, unRelativeToTrackedDeviceIndex, &result);
			visitor.visit_node(ss->spatial_sorts[i].trackers_sorted, result);

			sysw.GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass_TrackingReference, unRelativeToTrackedDeviceIndex, &result);
			visitor.visit_node(ss->spatial_sorts[i].reference_sorted, result);
		}
		else
		{
			visitor.visit_node(ss->spatial_sorts[i].hmds_sorted);
			visitor.visit_node(ss->spatial_sorts[i].controllers_sorted);
			visitor.visit_node(ss->spatial_sorts[i].trackers_sorted);
			visitor.visit_node(ss->spatial_sorts[i].reference_sorted);
		}
	}
	END_VECTOR(spatial_sorts);
	visitor.end_group_node(ss->get_url(), -1);
}


// structure check means to 
// expand internal containers when required
template <typename ResultType>
void structure_check(int *structure_version,
	vr_state::VECTOR_OF_NODES<ResultType> &vec,
	//std::vector<HTYPE, VRAllocator> &vec,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type, VRAllocator &allocator)
{
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	if (size_as_int(vec.size()) < num_props)
	{
		vec.reserve(num_props);
		for (int index = 0; index < num_props; index++)
		{
			const char *prop_name = indexer->GetName(prop_type, index);
			vec.emplace_back(vec.make_name(prop_name), allocator);
		}
		*structure_version += 1;
	}
}


template <typename visitor_fn, typename ResultType>
void visit_vec(visitor_fn &visitor,
	vr_state::VECTOR_OF_NODES<ResultType> &vec,
	ApplicationsWrapper &wrap,
	const char *app_key,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	visitor.start_vector(vector_name, vec);
	for (int index = 0; index < num_props; index++)
	{
		type_name, EVRApplicationError> result;
		if (visitor.visit_source_interfaces())
		{
			vr::EVRApplicationProperty prop = (vr::EVRApplicationProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetProperty(app_key, prop, &result);
			visitor.visit_node(vec[index], result);
		}
		else
		{
			visitor.visit_node(vec[index]);
		}
	}
	visitor.end_vector(vector_name, vec);
}


template <typename visitor_fn, typename ResultType>
void visit_string_vec(visitor_fn &visitor,
	vr_state::VECTOR_OF_NODES<ResultType> &vec,
	ApplicationsWrapper &wrap,
	const char *app_key,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{
	visitor.start_vector(vector_name, vec);
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	if (visitor.visit_source_interfaces())
	{
		vector_result<char, EVRApplicationError> result(wrap.string_pool);
		for (int index = 0; index < num_props; index++)
		{
			vr::EVRApplicationProperty prop = (vr::EVRApplicationProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetStringProperty(app_key, prop, &result);
			visitor.visit_node(vec[index], result.val.data(), result.return_code, result.count);
		}
	}
	else
	{
		for (int index = 0; index < num_props; index++)
		{
			visitor.visit_node(vec[index]);
		}
	}
	visitor.end_vector(vector_name, vec);
}


template <typename visitor_fn>
void visit_application_state(visitor_fn &visitor, vr_state::applications_schema *applications,
	ApplicationsWrapper &wrap, uint32_t app_index,
	vr_keys &resource_keys, VRAllocator &allocator)
{
	const char *app_key = nullptr;
	int app_key_string_size;
	app_key = resource_keys.GetApplicationsIndexer().get_key_for_index(app_index, &app_key_string_size);
	visitor.start_group_node(app_key, -1);

	vr_state::application_schema *ss = &applications->applications[app_index];


	if (visitor.visit_source_interfaces())
	{
		visitor.visit_node(ss->application_key, app_key, app_key_string_size);
		Uint32<> process_id_tmp = wrap.GetApplicationProcessId(app_key);
		LEAF_SCALAR(process_id, process_id_tmp);
		LEAF_VECTOR0(application_launch_arguments, wrap.GetApplicationLaunchArguments(process_id_tmp.val));
	}
	else
	{
		visitor.visit_node(ss->application_key);
		visitor.visit_node(ss->process_id);
		visitor.visit_node(ss->application_launch_arguments);
	}

	LEAF_SCALAR(is_installed, wrap.IsApplicationInstalled(app_key));
	LEAF_SCALAR(auto_launch, wrap.GetApplicationAutoLaunch(app_key));
	LEAF_VECTOR1(supported_mime_types, wrap.GetApplicationSupportedMimeTypes(app_key));

	ApplicationsPropertiesIndexer *indexer = &resource_keys.GetApplicationsPropertiesIndexer();

	structure_check(&applications->structure_version, ss->string_props, indexer, PropertiesIndexer::PROP_STRING, allocator);
	visit_string_vec(visitor, ss->string_props, wrap, app_key, indexer, PropertiesIndexer::PROP_STRING, "string_props");

	structure_check(&applications->structure_version, ss->bool_props, indexer, PropertiesIndexer::PROP_BOOL, allocator);
	visit_vec(visitor, ss->bool_props, wrap, app_key, indexer, PropertiesIndexer::PROP_BOOL, "bool_props");

	structure_check(&applications->structure_version, ss->uint64_props, indexer, PropertiesIndexer::PROP_UINT64, allocator);
	visit_vec(visitor, ss->uint64_props, wrap, app_key, indexer, PropertiesIndexer::PROP_UINT64, "uint64_props");

	visitor.end_group_node(app_key, -1);
}


template <typename visitor_fn>
void visit_mime_type_schema(visitor_fn &visitor, vr_state::mime_type_schema *ss,
	ApplicationsWrapper wrap, uint32_t mime_index, vr_keys &resource_keys)
{
	const char *mime_type = resource_keys.GetMimeTypesIndexer().GetNameForIndex(mime_index);
	if (visitor.visit_source_interfaces())
	{
		visitor.visit_node(ss->mime_type, mime_type, (int)strlen(mime_type) + 1);
	}
	else
	{
		visitor.visit_node(ss->mime_type);
	}

	LEAF_VECTOR1(default_application, wrap.GetDefaultApplicationForMimeType(mime_type));
	LEAF_VECTOR0(applications_that_support_mime_type, wrap.GetApplicationsThatSupportMimeType(mime_type));
}


template <typename visitor_fn>
static void visit_applications_node(visitor_fn &visitor, vr_state::applications_schema *ss, ApplicationsWrapper &wrap,
	vr_keys &resource_keys, VRAllocator &allocator)
{
	visitor.start_group_node("applications", -1);

	if (visitor.visit_source_interfaces())
	{
		std::vector<int> active_indexes;
		resource_keys.GetApplicationsIndexer().update(&active_indexes, wrap);

		int *ptr = nullptr;
		if (active_indexes.size() > 0)
		{
			ptr = &active_indexes.at(0);
		}
		visitor.visit_node(ss->active_application_indexes, ptr, (int)active_indexes.size());
	}
	else
	{
		visitor.visit_node(ss->active_application_indexes);
	}

	if (resource_keys.GetApplicationsIndexer().get_num_applications() > (int)ss->applications.size())
	{
		ss->applications.reserve(resource_keys.GetApplicationsIndexer().get_num_applications());
		while ((int)ss->applications.size() < resource_keys.GetApplicationsIndexer().get_num_applications())
		{
			ss->applications.emplace_back(URL(), allocator);
		}
		ss->structure_version++;
	}

	LEAF_VECTOR1(starting_application, wrap.GetStartingApplication());
	LEAF_SCALAR(transition_state, wrap.GetTransitionState());
	LEAF_SCALAR(is_quit_user_prompt, wrap.IsQuitUserPromptRequested());
	LEAF_SCALAR(current_scene_process_id, wrap.GetCurrentSceneProcessId());

	int num_mime_types = resource_keys.GetMimeTypesIndexer().GetNumMimeTypes();
	ss->mime_types.reserve(num_mime_types);
	while (ss->mime_types.size() < num_mime_types)
	{
		ss->mime_types.emplace_back(URL(), allocator);
		ss->structure_version++;
	}

	START_VECTOR(mime_types);
	for (int i = 0; i < num_mime_types; i++)
	{
		visit_mime_type_schema(visitor, &ss->mime_types[i], wrap, i, resource_keys);
	}
	END_VECTOR(mime_types);

	START_VECTOR(applications);
	for (int i = 0; i < (int)ss->applications.size(); i++)
	{
		visit_application_state(visitor, ss, wrap, i, resource_keys, allocator);
	}
	END_VECTOR(applications);

	visitor.end_group_node("applications", -1);
}


template <typename HTYPE>
void structure_check(
	int *structure_version,
	std::vector<HTYPE, VRAllocator> &vec,
	SettingsIndexer *indexer,
	const char *section_name,
	SettingsIndexer::SectionSettingType setting_type, VRAllocator &allocator)
{
	int required_size = indexer->GetNumFields(section_name, setting_type);
	if (vec.size() < required_size)
	{
		vec.reserve(required_size);
		const char **field_names = indexer->GetFieldNames(section_name, setting_type);
		for (int i = vec.size(); i < required_size; i++)
		{
			const char *field_name = field_names[i];
			vec.emplace_back(URL(), field_name, allocator);
		}
		*structure_version += 1;
	}
}

template <typename visitor_fn, typename T>
void visit_subtable2(visitor_fn &visitor,
	vr_state::VECTOR_OF_NODES<Result<T,EVRSettingsError>> &subtable,
	SettingsWrapper sw, const char *section_name,
	SettingsIndexer::SectionSettingType setting_type,
	vr_keys &resource_keys)
{
	visitor.start_group_node(section_name, -1);
	visitor.start_vector("nodes", subtable);
	for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
	{
		T, EVRSettingsError> result;
		if (visitor.visit_source_interfaces())
		{
			// the name of the node is the setting name
			const char *setting_name = iter-.name;
			sw.GetSetting(section_name, setting_name, &result);
			visitor.visit_node(iter-, result);
		}
		else
		{
			visitor.visit_node(iter-);
		}
	}
	visitor.end_vector("nodes", subtable);
	visitor.end_group_node(section_name, -1);
}


template <typename visitor_fn, typename T>
void visit_string_subtable2(visitor_fn &visitor,
	vr_state::VECTOR_OF_NODES<Result<T, EVRSettingsError>> &subtable,
	SettingsWrapper &wrap, const char *section_name,
	SettingsIndexer::SectionSettingType setting_type,
	vr_keys &resource_keys)
{
	visitor.start_group_node(section_name, -1);
	visitor.start_vector("nodes", subtable);
	if (visitor.visit_source_interfaces())
	{
		vector_result<char, EVRSettingsError> result(wrap.string_pool);
		for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
		{
			// the name of the node is the setting name
			const char *setting_name = iter-.name;
			wrap.GetStringSetting(section_name, setting_name, &result);
			visitor.visit_node(iter-, result.val.data(), result.return_code, result.count);
		}
	}
	else
	{
		for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
		{
			visitor.visit_node(iter-);
		}
	}
	visitor.end_vector("nodes", subtable);
	visitor.end_group_node(section_name, -1);
}

template <typename visitor_fn>
static void visit_section(
	visitor_fn &visitor,
	const char *section_name,
	vr_state::section_schema *s,
	int *structure_version,
	SettingsWrapper &wrap,
	vr_keys &resource_keys,
	VRAllocator &allocator
)
{
	SettingsIndexer * indexer = &resource_keys.GetSettingsIndexer();

	structure_check(structure_version, s->bool_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_BOOL, allocator);
	visit_subtable2<visitor_fn, bool>(visitor, s->bool_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_BOOL, resource_keys);

	structure_check(structure_version, s->string_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_STRING, allocator);
	visit_string_subtable2(visitor, s->string_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_STRING, resource_keys);

	structure_check(structure_version, s->float_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_FLOAT, allocator);
	visit_subtable2(visitor, s->float_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_FLOAT, resource_keys);

	structure_check(structure_version, s->int32_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_INT32, allocator);
	visit_subtable2(visitor, s->int32_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_INT32, resource_keys);
}


template <typename visitor_fn>
static void visit_settings_node(
	visitor_fn &visitor,
	vr_state::settings_schema *ss,
	SettingsWrapper& wrap,
	vr_keys &resource_keys,
	VRAllocator &allocator)
{
	visitor.start_group_node("settings", -1);

	int required_size = resource_keys.GetSettingsIndexer().GetNumSections();
	if (ss->sections.size() < required_size)
	{
		ss->sections.reserve(vr::k_unMaxTrackedDeviceCount);
		while (ss->sections.size() < required_size)
		{
			ss->sections.emplace_back(allocator);
		}
	}

	START_VECTOR(sections);
	for (int index = 0; index < required_size; index++)
	{
		visit_section(visitor, resource_keys.GetSettingsIndexer().GetSectionName(index), &ss->sections[index],
			&ss->structure_version, wrap, resource_keys, allocator);
	}
	END_VECTOR(sections);

	visitor.end_group_node("settings", -1);
}


template <typename visitor_fn>
static void visit_chaperone_node(
	visitor_fn &visitor,
	vr_state::chaperone_schema *ss,
	ChaperoneWrapper &wrap,
	const vr_keys &resource_keys)
{
	visitor.start_group_node("chaperone", -1);
	LEAF_SCALAR(calibration_state, wrap.GetCalibrationState());
	LEAF_SCALAR(bounds_visible, wrap.AreBoundsVisible());
	LEAF_SCALAR(play_area_rect, wrap.GetPlayAreaRect());
	LEAF_SCALAR(play_area_size, wrap.GetPlayAreaSize());

	if (visitor.visit_source_interfaces())
	{
		vector_result<HmdColor_t> colors(wrap.string_pool);
		scalar<HmdColor_t> camera_color;

		wrap.GetBoundsColor(&colors, resource_keys.GetNumBoundsColors(),
			resource_keys.GetCollisionBoundsFadeDistance(),
			&camera_color);

		visitor.visit_node(ss->bounds_colors, colors.val.data(), colors.count);
		visitor.visit_node(ss->camera_color, camera_color);
	}
	else
	{
		visitor.visit_node(ss->bounds_colors);
		visitor.visit_node(ss->camera_color);
	}

	visitor.end_group_node("chaperone_schema", -1);
}


template <typename visitor_fn>
static void visit_chaperone_setup_node(visitor_fn &visitor, vr_state::chaperonesetup_schema *ss, ChaperoneSetupWrapper wrap)
{
	visitor.start_group_node("chaperone_setup", -1);
	LEAF_SCALAR(working_play_area_size, wrap.GetWorkingPlayAreaSize());
	LEAF_SCALAR(working_play_area_rect, wrap.GetWorkingPlayAreaRect());
	LEAF_VECTOR1(working_collision_bounds_info, wrap.GetWorkingCollisionBoundsInfo());
	LEAF_VECTOR1(live_collision_bounds_info, wrap.GetLiveCollisionBoundsInfo());
	LEAF_SCALAR(working_seated2rawtracking, wrap.GetWorkingSeatedZeroPoseToRawTrackingPose());
	LEAF_SCALAR(working_standing2rawtracking, wrap.GetWorkingStandingZeroPoseToRawTrackingPose());
	LEAF_VECTOR1(live_collision_bounds_tags_info, wrap.GetLiveCollisionBoundsTagsInfo());
	LEAF_SCALAR(live_seated2rawtracking, wrap.GetLiveSeatedZeroPoseToRawTrackingPose());
	LEAF_VECTOR1(live_physical_bounds_info, wrap.GetLivePhysicalBoundsInfo());
	visitor.end_group_node("chaperone_setup", -1);
}

template <typename visitor_fn>
static void visit_compositor_controller(visitor_fn &visitor,
	vr_state::compositor_controller_schema *ss,
	CompositorWrapper cw, TrackedDeviceIndex_t unDeviceIndex)
{
	visitor.start_group_node("controller", unDeviceIndex);
	if (visitor.visit_source_interfaces())
	{
		DevicePose<EVRCompositorError> last_render_pose;
		DevicePose<EVRCompositorError> last_game_pose;
		cw.GetLastPoseForTrackedDeviceIndex(unDeviceIndex, &last_render_pose, &last_game_pose);
		visitor.visit_node(ss->last_render_pose, last_render_pose);
		visitor.visit_node(ss->last_game_pose, last_game_pose);
	}
	else
	{
		visitor.visit_node(ss->last_render_pose);
		visitor.visit_node(ss->last_game_pose);
	}

	visitor.end_group_node("controller", unDeviceIndex);
}

template <typename visitor_fn>
static void visit_compositor_state(visitor_fn &visitor,
	vr_state::compositor_schema *ss, CompositorWrapper wrap,
	vr_keys &config, VRAllocator &allocator)
{
	visitor.start_group_node("compositor", -1);
	{
		LEAF_SCALAR(tracking_space, wrap.GetTrackingSpace());
		LEAF_SCALAR(frame_timing, wrap.GetFrameTiming(config.GetFrameTimingFramesAgo()));
		LEAF_SCALAR(frame_time_remaining, wrap.GetFrameTimeRemaining());
		LEAF_SCALAR(cumulative_stats, wrap.GetCumulativeStats());
		LEAF_SCALAR(foreground_fade_color, wrap.GetForegroundFadeColor());
		LEAF_SCALAR(background_fade_color, wrap.GetBackgroundFadeColor());
		LEAF_SCALAR(grid_alpha, wrap.GetCurrentGridAlpha());
		LEAF_SCALAR(is_fullscreen, wrap.IsFullscreen());
		LEAF_SCALAR(current_scene_focus_process, wrap.GetCurrentSceneFocusProcess());
		LEAF_SCALAR(last_frame_renderer, wrap.GetLastFrameRenderer());
		LEAF_SCALAR(can_render_scene, wrap.CanRenderScene());
		LEAF_SCALAR(is_mirror_visible, wrap.IsMirrorWindowVisible());
		LEAF_SCALAR(should_app_render_with_low_resource, wrap.ShouldAppRenderWithLowResources());
	}

	if (visitor.visit_source_interfaces())
	{
		vector_result<Compositor_FrameTiming> frame_timings(wrap.string_pool);
		wrap.GetFrameTimings(config.GetFrameTimingsNumFrames(), &frame_timings);
		visitor.visit_node(ss->frame_timings, frame_timings.val.data(), frame_timings.count);
	}
	else
	{
		visitor.visit_node(ss->frame_timings);
	}


	{
		twrap t(" compositor_schema controllers");
		ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			ss->controllers.emplace_back(allocator);
		}

		START_VECTOR(controllers);
		for (int i = 0; i < (int)ss->controllers.size(); i++)
		{
			visit_compositor_controller(visitor, &ss->controllers[i], wrap, i);
		}
		END_VECTOR(controllers);
	}

	if (visitor.visit_source_interfaces())
	{
		twrap t(" compositor_schema vulkan");
		vector_result<char> extensions(wrap.string_pool);
		wrap.GetVulkanInstanceExtensionsRequired(&extensions);
		visitor.visit_node(ss->instance_extensions_required, extensions.val.data(), extensions.count);
	}
	else
	{
		visitor.visit_node(ss->instance_extensions_required);
	}

	visitor.end_group_node("compositor_schema", -1);
}

template <typename visitor_fn>
static void visit_permodelcomponent(
	visitor_fn &visitor,
	vr_state::rendermodel_component_schema *ss,
	RenderModelWrapper wrap,
	const char *pchRenderModelName, uint32_t component_index)
{
	visitor.start_group_node("component", component_index);

	if (visitor.visit_source_interfaces())
	{
		vector_result<char> component_model_name
			= wrap.GetComponentModelName(pchRenderModelName, component_index);
		visitor.visit_node(ss->component_name, component_model_name.val.data(), component_model_name.count);
		LEAF_SCALAR(button_mask, wrap.GetComponentButtonMask(pchRenderModelName, component_model_name.val.data()));
		LEAF_VECTOR1(render_model_name, wrap.GetComponentRenderModelName(pchRenderModelName, component_model_name.val.data()));
	}
	else
	{
		visitor.visit_node(ss->component_name);
		visitor.visit_node(ss->button_mask);
		visitor.visit_node(ss->render_model_name);
	}

	visitor.end_group_node("component", component_index);
}

template <typename visitor_fn>
static void visit_rendermodel(visitor_fn &visitor,
	vr_state::rendermodel_schema *ss,
	int *structure_version,
	RenderModelWrapper wrap,
	uint32_t unRenderModelIndex, VRAllocator &allocator)
{

	visitor.start_group_node("model", unRenderModelIndex);
	if (visitor.visit_source_interfaces())
	{
		vector_result<char> render_model_name_result(wrap.string_pool);
		render_model_name_result = wrap.GetRenderModelName(unRenderModelIndex);
		visitor.visit_node(ss->render_model_name, render_model_name_result.val.data(), render_model_name_result.count);
		LEAF_VECTOR1(thumbnail_url, wrap.GetRenderModelThumbnailURL(render_model_name_result.val.data()));
		LEAF_VECTOR1(original_path, wrap.GetRenderModelOriginalPath(render_model_name_result.val.data()));
	}
	else
	{
		visitor.visit_node(ss->render_model_name);
		visitor.visit_node(ss->thumbnail_url);
		visitor.visit_node(ss->original_path);
	}

	// every body wants the render model name
	const char *render_model_name = &ss->render_model_name.latest().at(0);

	RenderModel_t *pRenderModel = nullptr;
	RenderModel_TextureMap_t *pTexture = nullptr;
	EVRRenderModelError rc = VRRenderModelError_None;
	const RenderModel_Vertex_t *rVertexData = nullptr;	// Vertex data for the mesh
	uint32_t unVertexCount = 0;						// Number of vertices in the vertex data
	const uint16_t *rIndexData = nullptr;
	uint32_t unTriangleCount = 0;
	uint16_t unWidth, unHeight; // width and height of the texture map in pixels
	unWidth = unHeight = 0;
	const uint8_t *rubTextureMapData = nullptr;
	if (visitor.visit_source_interfaces())
	{
		if (visitor.reload_render_models() || ss->vertex_data.empty())
		{
			rc = wrap.LoadRenderModel(render_model_name, &pRenderModel, &pTexture);
			if (pRenderModel)
			{
				rVertexData = pRenderModel->rVertexData;
				unVertexCount = pRenderModel->unVertexCount;
				rIndexData = pRenderModel->rIndexData;
				unTriangleCount = pRenderModel->unTriangleCount;
			}
			if (pTexture)
			{
				unWidth = pTexture->unWidth;
				unHeight = pTexture->unHeight;
				rubTextureMapData = pTexture->rubTextureMapData;
			}
			visitor.visit_node(ss->vertex_data, rVertexData, rc, unVertexCount);
			visitor.visit_node(ss->index_data, rIndexData, rc, unTriangleCount * 3);
			visitor.visit_node(ss->texture_map_data, rubTextureMapData, rc, unWidth * unHeight * 4);

			uint16_t, EVRRenderModelError> height(unHeight, rc);
			uint16_t, EVRRenderModelError> width(unWidth, rc);
			visitor.visit_node(ss->texture_height, height);
			visitor.visit_node(ss->texture_width, width);

			// Note: TextureID_t::diffuseTextureId is not stored because the whole texture is stored.

			if (pRenderModel)
			{
				wrap.FreeRenderModel(pRenderModel, pTexture);
			}
		}
	}
	else
	{
		visitor.visit_node(ss->vertex_data);
		visitor.visit_node(ss->index_data);
		visitor.visit_node(ss->texture_map_data);
		visitor.visit_node(ss->texture_height);
		visitor.visit_node(ss->texture_width);
	}

	// set default
	int component_count = (int)ss->components.size();
	if (visitor.visit_source_interfaces())
	{
		component_count = (int)wrap.GetComponentCount(render_model_name);
		ss->components.reserve(component_count);
		while ((int)ss->components.size() < component_count)
		{
			ss->components.emplace_back(allocator);
			*structure_version += 1;
		}
	}
	// consider the decoder - he needs to know how many componets are going to
	// be sent to him
	START_VECTOR(components);
	for (int i = 0; i < (int)ss->components.size(); i++)
	{
		visit_permodelcomponent(visitor, &ss->components[i], wrap, render_model_name, i);
	}
	END_VECTOR(components);
	visitor.end_group_node("model", unRenderModelIndex);
}


template <typename visitor_fn>
static void visit_per_overlay(
	visitor_fn &visitor,
	vr_state::overlay_schema *overlay_state,
	OverlayWrapper wrap,
	uint32_t overlay_index,
	vr_keys &config,
	VRAllocator &allocator)
{
	visitor.start_group_node("overlay", overlay_index);

	vr_state::per_overlay_state *ss = &overlay_state->overlays[overlay_index];
	vr::VROverlayHandle_t handle = 0;

	if (visitor.visit_source_interfaces())
	{
		const std::string &key = config.GetOverlayIndexer().get_overlay_key_for_index(overlay_index);
		VROverlayHandle_t, EVROverlayError> handle_result = wrap.GetOverlayHandle(key.c_str());
		handle = handle_result.val;
		vector_result<char, vr::EVROverlayError> name(wrap.string_pool);
		wrap.GetOverlayName(handle, &name);

		visitor.visit_node(ss->overlay_key, key.c_str(), key.size() + 1);
		visitor.visit_node(ss->overlay_handle, handle_result);
		visitor.visit_node(ss->overlay_name, name.val.data(), name.return_code, name.count);

		uint32_t, EVROverlayError> width;
		uint32_t, EVROverlayError> height;
		uint8_t *ptr;
		uint32_t size;
		EVROverlayError err = wrap.GetImageData(handle, &width, &height, &ptr, &size);
		visitor.visit_node(ss->overlay_image_width, width);
		visitor.visit_node(ss->overlay_image_height, height);
		visitor.visit_node(ss->overlay_image_data, ptr, err, size);
		wrap.FreeImageData(ptr);
	}
	else
	{
		visitor.visit_node(ss->overlay_key);
		visitor.visit_node(ss->overlay_handle);
		visitor.visit_node(ss->overlay_name);

		visitor.visit_node(ss->overlay_image_width);
		visitor.visit_node(ss->overlay_image_height);
		visitor.visit_node(ss->overlay_image_data);
	}

	LEAF_SCALAR(overlay_rendering_pid, wrap.GetOverlayRenderingPid(handle));
	LEAF_SCALAR(overlay_flags, wrap.GetOverlayFlags(handle));
	LEAF_SCALAR(overlay_color, wrap.GetOverlayColor(handle));
	LEAF_SCALAR(overlay_alpha, wrap.GetOverlayAlpha(handle));
	LEAF_SCALAR(overlay_texel_aspect, wrap.GetOverlayTexelAspect(handle));
	LEAF_SCALAR(overlay_sort_order, wrap.GetOverlaySortOrder(handle));
	LEAF_SCALAR(overlay_width_in_meters, wrap.GetOverlayWidthInMeters(handle));
	LEAF_SCALAR(overlay_auto_curve_range_in_meters, wrap.GetOverlayAutoCurveDistanceRangeInMeters(handle));
	LEAF_SCALAR(overlay_texture_color_space, wrap.GetOverlayTextureColorSpace(handle));
	LEAF_SCALAR(overlay_texture_bounds, wrap.GetOverlayTextureBounds(handle));
	LEAF_SCALAR(overlay_transform_type, wrap.GetOverlayTransformType(handle));
	LEAF_SCALAR(overlay_transform_absolute, wrap.GetOverlayTransformAbsolute(handle));
	LEAF_SCALAR(overlay_transform_device_relative, wrap.GetOverlayTransformTrackedDeviceRelative(handle));

	LEAF_SCALAR(overlay_input_method, wrap.GetOverlayInputMethod(handle));
	LEAF_SCALAR(overlay_mouse_scale, wrap.GetOverlayMouseScale(handle));
	LEAF_SCALAR(overlay_is_hover_target, wrap.IsHoverTargetOverlay(handle));
	LEAF_SCALAR(overlay_is_visible, wrap.IsOverlayVisible(handle));
	LEAF_SCALAR(overlay_is_active_dashboard, wrap.IsActiveDashboardOverlay(handle));
	LEAF_SCALAR(overlay_dashboard_scene_process, wrap.GetDashboardOverlaySceneProcess(handle));
	LEAF_SCALAR(overlay_texture_size, wrap.GetOverlayTextureSize(handle));


	if (visitor.visit_source_interfaces())
	{
		TrackedDeviceIndex_t, EVROverlayError> device_index;
		vector_result<char, EVROverlayError> name(wrap.string_pool);
		wrap.GetOverlayTransformTrackedDeviceComponent(handle, &device_index, &name);

		visitor.visit_node(ss->overlay_transform_component_relative_device_index, device_index);
		visitor.visit_node(ss->overlay_transform_component_relative_name, name.val.data(), name.return_code, name.count);
	}
	else
	{
		visitor.visit_node(ss->overlay_transform_component_relative_device_index);
		visitor.visit_node(ss->overlay_transform_component_relative_name);
	}

	visitor.end_group_node("overlay", overlay_index);
}

template <typename visitor_fn>
static void visit_overlay_state(visitor_fn &visitor, vr_state::overlay_schema *ss,
	OverlayWrapper wrap,
	vr_keys &config,
	VRAllocator &allocator)
{
	visitor.start_group_node("overlays", -1);

	LEAF_SCALAR(gamepad_focus_overlay, wrap.GetGamepadFocusOverlay());
	LEAF_SCALAR(primary_dashboard_device, wrap.GetPrimaryDashboardDevice());
	LEAF_SCALAR(is_dashboard_visible, wrap.IsDashboardVisible());

	if (visitor.visit_source_interfaces())
	{
		vector_result<char> keyboard_text(wrap.string_pool);
		wrap.GetKeyboardText(&keyboard_text);
		visitor.visit_node(ss->keyboard_text, keyboard_text.val.data(), keyboard_text.count);
	}
	else
	{
		visitor.visit_node(ss->keyboard_text);
	}


	if (visitor.visit_source_interfaces())
	{
		std::vector<int> active_indexes;

		config.GetOverlayIndexer().update(&active_indexes, wrap);

		int *ptr = nullptr;
		if (active_indexes.size() > 0)
		{
			ptr = &active_indexes.at(0);
		}
		visitor.visit_node(ss->active_overlay_indexes, ptr, active_indexes.size());
	}
	else
	{
		visitor.visit_node(ss->active_overlay_indexes);
	}

	if (config.GetOverlayIndexer().get_num_overlays() > (int)ss->overlays.size())
	{
		ss->overlays.reserve(config.GetOverlayIndexer().get_num_overlays());
		while ((int)ss->overlays.size() < config.GetOverlayIndexer().get_num_overlays())
		{
			ss->overlays.emplace_back(allocator);
		}
	}

	// history traversal always goes through the complete set
	// see "Hwrap to track applications and overlays.docx"
	START_VECTOR(overlays);
	for (int i = 0; i < (int)ss->overlays.size(); i++)
	{
		visit_per_overlay(visitor, ss, wrap, i, config, allocator);
	}
	END_VECTOR(overlays);

	visitor.end_group_node("overlays", -1);
}

template <typename visitor_fn>
static void visit_rendermodel_state(visitor_fn &visitor, vr_state::rendermodels_schema *ss, RenderModelWrapper rmw, VRAllocator &allocator)
{
	visitor.start_group_node("render_model", -1);

	if (visitor.visit_source_interfaces())
	{
		Uint32<> current_rendermodels = rmw.GetRenderModelCount();
		int num_render_models = current_rendermodels.val;
		ss->models.reserve(num_render_models);
		while ((int)ss->models.size() < num_render_models)
		{
			ss->models.emplace_back(allocator);
			ss->structure_version += 1;
		}
	}

	START_VECTOR(models);
	for (int i = 0; i < (int)ss->models.size(); i++)
	{
		visit_rendermodel(visitor, &ss->models[i], &ss->structure_version, rmw, i, allocator);
	}
	END_VECTOR(models);
	visitor.end_group_node("render_model", -1);
}


template <typename visitor_fn>
static void visit_extended_display_state(visitor_fn &visitor, vr_state::extendeddisplay_schema *ss, ExtendedDisplayWrapper ew)
{
	visitor.start_group_node("extended_display", -1);
	LEAF_SCALAR(window_bounds, ew.GetWindowBounds());
	LEAF_SCALAR(left_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Left));
	LEAF_SCALAR(right_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Right));
	visitor.end_group_node("extended_display", -1);
}

static const char *FrameTypeToGroupName(EVRTrackedCameraFrameType f)
{
	switch (f)
	{
	case VRTrackedCameraFrameType_Distorted:	return "distorted";
	case VRTrackedCameraFrameType_Undistorted: return "undistorted";
	case VRTrackedCameraFrameType_MaximumUndistorted: return "max_undistorted";
	case MAX_CAMERA_FRAME_TYPES:
		assert(0);
	}
	return "unknown";
}


template <typename visitor_fn>
static void visit_cameraframetype_schema(visitor_fn &visitor,
	vr_state::cameraframetype_schema *ss, TrackedCameraWrapper tcw,
	int device_index,
	EVRTrackedCameraFrameType frame_type,
	vr_keys &config)
{
	visitor.start_group_node(FrameTypeToGroupName(frame_type), -1);

	CameraFrameSize_t, EVRTrackedCameraError> f;
	CameraFrameIntrinsics_t, EVRTrackedCameraError> intrinsics;
	HmdMatrix44_t, EVRTrackedCameraError> projection;
	VideoStreamTextureSize_t, EVRTrackedCameraError> video_texture_size;

	LEAF_SCALAR(frame_size, tcw.GetCameraFrameSize(device_index, frame_type, &f));
	LEAF_SCALAR(intrinsics, tcw.GetCameraIntrinsics(device_index, frame_type, &intrinsics));
	LEAF_SCALAR(projection, tcw.GetCameraProjection(device_index, frame_type, config.GetNearZ(), config.GetFarZ(), &projection));
	LEAF_SCALAR(video_texture_size, tcw.GetVideoStreamTextureSize(device_index, frame_type, &video_texture_size));

	visitor.end_group_node("cameraframetypes", device_index);
}

template <typename visitor_fn>
static void visit_per_controller_state(visitor_fn &visitor,
	vr_state::controller_camera_schema *ss, TrackedCameraWrapper tcw,
	int device_index, vr_keys &resource_keys, VRAllocator &allocator)
{
	visitor.start_group_node("controller", device_index);
	LEAF_SCALAR(has_camera, tcw.HasCamera(device_index));

	if (ss->cameraframetypes.size() < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES)
	{
		ss->cameraframetypes.reserve(EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES);
		for (int i = 0; i < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES; i++)
		{
			ss->cameraframetypes.emplace_back(allocator);
		}
	}
	START_VECTOR(cameraframetypes);
	for (int i = 0; i < (int)ss->cameraframetypes.size(); i++)
	{
		visit_cameraframetype_schema(visitor, &ss->cameraframetypes[i], tcw, device_index, (EVRTrackedCameraFrameType)i, resource_keys);
	}
	END_VECTOR(cameraframetypes);
	visitor.end_group_node("controller", device_index);
}

template <typename visitor_fn>
static void visit_trackedcamera_state(visitor_fn &visitor,
	vr_state::trackedcamera_schema *ss, TrackedCameraWrapper tcw,
	vr_keys &resource_keys,
	VRAllocator &allocator)
{
	visitor.start_group_node("camera", -1);
	ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
	while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
	{
		ss->controllers.emplace_back(allocator);
	}

	START_VECTOR(controllers);
	for (int i = 0; i < (int)ss->controllers.size(); i++)
	{
		visit_per_controller_state(visitor, &ss->controllers[i], tcw, i, resource_keys, allocator);
	}
	END_VECTOR(controllers);

	visitor.end_group_node("camera", -1);
}

template <typename visitor_fn>
static void visit_per_resource(visitor_fn &visitor,
	vr_state::resources_schema *ss, ResourcesWrapper &wrap,
	int i, vr_keys &resource_keys,
	VRAllocator &allocator)
{
	visitor.start_group_node("resource", i);
	if (visitor.visit_source_interfaces())
	{
		int fname_size;
		const char *fname = resource_keys.GetResourcesIndexer().get_filename_for_index(i, &fname_size);
		int dname_size;
		const char *dname = resource_keys.GetResourcesIndexer().get_directoryname_for_index(i, &dname_size);
		visitor.visit_node(ss->resources[i].resource_name, fname, fname_size);
		visitor.visit_node(ss->resources[i].resource_directory, dname, dname_size);

		vector_result<char> full_path(wrap.string_pool);
		wrap.GetFullPath(
			fname,
			dname,
			&full_path);

		visitor.visit_node(ss->resources[i].resource_full_path, full_path.val.data(), full_path.count);

		uint8_t *data;
		uint32_t size = wrap.GetImageData(full_path.val.data(), &data);
		visitor.visit_node(ss->resources[i].resource_data, data, size);
		wrap.FreeImageData(data);
	}
	else
	{
		visitor.visit_node(ss->resources[i].resource_name);
		visitor.visit_node(ss->resources[i].resource_directory);
		visitor.visit_node(ss->resources[i].resource_full_path);
		visitor.visit_node(ss->resources[i].resource_data);
	}
	visitor.end_group_node("resource", i);
}


template <typename visitor_fn>
static void visit_resources_state(visitor_fn &visitor,
	vr_state::resources_schema *ss, ResourcesWrapper &wrap,
	vr_keys &resource_keys,
	VRAllocator &allocator)
{
	visitor.start_group_node("resources", -1);

	if ((int)ss->resources.size() < resource_keys.GetResourcesIndexer().get_num_resources())
	{
		ss->resources.reserve(resource_keys.GetResourcesIndexer().get_num_resources());
		while ((int)ss->resources.size() < resource_keys.GetResourcesIndexer().get_num_resources())
		{
			ss->resources.emplace_back(allocator);
		}
	}

	START_VECTOR(resources);
	for (int i = 0; i < (int)ss->resources.size(); i++)
	{
		visit_per_resource(visitor, ss, wrap, i, resource_keys, allocator);
	}
	END_VECTOR(resources);

	visitor.end_group_node("resources", -1);
}


} // end of namespace