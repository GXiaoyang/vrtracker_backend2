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
#include "openvr_string.h"

typedef int PlaceHolderAllocator;

namespace vr_result
{

#define START_VECTOR(vector_name) \
visitor.start_vector(ss->vector_name.get_url(), ss->vector_name)

#define END_VECTOR(vector_name) \
visitor.end_vector(ss->vector_name.get_url(), ss->vector_name)


#define VISIT(member_name, wrapper_call)\
if (visitor.visit_source_interfaces())\
{\
	visitor.visit_node(ss->member_name, wrapper_call);\
}\
else\
{\
	visitor.visit_node(ss->member_name);\
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
	VISIT(projection, wrap.GetProjectionMatrix(eEye, c.GetNearZ(), c.GetFarZ()));
	VISIT(eye2head, wrap.GetEyeToHeadTransform(eEye));
	VISIT(projection_raw, wrap.GetProjectionRaw(eEye));

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

	if (visitor.expand_structure())
	{
		while (ss->hidden_meshes.size() < 3)
		{
			ss->hidden_meshes.reserve(3);
			const char *mesh_name = openvr_string::EHiddenAreaMeshTypeToString(EHiddenAreaMeshType(ss->hidden_meshes.size()));
			ss->hidden_meshes.emplace_back(ss->hidden_meshes.make_url_for_child(mesh_name));
			system_ss->structure_version++;
		}
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

	if (visitor.visit_source_interfaces() )
	{
		if (!controller_state.is_present())
		{
			transforms.return_code = false;
			transforms_scroll_wheel.return_code = false;
		}
		else
		{
			wrap.GetComponentState(
				render_model_name,
				ss->get_name().c_str(),
				controller_state.val,
				false,
				&transforms);

			wrap.GetComponentState(
				render_model_name,
				ss->get_name().c_str(),
				controller_state.val,
				true,							// scroll_wheel set to true
				&transforms_scroll_wheel);
		}
	}
	VISIT(transforms, transforms);
	VISIT(transforms_scroll_wheel, transforms_scroll_wheel);

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

	if (visitor.visit_source_interfaces())
	{
		ResultType result;
		for (int index = 0; index < num_props; index++)
		{
			vr::ETrackedDeviceProperty prop = (vr::ETrackedDeviceProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetTrackedDeviceProperty(unDeviceIndex, prop, &result);
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
		VISIT(activity_level, wrap.GetTrackedDeviceActivityLevel(controller_index));
		VISIT(controller_role, wrap.GetControllerRoleForTrackedDeviceIndex(controller_index));
		VISIT(device_class, wrap.GetTrackedDeviceClass(controller_index));
		VISIT(connected, wrap.IsTrackedDeviceConnected(controller_index));
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
	if (visitor.expand_structure())
	{
		structure_check(&system_ss->structure_version, ss->string_props, indexer, PropertiesIndexer::PROP_STRING, allocator);
		structure_check(&system_ss->structure_version, ss->bool_props, indexer, PropertiesIndexer::PROP_BOOL, allocator);
		structure_check(&system_ss->structure_version, ss->float_props, indexer, PropertiesIndexer::PROP_FLOAT, allocator);
		structure_check(&system_ss->structure_version, ss->mat34_props, indexer, PropertiesIndexer::PROP_MAT34, allocator);
		structure_check(&system_ss->structure_version, ss->int32_props, indexer, PropertiesIndexer::PROP_INT32, allocator);
		structure_check(&system_ss->structure_version, ss->uint64_props, indexer, PropertiesIndexer::PROP_UINT64, allocator);
	}
	
	visit_string_vec(visitor, controller_index, ss->string_props, wrap, indexer, PropertiesIndexer::PROP_STRING, "string props");
	visit_vec(visitor, controller_index, ss->bool_props, wrap, indexer, PropertiesIndexer::PROP_BOOL, "bool props");
	visit_vec(visitor, controller_index, ss->float_props, wrap, indexer, PropertiesIndexer::PROP_FLOAT, "float props");
	visit_vec(visitor, controller_index, ss->mat34_props, wrap, indexer, PropertiesIndexer::PROP_MAT34, "mat34 props");
	visit_vec(visitor, controller_index, ss->int32_props, wrap, indexer, PropertiesIndexer::PROP_INT32, "int32 props");
	visit_vec(visitor, controller_index, ss->uint64_props, wrap, indexer, PropertiesIndexer::PROP_UINT64, "uint64 props");
	
	// update the component states on this controller
	// based on: component name
	//           render model name
	//           controller state

	// render model name comes from a property.  to avoid coupling to visit_string_properties, 
	// just look it up again

	// gymnastics to avoid allocating the tmp string
	Result<vr_empty_vector<char>, NoReturnCode> tmp;
	const char *render_model_name = nullptr;
	if (visitor.expand_structure() || visitor.visit_source_interfaces())
	{
		TMPString<ETrackedPropertyError> render_model;
		wrap.GetStringTrackedDeviceProperty(controller_index, vr::Prop_RenderModelName_String, &render_model);
		tmp.val = std::move(render_model.val); // move assignment
		render_model_name = tmp.val.data();
	}
	

	if (visitor.expand_structure())
	{
		if (render_model_name)
		{
			int component_count = rmw.GetComponentCount(render_model_name);
			while (size_as_int(ss->components.size()) < component_count)
			{
				ss->components.reserve(component_count);
				TMPString<> component_name;
				int component_index = ss->components.size();
				rmw.GetComponentModelName(render_model_name, component_index, &component_name);
				ss->components.emplace_back(ss->components.make_url_for_child(component_name.val.data()));
				system_ss->structure_version++;
			}
		}
	}

	START_VECTOR(components);
	for (int i = 0; i < size_as_int(ss->components.size()); i++)
	{
		visit_component_on_controller_schema(visitor, &ss->components[i], rmw, render_model_name, controller_state, i);
	}
	END_VECTOR(components);
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
		VISIT(recommended_target_size, sysw.GetRecommendedRenderTargetSize());
		VISIT(is_display_on_desktop, sysw.GetIsDisplayOnDesktop());
		VISIT(seated2standing, sysw.GetSeatedZeroPoseToStandingAbsoluteTrackingPose());
		VISIT(raw2standing, sysw.GetRawZeroPoseToStandingAbsoluteTrackingPose());

		VISIT(num_hmd, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_HMD));
		VISIT(num_controller, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_Controller));
		VISIT(num_tracking, sysw.CountDevicesOfClass(vr::TrackedDeviceClass_GenericTracker));
		VISIT(num_reference, sysw.CountDevicesOfClass(TrackedDeviceClass_TrackingReference));

		VISIT(input_focus_captured_by_other, sysw.IsInputFocusCapturedByAnotherProcess());


		if (visitor.visit_source_interfaces())
		{
			Float<bool>		seconds_since_last_vsync;
			Uint64<bool>	frame_counter_since_last_vsync;
			sysw.GetTimeSinceLastVsync(&seconds_since_last_vsync, &frame_counter_since_last_vsync);
			visitor.visit_node(ss->seconds_since_last_vsync, seconds_since_last_vsync);
			visitor.visit_node(ss->frame_counter_since_last_vsync, frame_counter_since_last_vsync);
		}
		else
		{
			visitor.visit_node(ss->seconds_since_last_vsync);
			visitor.visit_node(ss->frame_counter_since_last_vsync);
		}

		VISIT(d3d9_adapter_index, sysw.GetD3D9AdapterIndex());
		VISIT(dxgi_output_info, sysw.GetDXGIOutputInfo());
	}

	//
	// eyes
	//
	if (visitor.expand_structure())
	{
		if (ss->eyes.size() < 2)
		{
			ss->eyes.reserve(2);
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
	if (visitor.expand_structure())
	{
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
			std::string name(std::to_string(ss->controllers.size()));
			ss->controllers.emplace_back(ss->controllers.make_url_for_child(name.c_str()));
			ss->structure_version++;
		}
	}

	START_VECTOR(controllers);
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
		VISIT(controllers[i].raw_tracking_pose, make_result(raw_pose_array[i]));
		VISIT(controllers[i].standing_tracking_pose, make_result(standing_pose_array[i]));
		VISIT(controllers[i].seated_tracking_pose, make_result(seated_pose_array[i]));
		visit_controller_state(visitor, &ss->controllers[i], ss, sysw, rmw, i, indexer, allocator);
		visitor.end_group_node(ss->controllers.get_url(), i);
	}
	END_VECTOR(controllers);

	//
	// spatial sorts
	//
	if (visitor.expand_structure())
	{
		while (ss->spatial_sorts.size() < vr::k_unMaxTrackedDeviceCount + 1)
		{
			ss->spatial_sorts.reserve(vr::k_unMaxTrackedDeviceCount + 1);
			std::string name;
			if (ss->spatial_sorts.size() == 0)
			{
				name = "absolute";
			}
			else
			{
				name = "relative_to_" + std::to_string(ss->spatial_sorts.size() - 1);
			}
			ss->spatial_sorts.emplace_back(ss->spatial_sorts.make_url_for_child(name.c_str()));
		}
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

	if (visitor.visit_source_interfaces())
	{
		ResultType result;
		for (int index = 0; index < num_props; index++)
		{
			vr::EVRApplicationProperty prop = (vr::EVRApplicationProperty)indexer->GetEnumVal(prop_type, index);
			wrap.GetProperty(app_key, prop, &result);
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

	VISIT(application_key, make_result(gsl::make_span(app_key, app_key_string_size)));

	if (visitor.visit_source_interfaces())
	{
		Uint32<> process_id_tmp = wrap.GetApplicationProcessId(app_key);
		VISIT(process_id, process_id_tmp);
		TMPString<> tmp;
		VISIT(application_launch_arguments, wrap.GetApplicationLaunchArguments(process_id_tmp.val, &tmp));
	}
	else
	{
		visitor.visit_node(ss->process_id);
		visitor.visit_node(ss->application_launch_arguments);
	}

	VISIT(is_installed, wrap.IsApplicationInstalled(app_key));
	VISIT(auto_launch, wrap.GetApplicationAutoLaunch(app_key));
	VISIT(supported_mime_types, wrap.GetApplicationSupportedMimeTypes(app_key, &(TMPString<bool>())));

	ApplicationsPropertiesIndexer *indexer = &resource_keys.GetApplicationsPropertiesIndexer();

	if (visitor.expand_structure())
	{
		structure_check(&applications->structure_version, ss->string_props, indexer, PropertiesIndexer::PROP_STRING, allocator);
		structure_check(&applications->structure_version, ss->bool_props, indexer, PropertiesIndexer::PROP_BOOL, allocator);
		structure_check(&applications->structure_version, ss->uint64_props, indexer, PropertiesIndexer::PROP_UINT64, allocator);
	}

	visit_string_vec(visitor, ss->string_props, wrap, app_key, indexer, PropertiesIndexer::PROP_STRING, "string_props");
	visit_vec(visitor, ss->bool_props, wrap, app_key, indexer, PropertiesIndexer::PROP_BOOL, "bool_props");
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

	VISIT(default_application, wrap.GetDefaultApplicationForMimeType(mime_type, &(TMPString<bool>())));
	VISIT(applications_that_support_mime_type, wrap.GetApplicationsThatSupportMimeType(mime_type, &(TMPString<>())));
}


template <typename visitor_fn>
static void visit_applications_node(visitor_fn &visitor, vr_state::applications_schema *ss, 
	ApplicationsWrapper &wrap,
	vr_keys &resource_keys, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), -1);
	VISIT(active_application_indexes, resource_keys.GetApplicationsIndexer().update(&(TMPInt32String<>()), wrap));

	if (visitor.expand_structure())
	{
		if (resource_keys.GetApplicationsIndexer().get_num_applications() > size_as_int(ss->applications.size()))
		{
			ss->applications.reserve(resource_keys.GetApplicationsIndexer().get_num_applications());
			while (size_as_int(ss->applications.size()) < resource_keys.GetApplicationsIndexer().get_num_applications())
			{
				const char *application_name = resource_keys.GetApplicationsIndexer().get_key_for_index(ss->applications.size());
				ss->applications.emplace_back(ss->applications.make_url_for_child(application_name));
			}
			ss->structure_version++;
		}
	}

	VISIT(starting_application, wrap.GetStartingApplication(&(TMPString<vr::EVRApplicationError>())));
	VISIT(transition_state, wrap.GetTransitionState());
	VISIT(is_quit_user_prompt, wrap.IsQuitUserPromptRequested());
	VISIT(current_scene_process_id, wrap.GetCurrentSceneProcessId());

	if (visitor.expand_structure())
	{
		int num_mime_types = resource_keys.GetMimeTypesIndexer().GetNumMimeTypes();
		while (size_as_int(ss->mime_types.size()) < num_mime_types)
		{
			ss->mime_types.reserve(num_mime_types);
			const char *mime_type = resource_keys.GetMimeTypesIndexer().GetNameForIndex(ss->mime_types.size());
			ss->mime_types.emplace_back(ss->mime_types.make_url_for_child(mime_type));
			ss->structure_version++;
		}
	}

	START_VECTOR(mime_types);
	for (int i = 0; i < size_as_int(ss->mime_types.size()); i++)
	{
		visit_mime_type_schema(visitor, &ss->mime_types[i], wrap, i, resource_keys);
	}
	END_VECTOR(mime_types);

	START_VECTOR(applications);
	for (int i = 0; i < size_as_int(ss->applications.size()); i++)
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

	if (visitor.expand_structure())
	{
		structure_check(structure_version, s->bool_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_BOOL, allocator);
		structure_check(structure_version, s->string_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_STRING, allocator);
		structure_check(structure_version, s->float_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_FLOAT, allocator);
		structure_check(structure_version, s->int32_settings, indexer, section_name, SettingsIndexer::SETTING_TYPE_INT32, allocator);
	}
	
	visit_subtable2<visitor_fn, bool>(visitor, s->bool_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_BOOL, resource_keys);
	visit_string_subtable2(visitor, s->string_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_STRING, resource_keys);
	visit_subtable2(visitor, s->float_settings, wrap, section_name, SettingsIndexer::SETTING_TYPE_FLOAT, resource_keys);
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

	if (visitor.expand_structure())
	{
		int required_size = resource_keys.GetSettingsIndexer().GetNumSections();
		if (size_as_int(ss->sections.size()) < required_size)
		{
			ss->sections.reserve(vr::k_unMaxTrackedDeviceCount);
			while (size_as_int(ss->sections.size()) < required_size)
			{
				int section = ss->sections.size();
				ss->sections.emplace_back(ss->sections.make_url_for_child(resource_keys.GetSettingsIndexer().GetSectionName(section)));
			}
		}
	}

	START_VECTOR(sections);
	for (int index = 0; index < size_as_int(ss->sections.size()); index++)
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
	VISIT(calibration_state, wrap.GetCalibrationState());
	VISIT(bounds_visible, wrap.AreBoundsVisible());
	VISIT(play_area_rect, wrap.GetPlayAreaRect());
	VISIT(play_area_size, wrap.GetPlayAreaSize());

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
	VISIT(working_play_area_size, wrap.GetWorkingPlayAreaSize());
	VISIT(working_play_area_rect, wrap.GetWorkingPlayAreaRect());
	VISIT(working_collision_bounds_info, wrap.GetWorkingCollisionBoundsInfo(&(TMPHmdQuadString<bool>())));
	VISIT(live_collision_bounds_info, wrap.GetLiveCollisionBoundsInfo(&(TMPHmdQuadString<bool>())));
	VISIT(working_seated2rawtracking, wrap.GetWorkingSeatedZeroPoseToRawTrackingPose());
	VISIT(working_standing2rawtracking, wrap.GetWorkingStandingZeroPoseToRawTrackingPose());
	VISIT(live_collision_bounds_tags_info, wrap.GetLiveCollisionBoundsTagsInfo(&(TMPUint8String<bool>())));
	VISIT(live_seated2rawtracking, wrap.GetLiveSeatedZeroPoseToRawTrackingPose());
	VISIT(live_physical_bounds_info, wrap.GetLivePhysicalBoundsInfo(&(TMPHmdQuadString<bool>())));
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
		VISIT(tracking_space, wrap.GetTrackingSpace());
		VISIT(frame_timing, wrap.GetFrameTiming(config.GetFrameTimingFramesAgo()));
		VISIT(frame_time_remaining, wrap.GetFrameTimeRemaining());
		VISIT(cumulative_stats, wrap.GetCumulativeStats());
		VISIT(foreground_fade_color, wrap.GetForegroundFadeColor());
		VISIT(background_fade_color, wrap.GetBackgroundFadeColor());
		VISIT(grid_alpha, wrap.GetCurrentGridAlpha());
		VISIT(is_fullscreen, wrap.IsFullscreen());
		VISIT(current_scene_focus_process, wrap.GetCurrentSceneFocusProcess());
		VISIT(last_frame_renderer, wrap.GetLastFrameRenderer());
		VISIT(can_render_scene, wrap.CanRenderScene());
		VISIT(is_mirror_visible, wrap.IsMirrorWindowVisible());
		VISIT(should_app_render_with_low_resource, wrap.ShouldAppRenderWithLowResources());
		VISIT(frame_timings, wrap.GetFrameTimings(config.GetFrameTimingsNumFrames(),
			&(TMPCompositorFrameTimingString<>())));
	}

	if (visitor.expand_structure())
	{
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
			const char *child_name = std::to_string(ss->controllers.size()).c_str();
			ss->controllers.emplace_back(ss->controllers.make_url_for_child(child_name));
		}
	}

	START_VECTOR(controllers);
	for (int i = 0; i < size_as_int(ss->controllers.size()); i++)
	{
		visit_compositor_controller(visitor, &ss->controllers[i], wrap, i);
	}
	END_VECTOR(controllers);
	

	VISIT(instance_extensions_required, wrap.GetVulkanInstanceExtensionsRequired(&(TMPString<>())));
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
		const char *component_model_name = ss->get_name().c_str();
		VISIT(button_mask, wrap.GetComponentButtonMask(pchRenderModelName, component_model_name));
		TMPString<bool> tmp;
		VISIT(render_model_name, wrap.GetComponentRenderModelName(pchRenderModelName, component_model_name, &tmp));
	}
	else
	{
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

	const char *render_model_name = ss->get_name().c_str();

	visitor.start_group_node(ss->get_url(), unRenderModelIndex);
	VISIT(thumbnail_url, wrap.GetRenderModelThumbnailURL(render_model_name, &(TMPString<EVRRenderModelError>())));
	VISIT(original_path, wrap.GetRenderModelOriginalPath(render_model_name, &(TMPString<EVRRenderModelError>())));
	
	if (visitor.visit_source_interfaces())
	{
		if (visitor.reload_render_models() || ss->vertex_data.empty())
		{
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

	if (visitor.expand_structure())
	{
		int component_count = wrap.GetComponentCount(render_model_name);
		if (size_as_int(ss->components.size()) < component_count)
		{
			ss->components.reserve(component_count);
			TMPString<> component_model_name;
			while (size_as_int(ss->components.size()) < component_count)
			{
				wrap.GetComponentModelName(render_model_name, ss->components.size(), &component_model_name);
				ss->components.emplace_back(ss->components.make_url_for_child(component_model_name.val.data()));
				*structure_version += 1;
			}
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

	VISIT(overlay_rendering_pid, wrap.GetOverlayRenderingPid(handle));
	VISIT(overlay_flags, wrap.GetOverlayFlags(handle));
	VISIT(overlay_color, wrap.GetOverlayColor(handle));
	VISIT(overlay_alpha, wrap.GetOverlayAlpha(handle));
	VISIT(overlay_texel_aspect, wrap.GetOverlayTexelAspect(handle));
	VISIT(overlay_sort_order, wrap.GetOverlaySortOrder(handle));
	VISIT(overlay_width_in_meters, wrap.GetOverlayWidthInMeters(handle));
	VISIT(overlay_auto_curve_range_in_meters, wrap.GetOverlayAutoCurveDistanceRangeInMeters(handle));
	VISIT(overlay_texture_color_space, wrap.GetOverlayTextureColorSpace(handle));
	VISIT(overlay_texture_bounds, wrap.GetOverlayTextureBounds(handle));
	VISIT(overlay_transform_type, wrap.GetOverlayTransformType(handle));
	VISIT(overlay_transform_absolute, wrap.GetOverlayTransformAbsolute(handle));
	VISIT(overlay_transform_device_relative, wrap.GetOverlayTransformTrackedDeviceRelative(handle));
	VISIT(overlay_input_method, wrap.GetOverlayInputMethod(handle));
	VISIT(overlay_mouse_scale, wrap.GetOverlayMouseScale(handle));
	VISIT(overlay_is_hover_target, wrap.IsHoverTargetOverlay(handle));
	VISIT(overlay_is_visible, wrap.IsOverlayVisible(handle));
	VISIT(overlay_is_active_dashboard, wrap.IsActiveDashboardOverlay(handle));
	VISIT(overlay_dashboard_scene_process, wrap.GetDashboardOverlaySceneProcess(handle));
	VISIT(overlay_texture_size, wrap.GetOverlayTextureSize(handle));


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

	VISIT(gamepad_focus_overlay, wrap.GetGamepadFocusOverlay());
	VISIT(primary_dashboard_device, wrap.GetPrimaryDashboardDevice());
	VISIT(is_dashboard_visible, wrap.IsDashboardVisible());
	VISIT(keyboard_text, wrap.GetKeyboardText(&(TMPString<>())));
	VISIT(active_overlay_indexes,
		config.GetOverlayIndexer().update(&(TMPInt32String<>()), wrap));

	if (visitor.expand_structure())
	{
		if (config.GetOverlayIndexer().get_num_overlays() > size_as_int(ss->overlays.size()))
		{
			ss->overlays.reserve(config.GetOverlayIndexer().get_num_overlays());
			while (size_as_int(ss->overlays.size()) < config.GetOverlayIndexer().get_num_overlays())
			{
				const char *child_name = config.GetOverlayIndexer().get_overlay_key_for_index(ss->overlays.size());
				ss->overlays.emplace_back(ss->overlays.make_url_for_child(child_name));
			}
		}
	}

	// history traversal always goes through the complete set
	// see "Hwrap to track applications and overlays.docx"
	START_VECTOR(overlays);
	for (int i = 0; i < size_as_int(ss->overlays.size()); i++)
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

	if (visitor.expand_structure())
	{
		Uint32<> current_rendermodels = wrap.GetRenderModelCount();
		int num_render_models = current_rendermodels.val;
		while (size_as_int(ss->models.size()) < num_render_models)
		{
			ss->models.reserve(num_render_models);
			TMPString<> name;
			wrap.GetRenderModelName(ss->models.size(), &name);

			ss->models.emplace_back(ss->models.make_url_for_child(name.val.data()));
			ss->structure_version += 1;
		}
	}

	START_VECTOR(models);
	for (int i = 0; i < size_as_int(ss->models.size()); i++)
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
	VISIT(window_bounds, ew.GetWindowBounds());
	VISIT(left_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Left));
	VISIT(right_output_viewport, ew.GetEyeOutputViewport(vr::Eye_Right));
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
	visitor.start_group_node(ss->get_url(), device_index);

	CameraFrameSize<EVRTrackedCameraError> f;
	CameraFrameIntrinsics<EVRTrackedCameraError> intrinsics;
	HmdMatrix44<EVRTrackedCameraError> projection;
	VideoStreamTextureSize<EVRTrackedCameraError> video_texture_size;

	VISIT(frame_size, tcw.GetCameraFrameSize(device_index, frame_type, &f));
	VISIT(intrinsics, tcw.GetCameraIntrinsics(device_index, frame_type, &intrinsics));
	VISIT(projection, tcw.GetCameraProjection(device_index, frame_type, config.GetNearZ(), config.GetFarZ(), &projection));
	VISIT(video_texture_size, tcw.GetVideoStreamTextureSize(device_index, frame_type, &video_texture_size));

	visitor.end_group_node(ss->get_url(), device_index);
}

template <typename visitor_fn>
static void visit_per_controller_state(visitor_fn &visitor,
	vr_state::controller_camera_schema *ss, TrackedCameraWrapper tcw,
	int device_index, vr_keys &resource_keys, PlaceHolderAllocator &allocator)
{
	visitor.start_group_node(ss->get_url(), device_index);
	VISIT(has_camera, tcw.HasCamera(device_index));

	if (visitor.expand_structure())
	{
		if (ss->cameraframetypes.size() < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES)
		{
			ss->cameraframetypes.reserve(EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES);
			for (int i = 0; i < EVRTrackedCameraFrameType::MAX_CAMERA_FRAME_TYPES; i++)
			{
				const char *child_name = FrameTypeToGroupName(EVRTrackedCameraFrameType(i));
				ss->cameraframetypes.emplace_back(ss->cameraframetypes.make_url_for_child(child_name));
			}
		}
	}

	START_VECTOR(cameraframetypes);
	for (int i = 0; i < size_as_int(ss->cameraframetypes.size()); i++)
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
	if (visitor.expand_structure())
	{
		while (ss->controllers.size() < vr::k_unMaxTrackedDeviceCount)
		{
			ss->controllers.reserve(vr::k_unMaxTrackedDeviceCount);
			const char *child_name = std::to_string(ss->controllers.size()).c_str();
			ss->controllers.emplace_back(ss->controllers.make_url_for_child(child_name));
		}
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

	if (visitor.expand_structure())
	{
		if (size_as_int(ss->resources.size()) < resource_keys.GetResourcesIndexer().get_num_resources())
		{
			ss->resources.reserve(resource_keys.GetResourcesIndexer().get_num_resources());
			while ((int)ss->resources.size() < resource_keys.GetResourcesIndexer().get_num_resources())
			{
				const char *child_name = std::to_string(ss->resources.size()).c_str();
				ss->resources.emplace_back(ss->resources.make_url_for_child(child_name));
			}
		}
	}

	START_VECTOR(resources);
	for (int i = 0; i < size_as_int(ss->resources.size()); i++)
	{
		visit_per_resource(visitor, ss, wrap, i, resource_keys, allocator);
	}
	END_VECTOR(resources);

	visitor.end_group_node(ss->get_url(), -1);
}

} // end of namespace
