#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

struct VRApplicationsCursor : public VRApplicationsCppStub
{
	CursorContext *m_context;
	vr_result::applications_state &state_ref;
	vr_result::applications_iterator &iter_ref;

	ApplicationsIndexer *m_applications_indexer;
	ApplicationsPropertiesIndexer *m_property_indexer;

	VRApplicationsCursor(CursorContext *context);
	void SynchronizeChildVectors();
	bool IsApplicationInstalled(const char * pchAppKey) override;
	uint32_t GetApplicationCount() override;
	vr::EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
	vr::EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
	uint32_t GetApplicationProcessId(const char * pchAppKey) override;
	const char * GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error) override;
	uint32_t GetApplicationPropertyString(const char * pchAppKey, vr::EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, vr::EVRApplicationError * peError) override;
	bool GetApplicationPropertyBool(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) override;
	uint64_t GetApplicationPropertyUint64(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) override;
	bool GetApplicationAutoLaunch(const char * pchAppKey) override;
	bool GetDefaultApplicationForMimeType(const char * pchMimeType, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
	bool GetApplicationSupportedMimeTypes(const char * pchAppKey, char * pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override;
	uint32_t GetApplicationsThatSupportMimeType(const char * pchMimeType, char * pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override;
	uint32_t GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs) override;
	vr::EVRApplicationError GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
	vr::EVRApplicationTransitionState GetTransitionState() override;
	const char * GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state) override;
	bool IsQuitUserPromptRequested() override;
	uint32_t GetCurrentSceneProcessId() override;

	bool GetInternalIndexForAppKey(const char *pchKey, int *index);

	bool lookup_app_and_property_index(
		const char * pchAppKey, vr::EVRApplicationProperty prop_enum,
		PropertiesIndexer::PropertySettingType setting_type, vr::EVRApplicationError * pError,
		int *app_index, int *prop_index);
};
