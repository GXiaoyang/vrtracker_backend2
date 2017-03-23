#pragma once
#include "vr_types.h"
#include "time_containers.h"
#include "schema_common.h"
#include "segmented_list.h"
//#include "tbb/concurrent_vector.h"

#define INIT(var_name)			var_name(schema<is_iterator>::make_url_for_child( #var_name ))
#define ALLOC_DECL

namespace vr_result
{
	using namespace vr;

	// AllocatorTemplate chooses which allocator will be passed to the time_nodes and the 
	// named vector.
	//
	// For individual nodes that are strings or vectors, the allocator is chosen by
	// the ResultVector template in vr_types.h
	//
	template <bool is_iterator, template <typename> class AllocatorTemplate>
	struct vr_schema : schema<is_iterator>
	{
		template <typename ResultType>
		using TIMENODE = time_node<ResultType, segmented_list_1024, is_iterator, AllocatorTemplate>;

	//	template <typename ResultType>
	//	using TIMENODE = time_node<ResultType, std::vector, is_iterator, AllocatorTemplate>;

	//	template <typename ResultType>
	//	using CONC_TIMENODE = time_node<ResultType, tbb::concurrent_vector, is_iterator, AllocatorTemplate>;

		// two kinds of children.  
		//	child is a vector of schemas
		template <typename T>
		using VECTOR_OF_SCHEMAS = base::named_vector<T>;
			
		// child is a vector of nodes
		template <typename ResultType>
		using VECTOR_OF_TIMENODES = base::named_vector<TIMENODE<ResultType>>;

		struct hidden_mesh_schema : schema<is_iterator>
		{
			hidden_mesh_schema() {}
			explicit hidden_mesh_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(hidden_mesh_triangle_count),
				INIT(hidden_mesh_vertices)
			{}

			TIMENODE<Uint32<>>			hidden_mesh_triangle_count;
			TIMENODE<HmdVector2s>		hidden_mesh_vertices;
		};


		struct eye_schema : schema<is_iterator>
		{
			eye_schema() {}
			explicit eye_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(projection),
				INIT(projection_raw),
				INIT(distortion),
				INIT(eye2head),
				INIT(hidden_meshes)
			{}

			TIMENODE<HmdMatrix44<>>					projection;
			TIMENODE<HmdVector4<>>					projection_raw;
			TIMENODE<DistortionCoordinates<bool>>	distortion;
			TIMENODE<HmdMatrix34<>>					eye2head;
			VECTOR_OF_SCHEMAS<hidden_mesh_schema>	hidden_meshes;
		};


		struct component_on_controller_schema : schema<is_iterator>
		{
			component_on_controller_schema() {}
			explicit component_on_controller_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(transforms),
				INIT(transforms_scroll_wheel)
			{}

			TIMENODE<RenderModelComponentState<bool>> transforms;
			TIMENODE<RenderModelComponentState<bool>> transforms_scroll_wheel;
		};

		struct system_controller_schema : schema<is_iterator>
		{
			system_controller_schema() {}

			explicit system_controller_schema(const base::URL &name ALLOC_DECL)
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
			TIMENODE<DevicePose<>>		raw_tracking_pose;
			TIMENODE<DevicePose<>>		seated_tracking_pose;
			TIMENODE<DevicePose<>>		standing_tracking_pose;
			TIMENODE<ActivityLevel<>>	activity_level;
			TIMENODE<ControllerRole<>>	controller_role;
			TIMENODE<DeviceClass<>>	device_class;
			TIMENODE<Bool<>>           connected;

			TIMENODE<ControllerState<bool>>	controller_state;
			TIMENODE<DevicePose<bool>>		synced_seated_pose;
			TIMENODE<DevicePose<bool>>		synced_standing_pose;
			TIMENODE<DevicePose<bool>>		synced_raw_pose;

			VECTOR_OF_TIMENODES<String<ETrackedPropertyError>>		string_props;
			VECTOR_OF_TIMENODES<Bool<ETrackedPropertyError>>		bool_props;
			VECTOR_OF_TIMENODES<Float<ETrackedPropertyError>>		float_props;
			VECTOR_OF_TIMENODES<HmdMatrix34<ETrackedPropertyError>>	mat34_props;
			VECTOR_OF_TIMENODES<Int32<ETrackedPropertyError>>		int32_props;
			VECTOR_OF_TIMENODES<Uint64<ETrackedPropertyError>>	uint64_props;

			VECTOR_OF_SCHEMAS<component_on_controller_schema> components;
		};
		
