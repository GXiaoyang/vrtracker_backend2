#pragma once
#include "vr_tracker_updater.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "update_history_visitor.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "update_history_visitor.h"
#include "vr_tracker.h"

using namespace vr_result;

class named_task_group : public tbb::task_group
{
public:
	template<typename F>
	void run(const char *name, const F& f)
	{
		tbb::task_group::run(f);
	}
};

class ExecuteImmediatelyTaskGroup
{
public:
	template <typename T>
	void run(const char *name, T t)
	{
		uint64_t start = rdtsc();
		t();
		uint64_t end = rdtsc();
		//log_printf("%s %lld\n", name, end - start);
	}
	void wait() {}
};


template <typename TaskGroup, typename visitor_fn>
static void traverse_history_graph(visitor_fn *visitor, vr_tracker *outer_state, openvr_broker::open_vr_interfaces *interfaces)
{
	SystemWrapper			system_wrapper(interfaces->sysi);
	ApplicationsWrapper		application_wrapper(interfaces->appi);
	SettingsWrapper			settings_wrapper(interfaces->seti);
	ChaperoneWrapper		chaperone_wrapper(interfaces->chapi);
	ChaperoneSetupWrapper	chaperone_setup_wrapper(interfaces->chapsi);
	CompositorWrapper		compositor_wrapper(interfaces->compi);
	OverlayWrapper			overlay_wrapper(interfaces->ovi);
	RenderModelsWrapper		rendermodel_wrapper(interfaces->remi);
	ExtendedDisplayWrapper	extended_display_wrapper(interfaces->exdi);
	TrackedCameraWrapper	tracked_camera_wrapper(interfaces->taci);
	ResourcesWrapper		resources_wrapper(interfaces->resi);

	vr_state *s = &outer_state->m_state;
	vr_keys *keys = &outer_state->keys;
	TaskGroup g;

	g.run("visit_system_node",
		[&] {
		visit_system_node(visitor, &s->system_node, interfaces->sysi, &system_wrapper, &rendermodel_wrapper, keys, g);
	});

	g.run("visit_application_node", [&] {
		visit_applications_node(visitor, &s->applications_node, &application_wrapper, keys, g);
	});

	g.run("visit_settings_node",
		[&] {
		visit_settings_node(visitor, &s->settings_node, &settings_wrapper, keys, g);
	});

	g.run("visit_chaperone_node",
		[&] {
		visit_chaperone_node(visitor, &s->chaperone_node, &chaperone_wrapper, keys);
	});

	g.run("visit_chaperone_setup",
		[&] {
		visit_chaperone_setup_node(visitor, &s->chaperone_setup_node, &chaperone_setup_wrapper);
	});

	g.run("visit_compositor_state",
		[&] {
		visit_compositor_state(visitor, &s->compositor_node, &compositor_wrapper, keys, g);
	});

	g.run("visit_overlay_state",
		[&] {
		visit_overlay_state(visitor, &s->overlay_node, &overlay_wrapper, keys, g);
	});

	visit_rendermodel_state(visitor, &s->render_models_node, &rendermodel_wrapper, g);

	g.run("visit_extended_display_state",
		[&] {
		visit_extended_display_state(visitor, &s->extended_display_node, &extended_display_wrapper);
	});
	g.run("visit_tracked_camera_state",
		[&] {
		visit_trackedcamera_state(visitor, &s->tracked_camera_node, &tracked_camera_wrapper, keys);
	});
	g.run("visit_resources_state",
		[&] {
		visit_resources_state(visitor, &s->resources_node, &resources_wrapper, keys);
	});
	g.wait();
}

struct vr_tracker_updater::impl
{

};

vr_tracker_updater::vr_tracker_updater()
{
	m_pimpl = new vr_tracker_updater::impl;
}

vr_tracker_updater::~vr_tracker_updater()
{
	delete m_pimpl;
}

void vr_tracker_updater::update_tracker_parallel(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	update_history_visitor update_visitor(last_updated + 1);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<named_task_group>(&update_visitor, tracker, interfaces);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log_printf("parallel update took %lld us.\n",
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

	tracker->m_last_updated_frame_number = last_updated + 1;
}

void vr_tracker_updater::update_tracker_sequential(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	update_history_visitor update_visitor(last_updated + 1);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<ExecuteImmediatelyTaskGroup>(&update_visitor, tracker, interfaces);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log_printf("sequential update took %lld us.\n",
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	
	tracker->m_last_updated_frame_number = last_updated + 1;
}
