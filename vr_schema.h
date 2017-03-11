#pragma once
#include "vr_types.h"
#include "time_containers.h"
#include "schema_common.h"
#include "segmented_list.h"
#include <deque>

#define INIT(var_name)			var_name(make_url_for_child( #var_name ), alloc )
//#define INIT(var_name)			var_name(URL(), alloc )

namespace vr_result
{
	using namespace vr;

	template <bool is_iterator, class A>
	struct vr_schema : schema<is_iterator>
	{
	

		// define a scalar node
		template <typename ResultType> 
		using NODE = node<ResultType, std::deque, is_iterator, A>;

		//using NODE = node<ResultType, std::vector, is_iterator, A>;


		// two kinds of children.  
		//	child is a vector of schemas
		template <typename ResultType>
		using VECTOR_OF_SCHEMAS = named_vector<ResultType, A>;
			
		// child is a vector of nodes
		template <typename ResultType>
		using VECTOR_OF_NODES = named_vector<NODE<ResultType>, A>;

		struct hidden_mesh_schema : schema<is_iterator>
		{
			hidden_mesh_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(hidden_mesh_triangle_count),
				INIT(hidden_mesh_vertices)
			{}

			NODE<Uint32<>>			hidden_mesh_triangle_count;
			NODE<HmdVector2s<A>>	hidden_mesh_vertices;
		};


		struct eye_schema : schema<is_iterator>
		{
			eye_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(projection),
				INIT(projection_raw),
				INIT(distortion),
				INIT(eye2head),
				INIT(hidden_meshes)
			{}

			NODE<HmdMatrix44<>>							projection;
			NODE<HmdVector4<>>							projection_raw;
			NODE<DistortionCoordinates<A,bool>>			distortion;
			NODE<HmdMatrix34<>>							eye2head;
			VECTOR_OF_SCHEMAS<hidden_mesh_schema>	hidden_meshes;
		};


		struct component_on_controller_schema : schema<is_iterator>
		{
			component_on_controller_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(transforms),
				INIT(transforms_scroll_wheel)
			{}

			NODE<RenderModelComponentState<bool>> transforms;
			NODE<RenderModelComponentState<bool>> transforms_scroll_wheel;
		};

		struct system_controller_schema : schema<is_iterator>
		{
			system_controller_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(raw_tracking_pose),
				INIT(seated_tracking_pose),
				INIT(standing_tracking_pose),
				INIT(activity_level),
				INIT(controller_role),
				INIT(device_class),
				INIT(connected),
				INIT(controller_state),
				INIT(synced_seated_pose),
				INIT(synced_standing_pose),
				INIT(synced_raw_pose),
				INIT(string_props),
				INIT(bool_props),
				INIT(float_props),
				INIT(mat34_props),
				INIT(int32_props),
				INIT(uint64_props),
				INIT(components)
			{}
			NODE<DevicePose<>>		raw_tracking_pose;
			NODE<DevicePose<>>		seated_tracking_pose;
			NODE<DevicePose<>>		standing_tracking_pose;
			NODE<ActivityLevel<>>	activity_level;
			NODE<ControllerRole<>>	controller_role;
			NODE<DeviceClass<>>	device_class;
			NODE<Bool<>>           connected;

			NODE<ControllerState<bool>>	controller_state;
			NODE<DevicePose<bool>>		synced_seated_pose;
			NODE<DevicePose<bool>>		synced_standing_pose;
			NODE<DevicePose<bool>>		synced_raw_pose;

			VECTOR_OF_NODES<String<A, ETrackedPropertyError>> string_props;
			VECTOR_OF_NODES<Bool<ETrackedPropertyError>>		bool_props;
			VECTOR_OF_NODES<Float<ETrackedPropertyError>>		float_props;
			VECTOR_OF_NODES<HmdMatrix34<ETrackedPropertyError>>	mat34_props;
			VECTOR_OF_NODES<Int32<ETrackedPropertyError>>		int32_props;
			VECTOR_OF_NODES<Uint64<ETrackedPropertyError>>	uint64_props;

			VECTOR_OF_SCHEMAS<component_on_controller_schema> components;
		};
		
		struct spatial_sort_schema : schema<is_iterator>
		{
			spatial_sort_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(hmds_sorted),
				INIT(controllers_sorted),
				INIT(trackers_sorted),
				INIT(reference_sorted)
			{}

