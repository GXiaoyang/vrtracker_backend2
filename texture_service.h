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

	explicit texture(int texture_session_id)
		:
		m_state(INITIAL),
		m_texture_session_id(texture_session_id)
	{}

	enum texture_state
	{
		INITIAL,
		WAITING_TO_LOAD,
		LOADING,
		LOAD_FAILED,
		WAITING_TO_COMPRESS,	// in the compression queue
		COMPRESSING,			// actively being compressed
		COMPRESSED
	};

	bool operator ==(const texture &rhs) const
	{
		if (m_state != rhs.m_state)
			return false;
		if (m_load_result != rhs.m_load_result)
			return false;
		if (m_state == COMPRESSED)
		{
			return (m_width == rhs.m_width) && (m_height == rhs.m_height) &&
				(m_crc == rhs.m_crc);
		}
		else
		{
			return true;
		}
	}
	
	bool operator != (const texture &rhs) const
	{
		return !(*this == rhs);
	}

	void lock() { m_lock.lock(); }
	void unlock() { m_lock.unlock(); }

	void WriteCompressedTextureToStream(BaseStream &s)
	{
		assert(m_state == COMPRESSED || m_state == LOAD_FAILED);
		s.write_to_stream(&m_load_result, sizeof(m_load_result));
		if (m_load_result == vr::VRRenderModelError_None)
		{
			s.write_to_stream(&m_width, sizeof(m_width));
			s.write_to_stream(&m_height, sizeof(m_height));
			s.write_to_stream(&m_crc, sizeof(m_crc));
			s.contiguous_container_out_to_stream(m_compressed);
		}
	}

	void ReadCompressedTextureFromStream(BaseStream &s)
	{
		s.read_from_stream(&m_load_result, sizeof(m_load_result));
		if (m_load_result == vr::VRRenderModelError_None)
		{
			s.read_from_stream(&m_width, sizeof(m_width));
			s.read_from_stream(&m_height, sizeof(m_height));
			s.read_from_stream(&m_crc, sizeof(m_crc));
			s.contiguous_container_from_stream(m_compressed);
			m_state = COMPRESSED;
		}
		else
		{
			m_state = LOAD_FAILED;
		}
	}

	int get_width() const { return m_width; }
	void set_width(int w) { m_width = w; }

	int get_height() const { return m_height; }
	void set_height(int h) { m_height = h; }

	uint32_t get_crc() 
	{
		assert(m_state == COMPRESSED); // crc is set after compressed.  doesn't have to be, but currently is
		return m_crc; 
	}
	void set_crc(uint32_t crc) { m_crc = crc; }

	texture_state get_state() const { return m_state; }
	void set_state(texture_state s) { m_state = s; }
	int get_texture_session_id() const { return m_texture_session_id; }

	void set_load_result(vr::EVRRenderModelError r) { m_load_result = r; }
	vr::EVRRenderModelError get_load_result() const { return m_load_result; }

	vr::RenderModel_TextureMap_t *m_texture_map;
	std::vector<char> &get_compressed_buffer() { return m_compressed; }

private:
	texture_state m_state;
	int m_texture_session_id;
	vr::EVRRenderModelError m_load_result;
	int m_width;
	int m_height;
	uint32_t m_crc;
	
	std::vector<char> m_compressed;
	std::mutex m_lock;
};

struct texture_service
{
	texture_service();
	~texture_service();

	texture_service(const texture_service &rhs);
	texture_service &operator = (const texture_service &rhs);

	void start();
	void stop();
	void process_texture(std::shared_ptr<texture> t);
	void process_all_pending();

private:
	void load_task();
	void compression_task();
	void enqueue_texture_for_compression(std::shared_ptr<texture>);

	bool m_started;
	volatile bool m_stop_requested;

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
