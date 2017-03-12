#pragma once

#include "gsl.h"
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

typedef int PlaceHolderAllocator;

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
	visitor.visit_node(ss->member_name, member_name);\
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
	visitor.visit_node(ss->member_name, member_name);\
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
		visitor.visit_node(ss->hidden_mesh_vertices, make_result(gsl::make_span(vertex_data, vertex_data_count)));
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
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), eEye);

	LEAF_SCALAR(projection, wrap.GetProjectionMatrix(eEye, c.GetNearZ(), c.GetFarZ()));
	LEAF_SCALAR(eye2head, wrap.GetEyeToHeadTransform(eEye));
	LEAF_SCALAR(projection_raw, wrap.GetProjectionRaw(eEye));

	if (visitor.visit_source_interfaces())
	{
		DistortionCoordinates_t *coords;
		int count;

		bool rc = wrap.ComputeDistortion(eEye, c.GetDistortionSampleWidth(), c.GetDistortionSampleHeight(), &coords, &count);

		visitor.visit_node(ss->distortion, make_result(gsl::make_span(coords, count), rc));
		wrap.FreeDistortion(coords);
	}
	else
	{
		visitor.visit_node(ss->distortion);
	}

	if (ss->hidden_meshes.size() < 3)
	{
		ss->hidden_meshes.emplace_back( base::URL());
		ss->hidden_meshes.emplace_back( base::URL());
		ss->hidden_meshes.emplace_back( base::URL());
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
	vr_state::VECTOR_OF_TIMENODES<ResultType> &vec,		// vector of the "ResultTypes" ie bool, float etc etc
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
	vr_state::VECTOR_OF_TIMENODES<ResultType> &vec,
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
	PlaceHolderAllocator &allocator)
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
					ss->components.emplace_back( base::URL());
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
	PlaceHolderAllocator &allocator)
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
			ss->eyes.emplace_back(ss->eyes.make_url_for_child("left"));
			ss->eyes.emplace_back(ss->eyes.make_url_for_child("right"));
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
			ss->controllers.emplace_back(ss->controllers.make_url_for_child(name.c_str()));
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
		ss->spatial_sorts.emplace_back(ss->make_url_for_child(name.c_str()));
	}

	START_VECTOR(spatial_sorts);
	for (unsigned i = 0; i < k_unMaxTrackedDeviceCount + 1; i++)
	{
		// weird, but the iteration starts from -1 in unsigned space:
		/** Get a sorted array of device indices of a given class of tracked devices (e.g. controllers).  Devices are sorted right to left
		* relative to the specified tracked device (default: hmd -- pass in -1 for absolute tracking space).  Returns the number of devices
		* in the list, or the size of the array needed if not large enough. */
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
	vr_state::VECTOR_OF_TIMENODES<ResultType> &vec,
	//std::vector<HTYPE, PlaceHolderAllocator> &vec,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type, PlaceHolderAllocator &allocator)
{
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	if (size_as_int(vec.size()) < num_props)
	{
		vec.reserve(num_props);
		for (int index = 0; index < num_props; index++)
		{
			const char *prop_name = indexer->GetName(prop_type, index);
			vec.emplace_back(vec.make_url_for_child(prop_name));
		}
		*structure_version += 1;
	}
}


template <typename visitor_fn, typename ResultType>
void visit_vec(visitor_fn &visitor,
	vr_state::VECTOR_OF_TIMENODES<ResultType> &vec,
	ApplicationsWrapper &wrap,
	const char *app_key,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	visitor.start_vector(vec.get_url(), vec);
	for (int index = 0; index < num_props; index++)
	{
		ResultType result;
		//type_name, EVRApplicationError> result;
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
	visitor.end_vector(vec.get_url(), vec);
}


template <typename visitor_fn, typename ResultType>
void visit_string_vec(visitor_fn &visitor,
	vr_state::VECTOR_OF_TIMENODES<ResultType> &vec,
	ApplicationsWrapper &wrap,
	const char *app_key,
	PropertiesIndexer *indexer,
	PropertiesIndexer::PropertySettingType prop_type,
	const char* vector_name)
{
	visitor.start_vector(vec.get_url(), vec);
	int num_props = indexer->GetNumPropertiesOfType(prop_type);
	if (visitor.visit_source_interfaces())
	{
		TMPString<EVRApplicationError> result;
		for (int index = 0; index < num_props; index++)
		{
			vr::EVRApplicationProperty prop = (vr::EVRApplicationProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetStringProperty(app_key, prop, &result);
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
void visit_application_state(visitor_fn &visitor, vr_state::applications_schema *applications,
	ApplicationsWrapper &wrap, uint32_t app_index,
	vr_keys &resource_keys, PlaceHolderAllocator &allocator)
{
	const char *app_key = nullptr;
	int app_key_string_size;
	app_key = resource_keys.GetApplicationsIndexer().get_key_for_index(app_index, &app_key_string_size);
	vr_state::application_schema *ss = &applications->applications[app_index];
	visitor.start_group_node(ss->get_url(), -1);


	if (visitor.visit_source_interfaces())
	{
		visitor.visit_node(ss->application_key, make_result(gsl::make_span(app_key, app_key_string_size)));
		Uint32<> process_id_tmp = wrap.GetApplicationProcessId(app_key);
		LEAF_SCALAR(process_id, process_id_tmp);
		TMPString<> tmp;
		LEAF_VECTOR0(application_launch_arguments, wrap.GetApplicationLaunchArguments(process_id_tmp.val, &tmp));
	}
	else
	{
		visitor.visit_node(ss->application_key);
		visitor.visit_node(ss->process_id);
		visitor.visit_node(ss->application_launch_arguments);
	}

	LEAF_SCALAR(is_installed, wrap.IsApplicationInstalled(app_key));
	LEAF_SCALAR(auto_launch, wrap.GetApplicationAutoLaunch(app_key));
	TMPString<bool> tmp;
	LEAF_VECTOR1(supported_mime_types, wrap.GetApplicationSupportedMimeTypes(app_key, &tmp));

	ApplicationsPropertiesIndexer *indexer = &resource_keys.GetApplicationsPropertiesIndexer();

	structure_check(&applications->structure_version, ss->string_props, indexer, PropertiesIndexer::PROP_STRING, allocator);
	visit_string_vec(visitor, ss->string_props, wrap, app_key, indexer, PropertiesIndexer::PROP_STRING, "string_props");

	structure_check(&applications->structure_version, ss->bool_props, indexer, PropertiesIndexer::PROP_BOOL, allocator);
	visit_vec(visitor, ss->bool_props, wrap, app_key, indexer, PropertiesIndexer::PROP_BOOL, "bool_props");

	structure_check(&applications->structure_version, ss->uint64_props, indexer, PropertiesIndexer::PROP_UINT64, allocator);
	visit_vec(visitor, ss->uint64_props, wrap, app_key, indexer, PropertiesIndexer::PROP_UINT64, "uint64_props");

	visitor.end_group_node(ss->get_url(), -1);
}


template <typename visitor_fn>
void visit_mime_type_schema(visitor_fn &visitor, vr_state::mime_type_schema *ss,
	ApplicationsWrapper wrap, uint32_t mime_index, vr_keys &resource_keys)
{
	const char *mime_type = resource_keys.GetMimeTypesIndexer().GetNameForIndex(mime_index);
	if (visitor.visit_source_interfaces())
	{
		visitor.visit_node(ss->mime_type, make_result(gsl::make_span(mime_type, strlen(mime_type) + 1)));
	}
	else
	{
		visitor.visit_node(ss->mime_type);
	}

	TMPString<bool> tmp;
	LEAF_VECTOR1(default_application, wrap.GetDefaultApplicationForMimeType(mime_type, &tmp));
	TMPString<> tmp2;
	LEAF_VECTOR0(applications_that_support_mime_type, wrap.GetApplicationsThatSupportMimeType(mime_type, &tmp2));
}


template <typename visitor_fn>
static void visit_applications_node(visitor_fn &visitor, vr_state::applications_schema *ss, 
	ApplicationsWrapper &wrap,
	vr_keys &resource_keys, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);

	if (visitor.visit_source_interfaces())
	{
		TMPInt32String<> active_indexes;
		resource_keys.GetApplicationsIndexer().update(&active_indexes, wrap);
		visitor.visit_node(ss->active_application_indexes, active_indexes);
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
			const char *application_name = resource_keys.GetApplicationsIndexer().get_key_for_index(ss->applications.size());
			ss->applications.emplace_back(ss->applications.make_url_for_child(application_name));
		}
		ss->structure_version++;
	}

	TMPString<vr::EVRApplicationError> tmp;
	LEAF_VECTOR1(starting_application, wrap.GetStartingApplication(&tmp));
	LEAF_SCALAR(transition_state, wrap.GetTransitionState());
	LEAF_SCALAR(is_quit_user_prompt, wrap.IsQuitUserPromptRequested());
	LEAF_SCALAR(current_scene_process_id, wrap.GetCurrentSceneProcessId());

	int num_mime_types = resource_keys.GetMimeTypesIndexer().GetNumMimeTypes();
	ss->mime_types.reserve(num_mime_types);
	while (size_as_int(ss->mime_types.size()) < num_mime_types)
	{
		const char *mime_type = resource_keys.GetMimeTypesIndexer().GetNameForIndex(ss->mime_types.size());
		ss->mime_types.emplace_back(ss->make_url_for_child(mime_type));
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

	visitor.end_group_node(ss->get_url(), -1);
}


template <typename VecType>
void structure_check(
	int *structure_version,
	VecType &vec,
	SettingsIndexer *indexer,
	const char *section_name,
	SettingsIndexer::SectionSettingType setting_type, PlaceHolderAllocator &allocator)
{
	int required_size = indexer->GetNumFields(section_name, setting_type);
	if (size_as_int(vec.size()) < required_size)
	{
		vec.reserve(required_size);
		const char **field_names = indexer->GetFieldNames(section_name, setting_type);
		for (int i = vec.size(); i < required_size; i++)
		{
			const char *field_name = field_names[i];
			vec.emplace_back(vec.make_url_for_child(field_name));
		}
		*structure_version += 1;
	}
}

template <typename visitor_fn, typename T>
void visit_subtable2(visitor_fn &visitor,
	vr_state::VECTOR_OF_TIMENODES<Result<T,EVRSettingsError>> &subtable,
	SettingsWrapper sw, const char *section_name,
	SettingsIndexer::SectionSettingType setting_type,
	vr_keys &resource_keys)
{
	visitor.start_group_node(subtable.get_url(), -1);
	visitor.start_vector(subtable.get_url(), subtable);
	for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
	{
		Result<T, EVRSettingsError> result;
		if (visitor.visit_source_interfaces())
		{
			// the name of the node is the setting name
			const char *setting_name = iter->get_name().c_str();
			sw.GetSetting(section_name, setting_name, &result);
			visitor.visit_node(*iter, result);
		}
		else
		{
			visitor.visit_node(*iter);
		}
	}
	visitor.end_vector(subtable.get_url(), subtable);
	visitor.end_group_node(subtable.get_url(), -1);
}


template <typename visitor_fn, typename T>
void visit_string_subtable2(visitor_fn &visitor,
	vr_state::VECTOR_OF_TIMENODES<Result<T, EVRSettingsError>> &subtable,
	SettingsWrapper &wrap, const char *section_name,
	SettingsIndexer::SectionSettingType setting_type,
	vr_keys &resource_keys)
{
	visitor.start_group_node(subtable.get_url(), -1);
	visitor.start_vector(subtable.get_url(), subtable);
	if (visitor.visit_source_interfaces())
	{
		TMPString<EVRSettingsError> result;
		for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
		{
			// the name of the node is the setting name
			const char *setting_name = iter->get_name().c_str();
			wrap.GetStringSetting(section_name, setting_name, &result);
			visitor.visit_node(*iter, result);
		}
	}
	else
	{
		for (auto iter = subtable.begin(); iter != subtable.end(); iter++)
		{
			visitor.visit_node(*iter);
		}
	}
	visitor.end_vector(subtable.get_url(), subtable);
	visitor.end_group_node(subtable.get_url(), -1);
}

template <typename visitor_fn>
static void visit_section(
	visitor_fn &visitor,
	const char *section_name,
	vr_state::section_schema *s,
	int *structure_version,
	SettingsWrapper &wrap,
	vr_keys &resource_keys,
	PlaceHolderAllocator &allocator
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
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);

	int required_size = resource_keys.GetSettingsIndexer().GetNumSections();
	if (size_as_int(ss->sections.size()) < required_size)
	{
		ss->sections.reserve(vr::k_unMaxTrackedDeviceCount);
		while (size_as_int(ss->sections.size()) < required_size)
		{
			int section = ss->sections.size();
			ss->sections.emplace_back(ss->make_url_for_child(resource_keys.GetSettingsIndexer().GetSectionName(section)));
		}
	}

	START_VECTOR(sections);
	for (int index = 0; index < required_size; index++)
	{
		visit_section(visitor, resource_keys.GetSettingsIndexer().GetSectionName(index), &ss->sections[index],
			&ss->structure_version, wrap, resource_keys, allocator);
	}
	END_VECTOR(sections);

	visitor.end_group_node(ss->get_url(), -1);
}


template <typename visitor_fn>
static void visit_chaperone_node(
	visitor_fn &visitor,
	vr_state::chaperone_schema *ss,
	ChaperoneWrapper &wrap,
	const vr_keys &resource_keys)
{
	visitor.start_group_node(ss->get_url(), -1);
	LEAF_SCALAR(calibration_state, wrap.GetCalibrationState());
	LEAF_SCALAR(bounds_visible, wrap.AreBoundsVisible());
	LEAF_SCALAR(play_area_rect, wrap.GetPlayAreaRect());
	LEAF_SCALAR(play_area_size, wrap.GetPlayAreaSize());

	if (visitor.visit_source_interfaces())
	{
		TMPHMDColorString<> colors;
		HmdColor<> camera_color;

		wrap.GetBoundsColor(&colors, resource_keys.GetNumBoundsColors(),
			resource_keys.GetCollisionBoundsFadeDistance(),
			&camera_color);

		visitor.visit_node(ss->bounds_colors, colors);
		visitor.visit_node(ss->camera_color, camera_color);
	}
	else
	{
		visitor.visit_node(ss->bounds_colors);
		visitor.visit_node(ss->camera_color);
	}

	visitor.end_group_node(ss->get_url(), -1);
}


template <typename visitor_fn>
static void visit_chaperone_setup_node(visitor_fn &visitor, vr_state::chaperonesetup_schema *ss, ChaperoneSetupWrapper wrap)
{
	visitor.start_group_node(ss->get_url(), -1);
	LEAF_SCALAR(working_play_area_size, wrap.GetWorkingPlayAreaSize());
	LEAF_SCALAR(working_play_area_rect, wrap.GetWorkingPlayAreaRect());

	TMPHmdQuadString<bool> r;
	LEAF_VECTOR1(working_collision_bounds_info, wrap.GetWorkingCollisionBoundsInfo(&r));
	LEAF_VECTOR1(live_collision_bounds_info, wrap.GetLiveCollisionBoundsInfo(&r));

	LEAF_SCALAR(working_seated2rawtracking, wrap.GetWorkingSeatedZeroPoseToRawTrackingPose());
	LEAF_SCALAR(working_standing2rawtracking, wrap.GetWorkingStandingZeroPoseToRawTrackingPose());

	TMPUint8String<bool> r2;
	LEAF_VECTOR1(live_collision_bounds_tags_info, wrap.GetLiveCollisionBoundsTagsInfo(&r2));
	LEAF_SCALAR(live_seated2rawtracking, wrap.GetLiveSeatedZeroPoseToRawTrackingPose());

	LEAF_VECTOR1(live_physical_bounds_info, wrap.GetLivePhysicalBoundsInfo(&r));

	visitor.end_group_node(ss->get_url(), -1);
}

template <typename visitor_fn>
static void visit_compositor_controller(visitor_fn &visitor,
	vr_state::compositor_controller_schema *ss,
	CompositorWrapper cw, TrackedDeviceIndex_t unDeviceIndex)
{
	visitor.start_group_node(ss->get_url(), unDeviceIndex);
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

	visitor.end_group_node(ss->get_url(), unDeviceIndex);
}

template <typename visitor_fn>
static void visit_compositor_state(visitor_fn &visitor,
	vr_state::compositor_schema *ss, CompositorWrapper wrap,
	vr_keys &config, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);
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
		TMPCompositorFrameTimingString<> frame_timings;
		wrap.GetFrameTimings(config.GetFrameTimingsNumFrames(), &frame_timings);
		visitor.visit_node(ss->frame_timings, frame_timings);
	}
	else
	{
		visitor.visit_node(ss->frame_timings);
	}


	{
		ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			ss->controllers.emplace_back(ss->make_url_for_child(std::to_string(ss->controllers.size()).c_str()));
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
		TMPString<> result;
		wrap.GetVulkanInstanceExtensionsRequired(&result);
		visitor.visit_node(ss->instance_extensions_required, result);
	}
	else
	{
		visitor.visit_node(ss->instance_extensions_required);
	}

	visitor.end_group_node(ss->get_url(), -1);
}

template <typename visitor_fn>
static void visit_permodelcomponent(
	visitor_fn &visitor,
	vr_state::rendermodel_component_schema *ss,
	RenderModelWrapper wrap,
	const char *pchRenderModelName, uint32_t component_index)
{
	visitor.start_group_node(ss->get_url(), component_index);

	if (visitor.visit_source_interfaces())
	{
		TMPString<> component_model_name;
		wrap.GetComponentModelName(pchRenderModelName, component_index, &component_model_name);
		visitor.visit_node(ss->component_name, component_model_name);
		LEAF_SCALAR(button_mask, wrap.GetComponentButtonMask(pchRenderModelName, component_model_name.val.data()));
		TMPString<bool> tmp;
		LEAF_VECTOR1(render_model_name, 
				wrap.GetComponentRenderModelName(pchRenderModelName, component_model_name.val.data(), &tmp));
	}
	else
	{
		visitor.visit_node(ss->component_name);
		visitor.visit_node(ss->button_mask);
		visitor.visit_node(ss->render_model_name);
	}

	visitor.end_group_node(ss->get_url(), component_index);
}

template <typename visitor_fn>
static void visit_rendermodel(visitor_fn &visitor,
	vr_state::rendermodel_schema *ss,
	int *structure_version,
	RenderModelWrapper wrap,
	uint32_t unRenderModelIndex, PlaceHolderAllocator &allocator)
{

	visitor.start_group_node(ss->get_url(), unRenderModelIndex);
	if (visitor.visit_source_interfaces())
	{
		TMPString<> name;
		wrap.GetRenderModelName(unRenderModelIndex, &name);

		visitor.visit_node(ss->render_model_name, name);
		TMPString<EVRRenderModelError> tmp;
		visitor.visit_node(ss->thumbnail_url, wrap.GetRenderModelThumbnailURL(name.val.data(), &tmp));
		visitor.visit_node(ss->original_path, wrap.GetRenderModelOriginalPath(name.val.data(), &tmp));
	}
	else
	{
		visitor.visit_node(ss->render_model_name);
		visitor.visit_node(ss->thumbnail_url);
		visitor.visit_node(ss->original_path);
	}

	// every body wants the render model name
	// todo: index the rendermodels
	const char *render_model_name = ss->render_model_name.latest().get_value().val.data();

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
			visitor.visit_node(ss->vertex_data, make_result(gsl::make_span(rVertexData, unVertexCount), rc));
			visitor.visit_node(ss->index_data, make_result(gsl::make_span(rIndexData, unTriangleCount*3), rc));
			visitor.visit_node(ss->texture_map_data, make_result(gsl::make_span(rubTextureMapData, unWidth * unHeight * 4), rc));
			visitor.visit_node(ss->texture_height, make_result(unHeight, rc));
			visitor.visit_node(ss->texture_width, make_result(unWidth, rc));

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
		while (size_as_int(ss->components.size()) < component_count)
		{
			TMPString<> component_model_name;
			wrap.GetComponentModelName(render_model_name, ss->components.size(), &component_model_name);
			ss->components.emplace_back(ss->components.make_url_for_child(component_model_name.val.data()));
			*structure_version += 1;
		}
	}
	// consider the decoder - he needs to know how many componets are going to
	// be sent to him
	START_VECTOR(components);
	for (int i = 0; i < size_as_int(ss->components.size()); i++)
	{
		visit_permodelcomponent(visitor, &ss->components[i], wrap, render_model_name, i);
	}
	END_VECTOR(components);
	visitor.end_group_node(ss->get_url(), unRenderModelIndex);
}


template <typename visitor_fn>
static void visit_per_overlay(
	visitor_fn &visitor,
	vr_state::overlay_schema *overlay_state,
	OverlayWrapper wrap,
	uint32_t overlay_index,
	vr_keys &config,
	PlaceHolderAllocator &allocator)
{
	vr_state::per_overlay_state *ss = &overlay_state->overlays[overlay_index];
	visitor.start_group_node(ss->get_url(), overlay_index);
	
	vr::VROverlayHandle_t handle = 0;

	if (visitor.visit_source_interfaces())
	{
		const char *key = config.GetOverlayIndexer().get_overlay_key_for_index(overlay_index);
		OverlayHandle<EVROverlayError> handle_result = wrap.GetOverlayHandle(key);
		handle = handle_result.val;
		TMPString<vr::EVROverlayError> name;
		wrap.GetOverlayName(handle, &name);

		visitor.visit_node(ss->overlay_handle, handle_result);
		visitor.visit_node(ss->overlay_name, name);

		Uint32<EVROverlayError> width;
		Uint32<EVROverlayError> height;
		uint8_t *ptr;
		uint32_t size;
		EVROverlayError err = wrap.GetImageData(handle, &width, &height, &ptr, &size);
		visitor.visit_node(ss->overlay_image_width, width);
		visitor.visit_node(ss->overlay_image_height, height);
		auto result(make_result(gsl::make_span(ptr, size), err));
		visitor.visit_node(ss->overlay_image_data, result);
		wrap.FreeImageData(ptr);
	}
	else
	{
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
		DeviceIndex<EVROverlayError> device_index;
		TMPString<EVROverlayError> name;
		wrap.GetOverlayTransformTrackedDeviceComponent(handle, &device_index, &name);

		visitor.visit_node(ss->overlay_transform_component_relative_device_index, device_index);
		visitor.visit_node(ss->overlay_transform_component_relative_name, name);
	}
	else
	{
		visitor.visit_node(ss->overlay_transform_component_relative_device_index);
		visitor.visit_node(ss->overlay_transform_component_relative_name);
	}

	visitor.end_group_node(ss->get_url(), overlay_index);
}

template <typename visitor_fn>
static void visit_overlay_state(visitor_fn &visitor, vr_state::overlay_schema *ss,
	OverlayWrapper wrap,
	vr_keys &config,
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);

	LEAF_SCALAR(gamepad_focus_overlay, wrap.GetGamepadFocusOverlay());
	LEAF_SCALAR(primary_dashboard_device, wrap.GetPrimaryDashboardDevice());
	LEAF_SCALAR(is_dashboard_visible, wrap.IsDashboardVisible());

	if (visitor.visit_source_interfaces())
	{
		TMPString<> keyboard_text;
		wrap.GetKeyboardText(&keyboard_text);
		visitor.visit_node(ss->keyboard_text, keyboard_text);
	}
	else
	{
		visitor.visit_node(ss->keyboard_text);
	}


	if (visitor.visit_source_interfaces())
	{
		TMPInt32String<> active_indexes;
		config.GetOverlayIndexer().update(&active_indexes, wrap);
		visitor.visit_node(ss->active_overlay_indexes, active_indexes);
	}
	else
	{
		visitor.visit_node(ss->active_overlay_indexes);
	}

	if (config.GetOverlayIndexer().get_num_overlays() > (int)ss->overlays.size())
	{
		ss->overlays.reserve(config.GetOverlayIndexer().get_num_overlays());
		while (size_as_int(ss->overlays.size()) < config.GetOverlayIndexer().get_num_overlays())
		{
			const char *child_name = config.GetOverlayIndexer().get_overlay_key_for_index(ss->overlays.size());
			ss->overlays.emplace_back(ss->overlays.make_url_for_child(child_name));
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

	visitor.end_group_node(ss->get_url(), -1);
}

template <typename visitor_fn>
static void visit_rendermodel_state(visitor_fn &visitor, vr_state::rendermodels_schema *ss, 
	RenderModelWrapper wrap, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);

	if (visitor.visit_source_interfaces())
	{
		Uint32<> current_rendermodels = wrap.GetRenderModelCount();
		int num_render_models = current_rendermodels.val;
		ss->models.reserve(num_render_models);
		while (size_as_int(ss->models.size()) < num_render_models)
		{
			TMPString<> name;
			wrap.GetRenderModelName(ss->models.size(), &name);

			ss->models.emplace_back(ss->models.make_url_for_child(name.val.data()));
			ss->structure_version += 1;
		}
	}

	START_VECTOR(models);
	for (int i = 0; i < (int)ss->models.size(); i++)
	{
		visit_rendermodel(visitor, &ss->models[i], &ss->structure_version, wrap, i, allocator);
	}
	END_VECTOR(models);
	visitor.end_group_node(ss->get_url(), -1);
}


template <typename visitor_fn>
static void visit_extended_display_state(visitor_fn &visitor, vr_state::extendeddisplay_schema *ss, ExtendedDisplayWrapper ew)
{
	visitor.start_group_node(ss->get_url(), -1);
	LEAF_SCALAR(window_bounds, ew.GetWindowBounds());
	LEAF_SCALAR(left_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Left));
	LEAF_SCALAR(right_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Right));
	visitor.end_group_node(ss->get_url(), -1);
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
	visitor.start_group_node(ss->get_url(), -1);

	CameraFrameSize<EVRTrackedCameraError> f;
	CameraFrameIntrinsics<EVRTrackedCameraError> intrinsics;
	HmdMatrix44<EVRTrackedCameraError> projection;
	VideoStreamTextureSize<EVRTrackedCameraError> video_texture_size;

	LEAF_SCALAR(frame_size, tcw.GetCameraFrameSize(device_index, frame_type, &f));
	LEAF_SCALAR(intrinsics, tcw.GetCameraIntrinsics(device_index, frame_type, &intrinsics));
	LEAF_SCALAR(projection, tcw.GetCameraProjection(device_index, frame_type, config.GetNearZ(), config.GetFarZ(), &projection));
	LEAF_SCALAR(video_texture_size, tcw.GetVideoStreamTextureSize(device_index, frame_type, &video_texture_size));

	visitor.end_group_node(ss->get_url(), device_index);
}