			NODE<DeviceIndexes<A>> hmds_sorted;
			NODE<DeviceIndexes<A>> controllers_sorted;
			NODE<DeviceIndexes<A>> trackers_sorted;
			NODE<DeviceIndexes<A>> reference_sorted;
		};

		struct system_schema : schema<is_iterator>
		{
			system_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(recommended_target_size),
				INIT(seconds_since_last_vsync),
				INIT(frame_counter_since_last_vsync),
				INIT(is_display_on_desktop),
				INIT(seated2standing),
				INIT(raw2standing),
				INIT(num_hmd),
				INIT(num_controller),
				INIT(num_tracking),
				INIT(num_reference),
				INIT(input_focus_captured_by_other),
				INIT(d3d9_adapter_index),
				INIT(dxgi_output_info),
				structure_version(0),

				INIT(eyes),
				INIT(controllers),
				INIT(spatial_sorts)
			{}

			NODE<Uint32Size<>> recommended_target_size;
			NODE<Float<bool>> seconds_since_last_vsync;
			NODE<Uint64< bool>> frame_counter_since_last_vsync;
			NODE<Bool<>> is_display_on_desktop;
			NODE<HmdMatrix34<>> seated2standing;
			NODE<HmdMatrix34<>> raw2standing;
			NODE<Uint32<>> num_hmd;
			NODE<Uint32<>> num_controller;
			NODE<Uint32<>> num_tracking;
			NODE<Uint32<>> num_reference;
			NODE<Bool<>> input_focus_captured_by_other;
			NODE<Int32<>> d3d9_adapter_index;
			NODE<Int32<>> dxgi_output_info;
			int structure_version;

			VECTOR_OF_SCHEMAS<eye_schema>				eyes;
			VECTOR_OF_SCHEMAS<system_controller_schema>	controllers;
			VECTOR_OF_SCHEMAS<spatial_sort_schema>		spatial_sorts;
		};

		struct application_schema : schema<is_iterator>
		{
			application_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(application_key),
				INIT(is_installed),
				INIT(auto_launch),
				INIT(supported_mime_types),
				INIT(process_id),
				INIT(application_launch_arguments),
				INIT(string_props),
				INIT(uint64_props),
				INIT(bool_props)
			{}

			NODE<String<A>>		application_key;
			NODE<Bool<>>			is_installed;
			NODE<Bool<>>			auto_launch;
			NODE<String<A, bool>>	supported_mime_types;
			NODE<Uint32<>>			process_id;
			NODE<String<A>>		application_launch_arguments;

			VECTOR_OF_NODES<String<A, EVRApplicationError>>	string_props;
			VECTOR_OF_NODES<Uint64<EVRApplicationError>>		uint64_props;
			VECTOR_OF_NODES<Bool<EVRApplicationError>>		bool_props;
		};

		struct mime_type_schema : schema<is_iterator>
		{
			mime_type_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(mime_type),
				INIT(default_application),
				INIT(applications_that_support_mime_type)
			{}

			NODE<String<A>>			mime_type;
			NODE<String<A, bool>>		default_application;
			NODE<String<A>>			applications_that_support_mime_type;
		};

		struct applications_schema : schema<is_iterator>
		{
			applications_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(starting_application),
				INIT(transition_state),
				INIT(is_quit_user_prompt),
				INIT(current_scene_process_id),
				INIT(active_application_indexes),
				INIT(mime_types),
				INIT(applications),
				structure_version(0)
			{}

			NODE<String<A, EVRApplicationError>>	starting_application;
			NODE<ApplicationTransitionState<>>		transition_state;
			NODE<Bool<>>							is_quit_user_prompt;
			NODE<Uint32<>>							current_scene_process_id;
			NODE<Int32String<A>>					active_application_indexes;

			VECTOR_OF_SCHEMAS<mime_type_schema>		mime_types;
			VECTOR_OF_SCHEMAS<application_schema>		applications;
			int structure_version;
		};

		struct section_schema : schema<is_iterator>
		{
			section_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(bool_settings),
				INIT(string_settings),
				INIT(float_settings),
				INIT(int32_settings)
			{}

