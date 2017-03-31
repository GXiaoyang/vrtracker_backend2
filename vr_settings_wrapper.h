#pragma once


#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct SettingsWrapper
	{
		explicit SettingsWrapper(IVRSettings *setti_in)
			: setti(setti_in)
		{}

		inline void GetSetting(const char *pchSection, const char *pchSettingsKey, Bool<EVRSettingsError> *result)
		{
			result->val = setti->GetBool(pchSection, pchSettingsKey, &result->return_code);
		}

		inline void GetSetting(const char *pchSection, const char *pchSettingsKey, Float<EVRSettingsError> *result)
		{
			result->val = setti->GetFloat(pchSection, pchSettingsKey, &result->return_code);
		}

		inline void GetSetting(const char *pchSection, const char *pchSettingsKey, Int32<EVRSettingsError> *result)
		{
			result->val = setti->GetInt32(pchSection, pchSettingsKey, &result->return_code);
		}

		inline void GetStringSetting(const char *pchSection, const char *pchSettingsKey,
			TMPString<EVRSettingsError> *result)
		{
			query_vector_rcvoid(result, setti, &IVRSettings::GetString, pchSection, pchSettingsKey);
		}

		IVRSettings *setti;
	};
}
