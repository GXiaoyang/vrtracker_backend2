#include "vr_render_models_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"
#include "openvr_softcompare.h"
#include "gsl.h"

using namespace vr;

VRRenderModelsCursor::VRRenderModelsCursor(CursorContext *context)
	:
	m_context(context),
	iter_ref(context->get_iterators()->render_models_node),
	state_ref(context->get_state()->render_models_node)
{}

void VRRenderModelsCursor::SynchronizeChildVectors()
{
	if (iter_ref.structure_version == state_ref.structure_version)
		return;

	iter_ref.structure_version = state_ref.structure_version;

	iter_ref.models.resize(state_ref.models.size());

	for (int model_index = 0; model_index < size_as_int(iter_ref.models.size()); model_index++)
	{
		iter_ref.models[model_index].components.resize(state_ref.models[model_index].components.size());
	}
}

bool VRRenderModelsCursor::GetIndexForRenderModelName(const char *pchRenderModelName, int *index)
{
	SynchronizeChildVectors();
	bool rc = false;
	for (int i = 0; i < size_as_int(iter_ref.models.size()); i++)
	{
		if (state_ref.models[i].get_name() == pchRenderModelName)
		{
			*index = i;
			rc = true;
			break;
		}
	}
	return rc;
}

bool VRRenderModelsCursor::GetIndexForRenderModelAndComponent(
	const char *pchRenderModelName, const char *pchComponentName,
	int *rendermodel_index, int *component_index)
{
	bool rc = false;
	int render_model_index;
	if (GetIndexForRenderModelName(pchRenderModelName, &render_model_index))
	{
		for (int i = 0; i < size_as_int(iter_ref.models[render_model_index].components.size()); i++)
		{
			if (state_ref.models[render_model_index].components[i].get_path() == pchComponentName)
			{
				*rendermodel_index = render_model_index;
				*component_index = i;
				rc = true;
				break;
			}
		}
	}
	return rc;
}

vr::EVRRenderModelError VRRenderModelsCursor::LoadRenderModel_Async(const char * pchRenderModelName,
	struct vr::RenderModel_t ** ppRenderModel)
{
	LOG_ENTRY("CppStubLoadRenderModel_Async");

	if (!ppRenderModel)
	{
		return vr::VRRenderModelError_InvalidArg;
	}

	// 1/25/2017 passing in null returns no error and a 0 value
	if (!pchRenderModelName)
	{
		ppRenderModel = nullptr;
		return vr::VRRenderModelError_None;
	}

	vr::EVRRenderModelError rc;
	int index;
	if (GetIndexForRenderModelName(pchRenderModelName, &index))
	{
		// build the return value from vertex and index data
		CURSOR_SYNC_STATE(vertex_data, models[index].vertex_data);
		CURSOR_SYNC_STATE(index_data, models[index].index_data);

		if (vertex_data->is_present())
		{
			vr::RenderModel_t *m = new RenderModel_t;						// allocation to return to apps. caller calls FreeRenderModel
			m->rIndexData = &index_data->val.at(0);
			m->unTriangleCount = (uint32_t)index_data->val.size() / 3;
			m->rVertexData = &vertex_data->val.at(0);
			m->unVertexCount = (uint32_t)vertex_data->val.size();
			m->diffuseTextureId = index + 1000;	// we'll fake out the texture ids as indices
			*ppRenderModel = m;
		}
		rc = vertex_data->return_code;
	}
	else
	{
		// test shows this is what is returned for invalid names
		// (eventually - it returns Loading and then the final result)
		rc = vr::VRRenderModelError_InvalidModel;
	}

	LOG_EXIT_RC(rc, "CppStubLoadRenderModel_Async");
}

void VRRenderModelsCursor::FreeRenderModel(struct vr::RenderModel_t * pRenderModel)
{
	LOG_ENTRY("CppStubFreeRenderModel");

	if (pRenderModel)
	{
		delete pRenderModel;
	}

	LOG_EXIT("CppStubFreeRenderModel");
}

