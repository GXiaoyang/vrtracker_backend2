#pragma once
#include <openvr.h>
#include <vector>
#include <memory>
#include <mutex>
#include <queue>
#include <atomic>
#include <BaseStream.h>

struct texture
{
public:
	texture()
		:
		m_state(INITIAL)
	{
	}

	texture(int texture_session_id)
		:
		m_state(INITIAL),
		m_texture_session_id(texture_session_id)
	{}

	enum texture_state
	{
		INITIAL,
		LOADING,
		LOAD_FAILED,
		WAITING_TO_COMPRESS,	// in the compression queue
		COMPRESSING,			// actively being compressed
		COMPRESSED
	};

	void lock() { m_lock.lock(); }
	void unlock() { m_lock.unlock(); }

	void WriteCompressedTextureToStream(BaseStream &s)
	{
		assert(m_state == COMPRESSED || m_state == LOAD_FAILED);
		s.write_to_stream(&m_width, sizeof(m_width));
		s.write_to_stream(&m_height, sizeof(m_height));
		s.write_to_stream(&m_load_result, sizeof(m_load_result));
		s.contiguous_container_out_to_stream(m_compressed);
	}

	int get_width() const { return m_width; }
	int get_height() const { return m_height; }

	texture_state get_state() const { return m_state; }
	void set_state(texture_state s) { m_state = s; }
	int get_texture_session_id() const { return m_texture_session_id; }

	void set_load_result(vr::EVRRenderModelError r) { m_load_result = r; }
	vr::EVRRenderModelError get_load_result() const { return m_load_result; }

	std::vector<char> &get_uncompressed_buffer() { return m_uncompressed; }
	std::vector<char> &get_compressed_buffer() { return m_compressed; }

private:
	texture_state m_state;
	vr::EVRRenderModelError m_load_result;
	int m_width;
	int m_height;
	int crc;
	int m_texture_session_id;
	std::vector<char> m_compressed;
	std::vector<char> m_uncompressed;
	std::mutex m_lock;
};

struct texture_service
{
	void start();
	void stop();
	void process_texture(std::shared_ptr<texture> t);
	void process_all_pending();

private:
	void load_task();
	void compression_task();
	void enqueue_texture_for_compression(std::shared_ptr<texture>);

	vr::IVRRenderModels *m_remi;

	std::atomic<int> m_num_compressed;		  
	std::atomic<int> m_num_textures_submitted; 

	std::vector<std::thread> m_workers;

	std::mutex m_load_queue_mutex;
	std::condition_variable m_load_cv;
	std::queue<std::shared_ptr<texture>> m_load_queue;

	std::mutex m_compression_queue_mutex;
	std::condition_variable m_compression_cv;
	std::queue<std::shared_ptr<texture>> m_compression_queue;

};
