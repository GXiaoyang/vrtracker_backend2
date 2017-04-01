// setting up the capture requires a few steps. the purpose of this header
// is to provide a central place for test programs to do that.
#include "slab_allocator.h"
#include "vr_tmp_vector.h"
#include "capture.h"
#include "capture_test_context.h"

tmp_vector_pool<VRTMPSize> g_tmp_pool;

void capture_test_context::reset_globals()
{
	delete slab_allocator_base::m_temp_slab;
	slab_allocator_base::m_temp_slab = nullptr;
}

capture_test_context::capture_test_context()
	: 
	//s(1024 * 1024 * 32),	// todo - put textures onto their own slab (2048*2048*4 = 16MB)
	m_config(nullptr),
	m_capture(nullptr),
	m_interfaces(nullptr)
{
	if (!slab_allocator_base::m_temp_slab)
	{
		slab_allocator_base::m_temp_slab = new slab(1024 * 1024 * 32);
	}
	if (!vr_tmp_vector_base::m_global_pool)
	{
		vr_tmp_vector_base::m_global_pool = &g_tmp_pool;
	}
}

void capture_test_context::ForceInitAll()
{
	raw_vr_interfaces();
	get_capture();
}

capture_test_context::~capture_test_context()
{
	delete m_config;
	delete m_capture;
	delete m_interfaces;
}
#if 0
const tmp_vector_pool<VRTMPSize> *capture_test_context::get_tmp_pool() const
{
	return &m_tmp_pool;
}
#endif

CaptureConfig &capture_test_context::get_config()
{
	if (!m_config)
	{
		m_config = new CaptureConfig();
		m_config->set_default();
	}
	return *m_config;
}

capture& capture_test_context::get_capture()
{
	if(!m_capture)
	{
		m_capture = new capture;
		m_capture->m_keys.Init(get_config());
	}
	return *m_capture;
}

openvr_broker::open_vr_interfaces &capture_test_context::raw_vr_interfaces()
{
	if (!m_interfaces)
	{
		char *error;
		m_interfaces = new openvr_broker::open_vr_interfaces;
		bool acquired = openvr_broker::acquire_interfaces("raw", m_interfaces, &error);
		if (!acquired)
		{
			log_printf("error! %s", error);
			exit(1);
		}
	}
	return *m_interfaces;
}