			VECTOR_OF_NODES<Bool<EVRSettingsError>>		bool_settings;
			VECTOR_OF_NODES<String<A,EVRSettingsError>>	string_settigns;
			VECTOR_OF_NODES<Float<EVRSettingsError>>		float_settings;
			VECTOR_OF_NODES<Int32<EVRSettingsError>>		int32_settings;
		};

		struct settings_schema : schema<is_iterator>
		{
			settings_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(sections),
				structure_version(0)
			{}
			VECTOR_OF_SCHEMAS<section_schema>	sections;
			int structure_version;
		};


		struct chaperone_schema : schema<is_iterator>
		{
			chaperone_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(calibration_state),
				INIT(play_area_size),
				INIT(play_area_rect),
				INIT(bounds_visible),
				INIT(bounds_colors),
				INIT(camera_color)
			{}
			NODE<ChaperoneCalibrationState<>>	calibration_state;
			NODE<HmdVector2<bool>>				play_area_size;
			NODE<HmdQuad<bool>>					play_area_rect;
			NODE<Bool<>>						bounds_visible;
			NODE<HmdColorString<A>>				bounds_colors;
			NODE<HmdColor<>>					camera_color;
		};

		struct chaperonesetup_schema : schema<is_iterator>
		{
			chaperonesetup_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(working_play_area_size),
				INIT(working_play_area_rect),
				INIT(working_collision_bounds_info),
				INIT(live_collision_bounds_info),
				INIT(working_seated2rawtracking),
				INIT(working_standing2rawtracking),
				INIT(live_collision_bounds_tags_info),
				INIT(live_seated2rawtracking),
				INIT(live_physical_bounds_info)
			{}

			NODE<HmdVector2<bool>>	working_play_area_size;
			NODE<HmdQuad<bool>>		working_play_area_rect;
			NODE<HmdQuad<bool>>     working_collision_bounds_info;
			NODE<HmdQuad<bool>>     live_collision_bounds_info;
			NODE<HmdMatrix34<bool>>	working_seated2rawtracking;
			NODE<HmdMatrix34<bool>>	working_standing2rawtracking;
			
			NODE<Uint8String<A, bool>>	live_collision_bounds_tags_info;
			NODE<HmdMatrix34<bool>>		live_seated2rawtracking;
			NODE<HmdQuadString<A,bool>> live_physical_bounds_info;
		};

		struct compositor_controller_schema : schema<is_iterator>
		{
			compositor_controller_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(last_render_pose),
				INIT(last_game_pose)
			{}

			NODE<DevicePose<EVRCompositorError>> last_render_pose;
			NODE<DevicePose<EVRCompositorError>> last_game_pose;
		};

		struct compositor_schema : schema<is_iterator>
		{
			compositor_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(tracking_space),
				INIT(frame_timing),
				INIT(frame_timings),
				INIT(frame_time_remaining),
				INIT(cumulative_stats),
				INIT(foreground_fade_color),
				INIT(background_fade_color),
				INIT(grid_alpha),
				INIT(is_fullscreen),
				INIT(current_scene_focus_process),
				INIT(last_frame_renderer),
				INIT(can_render_scene),
				INIT(is_mirror_visible),
				INIT(should_app_render_with_low_resource),
				INIT(instance_extensions_required),
				INIT(controllers)
			{}
			
			NODE<TrackingUniverseOrigin<>> tracking_space;
			NODE<CompositorFrameTiming<bool>> frame_timing;
			NODE<CompositorFrameTimingString<A>> frame_timings;
			NODE<Float<>> frame_time_remaining;
			NODE<CompositorCumulativeStats<>> cumulative_stats;
			NODE<HmdColor<>> foreground_fade_color;
			NODE<HmdColor<>> background_fade_color;
			NODE<Float<>> grid_alpha;
			NODE<Bool<>> is_fullscreen;
			NODE<Uint32<>> current_scene_focus_process;
			NODE<Uint32<>> last_frame_renderer;
			NODE<Bool<>> can_render_scene;
			NODE<Bool<>> is_mirror_visible;
			NODE<Bool<>> should_app_render_with_low_resource;
			NODE<String<A>> instance_extensions_required;

			VECTOR_OF_SCHEMAS<compositor_controller_schema> controllers;
		};

