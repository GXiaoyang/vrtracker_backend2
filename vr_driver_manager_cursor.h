#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRDriverManagerCursor : public VRDriverManagerCppStub
{
	CursorContext *m_context;
	vr_result::driver_manager_state &state_ref;
	vr_result::driver_manager_iterator &iter_ref; 
public:
	explicit VRDriverManagerCursor(CursorContext *context);
	void SynchronizeChildVectors();
	
	uint32_t GetDriverCount() const override;
	uint32_t GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override;
	
};