		struct spatial_sort_schema : schema<is_iterator>
		{
			spatial_sort_schema() {}
			explicit spatial_sort_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(hmds_sorted),
				INIT(controllers_sorted),
				INIT(trackers_sorted),
				INIT(reference_sorted)
			{}

			TIMENODE<DeviceIndexes> hmds_sorted;
			TIMENODE<DeviceIndexes> controllers_sorted;
			TIMENODE<DeviceIndexes> trackers_sorted;
			TIMENODE<DeviceIndexes> reference_sorted;
		};

		struct system_schema : schema<is_iterator>
		{
			system_schema()
				: structure_version(0)
				 {}

			explicit system_schema(const base::URL &name ALLOC_DECL)
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

			TIMENODE<Uint32Size<>> recommended_target_size;
			TIMENODE<Float<bool>> seconds_since_last_vsync;
			TIMENODE<Uint64< bool>> frame_counter_since_last_vsync;
			TIMENODE<Bool<>> is_display_on_desktop;
			TIMENODE<HmdMatrix34<>> seated2standing;
			TIMENODE<HmdMatrix34<>> raw2standing;
			TIMENODE<Uint32<>> num_hmd;
			TIMENODE<Uint32<>> num_controller;
			TIMENODE<Uint32<>> num_tracking;
			TIMENODE<Uint32<>> num_reference;
			TIMENODE<Bool<>> input_focus_captured_by_other;
			TIMENODE<Int32<>> d3d9_adapter_index;
			TIMENODE<Int32<>> dxgi_output_info;
			int structure_version;

			VECTOR_OF_SCHEMAS<eye_schema>				eyes;
			VECTOR_OF_SCHEMAS<system_controller_schema>	controllers;
			VECTOR_OF_SCHEMAS<spatial_sort_schema>		spatial_sorts;
		};

		struct application_schema : schema<is_iterator>
		{
			application_schema() {}
			explicit application_schema(const base::URL &name ALLOC_DECL)
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

			TIMENODE<String<>>		application_key;
			TIMENODE<Bool<>>		is_installed;
			TIMENODE<Bool<>>		auto_launch;
			TIMENODE<String<bool>>	supported_mime_types;
			TIMENODE<Uint32<>>		process_id;
			TIMENODE<String<>>		application_launch_arguments;

			VECTOR_OF_TIMENODES<String<EVRApplicationError>>	string_props;
			VECTOR_OF_TIMENODES<Uint64<EVRApplicationError>>		uint64_props;
			VECTOR_OF_TIMENODES<Bool<EVRApplicationError>>		bool_props;
		};

		struct mime_type_schema : schema<is_iterator>
		{
			mime_type_schema() {}
			explicit mime_type_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(mime_type),
				INIT(default_application),
				INIT(applications_that_support_mime_type)
			{}

			TIMENODE<String<>>		mime_type;
			TIMENODE<String<bool>>	default_application;
			TIMENODE<String<>>		applications_that_support_mime_type;
		};

		struct applications_schema : schema<is_iterator>
		{
			applications_schema()
				: structure_version(0)
				 {}
			explicit applications_schema(const base::URL &name ALLOC_DECL)
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

			TIMENODE<String<EVRApplicationError>>	starting_application;
			TIMENODE<ApplicationTransitionState<>>	transition_state;
			TIMENODE<Bool<>>						is_quit_user_prompt;
			TIMENODE<Uint32<>>						current_scene_process_id;
			TIMENODE<Int32String<>>					active_application_indexes;

			VECTOR_OF_SCHEMAS<mime_type_schema>		mime_types;
			VECTOR_OF_SCHEMAS<application_schema>	applications;
			int structure_version;
		};

		struct section_schema : schema<is_iterator>
		{
			section_schema() {}
			explicit section_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(bool_settings),
				INIT(string_settings),
				INIT(float_settings),
				INIT(int32_settings)
			{}

			VECTOR_OF_TIMENODES<Bool<EVRSettingsError>>		bool_settings;
			VECTOR_OF_TIMENODES<String<EVRSettingsError>>	string_settings;
			VECTOR_OF_TIMENODES<Float<EVRSettingsError>>	float_settings;
			VECTOR_OF_TIMENODES<Int32<EVRSettingsError>>	int32_settings;
		};

		struct settings_schema : schema<is_iterator>
		{
			settings_schema()
				: structure_version(0)
				 {}
			explicit settings_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(sections),
				structure_version(0)
			{}
			VECTOR_OF_SCHEMAS<section_schema>	sections;
			int structure_version;
		};