		struct per_overlay_state : schema<is_iterator>
		{
			per_overlay_state(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(overlay_key),
				INIT(overlay_handle),
				INIT(overlay_name),
				INIT(overlay_image_width),
				INIT(overlay_image_height),
				INIT(overlay_image_data),
				INIT(overlay_rendering_pid),
				INIT(overlay_flags),
				INIT(overlay_color),
				INIT(overlay_alpha),
				INIT(overlay_texel_aspect),
				INIT(overlay_sort_order),
				INIT(overlay_width_in_meters),
				INIT(overlay_auto_curve_range_in_meters),
				INIT(overlay_texture_color_space),
				INIT(overlay_texture_bounds),
				INIT(overlay_transform_type),
				INIT(overlay_transform_absolute),
				INIT(overlay_transform_device_relative),
				INIT(overlay_transform_component_relative_device_index),
				INIT(overlay_transform_component_relative_name),
				INIT(overlay_input_method),
				INIT(overlay_mouse_scale),
				INIT(overlay_is_hover_target),
				INIT(overlay_is_visible),
				INIT(overlay_is_active_dashboard),
				INIT(overlay_texture_size),
				INIT(overlay_dashboard_scene_process),
				INIT(events)
			{}

			NODE<String<A>> overlay_key;
			NODE<OverlayHandle<EVROverlayError>> overlay_handle;   // i'm assuming keys are unique, not handles - handles can be reused
			NODE<String<A, EVROverlayError>> overlay_name;

			NODE<Uint32<EVROverlayError>> overlay_image_width;
			NODE<Uint32<EVROverlayError>> overlay_image_height;
			NODE<Uint8String<A, EVROverlayError>> overlay_image_data;

			NODE<Uint32<>> overlay_rendering_pid;
			NODE<Uint32<EVROverlayError>> overlay_flags;
			NODE<RGBColor<EVROverlayError>> overlay_color;
			NODE<Float<EVROverlayError>> overlay_alpha;
			NODE<Float<EVROverlayError>> overlay_texel_aspect;
			NODE<Uint32<EVROverlayError>> overlay_sort_order;
			NODE<Float<EVROverlayError>> overlay_width_in_meters;
			NODE<FloatRange<EVROverlayError>> overlay_auto_curve_range_in_meters;
			NODE<ColorSpace<EVROverlayError>> overlay_texture_color_space;
			NODE<TextureBounds<EVROverlayError>> overlay_texture_bounds;
			NODE<OverlayTransformType<EVROverlayError>> overlay_transform_type;
			NODE<AbsoluteTransform<EVROverlayError>> overlay_transform_absolute;
			NODE<TrackedDeviceRelativeTransform<EVROverlayError>> overlay_transform_device_relative;
			NODE<DeviceIndex<EVROverlayError>> overlay_transform_component_relative_device_index;
			NODE<String<A, EVROverlayError>> overlay_transform_component_relative_name;

			NODE<OverlayInputMethod<EVROverlayError>> overlay_input_method;
			NODE<HmdVector2<EVROverlayError>> overlay_mouse_scale;
			NODE<Bool<>> overlay_is_hover_target;
			NODE<Bool<>> overlay_is_visible;
			NODE<Bool<>> overlay_is_active_dashboard;
			NODE<Uint32Size<EVROverlayError>> overlay_texture_size;
			NODE<Uint32<EVROverlayError>> overlay_dashboard_scene_process;
			//NODE<VREvent<>> events;	// experiment

		};

		struct overlay_schema : schema<is_iterator>
		{
			overlay_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(gamepad_focus_overlay),
				INIT(primary_dashboard_device),
				INIT(is_dashboard_visible),
				INIT(active_overlay_indexes),
				INIT(keyboard_text),
				INIT(overlays)
			{}

			NODE<OverlayHandle<>> gamepad_focus_overlay;
			NODE<DeviceIndex<>> primary_dashboard_device;
			NODE<Bool<>> is_dashboard_visible;
			NODE<Int32String<A>> active_overlay_indexes;
			NODE<String<A>> keyboard_text;
			
			VECTOR_OF_SCHEMAS<per_overlay_state> overlays;
		};

		struct rendermodel_component_schema
		{
			rendermodel_component_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(component_name),
				INIT(button_mask),
				INIT(render_model_name)
			{}

			NODE<String<A>> component_name;
			NODE<Uint64<>> button_mask;
			NODE<String<A, bool>> render_model_name;
		};

