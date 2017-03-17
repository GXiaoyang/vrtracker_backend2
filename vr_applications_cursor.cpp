#include "vr_applications_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

using namespace vr;

#define ITER_NAME(local_name) local_name ## _iter
#define SYNC_APP_STATE(local_name, variable_name) \
SynchronizeChildVectors();\
auto ITER_NAME(local_name) = iter_ref.variable_name;\
update_iter(ITER_NAME(local_name),\
	state_ref.variable_name,\
	m_context->current_frame);\
auto *local_name = &ITER_NAME(local_name)->get_value();

#define MIME_LOOKUP(field_name)\
bool rc; \
int index;\
if (m_context->m_resource_keys->GetMimeTypesIndexer().GetIndexForMimeType(pchMimeType, &index))\
{\
	SYNC_APP_STATE(default_application, mime_types[index].default_application);\
	if (default_application->is_present())\
	{\
		if (util_vector_to_return_buf_rc(&default_application->val, Buffer, Len, nullptr))\
		{\
			rc = true;\
		}\
	}\
}\
rc = false;



VRApplicationsCursor::VRApplicationsCursor(CursorContext *context)
		:
		m_context(context),
		state_ref(m_context->state->applications_node),
		iter_ref(m_context->iterators->applications_node),
		m_applications_indexer(&m_context->m_resource_keys->GetApplicationsIndexer()),
		m_property_indexer(&m_context->m_resource_keys->GetApplicationsPropertiesIndexer())
{
	SynchronizeChildVectors();
}

void VRApplicationsCursor::SynchronizeChildVectors()
{
	if (iter_ref.structure_version == state_ref.structure_version)
		return;

	iter_ref.structure_version = state_ref.structure_version;

	iter_ref.applications.resize(state_ref.applications.size());
	iter_ref.mime_types.resize(state_ref.mime_types.size());

	for (int i = 0; i < size_as_int(iter_ref.applications.size()); i++)
	{
		iter_ref.applications[i].string_props.resize(state_ref.applications[i].string_props.size());
		iter_ref.applications[i].bool_props.resize(state_ref.applications[i].bool_props.size());
		iter_ref.applications[i].uint64_props.resize(state_ref.applications[i].uint64_props.size());
	}
}

bool VRApplicationsCursor::GetInternalIndexForAppKey(const char *key, int *index_ret)
{
	int index = m_applications_indexer->get_index_for_key(key);
	if (index != -1)
	{
		*index_ret = index;
		return true;
	}
	return false;
}

bool VRApplicationsCursor::IsApplicationInstalled(const char * pchAppKey)
{
	LOG_ENTRY("CppStubIsApplicationInstalled");
	bool rc = false;
	int index;
	if (GetInternalIndexForAppKey(pchAppKey, &index))
	{
		SYNC_APP_STATE(is_app_installed, applications[index].is_installed);
		if (is_app_installed->is_present() && is_app_installed->val)
		{
			rc = true;
		}
	}
	LOG_EXIT_RC(rc, "CppStubIsApplicationInstalled");
}

uint32_t VRApplicationsCursor::GetApplicationCount()
{
	LOG_ENTRY("CppStubGetApplicationCount");

	SYNC_APP_STATE(active_application_indexes, active_application_indexes);
	uint32_t rc = (uint32_t)active_application_indexes->val.size();
	LOG_EXIT_RC(rc, "CppStubGetApplicationCount");
}

template <typename T>
vr::EVRApplicationError return_app_key(T &app_key, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	vr::EVRApplicationError rc;
	if (app_key->is_present())
	{
		if (util_vector_to_return_buf_rc(&app_key->val, pchAppKeyBuffer, unAppKeyBufferLen, nullptr))
		{
			rc = VRApplicationError_None;
		}
		else
		{
			rc = VRApplicationError_BufferTooSmall;
		}
	}
	else
	{
		rc = vr::VRApplicationError_InvalidApplication;
	}
	return rc;
}

// since caller is asking for unApplicationIndex, need to map from External to internal index
vr::EVRApplicationError VRApplicationsCursor::GetApplicationKeyByIndex(uint32_t unExternalApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetApplicationKeyByIndex");

	vr::EVRApplicationError rc = VRApplicationError_InvalidIndex;

	SYNC_APP_STATE(active_application_indexes, active_application_indexes);		// <-- the answer is in here
	if (unExternalApplicationIndex < active_application_indexes->val.size())
	{
		int internal_index = active_application_indexes->val[unExternalApplicationIndex];
		int count;
		const char *app_key = m_applications_indexer->get_key_for_index(internal_index, &count);
		if (util_char_to_return_buf_rc(app_key, count, pchAppKeyBuffer, unAppKeyBufferLen, nullptr))
		{
			rc = VRApplicationError_None;
		}
		else
		{
			rc = VRApplicationError_BufferTooSmall;
		}
	}
	LOG_EXIT_RC(rc, "CppStubGetApplicationKeyByIndex");
}

