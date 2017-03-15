// setting up the tracker requires a few steps. the purpose of this header
// is to provide a central place for test programs to do that.

#pragma once
#include "slab_allocator.h"
#include "vr_tmp_vector.h"
#include "vr_tracker.h"
#include "openvr_broker.h"

struct test_context
{
private:
	slab s;
	tmp_vector_pool<VRTMPSize> m_tmp_pool;
	
	TrackerConfig *m_config;
	vr_tracker<slab_allocator<char>> *m_tracker;
	openvr_broker::open_vr_interfaces *m_interfaces;

public:
	test_context()
		: s(1024 * 1024 * 32),	// todo - put textures onto their own slab (2048*2048*4 = 16MB)
			m_config(nullptr),
			m_tracker(nullptr),
			m_interfaces(nullptr)
	{
		slab_allocator<char>::m_temp_slab = &s;
		vr_tmp_vector_base::m_global_pool = &m_tmp_pool;
	}

	~test_context()
	{
		delete m_config;
		delete m_tracker;
		delete m_interfaces;
	}

	const tmp_vector_pool<VRTMPSize> *tmp_pool() const
	{
		return &m_tmp_pool;
	}


	TrackerConfig &config()
	{
		if (!m_config)
		{
			m_config = new TrackerConfig();
			m_config->set_default();
		}
		return *m_config;
	}
	vr_tracker<slab_allocator<char>>& tracker()
	{
		if (!m_tracker)
		{
			m_tracker = new vr_tracker<slab_allocator<char>>(&s);
			m_tracker->keys.Init(config());
		}
		return *m_tracker;
	}


	openvr_broker::open_vr_interfaces &vr_interfaces()
	{
		if (!m_interfaces)
		{
			char *error;
			m_interfaces = new openvr_broker::open_vr_interfaces;
			bool acquired = openvr_broker::acquire_interfaces("raw", m_interfaces, &error);
			if (!acquired)
			{
				printf("error! %s", error);
				exit(1);
			}
		}
		return *m_interfaces;
	}
};