		struct rendermodel_schema : schema<is_iterator>
		{
			rendermodel_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(render_model_name),
				INIT(thumbnail_url),
				INIT(original_path),
				INIT(vertex_data),
				INIT(index_data),
				INIT(texture_width),
				INIT(texture_height),
				INIT(texture_map_data),
				INIT(components)
			{}
			NODE<String<A>> render_model_name;
			NODE<String<A, EVRRenderModelError>> thumbnail_url;
			NODE<String<A, EVRRenderModelError>> original_path;
			NODE<RenderModelVertexString<A, EVRRenderModelError>> vertex_data;
			NODE<Uint16String<A,EVRRenderModelError>> index_data;

			NODE<Uint16<EVRRenderModelError>> texture_width;
			NODE<Uint16<EVRRenderModelError>> texture_height;
			NODE<Uint8String<A,EVRRenderModelError>> texture_map_data;

			VECTOR_OF_SCHEMAS<rendermodel_component_schema> components;
		};

		struct rendermodels_schema : schema<is_iterator>
		{
			rendermodels_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(models),
				structure_version(-1)
			{}

			VECTOR_OF_SCHEMAS<rendermodel_schema> models;
			int structure_version;
		};

		struct cameraframetype_schema : schema<is_iterator>
		{
			cameraframetype_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(frame_size),
				INIT(intrinsics),
				INIT(projection),
				INIT(video_texture_size)
			{}

			NODE<CameraFrameSize<EVRTrackedCameraError>> frame_size;
			NODE<CameraFrameIntrinsics<EVRTrackedCameraError>> intrinsics;
			NODE<HmdMatrix44<EVRTrackedCameraError>> projection;
			NODE<VideoStreamTextureSize<EVRTrackedCameraError>> video_texture_size;
		};

		struct controller_camera_schema : schema<is_iterator>
		{
			controller_camera_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(has_camera),
				INIT(cameraframetypes)
			{}

			NODE<Bool<EVRTrackedCameraError>> has_camera;
			VECTOR_OF_SCHEMAS<cameraframetype_schema> cameraframetypes;
		};

		struct trackedcamera_schema : schema<is_iterator>
		{
			trackedcamera_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(controllers)
			{}

			VECTOR_OF_SCHEMAS<controller_camera_schema> controllers;
		};

		struct extendeddisplay_schema : schema<is_iterator>
		{
			extendeddisplay_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(window_bounds),
				INIT(left_output_viewport),
				INIT(right_output_viewport)
			{}

			NODE<WindowBounds<>> window_bounds;
			NODE<ViewPort<>> left_output_viewport;
			NODE<ViewPort<>> right_output_viewport;
		};

		struct resource_schema : schema<is_iterator>
		{
			resource_schema(const URL &name, const A &alloc) : schema<is_iterator>
				: schema<is_iterator>(name),
				INIT(resource_name),
				INIT(resource_directory),
				INIT(resource_full_path),
				INIT(resource_data)
			{}

			NODE<String<A>> resource_name;
			NODE<String<A>> resource_directory;
			NODE<String<A>> resource_full_path;
			NODE<Uint8String<A>> resource_data;
		};

		struct resources_schema : schema<is_iterator>
		{
			resources_schema(const URL &name, const A &alloc)
				: schema<is_iterator>(name),
				INIT(resources)
			{}

			VECTOR_OF_SCHEMAS<resource_schema> resources;
		};

		vr_schema(const URL &name, const A & alloc)
			: schema<is_iterator>(name),
			INIT(system_node),
			INIT(applications_node),
			INIT(settings_node),
			INIT(chaperone_node),
			INIT(chaperone_setup_node),
			INIT(compositor_node),
			INIT(overlay_node),
			INIT(rendermodels_node),
			INIT(extendeddisplay_node),
			INIT(trackedcamera_node),
			INIT(resources_node)
		{}

		system_schema			system_node;		// schema is the type. node is the instace
		applications_schema		applications_node;
		settings_schema			settings_node;
		chaperone_schema		chaperone_node;
		chaperonesetup_schema	chaperone_setup_node;
		compositor_schema		compositor_node;
		overlay_schema			overlay_node;
		rendermodels_schema		rendermodels_node;
		extendeddisplay_schema	extendeddisplay_node;
		trackedcamera_schema	trackedcamera_node;
		resources_schema        resources_node;

	};
	
	using vr_state = vr_schema<false, VRAllocator>;
}