#pragma once
// vr_wrappers.h - map all vr api calls to and from the types in vr_result

#include "vr_types.h"
#include "vr_wrappers_common.h"
#include "platform.h"

namespace vr_result
{
	using namespace vr;

	struct RenderModelsWrapper
	{
		explicit RenderModelsWrapper(IVRRenderModels *remi_in)
			: remi(remi_in)
		{}

		SCALAR_WRAP(IVRRenderModels, remi, GetRenderModelCount);

		inline TMPString<> &GetRenderModelName(uint32_t unRenderModelIndex, TMPString<> *s)
		{
			query_vector_rccount(s, remi, &IVRRenderModels::GetRenderModelName, unRenderModelIndex);
			return *s;
		}

		inline TMPString<EVRRenderModelError>& GetRenderModelThumbnailURL(const char *render_model_name, TMPString<EVRRenderModelError> *s)
		{
			query_vector_rccount(s, remi, &IVRRenderModels::GetRenderModelThumbnailURL, render_model_name);
			return *s;
		}

		inline TMPString<EVRRenderModelError>& GetRenderModelOriginalPath(const char *render_model_name, TMPString<EVRRenderModelError> *s)
		{
			query_vector_rccount(s, remi, &IVRRenderModels::GetRenderModelOriginalPath, render_model_name);
			return *s;
		}

		inline int GetComponentCount(const char *render_model_name)
		{
			return remi->GetComponentCount(render_model_name);
		}

		inline TMPString<> &GetComponentModelName(
			const char *render_model_name, uint32_t unComponentIndex, TMPString<> *s)
		{
			query_vector_rccount(s, remi, &IVRRenderModels::GetComponentName, render_model_name, unComponentIndex);
			return *s;
		}

		inline Uint64<> GetComponentButtonMask(const char *render_model_name, const char *component_name)
		{
			return make_result(remi->GetComponentButtonMask(render_model_name, component_name));
		}

		inline TMPString<bool> &GetComponentRenderModelName(const char *render_model_name, const char *component_name, TMPString<bool> *s)
		{
			query_vector_zero_means_not_present(s, remi, &IVRRenderModels::GetComponentRenderModelName, render_model_name, component_name);
			return *s;
		}

		inline void GetComponentState(const char *render_model_name, const char *component_name,
			const VRControllerState_t &controller_state,
			bool scroll_wheel_visible,
			RenderModelComponentState<bool> *transforms)
		{
			RenderModel_ControllerMode_State_t controller_mode_state;
			controller_mode_state.bScrollWheelVisible = scroll_wheel_visible;

			bool rc = remi->GetComponentState(
				render_model_name,
				component_name,
				&controller_state,
				&controller_mode_state,
				&transforms->val);

			transforms->return_code = rc;
		}

		EVRRenderModelError LoadRenderModel(
			const char *render_model_name,
			RenderModel_t **pRenderModel,
			RenderModel_TextureMap_t **pTexture)
		{
			EVRRenderModelError error;

			RenderModel_t *model = nullptr;
			RenderModel_TextureMap_t *texture = nullptr;
			while (1)
			{
				error = remi->LoadRenderModel_Async(render_model_name, &model);
				if (error != vr::VRRenderModelError_Loading)
					break;
				plat::sleep_ms(1);
			}
			if (error == VRRenderModelError_None)
			{
				while (1)
				{
					assert(model->diffuseTextureId >= 0); // are there models with no textures?

					error = remi->LoadTexture_Async(model->diffuseTextureId, &texture);
					if (error != vr::VRRenderModelError_Loading)
						break;
					plat::sleep_ms(1);
				}
			}
			if (error == VRRenderModelError_None)
			{
				*pRenderModel = model;
				*pTexture = texture;
			}
			else
			{
				*pRenderModel = nullptr;
				*pTexture = nullptr;
			}
			return error;
		}

		void FreeRenderModel(
			RenderModel_t *pRenderModel,
			RenderModel_TextureMap_t *pTexture)
		{
			remi->FreeRenderModel(pRenderModel);
			remi->FreeTexture(pTexture);
		}

		IVRRenderModels *remi;
	};
}
