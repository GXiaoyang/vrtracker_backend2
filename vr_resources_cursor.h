#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRResourcesCursor : public VRResourcesCppStub
{
	CursorContext *m_context;
	vr_result::resources_state &state_ref;
	vr_result::resources_iterator &iter_ref; 
public:
	explicit VRResourcesCursor(CursorContext *context);
	void SynchronizeChildVectors();
	
	bool GetIndexForResourceName(const char *pchResourceName, int *index);
	bool GetIndexForResourceNameAndDirectory(const char *pchResourceName, const char *pchDirectoryName, int *index);
	bool GetIndexForFullPath(const char *pchResourceName, int *index);
	bool GetIndexWithFallbacks(const char * pchResourceName, int *index);
	uint32_t LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen) override;
	uint32_t GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory, char * pchPathBuffer, uint32_t unBufferLen) override;
};
