#include <time.h>
#include "crc_32.h"
#include "capture_traverser.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "capture_updater.h"
#include "capture_encoder.h"
#include "capture_decoder.h"
#include "capture_id_fixer.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "capture.h"
#include "openvr_serialization.h"
#include "MemoryStream.h"
#include "FileStream.h"
#include <fstream>
#include <algorithm>

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

struct WrapperSet
{
	WrapperSet()
	:
	system_wrapper(nullptr),
	application_wrapper(nullptr),
	settings_wrapper(nullptr),
	chaperone_wrapper(nullptr),
	chaperone_setup_wrapper(nullptr),
	compositor_wrapper(nullptr),
	overlay_wrapper(nullptr),
	rendermodel_wrapper(nullptr),
	extended_display_wrapper(nullptr),
	tracked_camera_wrapper(nullptr),
	resources_wrapper(nullptr),
	driver_manager_wrapper(nullptr)
	{}

	void assign(openvr_broker::open_vr_interfaces *interfaces)
	{
		system_wrapper = new SystemWrapper(interfaces->sysi);
		application_wrapper = new ApplicationsWrapper(interfaces->appi);
		settings_wrapper = new SettingsWrapper(interfaces->seti);
		chaperone_wrapper = new ChaperoneWrapper(interfaces->chapi);
		chaperone_setup_wrapper = new ChaperoneSetupWrapper(interfaces->chapsi);
		compositor_wrapper = new CompositorWrapper(interfaces->compi);
		overlay_wrapper = new OverlayWrapper(interfaces->ovi);
		rendermodel_wrapper = new RenderModelsWrapper(interfaces->remi);
		extended_display_wrapper = new ExtendedDisplayWrapper(interfaces->exdi);
		tracked_camera_wrapper = new TrackedCameraWrapper(interfaces->taci);
		resources_wrapper = new ResourcesWrapper(interfaces->resi);
		driver_manager_wrapper = new DriverManagerWrapper(interfaces->drivi);
	}

	~WrapperSet()
	{
		delete system_wrapper;
		delete application_wrapper;
		delete settings_wrapper;
		delete chaperone_wrapper;
		delete chaperone_setup_wrapper;
		delete compositor_wrapper;
		delete overlay_wrapper;
		delete rendermodel_wrapper;
		delete extended_display_wrapper;
		delete tracked_camera_wrapper;
		delete resources_wrapper;
		delete driver_manager_wrapper;
	}
	SystemWrapper			*system_wrapper;
	ApplicationsWrapper		*application_wrapper;
	SettingsWrapper			*settings_wrapper;
	ChaperoneWrapper		*chaperone_wrapper;
	ChaperoneSetupWrapper	*chaperone_setup_wrapper;
	CompositorWrapper		*compositor_wrapper;
	OverlayWrapper			*overlay_wrapper;
	RenderModelsWrapper		*rendermodel_wrapper;
	ExtendedDisplayWrapper	*extended_display_wrapper;
	TrackedCameraWrapper	*tracked_camera_wrapper;
	ResourcesWrapper		*resources_wrapper;
	DriverManagerWrapper	*driver_manager_wrapper;
};

