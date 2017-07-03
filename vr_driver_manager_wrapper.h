#pragma once

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct DriverManagerWrapper
	{
		explicit DriverManagerWrapper(IVRDriverManager *drivi_in)
			: drivi(drivi_in)
		{}

		inline int GetDriverCount(void)
		{
			return drivi->GetDriverCount();
		}

		inline TMPString<> &GetDriverName(vr::DriverId_t nDriver, TMPString<> *s)
		{
			query_vector_rccount(s, drivi, &IVRDriverManager::GetDriverName, nDriver);
			return *s;
		}

		IVRDriverManager *drivi;
	};
}
