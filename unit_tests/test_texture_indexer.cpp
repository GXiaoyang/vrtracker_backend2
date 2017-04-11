#include "vr_render_models_wrapper.h"
#include "vr_texture_indexer.h"
#include "capture_test_context.h"
#include <thread>
#include "vr_applications_wrapper.h"

void test_texture_indexer()
{
	capture_test_context context;

	TextureIndexer ti;

	char namebuf[256];
	context.raw_vr_interfaces().remi->GetRenderModelName(0, namebuf, sizeof(namebuf));
	vr::RenderModel_t *model = nullptr;
	vr::EVRRenderModelError ret = context.raw_vr_interfaces().remi->LoadRenderModel_Async(namebuf, &model);
	while (1)
	{
		ret = context.raw_vr_interfaces().remi->LoadRenderModel_Async(namebuf, &model);
		if (ret != vr::VRRenderModelError_Loading)
			break;
		plat::sleep_ms(1);
	}

	if (ret == vr::VRRenderModelError_None)
	{
		ti.add_texture(model->diffuseTextureId, namebuf);
	}

	log_printf("boo\n");
}