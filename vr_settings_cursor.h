#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRSettingsCursor : public VRSettingsCppStub
{
	CursorContext *m_context;
	vr_result::settings_state &state_ref;
	vr_result::settings_iterator &iter_ref;
	SettingsIndexer *m_settings_indexer;

public:
	explicit VRSettingsCursor(CursorContext *context);
	void SynchronizeChildVectors();
	const char * GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError) override;
	bool GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
	int32_t GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
	float GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
	void GetString(const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, vr::EVRSettingsError * peError) override;
	bool GetIndexForSection(const char * pchSection, int *index);
};