template <typename TaskGroup, typename visitor_fn>
static void traverse_history_graph(visitor_fn *visitor, capture *outer_state, WrapperSet *wrappers)
{
	vr_state *s = &outer_state->m_state;
	vr_keys *keys = &outer_state->m_keys;

	TaskGroup g;
	
	g.run("visit_system_node",
		[&visitor, s, wrappers, keys, &g] {
		visit_system_node(visitor, &s->system_node, wrappers->system_wrapper, wrappers->rendermodel_wrapper, keys, g);
	});

	g.run("visit_application_node", 
		[&visitor, s, wrappers, keys, &g] {
		visit_applications_node(visitor, &s->applications_node, wrappers->application_wrapper, keys, g);
	});

	g.run("visit_settings_node",
		[&visitor, s, wrappers, keys, &g] {
		visit_settings_node(visitor, &s->settings_node, wrappers->settings_wrapper, keys, g);
	});

	g.run("visit_chaperone_node",
		[&visitor, s, wrappers, keys, &g] {
		visit_chaperone_node(visitor, &s->chaperone_node, wrappers->chaperone_wrapper, keys);
	});

	g.run("visit_chaperone_setup",
		[&visitor, s, wrappers, keys, &g] { 
		visit_chaperone_setup_node(visitor, &s->chaperone_setup_node, wrappers->chaperone_setup_wrapper);
	});

	g.run("visit_compositor_state",
		[&visitor, s, wrappers, keys, &g] {
		visit_compositor_state(visitor, &s->compositor_node, wrappers->compositor_wrapper, keys, g);
	});

	g.run("visit_overlay_state",
		[&visitor, s, wrappers, keys, &g] {
		visit_overlay_state(visitor, &s->overlay_node, wrappers->overlay_wrapper, keys, g);
	});

	visit_rendermodel_state(visitor, &s->render_models_node, wrappers->rendermodel_wrapper, keys, g);

	g.run("visit_extended_display_state",
		[&visitor, s, wrappers, keys] {
		visit_extended_display_state(visitor, &s->extended_display_node, wrappers->extended_display_wrapper);
	});
	g.run("visit_tracked_camera_state",
		[&visitor, s, wrappers, keys] {
		visit_trackedcamera_state(visitor, &s->tracked_camera_node, wrappers->tracked_camera_wrapper, keys);
	});
	g.run("visit_resources_state",
		[&visitor, s, wrappers, keys] {
		visit_resources_state(visitor, &s->resources_node, wrappers->resources_wrapper, keys);
	});
	g.run("visit_driver_manager_state",
		[&visitor, s, wrappers, keys] {
		visit_driver_manager_state(visitor, &s->driver_manager_node, wrappers->driver_manager_wrapper, keys);
	});
	g.wait();
}

struct capture_traverser::impl
{
	WrapperSet null_wrappers;

	// since the objects are not constructed in a deterministic order, we need to save it
	// so it can be restored on re-load
	uint64_t calc_registry_id_table_size(capture *capture)
	{
		MemoryStream count_stream(nullptr, 0, true);
		encode_registry_table(count_stream, capture);
		return count_stream.buf_pos;
	}

	void encode_registry_table(BaseStream &stream, capture *capture)
	{
		int num_entries = capture->m_state_registry.GetNumRegistered();
		stream.write_to_stream(&num_entries, sizeof(num_entries));
		for (int i = 0; i < num_entries; i++)
		{
			RegisteredSerializable *r = capture->m_state_registry.registered[i];
			stream.contiguous_container_out_to_stream(r->get_serialization_url().get_full_path());
		}
	}

