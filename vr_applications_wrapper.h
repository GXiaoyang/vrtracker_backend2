#pragma once


#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct ApplicationsWrapper
	{
		explicit ApplicationsWrapper(IVRApplications *appi_in)
			: appi(appi_in)
		{}

		SCALAR_WRAP(IVRApplications, appi, GetApplicationCount);
		SCALAR_WRAP(IVRApplications, appi, GetTransitionState);
		SCALAR_WRAP(IVRApplications, appi, IsQuitUserPromptRequested);
		SCALAR_WRAP(IVRApplications, appi, GetCurrentSceneProcessId);

		SCALAR_WRAP_INDEXED(IVRApplications, appi, bool, IsApplicationInstalled, const char *);
		SCALAR_WRAP_INDEXED(IVRApplications, appi, bool, GetApplicationAutoLaunch, const char *);
		SCALAR_WRAP_INDEXED(IVRApplications, appi, uint32_t, GetApplicationProcessId, const char *);

		inline TMPString<EVRApplicationError> &GetStartingApplication(TMPString<EVRApplicationError> *result)
		{
			query_vector_rcerror(result, appi, &IVRApplications::GetStartingApplication);
			return *result;
		}

		inline TMPString<EVRApplicationError> &GetApplicationKeyByIndex(uint32_t app_index, 
			TMPString<EVRApplicationError> *result)
		{
			query_vector_rcerror(result, appi, &IVRApplications::GetApplicationKeyByIndex, app_index);
			return *result;
		}

		inline TMPString<bool> &GetApplicationSupportedMimeTypes(const char *index, TMPString<bool> *result)
		{
			result->val.data()[0] = 0;
			query_vector_rcerror(result, appi, &IVRApplications::GetApplicationSupportedMimeTypes, index);
			return *result;
		}

		inline TMPString<> &GetApplicationLaunchArguments(uint32_t unHandle, TMPString<> *result)
		{		
			result->val.data()[0] = 0;
			query_vector_rccount(result, appi, &IVRApplications::GetApplicationLaunchArguments, unHandle);
			return *result;
		}

		inline TMPString<> &GetApplicationsThatSupportMimeType(const char *mime_type, TMPString<> *result)
		{
			query_vector_rccount(result, appi, &IVRApplications::GetApplicationsThatSupportMimeType, mime_type);
			return *result;
		}

		inline TMPString<bool> &GetDefaultApplicationForMimeType(const char *mime_type, TMPString<bool> *result)
		{
			query_vector_rcerror(result, appi, &IVRApplications::GetDefaultApplicationForMimeType, mime_type);
			return *result;
		}

		inline Bool<EVRApplicationError> &GetProperty(const char *appkey, EVRApplicationProperty prop,
			Bool<EVRApplicationError> *result)
		{
			result->val = appi->GetApplicationPropertyBool(appkey, prop, &result->return_code);
			return *result;
		}

		inline Uint64<EVRApplicationError> &GetProperty(const char *appkey, EVRApplicationProperty prop,
			Uint64<EVRApplicationError>*result)
		{
			result->val = appi->GetApplicationPropertyUint64(appkey, prop, &result->return_code);
			return *result;
		}

		inline TMPString<EVRApplicationError> &GetStringProperty(const char *appkey, EVRApplicationProperty prop,
			TMPString<EVRApplicationError> *result)
		{
			query_vector_rccount(result, appi, &IVRApplications::GetApplicationPropertyString, appkey, prop);
			return *result;
		}

		IVRApplications *appi;
	};
}