template <typename visitor_fn>
static void visit_per_controller_state(visitor_fn &visitor,
	vr_state::controller_camera_schema *ss, TrackedCameraWrapper tcw,
	int device_index, vr_keys &resource_keys, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), device_index);
	LEAF_SCALAR(has_camera, tcw.HasCamera(device_index));

	if (ss->cameraframetypes.size() < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES)
	{
		ss->cameraframetypes.reserve(EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES);
		for (int i = 0; i < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES; i++)
		{
			const char *child_name = FrameTypeToGroupName(EVRTrackedCameraFrameType(i));
			ss->cameraframetypes.emplace_back(ss->cameraframetypes.make_url_for_child(child_name));
		}
	}
	START_VECTOR(cameraframetypes);
	for (int i = 0; i < (int)ss->cameraframetypes.size(); i++)
	{
		visit_cameraframetype_schema(visitor, &ss->cameraframetypes[i], tcw, device_index, (EVRTrackedCameraFrameType)i, resource_keys);
	}
	END_VECTOR(cameraframetypes);
	visitor.end_group_node(ss->get_url(), device_index);
}

template <typename visitor_fn>
static void visit_trackedcamera_state(visitor_fn &visitor,
	vr_state::trackedcamera_schema *ss, TrackedCameraWrapper tcw,
	vr_keys &resource_keys,
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);
	ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
	while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
	{
		const char *child_name = std::to_string(ss->controllers.size()).c_str();
		ss->controllers.emplace_back(ss->controllers.make_url_for_child(child_name));
	}

	START_VECTOR(controllers);
	for (int i = 0; i < size_as_int(ss->controllers.size()); i++)
	{
		visit_per_controller_state(visitor, &ss->controllers[i], tcw, i, resource_keys, allocator);
	}
	END_VECTOR(controllers);

	visitor.end_group_node(ss->get_url(), -1);
}

