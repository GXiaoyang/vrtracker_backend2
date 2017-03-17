#include "vr_settings_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"


#define GET_SETTING(setting_type, settings_field_name) \
SynchronizeChildVectors();\
vr::EVRSettingsError settings_error = vr::VRSettingsError_UnsetSettingHasNoDefault;\
int section_index;\
int subsection_index;\
if (m_settings_indexer->GetIndexes(pchSection, setting_type, pchSettingsKey, &section_index, &subsection_index))\
{\
	auto iter = iter_ref.sections[section_index].settings_field_name[subsection_index];\
	auto state =state_ref.sections[section_index].settings_field_name[subsection_index];\
	update_iter(iter, state, m_context->current_frame);\
\
	settings_error = iter->get_value().return_code;\
		\
	if (iter->get_value().is_present())\
	{\
		rc = iter->get_value().val;\
	}\
}\
if (pError)\
{\
	*pError = settings_error; \
}

VRSettingsCursor::VRSettingsCursor(CursorContext *context)
		:
		m_context(context),
		iter_ref(context->iterators->settings_node),
		state_ref(context->state->settings_node),
		m_settings_indexer(&context->m_resource_keys->GetSettingsIndexer())
{}

void VRSettingsCursor::SynchronizeChildVectors()
{
	if (iter_ref.structure_version == state_ref.structure_version)
		return;
	iter_ref.structure_version = state_ref.structure_version;

	iter_ref.sections.resize(state_ref.sections.size());
	for (int index = 0; index < size_as_int(iter_ref.sections.size()); index++)
	{
		iter_ref.sections[index].bool_settings.resize(state_ref.sections[index].bool_settings.size());
		iter_ref.sections[index].float_settings.resize(state_ref.sections[index].float_settings.size());
		iter_ref.sections[index].int32_settings.resize(state_ref.sections[index].int32_settings.size());
		iter_ref.sections[index].string_settings.resize(state_ref.sections[index].string_settings.size());
	}
}

const char * VRSettingsCursor::GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError)
{
	LOG_ENTRY("CppStubGetSettingsErrorNameFromEnum");
	const char * rc = openvr_string::EVRSettingsErrorToString(eError);
	LOG_EXIT_RC(rc, "CppStubGetSettingsErrorNameFromEnum");
}

bool VRSettingsCursor::GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * pError)
{
	LOG_ENTRY("CppStubGetBool");
	bool rc = false;
	GET_SETTING(SettingsIndexer::SETTING_TYPE_BOOL, bool_settings);
	LOG_EXIT_RC(rc, "CppStubGetBool");
}

int32_t VRSettingsCursor::GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * pError)
{
	LOG_ENTRY("CppStubGetInt32");
	int32_t rc = 0;
	GET_SETTING(SettingsIndexer::SETTING_TYPE_INT32, int32_settings);
	LOG_EXIT_RC(rc, "CppStubGetInt32");
}

float VRSettingsCursor::GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * pError)
{
	LOG_ENTRY("CppStubGetFloat");
	float rc = 0.0f;
	GET_SETTING(SettingsIndexer::SETTING_TYPE_FLOAT, float_settings);
	LOG_EXIT_RC(rc, "CppStubGetFloat");
}

void VRSettingsCursor::GetString(const char * pchSection, const char * pchSettingsKey,
	char * Buffer, uint32_t Len, vr::EVRSettingsError * pError)
{
	LOG_ENTRY("CppStubGetString");

	bool rc = false;
	int section_index;
	int subsection_index;

	SynchronizeChildVectors();
	if (m_settings_indexer->GetIndexes(pchSection, SettingsIndexer::SETTING_TYPE_STRING,
		pchSettingsKey, &section_index, &subsection_index))

	{
		auto iter = iter_ref.sections[section_index].string_settings[subsection_index];
		auto state = state_ref.sections[section_index].string_settings[subsection_index];
		update_iter(iter, state, m_context->current_frame);

		if (pError)
		{
			*pError = iter->get_value().return_code;
		}

		if (iter->get_value().is_present())
		{
			util_vector_to_return_buf_rc(&iter->get_value().val, Buffer, Len, nullptr);
		}

		rc = iter->get_value().is_present();
	}
	LOG_EXIT("CppStubGetString");
}
