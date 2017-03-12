// rangesplay.cpp : Defines the entry point for the console application.
//


#include "vr_tracker.h"
#include "update_history_visitor.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "openvr_broker.h"
#include "update_history_visitor.h"

using namespace vr_result;

template <typename visitor_fn>
static void traverse_history_graph_sequential(visitor_fn &visitor, 
	vr_tracker<slab_allocator<char>> *outer_state,
	openvr_broker::open_vr_interfaces &interfaces)
{
	SystemWrapper			system_wrapper(interfaces.sysi);
	ApplicationsWrapper		application_wrapper(interfaces.appi);
	SettingsWrapper			settings_wrapper(interfaces.seti);
	ChaperoneWrapper		chaperone_wrapper(interfaces.chapi);
	ChaperoneSetupWrapper	chaperone_setup_wrapper(interfaces.chapsi);
	CompositorWrapper		compositor_wrapper(interfaces.compi);
	OverlayWrapper			overlay_wrapper(interfaces.ovi);
	RenderModelWrapper		rendermodel_wrapper(interfaces.remi);
	ExtendedDisplayWrapper	extended_display_wrapper(interfaces.exdi);
	TrackedCameraWrapper	tracked_camera_wrapper(interfaces.taci);
	ResourcesWrapper		resources_wrapper(interfaces.resi);

	vr_state *s = &outer_state->m_state;
	//VRAllocator &allocator = outer_state->m_allocator;
	PlaceHolderAllocator p = 0;

	visit_system_node(visitor, &s->system_node, interfaces.sysi, system_wrapper, rendermodel_wrapper,
			outer_state->additional_resource_keys, p);

	visit_applications_node(visitor, &s->applications_node, application_wrapper, 
		outer_state->additional_resource_keys, p);
	
#if 0
	//if (interfaces.seti)
	{
		twrap t("settings_node");
		visit_settings_node(visitor, &s->settings_node, settings_wrapper, outer_state->additional_resource_keys, allocator);
	}

	//if (interfaces.chapi)
	{
		twrap t("chaperone_node");
		visit_chaperone_node(visitor, &s->chaperone_node, chaperone_wrapper, outer_state->additional_resource_keys);
	}

	//if (interfaces.chapsi)
	{
		twrap t("chaperone_setup_node");
		visit_chaperone_setup_node(visitor, &s->chaperone_setup_node, chaperone_setup_wrapper);
	}

	//if (interfaces.compi)
	{
		twrap t("compositor_node");
		visit_compositor_state(visitor, &s->compositor_node, compositor_wrapper, outer_state->additional_resource_keys, allocator);
	}

	//if (interfaces.ovi)
	{
		twrap t("overlay_node");
		visit_overlay_state(visitor, &s->overlay_node, overlay_wrapper, outer_state->additional_resource_keys, allocator);
	}

	//if (interfaces.remi)
	{
		twrap t("rendermodels_node");
		visit_rendermodel_state(visitor, &s->rendermodels_node, rendermodel_wrapper, allocator);
	}

	//if (interfaces.exdi)
	{
		twrap t("extendeddisplay_node");
		visit_extended_display_state(visitor, &s->extendeddisplay_node, extended_display_wrapper);
	}

	//if (interfaces.taci)
	{
		twrap t("trackedcamera_node");
		visit_trackedcamera_state(visitor, &s->trackedcamera_node, tracked_camera_wrapper,
			outer_state->additional_resource_keys, allocator);
	}

	//if (interfaces.resi)
	{
		twrap t("resources_node");
		visit_resources_state(visitor, &s->resources_node, resources_wrapper, outer_state->additional_resource_keys, allocator);
	}
#endif
}


void UPDATE_USE_CASE()
{
	using namespace vr;
	using namespace vr_result;
	using std::range;

	// 
	// Initialize base
	//
	slab s(1024 * 1024);
	slab_allocator<char>::m_temp_slab = &s;
	tmp_vector_pool<VRTMPSize> tmp_pool;
	vr_tmp_vector_base::m_global_pool = &tmp_pool;





	slab_allocator<char> allocator;
	vr_tracker<slab_allocator<char>> tracker(&s);

	auto &system = tracker.m_state.system_node;
	auto &controllers = tracker.m_state.system_node.controllers;


	update_history_visitor visitor(1);

	openvr_broker::open_vr_interfaces interfaces;
	char *error;
	openvr_broker::acquire_interfaces("raw", &interfaces, &error);
	traverse_history_graph_sequential(visitor, &tracker, interfaces);
}

