#include "crc_32.h"
#include "vr_tracker_traverse.h"
#include "vr_system_wrapper.h"
#include "traverse_graph.h"
#include "tracker_update_visitor.h"
#include "tracker_encode_visitor.h"
#include "tracker_decode_visitor.h"
#include "tracker_id_fixer_visitor.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "vr_tracker.h"
#include "openvr_serialization.h"
#include <fstream>

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
	resources_wrapper(nullptr)
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
};



template <typename TaskGroup, typename visitor_fn>
static void traverse_history_graph(visitor_fn *visitor, vr_tracker *outer_state, WrapperSet *wrappers)
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

	visit_rendermodel_state(visitor, &s->render_models_node, wrappers->rendermodel_wrapper, g);

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
	g.wait();
}

struct vr_tracker_traverse::impl
{
	WrapperSet null_wrappers;

	impl()
	{
	}

	// since the objects are not constructed in a deterministic order, we need to save it
	// so it can be restored on re-load
	uint64_t calc_registry_id_table_size(vr_tracker *tracker)
	{
		EncodeStream count_stream(nullptr, 0, true);
		encode_registry_table(count_stream, tracker);
		return count_stream.buf_pos;
	}

	void encode_registry_table(EncodeStream &stream, vr_tracker *tracker)
	{
		int num_entries = tracker->m_state_registry.GetNumRegistered();
		stream.memcpy_out_to_stream(&num_entries, sizeof(num_entries));
		for (int i = 0; i < num_entries; i++)
		{
			RegisteredSerializable *r = tracker->m_state_registry.registered[i];
			stream.contiguous_container_out_to_stream(r->get_serialization_url().get_full_path());
		}
	}

	void fixup_registry_ids(EncodeStream &e, vr_tracker *tracker)
	{
		int num_entries;
		e.memcpy_from_stream(&num_entries, sizeof(num_entries));
		tracker_id_fixer_visitor visitor;
		tracker->m_state_registry.clear();
		tracker->m_state_registry.reserve(num_entries);
		visitor.registry = &tracker->m_state_registry;
		for (int i = 0; i < num_entries; i++)
		{
			std::string full_url;
			e.contiguous_container_from_stream(full_url);
			visitor.url2id.insert({ full_url, i });
		}
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &null_wrappers);
	}


	uint64_t calc_keys_size(vr_tracker *tracker)
	{
		// state size
		EncodeStream count_stream(nullptr, 0, true);
		tracker->m_keys.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_state_size(vr_tracker *tracker)
	{
		// state size
		EncodeStream count_stream(nullptr, 0, true);
		tracker_encode_visitor visitor;
		visitor.m_stream = &count_stream;
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &null_wrappers);
		return count_stream.buf_pos;
	}

	uint64_t calc_vr_events_size(vr_tracker *tracker)
	{
		EncodeStream count_stream(nullptr, 0, true);
		tracker->m_vr_events.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_time_stamps_size(vr_tracker *tracker)
	{
		EncodeStream count_stream(nullptr, 0, true);
		count_stream.forward_container_out_to_stream(tracker->m_time_stamps);
		return count_stream.buf_pos;
	}

	uint64_t calc_keys_updates_size(vr_tracker *tracker)
	{
		EncodeStream count_stream(nullptr, 0, true);
		tracker->m_keys_updates.encode(count_stream);
		return count_stream.buf_pos;
	}

	uint64_t calc_state_update_bits_size(vr_tracker *tracker)
	{
		EncodeStream count_stream(nullptr, 0, true);
		tracker->m_state_update_bits.encode(count_stream);
		return count_stream.buf_pos;
	}
	

};

vr_tracker_traverse::vr_tracker_traverse()
{
	m_pimpl = new vr_tracker_traverse::impl;
}

vr_tracker_traverse::~vr_tracker_traverse()
{
	delete m_pimpl;
}

struct ConfigObserver : KeysObserver
{
	virtual void NewVRKeysUpdate(const VRKeysUpdate &e) override final
	{
		config_events.push_back(e);
	}
	tbb::concurrent_vector<VRKeysUpdate> config_events;
};

