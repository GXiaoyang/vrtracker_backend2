#include "vr_render_models_wrapper.h"
#include "vr_texture_indexer.h"
#include "capture_test_context.h"
#include <thread>
#include "vr_applications_wrapper.h"

void test_loading_render_model_and_texture(int index, vr::IVRRenderModels *remi, TextureIndexer &ti)
{
	//
	// test loading the first rendermodel and texture
	//
	
	char namebuf[256];
	remi->GetRenderModelName(index, namebuf, sizeof(namebuf));
	log_printf("testing texture indexer behaviour\n");
	log_printf("loading and indexing of rendermodel: %d %s\n", index, namebuf);
	vr::RenderModel_t *model = nullptr;
	vr::EVRRenderModelError ret = remi->LoadRenderModel_Async(namebuf, &model);
	while (1)
	{
		ret = remi->LoadRenderModel_Async(namebuf, &model);
		if (ret != vr::VRRenderModelError_Loading)
			break;
		plat::sleep_ms(1);
	}

	if (ret == vr::VRRenderModelError_None)
	{
		int texture_index = ti.add_texture(model->diffuseTextureId, namebuf);
		vr::EVRRenderModelError rc;
		vr::RenderModel_TextureMap_t *map1;
		rc = ti.get_texture(texture_index, &map1);
		while (rc == vr::VRRenderModelError_Loading)
		{
			plat::sleep_ms(20);
			rc = ti.get_texture(texture_index, &map1);
		}
		assert(rc == vr::VRRenderModelError_None);
		assert(map1);
		

		//
		// force all pending textures to be processed/compressed to test loading a compressed texture
		//
		ti.process_all_pending();
		vr::RenderModel_TextureMap_t *map2;
		rc = ti.get_texture(texture_index, &map2);
		while (rc == vr::VRRenderModelError_Loading)
		{
			plat::sleep_ms(20);
			rc = ti.get_texture(texture_index, &map2);
		}
		assert(rc == vr::VRRenderModelError_None);
		assert(map2);
		
		assert(map1->unHeight == map2->unHeight);
		assert(map1->unWidth == map2->unWidth);
		assert(memcmp(map1->rubTextureMapData, map2->rubTextureMapData, 4 * map1->unHeight * map2->unWidth) == 0);

		ti.free_texture(map1);
		ti.free_texture(map2);
	}
}

void test_texture_indexer()
{
	capture_test_context context;
	TextureIndexer& ti(context.get_capture().m_keys.GetTextureIndexer());
	vr::IVRRenderModels *remi = context.raw_vr_interfaces().remi;

	for (int i = 0; i < size_as_int(remi->GetRenderModelCount()); i++)
	{
		test_loading_render_model_and_texture(i, remi, ti);
	}


	log_printf("boo\n");
}