vr::EVRApplicationError VRApplicationsCursor::GetApplicationKeyByProcessId(
	uint32_t unProcessId,
	char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetApplicationKeyByProcessId");

	vr::EVRApplicationError rc = VRApplicationError_InvalidParameter;
	for (int i = 0; i < (int)m_context->iterators->applications_node.applications.size(); i++)
	{
		SYNC_APP_STATE(app_key, applications[i].application_key);
		SYNC_APP_STATE(process_id, applications[i].process_id);

		if (process_id->is_present() && process_id->val == unProcessId)
		{
			rc = return_app_key(app_key, pchAppKeyBuffer, unAppKeyBufferLen);
			break;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationKeyByProcessId");
}

uint32_t VRApplicationsCursor::GetApplicationProcessId(const char * pchAppKey)
{
	LOG_ENTRY("CppStubGetApplicationProcessId");

	uint32_t rc = 0;
	int internal_index;
	if (GetInternalIndexForAppKey(pchAppKey, &internal_index))
	{
		SYNC_APP_STATE(process_id, applications[internal_index].process_id);
		if (process_id->is_present())
		{
			rc = process_id->val;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationProcessId");
}

const char * VRApplicationsCursor::GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error)
{
	LOG_ENTRY("CppStubGetApplicationsErrorNameFromEnum");
	const char *rc = openvr_string::EVRApplicationErrorToString(error);
	LOG_EXIT_RC(rc, "CppStubGetApplicationsErrorNameFromEnum");
}

bool VRApplicationsCursor::lookup_app_and_property_index(
	const char * pchAppKey, vr::EVRApplicationProperty prop_enum,
	PropertiesIndexer::PropertySettingType setting_type, vr::EVRApplicationError * pError,
	int *app_index, int *prop_index)
{
	bool rc = false;
	vr::EVRApplicationError app_error = vr::VRApplicationError_None;
	if (GetInternalIndexForAppKey(pchAppKey, app_index))
	{
		if (m_property_indexer->GetIndexForEnum(setting_type, prop_enum, prop_index))
		{
			rc = true;
		}
		else
		{
			app_error = vr::VRApplicationError_UnknownProperty;
		}
	}
	else
	{
		app_error = vr::VRApplicationError_UnknownApplication;
	}
	if (pError)
	{
		*pError = app_error;
	}
	return rc;
}

uint32_t VRApplicationsCursor::GetApplicationPropertyString(
	const char * pchAppKey,
	vr::EVRApplicationProperty prop_enum,
	char * pchValue, uint32_t unBufferSize,
	vr::EVRApplicationError * pError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyString");

	uint32_t rc = 1;	// 2/1/2017 - default is to return a 1 for empty string
	if (unBufferSize > 0)
	{
		pchValue[0] = '\0';
	}

	int app_index;
	int prop_index;
	if (lookup_app_and_property_index(pchAppKey, prop_enum, PropertiesIndexer::PROP_STRING, pError, &app_index, &prop_index))
	{
		SYNC_APP_STATE(string_prop, applications[app_index].string_props[prop_index]);
		util_vector_to_return_buf_rc(&string_prop->val, pchValue, unBufferSize, &rc);
		if (pError)
		{
			*pError = string_prop->return_code;
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyString");
}




bool VRApplicationsCursor::GetApplicationPropertyBool(
	const char * pchAppKey,
	vr::EVRApplicationProperty prop_enum, vr::EVRApplicationError * pError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyBool");
	bool rc = false;
	int app_index;
	int prop_index;
	if (lookup_app_and_property_index(pchAppKey, prop_enum, PropertiesIndexer::PROP_BOOL, pError, &app_index, &prop_index))
	{
		SYNC_APP_STATE(bool_prop, applications[app_index].bool_props[prop_index]);
		rc = bool_prop->val;
		if (pError)
			*pError = bool_prop->return_code;
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyBool");
}

uint64_t VRApplicationsCursor::GetApplicationPropertyUint64(const char * pchAppKey,
	vr::EVRApplicationProperty prop_enum, vr::EVRApplicationError * pError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyUint64");

	uint64_t rc = 0;

	int app_index;
	int prop_index;
	if (lookup_app_and_property_index(pchAppKey, prop_enum, PropertiesIndexer::PROP_UINT64, pError, &app_index, &prop_index))
	{
		SYNC_APP_STATE(uint64_prop, applications[app_index].uint64_props[prop_index]);
		rc = uint64_prop->val;
		if (pError)
			*pError = uint64_prop->return_code;
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyUint64");
}

bool VRApplicationsCursor::GetApplicationAutoLaunch(const char * pchAppKey)
{
	LOG_ENTRY("CppStubGetApplicationAutoLaunch");
	bool rc = false;
	int internal_index;
	if (GetInternalIndexForAppKey(pchAppKey, &internal_index))
	{
		SYNC_APP_STATE(auto_launch, applications[internal_index].auto_launch);
		rc = auto_launch->is_present() && auto_launch->val;
	}
	LOG_EXIT_RC(rc, "CppStubGetApplicationAutoLaunch");
}


bool VRApplicationsCursor::GetDefaultApplicationForMimeType(
	const char * pchMimeType, char * Buffer, uint32_t Len)
{
	LOG_ENTRY("CppStubGetDefaultApplicationForMimeType");
	MIME_LOOKUP(default_application);
	LOG_EXIT_RC(rc, "CppStubGetDefaultApplicationForMimeType");
}

bool VRApplicationsCursor::GetApplicationSupportedMimeTypes(const char * pchAppKey,
	char * Buffer, uint32_t Len)
{
	LOG_ENTRY("CppStubGetApplicationSupportedMimeTypes");

	bool rc = false;
	int internal_index;
	if (GetInternalIndexForAppKey(pchAppKey, &internal_index))
	{
		SYNC_APP_STATE(supported_mime_types, applications[internal_index].supported_mime_types);
		if (supported_mime_types->is_present())
		{
			rc = util_vector_to_return_buf_rc(&supported_mime_types->val, Buffer, Len, nullptr);
		}
	}

	LOG_EXIT_RC(rc, "CppStubGetApplicationSupportedMimeTypes");
}

uint32_t VRApplicationsCursor::GetApplicationsThatSupportMimeType(const char * pchMimeType, char * Buffer, uint32_t Len)
{
	LOG_ENTRY("CppStubGetApplicationsThatSupportMimeType");
	MIME_LOOKUP(applications_that_support_mime_type);
	LOG_EXIT_RC(rc, "CppStubGetApplicationsThatSupportMimeType");
}

uint32_t VRApplicationsCursor::GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs)
{
	LOG_ENTRY("CppStubGetApplicationLaunchArguments");

	// This is a todo because it relies on the args handle that comes from one of theses events:
	// struct VREvent_ApplicationLaunch_t
	//{
	//	uint32_t pid;
	//	uint32_t unArgsHandle;
	//};
	uint32_t rc;
	if (unArgs > 0)
	{
		pchArgs = "";
		rc = 1;
	}
	else
	{
		rc = 1;
	}

	//assert(0);  // todo after i figure out app launch arguments 
	LOG_EXIT_RC(rc, "CppStubGetApplicationLaunchArguments");
}

vr::EVRApplicationError VRApplicationsCursor::GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetStartingApplication");

	vr::EVRApplicationError rc;
	SYNC_APP_STATE(starting_application, starting_application);

	if (starting_application->is_present())
	{
		if (!util_vector_to_return_buf_rc(&starting_application->val, pchAppKeyBuffer, unAppKeyBufferLen, nullptr))
		{
			rc = VRApplicationError_BufferTooSmall;
		}
		else
		{
			rc = starting_application->return_code;
		}
	}
	else
	{
		rc = starting_application->return_code;
	}
	LOG_EXIT_RC(rc, "CppStubGetStartingApplication");
}

vr::EVRApplicationTransitionState VRApplicationsCursor::GetTransitionState()
{
	LOG_ENTRY("CppStubGetTransitionState");
	SYNC_APP_STATE(transition_state, transition_state);
	vr::EVRApplicationTransitionState rc = transition_state->val;
	LOG_EXIT_RC(rc, "CppStubIsQuitUserPromptRequested");
}

const char * VRApplicationsCursor::GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state)
{
	LOG_ENTRY("CppStubGetApplicationsTransitionStateNameFromEnum");
	const char * rc = openvr_string::EVRApplicationTransitionStateToString(state);
	LOG_EXIT_RC(rc, "CppStubGetApplicationsTransitionStateNameFromEnum");
}

bool VRApplicationsCursor::IsQuitUserPromptRequested()
{
	LOG_ENTRY("CppStubIsQuitUserPromptRequested");
	bool rc;
	SYNC_APP_STATE(is_quit_user_prompt, is_quit_user_prompt);
	rc = is_quit_user_prompt->is_present() && is_quit_user_prompt->val;
	LOG_EXIT_RC(rc, "CppStubIsQuitUserPromptRequested");
}

uint32_t VRApplicationsCursor::GetCurrentSceneProcessId()
{
	LOG_ENTRY("CppStubGetCurrentSceneProcessId");
	uint32_t rc;
	SYNC_APP_STATE(current_scene_process_id, current_scene_process_id);
	rc = current_scene_process_id->val;
	LOG_EXIT_RC(rc, "CppStubGetCurrentSceneProcessId");
}
