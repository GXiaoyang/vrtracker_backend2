#pragma once

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct ChaperoneSetupWrapper
	{
		ChaperoneSetupWrapper(IVRChaperoneSetup *chapsi_in)
			: chapsi(chapsi_in)
		{}

		inline HmdVector2<bool> GetWorkingPlayAreaSize()
		{
			HmdVector2<bool> result;
			result.return_code = chapsi->GetWorkingPlayAreaSize(&result.val.v[0], &result.val.v[1]);
			return result;
		}

		inline HmdQuad<bool> GetWorkingPlayAreaRect()
		{
			HmdQuad<bool> result;
			result.return_code = chapsi->GetWorkingPlayAreaRect(&result.val);
			return result;
		}

		inline TMPHmdQuadString<bool> &GetWorkingCollisionBoundsInfo(TMPHmdQuadString<bool> *result)
		{
			query_vector_rcbool_wrinkle_form(result, chapsi, &vr::IVRChaperoneSetup::GetWorkingCollisionBoundsInfo);
			return *result;
		}

		inline TMPHmdQuadString<bool> &GetLiveCollisionBoundsInfo(TMPHmdQuadString<bool> *result)
		{
			query_vector_rcbool_wrinkle_form(result, chapsi, &vr::IVRChaperoneSetup::GetLiveCollisionBoundsInfo);
			return *result;
		}

		inline HmdMatrix34<bool> GetWorkingSeatedZeroPoseToRawTrackingPose()
		{
			HmdMatrix34<bool> result;
			result.return_code = chapsi->GetWorkingSeatedZeroPoseToRawTrackingPose(&result.val);
			return result;
		}

		inline HmdMatrix34<bool> GetWorkingStandingZeroPoseToRawTrackingPose()
		{
			HmdMatrix34<bool> result;
			result.return_code = chapsi->GetWorkingStandingZeroPoseToRawTrackingPose(&result.val);
			return result;
		}

		inline TMPUint8String<bool> &GetLiveCollisionBoundsTagsInfo(TMPUint8String<bool> *result )
		{
			query_vector_rcbool_wrinkle_form(result, chapsi, &vr::IVRChaperoneSetup::GetLiveCollisionBoundsTagsInfo);
			return *result;
		}

		inline HmdMatrix34<bool> GetLiveSeatedZeroPoseToRawTrackingPose()
		{
			HmdMatrix34<bool> result;
			result.return_code = chapsi->GetLiveSeatedZeroPoseToRawTrackingPose(&result.val);
			return result;
		}

		inline TMPHmdQuadString<bool> &GetLivePhysicalBoundsInfo(TMPHmdQuadString<bool> *result)
		{
			query_vector_rcbool_wrinkle_form(result, chapsi, &vr::IVRChaperoneSetup::GetLivePhysicalBoundsInfo);
			return *result;
		}

		IVRChaperoneSetup *chapsi;
	};
	
}
