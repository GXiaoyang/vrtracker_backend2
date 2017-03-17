#include "vr_resources_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

VRResourcesCursor::VRResourcesCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->state->resources_node),
	iter_ref(m_context->iterators->resources_node)
{
}

void VRResourcesCursor::SynchronizeChildVectors()
{
	iter_ref.resources.resize(state_ref.resources.size());
}

bool VRResourcesCursor::GetIndexForResourceName(const char *pchResourceName, int *index)
{
	bool rc = false;
	for (int i = 0; i < size_as_int(iter_ref.resources.size()); i++)
	{
		CURSOR_SYNC_STATE(name, resources[i].resource_name);
		if (name->is_present() && util_char_vector_cmp(pchResourceName, name->val) == 0)
		{
			*index = i;
			rc = true;
			break;
		}
	}
	return rc;
}

bool VRResourcesCursor::GetIndexForResourceNameAndDirectory(
	const char *pchResourceName,
	const char *pchDirectoryName, int *index)
{
	bool rc = false;
	SynchronizeChildVectors(); // dont erase this since this is a lookup 
	for (int i = 0; i < (int)iter_ref.resources.size(); i++)
	{
		CURSOR_SYNC_STATE(name, resources[i].resource_name);
		CURSOR_SYNC_STATE(directory, resources[i].resource_directory);

		if ((name->is_present() && util_char_vector_cmp(pchResourceName, name->val) == 0) &&
			(directory->is_present() && util_char_vector_cmp(pchDirectoryName, directory->val) == 0))
		{
			*index = i;
			rc = true;
			break;
		}
	}
	return rc;
}

// search based on the full path
bool VRResourcesCursor::GetIndexForFullPath(const char *pchResourceName, int *index)
{
	bool rc = false;
	SynchronizeChildVectors();
	for (int i = 0; i < (int)iter_ref.resources.size(); i++)
	{
		CURSOR_SYNC_STATE(full, resources[i].resource_full_path);

		if ((full->is_present() && util_char_vector_cmp(pchResourceName, full->val) == 0))
		{
			*index = i;
			rc = true;
			break;
		}
	}
	return rc;
}

bool VRResourcesCursor::GetIndexWithFallbacks(const char * pchResourceName, int *index_ret)
{
	bool rc = false;

	if (GetIndexForResourceName(pchResourceName, index_ret))
	{
		rc = true;
	}

	if (rc == false)
	{
		// resource name might have a directory embedded in it: e.g. "icons/banana.txt"
		const char *slash = nullptr;
		slash = strchr(pchResourceName, '/');
		if (!slash)
		{
			slash = strchr(pchResourceName, '\\');
		}
		if (slash)
		{
			std::string dir(pchResourceName, slash - pchResourceName);
			if (GetIndexForResourceNameAndDirectory(slash + 1, dir.c_str(), index_ret))
			{
				rc = true;
			}
		}
	}

	// might be the full path
	if (rc == false)
	{
		rc = GetIndexForFullPath(pchResourceName, index_ret);
	}

	return rc;
}


uint32_t VRResourcesCursor::LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen)
{
	LOG_ENTRY("CursorLoadSharedResource");

	int resource_index;
	uint32_t rc = 0;
	if (GetIndexWithFallbacks(pchResourceName, &resource_index))
	{
		CURSOR_SYNC_STATE(data, resources[resource_index].resource_data);
		if (data->is_present())
		{
			util_vector_to_return_buf_rc(&data->val, (uint8_t*)pchBuffer, unBufferLen, &rc);
		}
	}

	LOG_EXIT_RC(rc, "CursorLoadSharedResource");
}

uint32_t VRResourcesCursor::GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory,
	char * pchPathBuffer, uint32_t unBufferLen)
{
	LOG_ENTRY("CursorGetResourceFullPath");

	int resource_index;
	uint32_t rc = 0;
	if (GetIndexForResourceNameAndDirectory(pchResourceName, pchResourceTypeDirectory, &resource_index))
	{
		CURSOR_SYNC_STATE(data, resources[resource_index].resource_full_path);
		if (data->is_present())
		{
			util_vector_to_return_buf_rc(&data->val, pchPathBuffer, unBufferLen, &rc);
		}
	}

	LOG_EXIT_RC(rc, "CursorGetResourceFullPath");
}