void vr_tracker_traverse::update_tracker_parallel(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	tracker_update_visitor update_visitor(last_updated + 1);
	update_visitor.registry = &tracker->m_state_registry;

	ConfigObserver config_observer;
	tracker->m_keys.RegisterObserver(&config_observer);

	WrapperSet wrappers;
	wrappers.assign(interfaces);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<named_task_group>(&update_visitor, tracker, &wrappers);
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

	// any new keys discovered:
	tracker->m_keys.UnRegisterObserver(&config_observer);
	for (VRKeysUpdate& e : config_observer.config_events)
	{
		tracker->m_keys_updates.emplace_back(update_visitor.get_frame_number(), e);
	}

	if (!update_visitor.updated_node_bits.empty())
	{
		// any items that updated
		tracker->m_state_update_bits.emplace_back(update_visitor.get_frame_number(), update_visitor.updated_node_bits);
	}
	
	using us = std::chrono::duration<int64_t, std::micro>;
	us frame_time = std::chrono::duration_cast<std::chrono::microseconds>(start - tracker->start);
	tracker->m_time_stamps.push_back(frame_time.count());
	tracker->m_last_updated_frame_number = update_visitor.get_frame_number();
}

void vr_tracker_traverse::update_tracker_sequential(vr_tracker *tracker, openvr_broker::open_vr_interfaces *interfaces)
{
	time_index_t last_updated = tracker->get_last_updated_frame();
	tracker_update_visitor update_visitor(last_updated + 1);
	update_visitor.registry = &tracker->m_state_registry;

	WrapperSet wrappers;
	wrappers.assign(interfaces);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	traverse_history_graph<ExecuteImmediatelyTaskGroup>(&update_visitor, tracker, &wrappers);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log_printf("sequential update took %lld us.\n",
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	
	tracker->m_last_updated_frame_number = last_updated + 1;
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

	void encode(EncodeStream &e) const
	{
		e.memcpy_out_to_stream(this, sizeof(*this));
	}

	void decode(EncodeStream &e) 
	{
		e.memcpy_from_stream(this, sizeof(*this));
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
	return (in + 3) & ~0x3;
}

bool vr_tracker_traverse::save_tracker_to_binary_file(vr_tracker *tracker, const char *filename)
{
	bool rc = true;
	header_t header;
	memset(&header, 0, sizeof(header));
	header.magic = HEADER_MAGIC;
	header.summary_size		 = sizeof(save_summary);
	header.registry_size = m_pimpl->calc_registry_id_table_size(tracker);
	header.keys_size		 = m_pimpl->calc_keys_size(tracker);
	header.state_size		 = m_pimpl->calc_state_size(tracker);
	uint64_t count_again0 = m_pimpl->calc_state_size(tracker);

	if (count_again0 != header.state_size)
	{
		size_t buf_size = max(count_again0, header.state_size) + 4096;
		char *debuga = (char *)malloc(buf_size);

		EncodeStream a(debuga, buf_size, false);
		tracker_encode_visitor visitor;
		visitor.m_stream = &a;

		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &m_pimpl->null_wrappers);
		FILE *pf = fopen("c:\\temp\\one.bin", "wb");
		fwrite(debuga, 1, a.buf_pos, pf);
		fclose(pf);

		a.reset_buf_pos();
		traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &m_pimpl->null_wrappers);
		pf = fopen("c:\\temp\\two.bin", "wb");
		fwrite(debuga, 1, a.buf_pos, pf);
		fclose(pf);
		free(debuga);
	}

	uint64_t count_again1 = m_pimpl->calc_state_size(tracker);
	assert(count_again0 == header.state_size);
	assert(count_again1 == header.state_size);
	header.events_size		 = m_pimpl->calc_vr_events_size(tracker);
	header.time_stamps_size	 = m_pimpl->calc_time_stamps_size(tracker);
	header.keys_updates_size = m_pimpl->calc_keys_updates_size(tracker);
	header.state_update_bits_size = m_pimpl->calc_state_update_bits_size(tracker);

	header.summary_offset           = sizeof(header);
	header.registry_offset          = header.summary_offset     + pad_size(header.registry_size);
	header.keys_offset              = header.registry_offset	+ pad_size(header.registry_size);
	header.state_offset             = header.keys_offset		+ pad_size(header.keys_size);
	header.events_offset            = header.state_offset		+ pad_size(header.state_size);
	header.time_stamps_offset       = header.events_offset		+ pad_size(header.events_size);
	header.keys_updates_offset      = header.time_stamps_offset + pad_size(header.time_stamps_size);
	header.state_update_bits_offset = header.keys_updates_offset + pad_size(header.keys_updates_size);
	header.updates_offset           = header.state_update_bits_offset + pad_size(header.state_update_bits_size);

	std::time_t start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	ctime_s(tracker->m_save_summary.start_date_string, sizeof(tracker->m_save_summary.start_date_string), &start);
	tracker->m_save_summary.last_encoded_frame = tracker->m_last_updated_frame_number;

	// LAST STEP after writing into the header is to write the crc
	header.crc = crc32buf((char *)&header, sizeof(header));

	// make sure the size fits in whatever size_t is
	assert(size_t(header.updates_offset) == header.updates_offset);
	char *big_buf = (char *)malloc(static_cast<size_t>(header.updates_offset));
	if (big_buf)
	{
		{
			EncodeStream e(big_buf, sizeof(header), false);
			header.encode(e);
		}
		{
			EncodeStream e(big_buf + header.summary_offset, header.summary_size, false);
			tracker->m_save_summary.encode(e);
		}
		{
			EncodeStream e(big_buf + header.registry_offset, header.registry_size, false);
			m_pimpl->encode_registry_table(e, tracker);
		}
		{
			EncodeStream e(big_buf + header.keys_offset, header.keys_size, false);
			tracker->m_keys.encode(e);
		}
		{
			EncodeStream e(big_buf + header.state_offset, header.state_size, false);
			tracker_encode_visitor visitor;
			visitor.m_stream = &e;
			traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &m_pimpl->null_wrappers);
		}
		{
			EncodeStream e(big_buf + header.events_offset, header.events_size, false);
			tracker->m_vr_events.encode(e);
		}
		{
			EncodeStream e(big_buf + header.time_stamps_offset, header.time_stamps_size, false);
			e.forward_container_out_to_stream(tracker->m_time_stamps);
		}
		{
			EncodeStream e(big_buf + header.keys_updates_offset, header.keys_updates_size, false);
			tracker->m_keys_updates.encode(e);
		}
		{
			EncodeStream e(big_buf + header.state_update_bits_offset, header.state_update_bits_size, false);
			tracker->m_state_update_bits.encode(e);
		}
		std::fstream fs;
		fs.open(filename, std::fstream::out | std::fstream::binary);
		fs.write(big_buf, static_cast<size_t>(header.updates_offset));
		fs.close();
		free(big_buf);
	}
	else
	{
		rc = false;
	}

	return rc;
}

