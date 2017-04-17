#include <texture_service.h>
#include "lz4.h"

void texture_service::start()
{
	m_workers.push_back(std::thread([this]()
		{
			load_task();
		}));
}

void texture_service::stop()
{
	std::for_each(m_workers.begin(), m_workers.end(), [](std::thread &t)
	{
		t.join();
	});
}

void texture_service::process_texture(std::shared_ptr<texture> tex)
{
	m_load_queue_mutex.lock();
		m_num_textures_submitted++;
		m_load_queue.push(tex);
	m_load_queue_mutex.unlock();

	m_load_cv.notify_one();
}

void texture_service::process_all_pending()
{
	std::unique_lock<std::mutex> lock(m_compression_queue_mutex);
	if (m_num_compressed == m_num_textures_submitted)
	{
		return; // auto releases lock
	}

	while (1)
	{
		m_compression_cv.wait(lock);	// on going out, reacquires lock
		if (m_num_compressed == m_num_textures_submitted)
		{
			return; // auto releases lock
		}
	}
}

void texture_service::load_task()
{
	std::unique_lock<std::mutex> lock(m_load_queue_mutex);
	for (;;)
	{							// on going in, releases lock and waits
		m_load_cv.wait(lock);	// on going out, reacquires lock
		if (!m_load_queue.empty())
		{
			std::shared_ptr<texture> tex = m_load_queue.back();
			struct vr::RenderModel_TextureMap_t *texture_map;
			vr::EVRRenderModelError vre = m_remi->LoadTexture_Async(tex->get_texture_session_id(), &texture_map);
			if (vre != vr::VRRenderModelError_Loading)
			{
				// the loading has completed so remove it from the load queue
				m_load_queue.pop();
				tex->set_load_result(vre);
				if (vre == vr::VRRenderModelError_None)
				{
					// successfully loaded. move to the next state
					tex->set_state(texture::WAITING_TO_COMPRESS);
					enqueue_texture_for_compression(tex);
				}
			}
		}
	}
}

void texture_service::compression_task()
{
	std::unique_lock<std::mutex> lock(m_compression_queue_mutex);
	for (;;)
	{							// on going in, releases lock and waits
		m_compression_cv.wait(lock);	// on going out, reacquires lock
		if (!m_compression_queue.empty())
		{
			std::shared_ptr<texture> tex = m_compression_queue.back();
			m_compression_queue.pop();
			m_compression_queue_mutex.unlock();

			// do compression work
			size_t source_size = tex->get_uncompressed_buffer().size();
			size_t max_dest_size = LZ4_COMPRESSBOUND(source_size);
			tex->get_compressed_buffer().resize(max_dest_size);
			int compressed_size = LZ4_compress_default(tex->get_uncompressed_buffer().data(),
									tex->get_compressed_buffer().data(),
									source_size,
									max_dest_size);
			tex->get_compressed_buffer().resize(compressed_size);

			// update texture
			tex->lock();
			tex->set_state(texture::COMPRESSED);

			// clear uncompressed buffer
			std::vector<char> tmp;
			tex->get_uncompressed_buffer().swap(tmp); 

			// shrink compressed buffer
			tex->get_compressed_buffer().resize(compressed_size);
			tex->get_compressed_buffer().shrink_to_fit();

			m_num_compressed++;
			// done modifying texture
			tex->unlock();

			m_compression_cv.notify_all();
		}
	}
}

void texture_service::enqueue_texture_for_compression(std::shared_ptr<texture> tex)
{
	m_compression_queue_mutex.lock();
	m_compression_queue.push(tex);
	m_compression_queue_mutex.unlock();
	m_compression_cv.notify_one();
}

