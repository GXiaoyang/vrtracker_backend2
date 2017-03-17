#include "vr_chaperone_setup_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"

/** Returns the number of Quads if the buffer points to null. Otherwise it returns Quads
* into the buffer up to the max specified. */

#define GetWrinkleBounds(param_name)\
bool rc = false; \
CURSOR_SYNC_STATE(param_name, param_name);\
if (param_name->is_present())\
{\
	if (!punCount)\
	{\
		rc = false;\
	}\
	else if (punCount && punCount != 0 && pBuffer == nullptr)\
	{\
		rc = false;\
	}\
	else\
	{\
		rc = util_vector_to_return_buf_rc(&param_name->val, pBuffer, *punCount, punCount);\
	}\
}


VRChaperoneSetupCursor::VRChaperoneSetupCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->state->chaperone_setup_node),
	iter_ref(m_context->iterators->chaperone_setup_node)
{
}

void VRChaperoneSetupCursor::SynchronizeChildVectors()
{
}

bool VRChaperoneSetupCursor::GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ)
{
	LOG_ENTRY("CppStubGetWorkingPlayAreaSize");
	CURSOR_SYNC_STATE(working_play_area_size, working_play_area_size);

	if (working_play_area_size->is_present())
	{
		if (pSizeX)
			*pSizeX = working_play_area_size->val.v[0];
		if (pSizeZ)
			*pSizeZ = working_play_area_size->val.v[1];
	}

	bool rc = working_play_area_size->return_code;
	LOG_EXIT_RC(rc, "CppStubGetWorkingPlayAreaSize");
}

bool VRChaperoneSetupCursor::GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect)
{
	LOG_ENTRY("CppStubGetWorkingPlayAreaRect");
	CURSOR_SYNC_STATE(working_play_area_rect, working_play_area_rect);
	if (working_play_area_rect->is_present())
	{
		if (rect)
		{
			*rect = working_play_area_rect->val;
		}
	}
	bool rc = working_play_area_rect->return_code;
	LOG_EXIT_RC(rc, "CppStubGetWorkingPlayAreaRect");
}

// return semantics with a wrinkle



bool VRChaperoneSetupCursor::GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pBuffer, uint32_t * punCount)
{
	LOG_ENTRY("CppStubGetWorkingCollisionBoundsInfo");

	GetWrinkleBounds(working_collision_bounds_info);

	LOG_EXIT_RC(rc, "CppStubGetWorkingCollisionBoundsInfo");
}

bool VRChaperoneSetupCursor::GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pBuffer, uint32_t * punCount)
{
	LOG_ENTRY("CppStubGetLiveCollisionBoundsInfo");

	GetWrinkleBounds(live_collision_bounds_info);

	LOG_EXIT_RC(rc, "CppStubGetLiveCollisionBoundsInfo");
}

#define GETPOSE(param_name)\
bool rc;\
\
CURSOR_SYNC_STATE(param_name, param_name);\
if (pose_ret && param_name->is_present())\
{\
	*pose_ret = param_name->val;\
	rc = true;\
}\
else\
{\
	rc = false;\
}\

bool VRChaperoneSetupCursor::GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pose_ret)
{
	LOG_ENTRY("CppStubGetWorkingSeatedZeroPoseToRawTrackingPose");
	GETPOSE(working_seated2rawtracking);
	LOG_EXIT_RC(rc, "CppStubGetWorkingSeatedZeroPoseToRawTrackingPose");
}

bool VRChaperoneSetupCursor::GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pose_ret)
{
	LOG_ENTRY("CppStubGetWorkingStandingZeroPoseToRawTrackingPose");
	GETPOSE(working_standing2rawtracking);
	LOG_EXIT_RC(rc, "CppStubGetWorkingStandingZeroPoseToRawTrackingPose");
}

bool VRChaperoneSetupCursor::GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pose_ret)
{
	LOG_ENTRY("CppStubGetLiveSeatedZeroPoseToRawTrackingPose");
	GETPOSE(live_seated2rawtracking);
	LOG_EXIT_RC(rc, "CppStubGetLiveSeatedZeroPoseToRawTrackingPose");
}

bool VRChaperoneSetupCursor::GetLiveCollisionBoundsTagsInfo(uint8_t * pBuffer, uint32_t * punCount)
{
	LOG_ENTRY("CppStubGetLiveCollisionBoundsTagsInfo");
	GetWrinkleBounds(live_collision_bounds_tags_info);
	LOG_EXIT_RC(rc, "CppStubGetLiveCollisionBoundsTagsInfo");
}

bool VRChaperoneSetupCursor::GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pBuffer, uint32_t * punCount)
{
	LOG_ENTRY("CppStubGetLivePhysicalBoundsInfo");
	GetWrinkleBounds(live_physical_bounds_info);
	LOG_EXIT_RC(rc, "CppStubGetLivePhysicalBoundsInfo");
}


