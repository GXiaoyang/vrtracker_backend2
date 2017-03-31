#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

struct VRExtendedDisplayCursor : public VRExtendedDisplayCppStub
{
	CursorContext *m_context;
	vr_result::extended_display_state &state_ref;
	vr_result::extended_display_iterator &iter_ref;

	explicit VRExtendedDisplayCursor(CursorContext *context);
	void SynchronizeChildVectors();
	void GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
	void GetEyeOutputViewport(vr::EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
};
