#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

struct VRChaperoneCursor : public VRChaperoneCppStub
{
	CursorContext *m_context;
	vr_result::chaperone_state &state_ref;
	vr_result::chaperone_iterator &iter_ref;

	explicit VRChaperoneCursor(CursorContext *context);
	void SynchronizeChildVectors();

	vr::ChaperoneCalibrationState GetCalibrationState() override;
	bool GetPlayAreaSize(float * pSizeX, float * pSizeZ) override;
	bool GetPlayAreaRect(struct vr::HmdQuad_t * rect) override;
	bool AreBoundsVisible() override;
	void GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor) override;
};
