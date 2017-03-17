#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRRenderModelsCursor : public VRRenderModelsCppStub
{
public:
	CursorContext *m_context;
	vr_result::render_models_state &state_ref;
	vr_result::render_models_iterator &iter_ref;

	VRRenderModelsCursor(CursorContext *context);
	void SynchronizeChildVectors();

	vr::EVRRenderModelError LoadRenderModel_Async(
		const char * pchRenderModelName,
		struct vr::RenderModel_t ** ppRenderModel) override;

	void FreeRenderModel(struct vr::RenderModel_t * pRenderModel) override;

	vr::EVRRenderModelError LoadTexture_Async(vr::TextureID_t textureId,
		struct vr::RenderModel_TextureMap_t ** ppTexture) override;

	void FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture) override;

	// TODO?
	vr::EVRRenderModelError LoadTextureD3D11_Async(vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D) override;
	vr::EVRRenderModelError LoadIntoTextureD3D11_Async(vr::TextureID_t textureId, void * pDstTexture) override;

	uint32_t GetRenderModelName(uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen) override;
	uint32_t GetRenderModelCount() override;
	uint32_t GetComponentCount(const char * pchRenderModelName) override;
	uint32_t GetComponentName(const char * pchRenderModelName, uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen) override;
	uint64_t GetComponentButtonMask(const char * pchRenderModelName, const char * pchComponentName) override;
	uint32_t GetComponentRenderModelName(const char * pchRenderModelName, const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override;
	bool GetComponentState(const char * pchRenderModelName, const char * pchComponentName, const vr::VRControllerState_t * pControllerState, const struct vr::RenderModel_ControllerMode_State_t * pState, struct vr::RenderModel_ComponentState_t * pComponentState) override;
	bool RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName) override;
	uint32_t GetRenderModelThumbnailURL(const char * pchRenderModelName, char * pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError) override;
	uint32_t GetRenderModelOriginalPath(const char * pchRenderModelName, char * pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError * peError) override;
	const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override;

	bool GetIndexForRenderModelName(const char *pchRenderModelName, int *index);

	bool GetIndexForRenderModelAndComponent(const char *pchRenderModelName, const char *pchComponentName,
		int *rendermodel_index, int *component_index);

	void GetControllerIndicesMatchingRenderModel(const char *pchRenderModelName, std::vector<int> *indices);
};