template <typename visitor_fn>
static void visit_per_resource(visitor_fn &visitor,
	vr_state::resources_schema *ss, ResourcesWrapper &wrap,
	int i, vr_keys &resource_keys,
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), i);
	if (visitor.visit_source_interfaces())
	{
		int fname_size;
		const char *fname = resource_keys.GetResourcesIndexer().get_filename_for_index(i, &fname_size);
		int dname_size;
		const char *dname = resource_keys.GetResourcesIndexer().get_directoryname_for_index(i, &dname_size);
		visitor.visit_node(ss->resources[i].resource_name, make_result(gsl::make_span(fname, fname_size)));
		visitor.visit_node(ss->resources[i].resource_directory, make_result(gsl::make_span(dname, dname_size)));

		TMPString<> full_path;
		wrap.GetFullPath(
			fname,
			dname,
			&full_path);

		visitor.visit_node(ss->resources[i].resource_full_path, full_path);

		uint8_t *data;
		uint32_t size = wrap.GetImageData(full_path.val.data(), &data);
		visitor.visit_node(ss->resources[i].resource_data, make_result(gsl::make_span(data, size)));
		wrap.FreeImageData(data);
	}
	else
	{
		visitor.visit_node(ss->resources[i].resource_name);
		visitor.visit_node(ss->resources[i].resource_directory);
		visitor.visit_node(ss->resources[i].resource_full_path);
		visitor.visit_node(ss->resources[i].resource_data);
	}
	visitor.end_group_node(ss->get_url(), i);
}


template <typename visitor_fn>
static void visit_resources_state(visitor_fn &visitor,
	vr_state::resources_schema *ss, ResourcesWrapper &wrap,
	vr_keys &resource_keys,
	PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);

	if ((int)ss->resources.size() < resource_keys.GetResourcesIndexer().get_num_resources())
	{
		ss->resources.reserve(resource_keys.GetResourcesIndexer().get_num_resources());
		while ((int)ss->resources.size() < resource_keys.GetResourcesIndexer().get_num_resources())
		{
			const char *child_name = std::to_string(ss->resources.size()).c_str();
			ss->resources.emplace_back(ss->resources.make_url_for_child(child_name));
		}
	}

	START_VECTOR(resources);
	for (int i = 0; i < (int)ss->resources.size(); i++)
	{
		visit_per_resource(visitor, ss, wrap, i, resource_keys, allocator);
	}
	END_VECTOR(resources);

	visitor.end_group_node(ss->get_url(), -1);
}


} // end of namespace