vr::EVRRenderModelError VRRenderModelsCursor::LoadTexture_Async(
	vr::TextureID_t textureId,
	struct vr::RenderModel_TextureMap_t ** ppTexture)
{
	LOG_ENTRY("CppStubLoadTexture_Async");

	if (!ppTexture)
	{
		return vr::VRRenderModelError_InvalidArg;
	}
	TextureIndexer& ti(this->m_context->get_keys()->GetTextureIndexer());
	vr::EVRRenderModelError rc = ti.get_texture(textureId, ppTexture);

	LOG_EXIT_RC(rc, "CppStubLoadTexture_Async");
}

void VRRenderModelsCursor::FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture)
{
	LOG_ENTRY("CppStubFreeTexture");
	
	TextureIndexer& ti(this->m_context->get_keys()->GetTextureIndexer());
	ti.free_texture(pTexture);

	LOG_EXIT("CppStubFreeTexture");
}

vr::EVRRenderModelError VRRenderModelsCursor::LoadTextureD3D11_Async(
	vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D)
{
	LOG_ENTRY("CppStubLoadTextureD3D11_Async");
	assert(0);	// todo
	vr::EVRRenderModelError rc = vr::VRRenderModelError_None;

	LOG_EXIT_RC(rc, "CppStubLoadTextureD3D11_Async");
}

vr::EVRRenderModelError VRRenderModelsCursor::LoadIntoTextureD3D11_Async(
	vr::TextureID_t textureId, void * pDstTexture)
{
	LOG_ENTRY("CppStubLoadIntoTextureD3D11_Async");
	assert(0);	// todo
	static vr::EVRRenderModelError rc;
	LOG_EXIT_RC(rc, "CppStubLoadIntoTextureD3D11_Async");
}

uint32_t VRRenderModelsCursor::GetRenderModelName(
	uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen)
{
	LOG_ENTRY("CppStubGetRenderModelName");

	uint32_t rc = 0;
	SynchronizeChildVectors();
	if (unRenderModelIndex < iter_ref.models.size())
	{
		const auto &name = state_ref.models[unRenderModelIndex].get_name();
		util_vector_to_return_buf_rc(&gsl::make_span(name.c_str(), name.size() + 1), pchRenderModelName, unRenderModelNameLen, &rc);
	}

	LOG_EXIT_RC(rc, "CppStubGetRenderModelName");
}

uint32_t VRRenderModelsCursor::GetRenderModelCount() 
{
	LOG_ENTRY("CppStubGetRenderModelCount");
	SynchronizeChildVectors();
	uint32_t rc = (uint32_t)iter_ref.models.size();
	LOG_EXIT_RC(rc, "CppStubGetRenderModelCount");
}

uint32_t VRRenderModelsCursor::GetComponentCount(const char * pchRenderModelName)
{
	LOG_ENTRY("CppStubGetComponentCount");

	uint32_t rc = 0;
	int index;
	if (GetIndexForRenderModelName(pchRenderModelName, &index))
	{
		rc = (uint32_t)iter_ref.models[index].components.size();
	}

	LOG_EXIT_RC(rc, "CppStubGetComponentCount");
}