	void fixup_registry_ids(BaseStream &e, capture *capture)
	{
		int num_entries;
		e.read_from_stream(&num_entries, sizeof(num_entries));
		capture_id_fixer visitor;
		capture->m_state_registry.clear();
		capture->m_state_registry.reserve(num_entries);
		visitor.registry = &capture->m_state_registry;
		for (int i = 0; i < num_entries; i++)
		{
			std::string full_url;
			e.contiguous_container_from_stream(full_url);
			visitor.url2id.insert({ full_url, i });
		}
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, capture, &null_wrappers);

	}

	uint64_t calc_keys_size(capture *capture)
	{
		// state size
		MemoryStream count_stream(nullptr, 0, true);
		capture->m_keys.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_state_size(capture *capture)
	{
		// state size
		MemoryStream count_stream(nullptr, 0, true);
		capture_encode_visitor visitor;
		visitor.m_stream = &count_stream;
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, capture, &null_wrappers);
		return count_stream.buf_pos;
	}

	uint64_t calc_vr_events_size(capture *capture)
	{
		MemoryStream count_stream(nullptr, 0, true);
		capture->m_vr_events.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_time_stamps_size(capture *capture)
	{
		MemoryStream count_stream(nullptr, 0, true);
		count_stream.forward_container_out_to_stream(capture->m_time_stamps);
		return count_stream.buf_pos;
	}

	uint64_t calc_keys_updates_size(capture *capture)
	{
		MemoryStream count_stream(nullptr, 0, true);
		capture->m_keys_updates.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_state_update_bits_size(capture *capture)
	{
		MemoryStream count_stream(nullptr, 0, true);
		capture->m_state_update_bits.encode(count_stream);
		return count_stream.buf_pos;
	}
};

capture_traverser::capture_traverser()
{
	m_pimpl = new capture_traverser::impl;
}

capture_traverser::~capture_traverser()
{
	delete m_pimpl;
}

struct ConfigObserver : KeysObserver
{
	void NewVRKeysUpdate(const VRKeysUpdate &e) final
	{
		config_events.push_back(e);
	}
	tbb::concurrent_vector<VRKeysUpdate> config_events;
};

void capture_traverser::update_capture(capture *capture,
	openvr_broker::open_vr_interfaces *interfaces,
	time_stamp_t update_time,
	bool parallel)
{
	time_index_t last_updated = capture->get_last_updated_frame();
	capture_update_visitor update_visitor(last_updated + 1);			// setup the visitor with the new frame number
	update_visitor.registry = &capture->m_state_registry;				// setup the visitor so he can register any new state objects

	ConfigObserver config_observer;
	capture->m_keys.RegisterObserver(&config_observer);

	WrapperSet wrappers;
	wrappers.assign(interfaces);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	if (parallel)
	{
		traverse_history_graph<named_task_group>(&update_visitor, capture, &wrappers);
	}
	else
	{
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&update_visitor, capture, &wrappers);
	}
	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	bool quiet = true;
	if (!quiet)
	{
		log_printf("parallel update took %lld us.\n",
			std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	//
	// after updating the frame, finalize any per frame stats and then advance the frame number
	//

	// after update, log any new keys discovered:
	capture->m_keys.UnRegisterObserver(&config_observer);
	for (VRKeysUpdate& e : config_observer.config_events)
	{
		capture->m_keys_updates.emplace_back(update_visitor.get_frame_number(), e);
	}

	// after update, log updated nodes
	if (!update_visitor.updated_node_bits.empty())
	{
		// any items that updated
		capture->m_state_update_bits.emplace_back(update_visitor.get_frame_number(), update_visitor.updated_node_bits);
	}
	
	// after update, log the frame_time
	capture->m_time_stamps.push_back(update_time);

	// after update, finally update m_last_updated_frame_number
	capture->increment_last_updated_frame();
}


static const uint32_t HEADER_MAGIC = 0x7;

// file format starts with a header:
struct header_t
{
	uint32_t magic;
	uint32_t crc;
	uint64_t summary_offset;
	uint64_t summary_size;
	uint64_t registry_offset;
	uint64_t registry_size;
	uint64_t keys_offset;
	uint64_t keys_size;
	uint64_t state_offset;
	uint64_t state_size;
	uint64_t events_offset;
	uint64_t events_size;

	uint64_t time_stamps_offset;
	uint64_t time_stamps_size;
	uint64_t keys_updates_offset;
	uint64_t keys_updates_size;
	uint64_t state_update_bits_offset;
	uint64_t state_update_bits_size;
	uint64_t updates_offset;	// no size since it's streaming

	void encode(BaseStream &e) const
	{
		e.write_to_stream(this, sizeof(*this));
	}

	void decode(BaseStream &e) 
	{
		e.read_from_stream(this, sizeof(*this));
	}

	bool verify()
	{
		if (magic != HEADER_MAGIC)
		{
			return false;
		}
		uint32_t tmp = crc;
		crc = 0;
		uint32_t calculated_crc = crc32buf((char*)this, sizeof(*this));
		crc = tmp;
		if (tmp != calculated_crc)
		{
			return false; // crc mismatch
		}
		return true;
	}
};

inline uint64_t pad_size(uint64_t in)
{
	return in;
	// for valgrind, try and not use padding at allreturn (in + 3) & ~0x3;
}

bool capture_traverser::save_capture_to_binary_file(capture *capture, const char *filename)
{
	bool rc = true;
	header_t header;
	memset(&header, 0, sizeof(header));
	header.magic = HEADER_MAGIC;
	header.summary_size		 = sizeof(save_summary);
	header.registry_size = m_pimpl->calc_registry_id_table_size(capture);
	header.keys_size		 = m_pimpl->calc_keys_size(capture);
	header.state_size		 = m_pimpl->calc_state_size(capture);
	header.events_size		 = m_pimpl->calc_vr_events_size(capture);
	header.time_stamps_size	 = m_pimpl->calc_time_stamps_size(capture);
	header.keys_updates_size = m_pimpl->calc_keys_updates_size(capture);
	header.state_update_bits_size = m_pimpl->calc_state_update_bits_size(capture);

	header.summary_offset           = sizeof(header);
	header.registry_offset          = header.summary_offset     + pad_size(header.summary_size);
	header.keys_offset              = header.registry_offset	+ pad_size(header.registry_size);
	header.state_offset             = header.keys_offset		+ pad_size(header.keys_size);
	header.events_offset            = header.state_offset		+ pad_size(header.state_size);
	header.time_stamps_offset       = header.events_offset		+ pad_size(header.events_size);
	header.keys_updates_offset      = header.time_stamps_offset + pad_size(header.time_stamps_size);
	header.state_update_bits_offset = header.keys_updates_offset + pad_size(header.keys_updates_size);
	header.updates_offset           = header.state_update_bits_offset + pad_size(header.state_update_bits_size);

	std::time_t start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef _WIN32
	ctime_s(capture->m_save_summary.start_date_string, sizeof(capture->m_save_summary.start_date_string), &start);
#else
	strcpy(capture->m_save_summary.start_date_string, ctime(&start));
#endif
	capture->m_save_summary.last_encoded_frame = capture->get_last_updated_frame();

	// LAST STEP after writing into the header is to write the crc
	header.crc = crc32buf((char *)&header, sizeof(header));

	// make sure the size fits in whatever size_t is
	assert(size_t(header.updates_offset) == header.updates_offset);
	FileStream f;
	
	if (f.open_file_for_write_plus(filename))
	{
		BaseStream &stream = f;
		{
			header.encode(stream);
		}
		{
			stream.set_pos(header.summary_offset);
			capture->m_save_summary.encode(stream);
		}
		{
			stream.set_pos(header.registry_offset);
			m_pimpl->encode_registry_table(stream, capture);
		}
		{
			stream.set_pos(header.keys_offset);
			capture->m_keys.encode(stream);
		}
		{
			stream.set_pos(header.state_offset);
			capture_encode_visitor visitor;
			visitor.m_stream = &stream;
			traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, capture, &m_pimpl->null_wrappers);
		}
		{
			stream.set_pos(header.events_offset);
			capture->m_vr_events.encode(stream);
		}
		{
			stream.set_pos(header.time_stamps_offset);
			stream.forward_container_out_to_stream(capture->m_time_stamps);
		}
		{
			stream.set_pos(header.keys_updates_offset);
			capture->m_keys_updates.encode(stream);
		}
		{
			stream.set_pos(header.state_update_bits_offset);
			capture->m_state_update_bits.encode(stream);
		}
	}
	else
	{
		rc = false;
	}
	return rc;
}

bool capture_traverser::load_capture_from_binary_file(capture *capture, const char *filename)
{
	bool rc = true;

	FileStream stream;
	if (!stream.open_file_for_read(filename))
	{
		rc = false;
	}
	else
	{
		header_t header;
		stream.set_pos(0);
		header.decode(stream);
		if (!header.verify()) // checks the magic value and the CRC
		{
			return false; // header is invalid
		}
		else
		{
			stream.set_pos(header.summary_offset);
			capture->m_save_summary.decode(stream);
		}
		{
			stream.set_pos(header.keys_offset);
			capture->m_keys.decode(stream);
		}
		{
			stream.set_pos(header.state_offset);
			capture_decode_visitor visitor;
			visitor.m_stream = &stream;
			visitor.registry = &capture->m_state_registry;
			traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, capture, &m_pimpl->null_wrappers);
		}
		{
			stream.set_pos(header.events_offset);
			capture->m_vr_events.decode(stream);
		}
		{
			stream.set_pos(header.time_stamps_offset);
			stream.forward_container_from_stream(capture->m_time_stamps);
		}
		{
			stream.set_pos(header.keys_updates_offset);
			capture->m_keys_updates.decode(stream);
		}
		{
			stream.set_pos(header.state_update_bits_offset);
			capture->m_state_update_bits.decode(stream);
		}

		// fixup the registry
		{
			stream.set_pos(header.registry_offset);
			m_pimpl->fixup_registry_ids(stream, capture);
		}

		// apply chunks here

		// write derived values
		capture->m_last_updated_frame_number = capture->m_save_summary.last_encoded_frame;
		rc = true;
	}
	return rc;
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

