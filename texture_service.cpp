#include <texture_service.h>
#include "lz4.h"
#include "crc_32.h"
#include "log.h"
#include "openvr_broker.h"

texture_service::texture_service()
	: m_started(false), m_stop_requested(false)
{
	m_num_compressed = 0;
	m_num_textures_submitted = 0;

	openvr_broker::open_vr_interfaces interfaces;
	char *error;
	if (!openvr_broker::acquire_interfaces("raw", &interfaces, &error))
	{
		ABORT("texture service failed to acquire interfaces: %s", error);
	}
	m_remi = interfaces.remi;
}


texture_service::~texture_service()
{
	stop();
}

texture_service::texture_service(const texture_service &rhs)
	: m_started(false), m_stop_requested(false)
{
	m_num_compressed = 0;
	m_num_textures_submitted = 0;
	m_remi = rhs.m_remi;
}

texture_service &texture_service::operator = (const texture_service &rhs)
{
	m_started = false;
	m_stop_requested = false;
	m_num_compressed = 0;
	m_num_textures_submitted = 0;
	m_remi = rhs.m_remi;
	return *this;
}


void texture_service::start()
{
	if (!m_started)
	{
		m_workers.push_back(std::thread([this]()
		{
			load_task();
		}));

		m_workers.push_back(std::thread([this]()
		{
			compression_task();
		}));
		m_started = true;
	}
}

void texture_service::stop()
{
	if (m_started)
	{
		m_stop_requested = true;
		m_load_cv.notify_all();	// wake all workers
		m_compression_cv.notify_all();
		std::for_each(m_workers.begin(), m_workers.end(), [](std::thread &t)
		{
			t.join();
		});
		m_started = false;
		m_stop_requested = false;
	}
}

void texture_service::process_texture(std::shared_ptr<texture> tex)
{
	assert(tex->get_state() == texture::INITIAL);
	m_load_queue_mutex.lock();
		m_num_textures_submitted++;
		tex->lock();
		tex->set_state(texture::WAITING_TO_LOAD);
		tex->unlock();
		m_load_queue.push(tex);
	m_load_queue_mutex.unlock();

	m_load_cv.notify_one();
}

void texture_service::process_all_pending()
{
	std::unique_lock<std::mutex> lock(m_compression_queue_mutex);
	if (m_num_compressed == m_num_textures_submitted)
	{
		return; 
	}

	while (1)
	{
		m_compression_cv.wait(lock);	// on going out, reacquires lock
		if (m_num_compressed == m_num_textures_submitted)
		{
			return; // return here since the submit and compression count match (auto releases lock)
		}
	}
}

void texture_service::load_task()
{
	
	while (!m_stop_requested)
	{							// on going in, releases lock and waits
		std::unique_lock<std::mutex> lock(m_load_queue_mutex);
		if (m_load_queue.empty())
			m_load_cv.wait(lock);	// on going out, reacquires lock
		if (!m_stop_requested && !m_load_queue.empty())
		{
			std::shared_ptr<texture> tex = m_load_queue.front();
			m_load_queue.pop();
			lock.unlock();

			tex->lock();
			tex->set_state(texture::LOADING);
			tex->unlock();

			// do load 'work'
			vr::EVRRenderModelError vre = m_remi->LoadTexture_Async(tex->get_texture_session_id(), &tex->m_texture_map);
			while (!m_stop_requested && vre == vr::VRRenderModelError_Loading)
			{
				plat::sleep_ms(10);
				vre = m_remi->LoadTexture_Async(tex->get_texture_session_id(), &tex->m_texture_map);
			}

			if (!m_stop_requested)
			{
				tex->lock();
				tex->set_width(tex->m_texture_map->unWidth);
				tex->set_height(tex->m_texture_map->unHeight);
				tex->set_load_result(vre);
				if (vre == vr::VRRenderModelError_None)
				{
					// successfully loaded. move to the next state
					tex->set_state(texture::WAITING_TO_COMPRESS);
					tex->unlock();
					enqueue_texture_for_compression(tex);
				}
				else
				{
					tex->unlock();
				}
			}
		}
		else
		{
			lock.unlock();
		}
	}
}

void texture_service::compression_task()
{
	while (!m_stop_requested)
	{							// on going in, releases lock and waits
		std::unique_lock<std::mutex> lock(m_compression_queue_mutex);
		if (m_compression_queue.empty())
			m_compression_cv.wait(lock);	// on going out, reacquires lock
		if (!m_stop_requested && !m_compression_queue.empty())
		{
			// for testing only
			plat::sleep_ms(100);

			// pop the queue and release the queue lock
			std::shared_ptr<texture> tex = m_compression_queue.front();
			m_compression_queue.pop();
			log_printf("compressing session_id %d\n", tex->get_texture_session_id());
			lock.unlock();

			tex->lock();
			assert(tex->get_state() == texture::WAITING_TO_COMPRESS);
			tex->set_state(texture::COMPRESSING);
			tex->unlock();

			// do compression work
			size_t source_size = tex->m_texture_map->unHeight * tex->m_texture_map->unWidth * 4;
			uint32_t crc = crc32buf((char*)tex->m_texture_map->rubTextureMapData, source_size);
			size_t max_dest_size = LZ4_COMPRESSBOUND(source_size);
			tex->get_compressed_buffer().resize(max_dest_size);
			int compressed_size = LZ4_compress_default((const char *)tex->m_texture_map->rubTextureMapData,
									tex->get_compressed_buffer().data(),
									source_size,
									max_dest_size);
			// update texture
			tex->lock();
			tex->set_crc(crc);
			tex->set_state(texture::COMPRESSED);

			// clear uncompressed buffer
			m_remi->FreeTexture(tex->m_texture_map);
			tex->m_texture_map = nullptr;

			// shrink compressed buffer
			tex->get_compressed_buffer().resize(compressed_size);
			tex->get_compressed_buffer().shrink_to_fit();

			m_num_compressed++;
			// done modifying texture
			tex->unlock();

			m_compression_cv.notify_all(); // do a notify in case there is a wait on compression counts (process_all_pending)
		}
		else
		{
			lock.unlock();
		}
	}
}

void texture_service::enqueue_texture_for_compression(std::shared_ptr<texture> tex)
{
	m_compression_queue_mutex.lock();
	tex->lock();
	assert(tex->get_state() == texture::WAITING_TO_COMPRESS);
	tex->unlock();
	m_compression_queue.push(tex);
	m_compression_queue_mutex.unlock();
	m_compression_cv.notify_all();
}