		struct chaperone_schema : schema<is_iterator>
		{
			chaperone_schema() {}
			explicit chaperone_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(calibration_state),
				INIT(play_area_size),
				INIT(play_area_rect),
				INIT(bounds_visible),
				INIT(bounds_colors),
				INIT(camera_color)
			{}
			TIMENODE<ChaperoneCalibrationState<>>	calibration_state;
			TIMENODE<HmdVector2<bool>>				play_area_size;
			TIMENODE<HmdQuad<bool>>					play_area_rect;
			TIMENODE<Bool<>>						bounds_visible;
			TIMENODE<HmdColorString<>>				bounds_colors;
			TIMENODE<HmdColor<>>					camera_color;
		};

		struct chaperone_setup_schema : schema<is_iterator>
		{
			chaperone_setup_schema() {}
			explicit chaperone_setup_schema(const base::URL &name ALLOC_DECL)
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

			TIMENODE<HmdVector2<bool>>	working_play_area_size;
			TIMENODE<HmdQuad<bool>>		working_play_area_rect;
			TIMENODE<HmdQuadString<bool>>     working_collision_bounds_info;
			TIMENODE<HmdQuadString<bool>>     live_collision_bounds_info;
			TIMENODE<HmdMatrix34<bool>>	working_seated2rawtracking;
			TIMENODE<HmdMatrix34<bool>>	working_standing2rawtracking;
			
			TIMENODE<Uint8String<bool>>	live_collision_bounds_tags_info;
			TIMENODE<HmdMatrix34<bool>>		live_seated2rawtracking;
			TIMENODE<HmdQuadString<bool>> live_physical_bounds_info;
		};

		struct compositor_controller_schema : schema<is_iterator>
		{
			compositor_controller_schema() {}
			explicit compositor_controller_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(last_render_pose),
				INIT(last_game_pose)
			{}

			TIMENODE<DevicePose<EVRCompositorError>> last_render_pose;
			TIMENODE<DevicePose<EVRCompositorError>> last_game_pose;
		};

		struct compositor_schema : schema<is_iterator>
		{
			compositor_schema() {}
			explicit compositor_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(tracking_space),
				INIT(frame_timing),
				INIT(frame_timings),
				INIT(frame_time_remaining),
				INIT(frame_time_remaining_seg),
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
			
			TIMENODE<TrackingUniverseOrigin<>> tracking_space;
			TIMENODE<CompositorFrameTiming<bool>> frame_timing;
			TIMENODE<CompositorFrameTimingString<>> frame_timings;
			TIMENODE<Float<>> frame_time_remaining;
			TIMENODE<Float<>> frame_time_remaining_seg;
			TIMENODE<Float<>> frame_time_remaining_conc;
			TIMENODE<CompositorCumulativeStats<>> cumulative_stats;
			TIMENODE<HmdColor<>> foreground_fade_color;
			TIMENODE<HmdColor<>> background_fade_color;
			TIMENODE<Float<>> grid_alpha;
			TIMENODE<Bool<>> is_fullscreen;
			TIMENODE<Uint32<>> current_scene_focus_process;
			TIMENODE<Uint32<>> last_frame_renderer;
			TIMENODE<Bool<>> can_render_scene;
			TIMENODE<Bool<>> is_mirror_visible;
			TIMENODE<Bool<>> should_app_render_with_low_resource;
			TIMENODE<String<>> instance_extensions_required;

			VECTOR_OF_SCHEMAS<compositor_controller_schema> controllers;
		};