bool vr_tracker_traverse::load_tracker_from_binary_file(vr_tracker *tracker, const char *filename)
{
	bool rc = true;
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	if (size >= sizeof(header_t))
	{
		file.seekg(0, std::ios::beg);
		assert(size_t(size) == size);
		char *big_buf = (char *)malloc(static_cast<size_t>(size));
		if (big_buf)
		{
			header_t header;
			if (file.read(big_buf, size))
			{
				file.close();
				EncodeStream e(big_buf, sizeof(header_t), false);
				header.decode(e);
				if (!header.verify()) // checks the magic value and the CRC
				{
					free(big_buf);
					return false; // header is invalid
				}
				
				{
					EncodeStream e(big_buf + header.summary_offset, header.summary_size, false);
					tracker->m_save_summary.decode(e);
				}
				{
					EncodeStream e(big_buf + header.keys_offset, header.keys_size, false);
					tracker->m_keys.decode(e);
				}
				{
					EncodeStream e(big_buf + header.state_offset, header.state_size, false);
					tracker_decode_visitor visitor;
					visitor.m_stream = &e;
					visitor.registry = &tracker->m_state_registry;
					traverse_history_graph<ExecuteImmediatelyTaskGroup>(&visitor, tracker, &m_pimpl->null_wrappers);
					
				}
				{
					EncodeStream e(big_buf + header.events_offset, header.events_size, false);
					tracker->m_vr_events.decode(e);
				}
				{
					EncodeStream e(big_buf + header.time_stamps_offset, header.time_stamps_size, false);
					e.forward_container_from_stream(tracker->m_time_stamps);
				}
				{
					EncodeStream e(big_buf + header.keys_updates_offset, header.keys_updates_size, false);
					tracker->m_keys_updates.decode(e);
				}
				{
					EncodeStream e(big_buf + header.state_update_bits_offset, header.state_update_bits_size, false);
					tracker->m_state_update_bits.decode(e);
				}

				// fixup the registry
				{
					EncodeStream e(big_buf + header.registry_offset, header.registry_size, false);
					m_pimpl->fixup_registry_ids(e, tracker);
				}

				// apply chunks here



				free(big_buf);

				// write derived values
				tracker->m_last_updated_frame_number = tracker->m_save_summary.last_encoded_frame;

			}
		}
		else
		{
			rc = false; // malloc failed
		}
	}
	return rc;
}

#if 0
// streamer 
void vr_tracker_traverse::streamer(vr_tracker *tracker, const char *filename)
{
	// write magic
	// write initial state with no updates
	//	* keys, m_state
	// after any tick is finalized
		// write my size
		// write timestamp
		// write key updates
		// write state update bits
		// write  state update blocks


}
#endif
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

