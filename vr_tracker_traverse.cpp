#include "vr_tracker_traverse.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "tracker_update_visitor.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "tracker_update_visitor.h"
#include "tracker_encode_visitor.h"
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
		// 
		// Intels rule of thumb is to parallelize work that is > 100k cycles.
		// so the following is a way to measure how much work is done to help
		// partition the jobs.
		//
		uint64_t start = rdtsc();
		t();
		uint64_t end = rdtsc();
		//log_printf("%s %lld\n", name, end - start);
	}
	void wait() {}
};

struct ConfigObserver : IndexerObserver
{
	virtual void NewVRConfigEvent(const VRConfigEvent &e)
	{
		config_events.push_back(e.clone());
	}
	std::vector<VRConfigEvent*> config_events;
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
	ConfigObserver config_observer;
	keys->RegisterObserver(&config_observer);
	keys->UnRegisterObserver(&config_observer);

	g.run("visit_system_node",
		[&] {
		visit_system_node(visitor, &s->system_node, interfaces->sysi, &system_wrapper, &rendermodel_wrapper, keys, g);
	});

	g.run("visit_application_node", [&] {
		visit_applications_node(visitor, &s->applications_node, &application_wrapper, keys, g);
	});

	g.run("visit_settings_node",
		[visitor, s, &settings_wrapper, keys, &g] {
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

struct vr_tracker_traverse::impl
{

};

vr_tracker_traverse::vr_tracker_traverse()
{
	m_pimpl = new vr_tracker_traverse::impl;
}

vr_tracker_traverse::~vr_tracker_traverse()
{
	delete m_pimpl;
}

void vr_tracker_traverse::update_tracker_parallel(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	tracker_update_visitor update_visitor(last_updated + 1);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<named_task_group>(&update_visitor, tracker, interfaces);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	bool quiet = true;
	if (!quiet)
	{
		log_printf("parallel update took %lld us.\n",
			std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	tracker->m_last_updated_frame_number = last_updated + 1;
}

void vr_tracker_traverse::update_tracker_sequential(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	tracker_update_visitor update_visitor(last_updated + 1);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<ExecuteImmediatelyTaskGroup>(&update_visitor, tracker, interfaces);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log_printf("sequential update took %lld us.\n",
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	
	tracker->m_last_updated_frame_number = last_updated + 1;
}

// file format starts with a header:
struct header
{
	uint64_t magic;
	uint64_t summary_offset;
	uint64_t summary_size;
	uint64_t state_offset;
	uint64_t state_size;
	uint64_t resource_keys_offset;
	uint64_t resource_keys_size;
	uint64_t event_offset;
	uint64_t event_size;
	uint64_t timestamp_offset;
	uint64_t timestamp_size;
};



void vr_tracker_traverse::save_tracker_to_binary_file(vr_tracker *tracker, const char *filename)
{
	// state size
	EncodeStream count_stream(nullptr, 0, true);
	tracker_encode_visitor visitor;
	visitor.m_stream = &count_stream;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	openvr_broker::open_vr_interfaces interfaces;
	openvr_broker::acquire_interfaces("null", &interfaces, nullptr);
	traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &interfaces);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	uint64_t state_size = count_stream.buf_pos + 1;
	uint64_t padded_state_size = (state_size + 3) & ~0x3;

	// key size
	uint64_t resource_keys_size = tracker->keys.GetEncodedSize();
	uint64_t padded_resource_keys_size = (resource_keys_size + 3) & ~0x3;

	// event size
	uint64_t event_size = tracker->m_events.size() * sizeof(tracker->m_events[0]);
	uint64_t padded_event_size = (event_size + 3) & ~0x3;

	// timestamp size
	uint64_t timestamp_size = tracker->m_time_stamps.size() * sizeof(tracker->m_time_stamps[0]);
	uint64_t padded_timestamp_size = (timestamp_size + 3) & ~0x3;

	// allocate a buffer
	uint64_t total_size = sizeof(header) +
		sizeof(tracker_save_summary) +
		padded_state_size +
		padded_resource_keys_size +
		padded_event_size +
		timestamp_size;
	assert(size_t(total_size) == total_size);	// make sure I haven't blown 32 bits

	char *buf = (char*)malloc(static_cast<size_t>(total_size));
}

// if every timestamp had also a list of objects that were updated
// serialization could use this to efficently stream updates

// algorithm
// log spawns and modifications for each tick

// deserializer can 
// instantiate 
// apply modifications for each tick

//key_spawns
//or 
//config spawns
//... missing spawns from the keys system

// ui could efficently ask for what changed with a range

// the url system could assign a hash
// 

// spawns and expand structure

