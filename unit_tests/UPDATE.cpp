// rangesplay.cpp : Defines the entry point for the console application.
//

#include "test_context.h"
#include "update_history_visitor.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "update_history_visitor.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include <iostream>

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
		printf("%s %lld\n", name, end - start);
	}
	void wait() {}
};


template <typename visitor_fn>
static void traverse_history_graph_threaded(visitor_fn &visitor,
	vr_tracker *outer_state,
	openvr_broker::open_vr_interfaces &interfaces) 
{
	SystemWrapper			system_wrapper(interfaces.sysi);
	ApplicationsWrapper		application_wrapper(interfaces.appi);
	SettingsWrapper			settings_wrapper(interfaces.seti);
	ChaperoneWrapper		chaperone_wrapper(interfaces.chapi);
	ChaperoneSetupWrapper	chaperone_setup_wrapper(interfaces.chapsi);
	CompositorWrapper		compositor_wrapper(interfaces.compi);
	OverlayWrapper			overlay_wrapper(interfaces.ovi);
	RenderModelsWrapper		rendermodel_wrapper(interfaces.remi);
	ExtendedDisplayWrapper	extended_display_wrapper(interfaces.exdi);
	TrackedCameraWrapper	tracked_camera_wrapper(interfaces.taci);
	ResourcesWrapper		resources_wrapper(interfaces.resi);

	std::vector<std::thread*> threads;
	vr_state *s = &outer_state->m_state;
	vr_keys *keys = &outer_state->keys;
	//ExecuteImmediatelyTaskGroup g;
	named_task_group g;

	g.run("visit_system_node", 
		[&] {
		visit_system_node(visitor, &s->system_node, interfaces.sysi, &system_wrapper, &rendermodel_wrapper, keys, g);
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


template <typename visitor_fn>
static void traverse_history_graph_sequential(visitor_fn &visitor, 
	vr_tracker *outer_state,
	openvr_broker::open_vr_interfaces &interfaces)
{
	SystemWrapper			system_wrapper(interfaces.sysi);
	ApplicationsWrapper		application_wrapper(interfaces.appi);
	SettingsWrapper			settings_wrapper(interfaces.seti);
	ChaperoneWrapper		chaperone_wrapper(interfaces.chapi);
	ChaperoneSetupWrapper	chaperone_setup_wrapper(interfaces.chapsi);
	CompositorWrapper		compositor_wrapper(interfaces.compi);
	OverlayWrapper			overlay_wrapper(interfaces.ovi);
	RenderModelsWrapper		rendermodel_wrapper(interfaces.remi);
	ExtendedDisplayWrapper	extended_display_wrapper(interfaces.exdi);
	TrackedCameraWrapper	tracked_camera_wrapper(interfaces.taci);
	ResourcesWrapper		resources_wrapper(interfaces.resi);

	vr_state *s = &outer_state->m_state;
	vr_keys *keys = &outer_state->keys;
	ExecuteImmediatelyTaskGroup dummy;

	uint64_t clock_start = rdtsc();
	tbb::tick_count t0 = tbb::tick_count::now();
	visit_system_node(visitor, &s->system_node, interfaces.sysi, &system_wrapper, &rendermodel_wrapper, keys, dummy);

	tbb::tick_count t1 = tbb::tick_count::now();
	visit_applications_node(visitor, &s->applications_node, &application_wrapper, 
		keys, dummy);

	tbb::tick_count t2 = tbb::tick_count::now();
	visit_settings_node(visitor, &s->settings_node, &settings_wrapper, keys, dummy);

	tbb::tick_count t3 = tbb::tick_count::now();
	visit_chaperone_node(visitor, &s->chaperone_node, &chaperone_wrapper, keys);
	tbb::tick_count t4 = tbb::tick_count::now();
	visit_chaperone_setup_node(visitor, &s->chaperone_setup_node, &chaperone_setup_wrapper);
	tbb::tick_count t5 = tbb::tick_count::now();
	
	visit_compositor_state(visitor, &s->compositor_node, &compositor_wrapper, keys, dummy);

	tbb::tick_count t6 = tbb::tick_count::now();
	visit_overlay_state(visitor, &s->overlay_node, &overlay_wrapper, keys, dummy);
	tbb::tick_count t7 = tbb::tick_count::now();
	visit_rendermodel_state(visitor, &s->render_models_node, &rendermodel_wrapper, dummy);
	tbb::tick_count t8 = tbb::tick_count::now();
	visit_extended_display_state(visitor, &s->extended_display_node, &extended_display_wrapper);
	tbb::tick_count t9 = tbb::tick_count::now();
	visit_trackedcamera_state(visitor, &s->tracked_camera_node, &tracked_camera_wrapper,
			keys);
	tbb::tick_count t10 = tbb::tick_count::now();
	visit_resources_state(visitor, &s->resources_node, &resources_wrapper, keys);
	tbb::tick_count t11 = tbb::tick_count::now();
	uint64_t clock_end = rdtsc();
	std::cout << "sequential visitor cycles: "
		<< clock_end - clock_start
		<< "\n";
}

struct read_only_visitor
{
	constexpr read_only_visitor()
	{}

	constexpr bool visit_source_interfaces() const { return false; }
	constexpr bool expand_structure() const { return false; }
	constexpr bool reload_render_models() const { return false; }
	constexpr bool recheck_distortion() const { return false; }

	inline void start_group_node(const base::URL &url_name, int group_id_index)
	{}

	inline void end_group_node(const base::URL &group_id_name, int group_id_index)
	{}

	// this should give the history half and the openvr half
	// because if openvr resizes something, then the history visitor should have a chance to resize
	// as well
	template <typename T>
	inline void start_vector(const base::URL &vector_name, T &vec)
	{}

	template <typename T>
	inline void end_vector(const base::URL &vector_name, T &vec)
	{}

	template <typename HistoryVectorType, typename ResultType>
	void visit_node(HistoryVectorType &history, ResultType &latest_result)
	{
	}

	template <typename HistoryVectorType>
	void visit_node(HistoryVectorType &history_node)
	{
	}
};


void load_one_overlay_image(const char *overlay_name, vr::IVROverlay *ovi)
{
	vr::VROverlayHandle_t handle;
	vr::EVROverlayError err = ovi->FindOverlay(overlay_name, &handle);
	if (err == vr::VROverlayError_None)
	{
		uint32_t width;
		uint32_t height;
		EVROverlayError err2 = ovi->GetOverlayImageData(
			handle, nullptr, 0, &width, &height);
		if (err2 == VROverlayError_ArrayTooSmall)
		{
			size_t required_size = width * height * 4;
			void *buf = malloc(required_size);
			if (buf)
			{
				EVROverlayError err3 = ovi->GetOverlayImageData(handle, buf, required_size, &width, &height);
				if (err3 == VROverlayError_None)
				{
					printf("success\n");
				}
				else
				{
					printf("failed\n");
				}
				free(buf);
			}
		}
	}
}

void parallel_overlay_test(vr::IVROverlay *ovi)
{
	for (int i = 0; i < 10; i++)
	{
		load_one_overlay_image("valve.steam.desktop", ovi);
		load_one_overlay_image("valve.steam.bigpicture", ovi);
		load_one_overlay_image("system.vrdashboard", ovi);
	}

	
	std::vector<std::thread *> threads;
	for (int i = 0; i < 100; i++)
	{
		threads.push_back(new std::thread(load_one_overlay_image, "valve.steam.bigpicture", ovi));
		threads.push_back(new std::thread(load_one_overlay_image, "system.vrdashboard", ovi));
	}

	for (auto thread : threads)
	{
		thread->join();
	}

	
	
}

void UPDATE_USE_CASE()
{
	using namespace vr;
	using namespace vr_result;
	using std::range;

	test_context context;

	&context.tracker();
	context.vr_interfaces();


	// 
	// Sequential visit 
	//
	update_history_visitor update_visitor(1);
	

	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		traverse_history_graph_sequential(update_visitor, &context.tracker(), context.vr_interfaces());
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "first sequential visit took "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< "ms.\n";
	}
	
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		traverse_history_graph_sequential(update_visitor, &context.tracker(), context.vr_interfaces());
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "second sequential visit took "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< "ms.\n";
	}

	
	update_visitor.m_frame_number++;
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		traverse_history_graph_threaded(update_visitor, &context.tracker(), context.vr_interfaces());
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "threaded visit took "
			<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
			<< "us.\n";
	}

	while (1)
	{
		{			
			int num_threads = update_visitor.m_frame_number % 8 + 1;
			tbb::task_scheduler_init init(num_threads);
		
			update_visitor.m_frame_number++;
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			traverse_history_graph_threaded(update_visitor, &context.tracker(), context.vr_interfaces());
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			std::cout 
				<< "frame:" << update_visitor.m_frame_number
				<< " num_threads:" << num_threads << " took "
				<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
				<< "us.\n";
		}
	}

	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		traverse_history_graph_sequential(update_visitor, &context.tracker(), context.vr_interfaces());
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "final sequential visit took "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< "ms.\n";
	}



	//
	// test the read only visitor
	//
	int allocs_before = context.tmp_pool()->get_num_alloc_one_calls();
	int frees_before = context.tmp_pool()->get_num_free_one_calls();
	read_only_visitor read_visitor;
	traverse_history_graph_sequential(read_visitor, &context.tracker(), context.vr_interfaces());

	// see if the read only allocated any temporaries
	assert(allocs_before == context.tmp_pool()->get_num_alloc_one_calls());
	assert(frees_before == context.tmp_pool()->get_num_free_one_calls());

}

