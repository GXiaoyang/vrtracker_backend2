#include "vr_texture_indexer.h"
#include "lz4.h"

TextureIndexer::TextureIndexer()
{
}

TextureIndexer::TextureIndexer(const TextureIndexer &rhs)
	: 
	m_session2internal_id(rhs.m_session2internal_id),
	m_render_model_name2id(rhs.m_render_model_name2id),
	m_textures(rhs.m_textures)
{
}

TextureIndexer& TextureIndexer::operator=(const TextureIndexer &rhs)
{
	m_session2internal_id = rhs.m_session2internal_id;
	m_render_model_name2id = rhs.m_render_model_name2id;
	m_textures = rhs.m_textures;
	return *this;
}

bool TextureIndexer::operator == (const TextureIndexer &rhs) const
{
	return (m_session2internal_id == rhs.m_session2internal_id) &&
		(m_render_model_name2id == rhs.m_render_model_name2id) &&
		(m_textures == rhs.m_textures);
}

bool TextureIndexer::operator != (const TextureIndexer &rhs) const
{
	return !(*this == rhs);
}

void TextureIndexer::WriteToStream(BaseStream &s) const
{
	m_texture_service.process_all_pending();

	// write out the rendermodel_name -> internal id map
	for (auto iter = m_render_model_name2id.begin();
		iter != m_render_model_name2id.end();
		iter++)
	{
		s.contiguous_container_out_to_stream(iter->first);
		s.write_to_stream(&iter->second, sizeof(iter->second));
	}
	
	// write out the textures
	for (auto iter = m_textures.begin(); iter != m_textures.end(); iter++)
	{
		(*iter)->WriteCompressedTextureToStream(s);
	}
}

void TextureIndexer::ReadFromStream(BaseStream &s)
{

}


// if this is a new texture_session_id,
//	* start loading this texture
int TextureIndexer::add_texture(int texture_session_id, const char *render_model_name)
{
	std::lock_guard<std::mutex> lock(m_list_lock);

	int internal_id;
	auto iter = m_session2internal_id.find(texture_session_id);
	if (iter == m_session2internal_id.end())
	{

		internal_id = m_textures.size();
		m_textures.emplace_back(std::make_shared<texture>(texture_session_id));
		m_texture_service.process_texture(m_textures[internal_id]);
	}
	else
	{
		// already have this texture id in my map, so use it
		internal_id = iter->second;
	}

	// either way make sure this map is up to date
	std::string s(render_model_name);
	m_render_model_name2id[s] = internal_id;

	return internal_id;
}

// if the texture has been loaded and is in the uncompressed set, allocate memory for it and return it
// if the texture has been loaded and is in the compressed set, allocate memory for it, uncompress it and return it
// if the texture status is an error, return the error EVRRenderModelError
// if the texture is queued or in process of being loaded, force load it now
vr::EVRRenderModelError TextureIndexer::get_texture(int texture_session_id, vr::RenderModel_TextureMap_t **map_ret)
{
	vr::EVRRenderModelError rc = vr::VRRenderModelError_None;

	// lock and lookup in list
	std::shared_ptr<texture> ptexture;
	{
		std::lock_guard<std::mutex> lock(m_list_lock);
		auto iter = m_session2internal_id.find(texture_session_id);
		if (iter == m_session2internal_id.end())
		{
			rc = vr::VRRenderModelError_InvalidTexture;
		}
		else
		{
			ptexture = m_textures[iter->second];
		}
	}

	if (vr::VRRenderModelError_None)
	{
		std::lock_guard<texture> lock(*ptexture);
		// if I got a texture, lock it and handle it

		switch (ptexture->get_state())
		{
		case texture::INITIAL:
			assert(0);
			rc = vr::VRRenderModelError_InvalidArg;
			break;
		case texture::LOADING:
			// wait
			rc = vr::VRRenderModelError_Loading;
			break;
		case texture::LOAD_FAILED:
			rc = ptexture->get_load_result();
			break;
		case texture::WAITING_TO_COMPRESS:
		case texture::COMPRESSING:
			// return uncompressed version
			if (map_ret)
			{
				vr::RenderModel_TextureMap_t *tex_map = new vr::RenderModel_TextureMap_t();
				tex_map->unWidth = ptexture->get_width();
				tex_map->unHeight = ptexture->get_height();
				size_t texture_size = tex_map->unWidth*tex_map->unHeight * 4;
				uint8_t *buf = new uint8_t[texture_size];
				memcpy(buf, ptexture->get_uncompressed_buffer().data(), texture_size);
				tex_map->rubTextureMapData = buf;
				*map_ret = tex_map;
			}
			break;
		case texture::COMPRESSED:
			// decompress and return it
			if (map_ret)
			{
				vr::RenderModel_TextureMap_t *tex_map = new vr::RenderModel_TextureMap_t();
				tex_map->unWidth = ptexture->get_width();
				tex_map->unHeight = ptexture->get_height();
				size_t texture_size = tex_map->unWidth*tex_map->unHeight * 4;
				uint8_t *buf = new uint8_t[texture_size];
				LZ4_decompress_fast(ptexture->get_uncompressed_buffer().data(), reinterpret_cast<char*>(buf), texture_size);
				tex_map->rubTextureMapData = buf;
				*map_ret = tex_map;
			}
			break;
		}
	}
	return rc;
}

void TextureIndexer::free_texture(vr::RenderModel_TextureMap_t *m)
{
	if (m != nullptr)
	{
		delete m->rubTextureMapData;
	}
	delete m;
}