		struct per_overlay_state : schema<is_iterator>
		{
			per_overlay_state() {}
			explicit per_overlay_state(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
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
				INIT(overlay_dashboard_scene_process)
				//,
				//INIT(events)
			{}

			TIMENODE<OverlayHandle<EVROverlayError>> overlay_handle;   // i'm assuming keys are unique, not handles - handles can be reused
			TIMENODE<String<EVROverlayError>> overlay_name;

			TIMENODE<Uint32<EVROverlayError>> overlay_image_width;
			TIMENODE<Uint32<EVROverlayError>> overlay_image_height;
			TIMENODE<Uint8String<EVROverlayError>> overlay_image_data;

			TIMENODE<Uint32<>> overlay_rendering_pid;
			TIMENODE<Uint32<EVROverlayError>> overlay_flags;
			TIMENODE<RGBColor<EVROverlayError>> overlay_color;
			TIMENODE<Float<EVROverlayError>> overlay_alpha;
			TIMENODE<Float<EVROverlayError>> overlay_texel_aspect;
			TIMENODE<Uint32<EVROverlayError>> overlay_sort_order;
			TIMENODE<Float<EVROverlayError>> overlay_width_in_meters;
			TIMENODE<FloatRange<EVROverlayError>> overlay_auto_curve_range_in_meters;
			TIMENODE<ColorSpace<EVROverlayError>> overlay_texture_color_space;
			TIMENODE<TextureBounds<EVROverlayError>> overlay_texture_bounds;
			TIMENODE<OverlayTransformType<EVROverlayError>> overlay_transform_type;
			TIMENODE<AbsoluteTransform<EVROverlayError>> overlay_transform_absolute;
			TIMENODE<TrackedDeviceRelativeTransform<EVROverlayError>> overlay_transform_device_relative;
			TIMENODE<DeviceIndex<EVROverlayError>> overlay_transform_component_relative_device_index;
			TIMENODE<String<EVROverlayError>> overlay_transform_component_relative_name;

			TIMENODE<OverlayInputMethod<EVROverlayError>> overlay_input_method;
			TIMENODE<HmdVector2<EVROverlayError>> overlay_mouse_scale;
			TIMENODE<Bool<>> overlay_is_hover_target;
			TIMENODE<Bool<>> overlay_is_visible;
			TIMENODE<Bool<>> overlay_is_active_dashboard;
			TIMENODE<Uint32Size<EVROverlayError>> overlay_texture_size;
			TIMENODE<Uint32<EVROverlayError>> overlay_dashboard_scene_process;
			//TIMENODE<VREvent<>> events;	// experiment

		};

		struct overlay_schema : schema<is_iterator>
		{
			overlay_schema() {}
			explicit overlay_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(gamepad_focus_overlay),
				INIT(primary_dashboard_device),
				INIT(is_dashboard_visible),
				INIT(active_overlay_indexes),
				INIT(keyboard_text),
				INIT(overlays)
			{}

			TIMENODE<OverlayHandle<>> gamepad_focus_overlay;
			TIMENODE<DeviceIndex<>> primary_dashboard_device;
			TIMENODE<Bool<>> is_dashboard_visible;
			TIMENODE<Int32String<>> active_overlay_indexes;
			TIMENODE<String<>> keyboard_text;
			
			VECTOR_OF_SCHEMAS<per_overlay_state> overlays;
		};

		struct rendermodel_component_schema : schema<is_iterator>
		{
			rendermodel_component_schema() {}
			explicit rendermodel_component_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				//INIT(component_name),
				INIT(button_mask),
				INIT(render_model_name)
			{}

//			TIMENODE<String<>> component_name;
			TIMENODE<Uint64<>> button_mask;
			TIMENODE<String<bool>> render_model_name;
		};

		struct rendermodel_schema : schema<is_iterator>
		{
			rendermodel_schema() {}
			explicit rendermodel_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(thumbnail_url),
				INIT(original_path),
				INIT(vertex_data),
				INIT(index_data),
				INIT(texture_width),
				INIT(texture_height),
				INIT(texture_map_data),
				INIT(components)
			{}
			TIMENODE<String<EVRRenderModelError>> thumbnail_url;
			TIMENODE<String<EVRRenderModelError>> original_path;
			TIMENODE<RenderModelVertexString<EVRRenderModelError>> vertex_data;
			TIMENODE<Uint16String<EVRRenderModelError>> index_data;

			TIMENODE<Uint16<EVRRenderModelError>> texture_width;
			TIMENODE<Uint16<EVRRenderModelError>> texture_height;
			TIMENODE<Uint8String<EVRRenderModelError>> texture_map_data;

			VECTOR_OF_SCHEMAS<rendermodel_component_schema> components;
		};

		struct render_models_schema : schema<is_iterator>
		{
			render_models_schema()
				: structure_version(-1)
				{}

			explicit render_models_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(models),
				structure_version(-1)
			{}

			VECTOR_OF_SCHEMAS<rendermodel_schema> models;
			int structure_version;
		};

		struct cameraframetype_schema : schema<is_iterator>
		{
			cameraframetype_schema() {}
			explicit cameraframetype_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(frame_size),
				INIT(intrinsics),
				INIT(projection),
				INIT(video_texture_size)
			{}

			TIMENODE<CameraFrameSize<EVRTrackedCameraError>> frame_size;
			TIMENODE<CameraFrameIntrinsics<EVRTrackedCameraError>> intrinsics;
			TIMENODE<HmdMatrix44<EVRTrackedCameraError>> projection;
			TIMENODE<VideoStreamTextureSize<EVRTrackedCameraError>> video_texture_size;
		};