uint32_t VRRenderModelsCursor::GetComponentName(
	const char * pchRenderModelName,
	uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen)
{
	LOG_ENTRY("CppStubGetComponentName");

	uint32_t rc = 0;
	int model_index;
	if (GetIndexForRenderModelName(pchRenderModelName, &model_index))
	{
		if (unComponentIndex < iter_ref.models[model_index].components.size())
		{
			const auto &name = state_ref.models[model_index].components[unComponentIndex].get_name();
			util_vector_to_return_buf_rc(&gsl::make_span(name.c_str(), name.size() + 1),  
										pchComponentName, unComponentNameLen, &rc);
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetComponentName");
}

uint64_t VRRenderModelsCursor::GetComponentButtonMask(
	const char * pchRenderModelName,
	const char * pchComponentName)
{
	LOG_ENTRY("CppStubGetComponentButtonMask");

	uint64_t rc = 0;
	int model_index;
	int component_index;
	if (GetIndexForRenderModelAndComponent(pchRenderModelName, pchComponentName, &model_index, &component_index))
	{
		CURSOR_SYNC_STATE(button_mask, models[model_index].components[component_index].button_mask);
		if (button_mask->is_present())
		{
			rc = button_mask->val;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetComponentButtonMask");
}

uint32_t VRRenderModelsCursor::GetComponentRenderModelName(
	const char * pchRenderModelName,
	const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen)
{
	LOG_ENTRY("CppStubGetComponentRenderModelName");

	uint32_t rc = 0;
	int model_index;
	int component_index;
	if (GetIndexForRenderModelAndComponent(pchRenderModelName, pchComponentName, &model_index, &component_index))
	{
		CURSOR_SYNC_STATE(render_model_name, models[model_index].components[component_index].render_model_name);

		if (render_model_name->is_present())
		{
			util_vector_to_return_buf_rc(&render_model_name->val, pchComponentRenderModelName, unComponentRenderModelNameLen, &rc);
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetComponentRenderModelName");
}

// let the render model cursor poke at the system state cursors
#define REMI_SYNC_SYSTEM_STATE(local_name, variable_name) \
auto local_name ## iter = m_context->get_iterators()->system_node.variable_name;\
update_iter(local_name ## iter, \
	m_context->get_state()->system_node.variable_name, \
	m_context->GetCurrentFrame());\
auto & local_name = local_name ## iter;

void VRRenderModelsCursor::GetControllerIndicesMatchingRenderModel(
	const char *pchRenderModelName,
	std::vector<int> *controller_indices)
{
	auto &system_iter = m_context->get_iterators()->system_node;
	int prop_index;

	if (m_context->get_keys()->GetDevicePropertiesIndexer().GetIndexForEnum(
		PropertiesIndexer::PROP_STRING, Prop_RenderModelName_String, &prop_index))
	{
		for (int i = 0; i < size_as_int(system_iter.controllers.size()); i++)
		{
			REMI_SYNC_SYSTEM_STATE(render_model_name, controllers[i].string_props[prop_index]);
			if (util_char_vector_cmp(pchRenderModelName, render_model_name->get_value().val) == 0)
			{
				controller_indices->push_back(i);
			}
		}
	}
}

bool VRRenderModelsCursor::GetComponentState(
	const char * pchRenderModelName,
	const char * pchComponentName,
	const vr::VRControllerState_t * pControllerState,					// input
	const struct vr::RenderModel_ControllerMode_State_t * pState,		// input
	struct vr::RenderModel_ComponentState_t * pComponentState)          // return value
{
	LOG_ENTRY("CppStubGetComponentState");
	bool rc = false;
	SynchronizeChildVectors();
	if (pchRenderModelName && pchComponentName && pControllerState && pState && pComponentState)
	{
		// TODO(sean) / Review the following after its up and running and think if there is a better way
		//
		// GetComponentState is close to a pure function. its parameteized by, controller state,
		// and pstate.  the unpure part is that it depends on the component.  Because of the pstate and controller state
		// parameters it is not possible to capture all possible return values.
		// 
		// The 'reasonable' thing that is captured is the last active controller
		// states.  Because that is likely to be of interest.
		//
		// So currently the behaviour is to find all the controllers with this model and component name
		// and choose the one with the closest controller state
		//  

		std::vector<int> controller_indices;
		GetControllerIndicesMatchingRenderModel(pchRenderModelName, &controller_indices);

		if (controller_indices.size() > 0)
		{
			int best_controller_index = controller_indices[0];
			if (controller_indices.size() > 1)
			{
				int best_score = std::numeric_limits<int>::max();
				for (int i = 0; i < size_as_int(controller_indices.size()) && best_score != 0; i++)
				{
					int controller_index = controller_indices[i];
					REMI_SYNC_SYSTEM_STATE(controller_state, controllers[controller_index].controller_state);
					if (controller_state->get_value().is_present())
					{
						int cur_score = softcompare_controllerstates(pControllerState, &controller_state->get_value().val);
						if (cur_score < best_score)
						{
							best_controller_index = controller_index;
							best_score = cur_score;
						}
					}
				}
			}
			// ok now got controller, return the captured component state	
			// what's the component index again?

			int render_model_index;
			int component_index;

			if (GetIndexForRenderModelAndComponent(pchRenderModelName, pchComponentName,
				&render_model_index, &component_index))
			{
				if (pState->bScrollWheelVisible)
				{
					REMI_SYNC_SYSTEM_STATE(transforms, controllers[best_controller_index].components[component_index].transforms_scroll_wheel);
					if (transforms->get_value().is_present())
					{
						*pComponentState = transforms->get_value().val;
						rc = true;
					}
				}
				else
				{
					REMI_SYNC_SYSTEM_STATE(transforms, controllers[best_controller_index].components[component_index].transforms);
					if (transforms->get_value().is_present())
					{
						*pComponentState = transforms->get_value().val;
						rc = true;
					}
				}
			}
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetComponentState");
}

bool VRRenderModelsCursor::RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName)
{
	LOG_ENTRY("CppStubRenderModelHasComponent");

	bool rc = false;
	int model_index;
	int component_index;
	if (GetIndexForRenderModelAndComponent(pchRenderModelName, pchComponentName, &model_index, &component_index))
	{
		rc = true;
	}

	LOG_EXIT_RC(rc, "CppStubRenderModelHasComponent");
}

uint32_t VRRenderModelsCursor::GetRenderModelThumbnailURL(
	const char * pchRenderModelName,
	char * pchThumbnailURL,
	uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError)
{
	LOG_ENTRY("CppStubGetRenderModelThumbnailURL");

	uint32_t rc = 1;	// 1/26/2017 - default is to return a 1 for empty string
	if (unThumbnailURLLen > 0)
	{
		pchThumbnailURL[0] = '\0';
	}
	int model_index;
	if (GetIndexForRenderModelName(pchRenderModelName, &model_index))
	{
		CURSOR_SYNC_STATE(url, models[model_index].thumbnail_url);
		if (url->is_present())
		{
			util_vector_to_return_buf_rc(&url->val, pchThumbnailURL, unThumbnailURLLen, &rc);
		}
		if (peError)
		{
			*peError = url->return_code;
		}
	}
	else
	{
		if (peError)
		{
			*peError = VRRenderModelError_InvalidModel;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetRenderModelThumbnailURL");
}

uint32_t VRRenderModelsCursor::GetRenderModelOriginalPath(
	const char * pchRenderModelName,
	char * pchOriginalPath,
	uint32_t unOriginalPathLen,
	vr::EVRRenderModelError * peError)
{
	LOG_ENTRY("CppStubGetRenderModelOriginalPath");

	uint32_t rc = 0;
	int model_index;
	if (GetIndexForRenderModelName(pchRenderModelName, &model_index))
	{
		CURSOR_SYNC_STATE(original_path, models[model_index].original_path);
		if (original_path->is_present())
		{
			util_vector_to_return_buf_rc(&original_path->val, pchOriginalPath, unOriginalPathLen, &rc);
		}
		if (peError)
		{
			*peError = original_path->return_code;
		}
	}
	else
	{
		if (peError)
		{
			*peError = VRRenderModelError_InvalidModel;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetRenderModelOriginalPath");
}

const char * VRRenderModelsCursor::GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error)
{
	LOG_ENTRY("CppStubGetRenderModelErrorNameFromEnum");
	const char * rc = openvr_string::EVRRenderModelErrorToString(error);
	LOG_EXIT_RC(rc, "CppStubGetRenderModelErrorNameFromEnum");
}

