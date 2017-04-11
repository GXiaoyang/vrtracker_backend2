// TextureIndexer
// * Track texture indexes and contents
#pragma once

#include "vr_types.h"
#include "vr_wrappers_common.h"
#include <queue>

struct Texture
{
public:
	Texture()
		: m_state(INITIAL)
	{
	}

	Texture(int texture_session_id)
		:
		m_state(INITIAL),
		m_texture_session_id(texture_session_id)
	{}

	enum texture_state
	{
		INITIAL,
		LOADING,
		LOADED,
		COMPRESSING,
		COMPRESSED
	};
	enum Compression
	{
		NONE,
		LZ4
	};
	
	texture_state get_state() const { return m_state; }
private:
	Compression compression;
	texture_state m_state;
	int m_texture_session_id;
	int m_width;
	int m_height;
	std::vector<uint8_t> data;
};

struct TextureIndexer
{

	TextureIndexer()
	{
	}

	TextureIndexer(const TextureIndexer &rhs);
	TextureIndexer&operator=(const TextureIndexer &rhs);

	// if textures are being loaded, wait until they are finished loading before writing to stream
	// 
	void WriteToStream(BaseStream &s) const;
	void ReadFromStream(BaseStream &s);

	// if this is a new key,
	//	* start loading this texture
	// if this key already exists
	//  * ignore this texture.  for debugging assert the diffuse_texture_id is the same
	// if the diffuse_texture_id exists
	//  * then it's a duplicate texture.  just add it to the map
	void add_texture(int texture_session_id, const char *render_model_name)
	{
		int internal_id;
		auto iter = m_session2internal_id.find(texture_session_id);
		if (iter == m_session2internal_id.end())
		{
			// new texture id
			internal_id = m_textures.size();
			m_textures.emplace_back(texture_session_id);
			m_load_queue.push(internal_id);
		}
		else
		{
			internal_id = iter->second;
		}
		
		// either way make sure this map is up to date
		std::string s(render_model_name);
		m_render_model_name2id[s] = internal_id;
	}

	// if the texture has been loaded and is in the uncompressed set, allocate memory for it and return it
	// if the texture has been loaded and is in the compressed set, allocate memory for it, uncompress it and return it
	// if the texture status is an error, return the error EVRRenderModelError
	// if the texture is queued or in process of being loaded, force load it now
	vr::EVRRenderModelError get_texture(int texture_session_id, vr::RenderModel_TextureMap_t **)
	{
		auto iter = m_session2internal_id.find(texture_session_id);
		if (iter == m_session2internal_id.end())
		{
			assert(0);	// should never happen
			return vr::VRRenderModelError_InvalidTexture;
		}
		else
		{
			int internal_id = iter->second;
			Texture *t = &m_textures[internal_id];
			switch (t->get_state())
			{
				case Texture::INITIAL:
					assert(0);
					break;
				case Texture::LOADING:
					// wait
					break;
				case Texture::LOADED:
				case Texture::COMPRESSING:
					// return uncompressed version
					break;
				case Texture::COMPRESSED:
					break;
			}
		}
	}

#if 0
	// load thread
	{
		while load_queue
			load it
			when loaded,
			add it to the uncompressed_texture_queue
			add a compression job
	}

	// compression thread
	{
		while compression_queue
		{
			compress it
			add it to the compressed textures
			remove it from the uncompressed textures
		}
	}
#endif

private:
	// one id can be uses by multiple textures
	using internal_id = int;
	using diffuse_id = int;

	// non persistant / 'per session'
	//	used by add_texture and get_texture
	std::unordered_map<diffuse_id, internal_id> m_session2internal_id;
	std::queue<internal_id> m_load_queue;
	std::queue<internal_id> m_compression_queue;
	
	// persistant
	std::unordered_map<std::string, internal_id> m_render_model_name2id;
	std::vector<Texture> m_textures;
};
