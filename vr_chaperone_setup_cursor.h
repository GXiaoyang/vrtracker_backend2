#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRChaperoneSetupCursor : public VRChaperoneSetupCppStub
{
	CursorContext *m_context;
	vr_result::chaperone_setup_state &state_ref;
	vr_result::chaperone_setup_iterator &iter_ref;
public:
	VRChaperoneSetupCursor(CursorContext *context);
	void SynchronizeChildVectors();

	bool GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ) override;
	bool GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect) override;
	bool GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;
	bool GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;
	bool GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) override;
	bool GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose) override;
	bool GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) override;
	bool GetLiveCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t * punTagCount) override;
	bool GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;

};