		struct controller_camera_schema : schema<is_iterator>
		{
			controller_camera_schema() {}
			explicit controller_camera_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(has_camera),
				INIT(cameraframetypes)
			{}

			TIMENODE<Bool<EVRTrackedCameraError>> has_camera;
			VECTOR_OF_SCHEMAS<cameraframetype_schema> cameraframetypes;
		};

		struct tracked_camera_schema : schema<is_iterator>
		{
			tracked_camera_schema() {}
			explicit tracked_camera_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(controllers)
			{}

			VECTOR_OF_SCHEMAS<controller_camera_schema> controllers;
		};

		struct extended_display_schema : schema<is_iterator>
		{
			extended_display_schema() {}
			explicit extended_display_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(window_bounds),
				INIT(left_output_viewport),
				INIT(right_output_viewport)
			{}

			TIMENODE<WindowBounds<>> window_bounds;
			TIMENODE<ViewPort<>> left_output_viewport;
			TIMENODE<ViewPort<>> right_output_viewport;
		};

		struct resource_schema : schema<is_iterator>
		{
			resource_schema() {}
			explicit resource_schema(const base::URL &name ALLOC_DECL) 
				: schema<is_iterator>(name),
				INIT(resource_name),
				INIT(resource_directory),
				INIT(resource_full_path),
				INIT(resource_data)
			{}

			TIMENODE<String<>> resource_name;
			TIMENODE<String<>> resource_directory;
			TIMENODE<String<>> resource_full_path;
			TIMENODE<Uint8String<>> resource_data;
		};

		struct resources_schema : schema<is_iterator>
		{
			resources_schema() {}
			explicit resources_schema(const base::URL &name ALLOC_DECL)
				: schema<is_iterator>(name),
				INIT(resources)
			{}

			VECTOR_OF_SCHEMAS<resource_schema> resources;
		};

		vr_schema() {}
		explicit vr_schema(const base::URL &name /*, const A & alloc*/)
			: schema<is_iterator>(name),
			INIT(system_node),
			INIT(applications_node),
			INIT(settings_node),
			INIT(chaperone_node),
			INIT(chaperone_setup_node),
			INIT(compositor_node),
			INIT(overlay_node),
			INIT(render_models_node),
			INIT(extended_display_node),
			INIT(tracked_camera_node),
			INIT(resources_node)
		{}

		system_schema			system_node;		// schema is the type. node is the instace
		applications_schema		applications_node;
		settings_schema			settings_node;
		chaperone_schema		chaperone_node;
		chaperone_setup_schema	chaperone_setup_node;
		compositor_schema		compositor_node;
		overlay_schema			overlay_node;
		render_models_schema	render_models_node;
		extended_display_schema	extended_display_node;
		tracked_camera_schema	tracked_camera_node;
		resources_schema        resources_node;
	};
	
	using vr_state = vr_schema<false, VRAllocatorTemplate>;
	using vr_iterator = vr_schema<true, VRAllocatorTemplate>;

	using system_state     =    vr_state::system_schema;
	using system_iterator  = vr_iterator::system_schema;

	using system_controller_state = vr_state::system_controller_schema;
	using system_controller_iterator = vr_iterator::system_controller_schema;

	using applications_state       = vr_state::applications_schema;
	using applications_iterator = vr_iterator::applications_schema;

	using settings_state    =    vr_state::settings_schema;
	using settings_iterator = vr_iterator::settings_schema;
	
	using chaperone_state       = vr_state::chaperone_schema;
	using chaperone_iterator = vr_iterator::chaperone_schema;

	using chaperone_setup_state		= vr_state::chaperone_setup_schema;
	using chaperone_setup_iterator  = vr_iterator::chaperone_setup_schema;

	using compositor_state       = vr_state::compositor_schema;
	using compositor_iterator = vr_iterator::compositor_schema;

	using compositor_controller_state = vr_state::compositor_controller_schema;
	using compositor_controller_iterator = vr_iterator::compositor_controller_schema;


	using overlay_state       = vr_state::overlay_schema;
	using overlay_iterator = vr_iterator::overlay_schema;

	using render_models_state       = vr_state::render_models_schema;
	using render_models_iterator = vr_iterator::render_models_schema;

	using extended_display_state       = vr_state::extended_display_schema;
	using extended_display_iterator = vr_iterator::extended_display_schema;

	using tracked_camera_state       = vr_state::tracked_camera_schema;
	using tracked_camera_iterator = vr_iterator::tracked_camera_schema;

	using resources_state       = vr_state::resources_schema;
	using resources_iterator = vr_iterator::resources_schema;

}
