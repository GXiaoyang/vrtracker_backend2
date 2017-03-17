// setting up the tracker requires a few steps. the purpose of this header
// is to provide a central place for test programs to do that.

#pragma once
#include "slab_allocator.h"
#include "vr_tmp_vector.h"
#include "vr_tracker.h"
#include "tracker_test_context.h"


tracker_test_context::tracker_test_context()
	: s(1024 * 1024 * 32),	// todo - put textures onto their own slab (2048*2048*4 = 16MB)
	m_config(nullptr),
	m_tracker(nullptr),
	m_interfaces(nullptr)
{
	slab_allocator_base::m_temp_slab = &s;
	vr_tmp_vector_base::m_global_pool = &m_tmp_pool;
}

void tracker_test_context::ForceInitAll()
{
	raw_vr_interfaces();
	get_tracker();
}

tracker_test_context::~tracker_test_context()
{
	delete m_config;
	delete m_tracker;
	delete m_interfaces;
}

const tmp_vector_pool<VRTMPSize> *tracker_test_context::get_tmp_pool() const
{
	return &m_tmp_pool;
}

TrackerConfig &tracker_test_context::get_config()
{
	if (!m_config)
	{
		m_config = new TrackerConfig();
		m_config->set_default();
	}
	return *m_config;
}

vr_tracker& tracker_test_context::get_tracker()
{
	if(!m_tracker)
	{
		m_tracker = new vr_tracker(&s);
		m_tracker->keys.Init(get_config());
	}
	return *m_tracker;
}

openvr_broker::open_vr_interfaces &tracker_test_context::raw_vr_interfaces()
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

