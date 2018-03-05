// TextureIndexer
//
// * Assign texture indexes and contents
// * Provide an asynchronous interface since loading textures can take awhile
//
// * Assumptions
//		I assume textures don't change for a given model.  If they did change, then I could fold
//      in some CRC mechanism to avoid having to maintain uncompressed textures to be able to do 
//      diffs.
// References:
//	See the comments in vr_keys for a discussion on rationale for having indexes.  Also vr_keys has 
//  other indexes similar to TextureIndexer.
//
#pragma once
#include "BaseStream.h"
#include "segmented_list.h"
#include <openvr.h>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <texture_service.h>

struct TextureIndexer
{
	TextureIndexer();

	TextureIndexer(const TextureIndexer &rhs);
	TextureIndexer&operator=(const TextureIndexer &rhs);

	// returns true if the texture set matches 
	bool operator == (const TextureIndexer &rhs) const;
	bool operator != (const TextureIndexer &rhs) const;

	// if textures are being loaded, wait until they are finished loading before writing to stream
	// 
	void WriteToStream(BaseStream &s) const;
	void ReadFromStream(BaseStream &s);

	// add_texture: add the following texture to the indexer database
	//
	int add_texture(int texture_session_id, const char *render_model_name);

	// given a texture session id, return the texture map and the return code
	vr::EVRRenderModelError get_texture(int texture_session_id, vr::RenderModel_TextureMap_t **);
	void free_texture(vr::RenderModel_TextureMap_t *);


	void process_all_pending();

	
private:
	// one id can be uses by multiple textures
	using internal_id = int;
	using diffuse_id = int;

	std::mutex m_list_lock;

	// non persistant / 'per session'
	//	used by add_texture and get_texture
	std::unordered_map<diffuse_id, internal_id> m_session2internal_id;
	mutable texture_service m_texture_service;
	
	// persistant
	std::unordered_map<std::string, internal_id> m_render_model_name2id;
	std::vector<std::shared_ptr<texture>> m_textures;	
};
