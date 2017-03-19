#pragma once
#include "vr_types.h"

#define SCALAR_WRAP(handle_type, handle, function) \
	inline Result<decltype(((handle_type*)nullptr)->function()),NoReturnCode> function() \
{\
	return make_result(handle->function());\
}

#define SCALAR_RESULT_WRAP_INDEXED(handle_type, handle, error_type, function, index_type, return_type)\
	inline Result<return_type, error_type> function(index_type index) \
	{ \
		Result<return_type,error_type> rc;\
		rc.return_code = handle->function(index, &rc.val);\
		return rc; \
	}

// construct a scalar only wrapper function indexed by index_type
#define SCALAR_WRAP_INDEXED(handle_type, handle, return_type, function, index_type)\
	inline Result<return_type,NoReturnCode> function(index_type index) \
	{ \
		return make_result(handle->function(index)); \
	}

namespace vr_result
{

	//		             Inputs: 0,1
	//		ERROR DETAILS PARAM: 0
	//		        RETURN CODE: count
	template<typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_vector_rccount(TMPString<> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->val.resize((ifh->*function_ptr)(params..., result->val.data(), result->val.max_size()));
		assert(result->val.size() < result->val.max_size());
	}


	//		             Inputs: 1,2
	//		ERROR DETAILS PARAM: 1
	//		        RETURN CODE: count
	template<typename ReturnType, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	inline void query_vector_rccount(Result<TMPStringVectorOnly, ReturnType> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->val.resize((ifh->*function_ptr)(params..., result->val.data(), result->val.max_size(), &result->return_code));
		assert(result->val.size() < result->val.max_size());
	}

	template<typename ElementType, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	inline void query_vector_zero_means_not_present(Result<ElementType, bool> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->val.resize((ifh->*function_ptr)(params..., result->val.data(), result->val.max_size()));
		result->return_code = result->val.size() != 0;
		assert(result->val.size() < result->val.max_size());
	}

	// virtual EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) = 0;
	//
	// FunctionPtr:
	//		 ERROR DETAILS: 0
	//		   RETURN CODE: Error
	template<typename ReturnType, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_vector_rcerror(Result<TMPStringVectorOnly, ReturnType> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->return_code = (ifh->*function_ptr)(params..., result->val.data(), result->val.max_size());
		result->val.resize(strlen(result->val.data()) + 1);
	}

	// getString returns void
	//virtual void GetString(
	//	const char *pchSection,
	//	const char *pchSettingsKey,
	//	uint32_t unValueLen,
	//	EVRSettingsError *peError = nullptr) = 0;
	template<typename ReturnType, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_vector_rcvoid(Result<TMPStringVectorOnly, ReturnType> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		(ifh->*function_ptr)(params..., result->val.data(), result->val.max_size(), &result->return_code);
		result->val.resize(strlen(result->val.data()) + 1); 
	}

	// rccount with a wrinkle: the count is a pointer - so you can call it as a query interface
	// virtual bool GetWorkingCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) = 0;
	// virtual bool GetLiveCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) = 0;
	// virtual bool GetLiveCollisionBoundsTagsInfo(VR_OUT_ARRAY_COUNT(punTagCount) uint8_t *pTagsBuffer, uint32_t *punTagCount) = 0;
	// virtual bool ExportLiveToBuffer(VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength) = 0;
	template<typename T, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_vector_rcbool_wrinkle_form(Result<T, bool> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		// ask how big it is
		uint32_t instance_count = 0;  // it is important that this is set to zero
		/*bool rc =*/ (ifh->*function_ptr)(params..., nullptr, &instance_count);
		{
			// I don't expect the following assert to be hit, since the tmp buffer is currently 16Kb
			// however put it here in case I need to handle this case
			assert(instance_count < result->val.max_size());
			uint32_t count = instance_count;
			bool rc = (ifh->*function_ptr)(params..., result->val.data(), &count);
			result->return_code = rc;
			result->val.resize(count);
		}
	}


#if 0

	template<typename T, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	inline void query_vector_zero_means_not_present(vector_result<T, bool> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->count = (ifh->*function_ptr)(params..., result->s.buf(), result->s.max_count());
		result->result_code = result->count != 0;
		assert(result->count < (int)result->s.max_count());
	}



	//



	// returns void and takes no params ... guaranteed to work
	// virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow,
	// VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) = 0;
	template<typename T, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_vector_rcvoid(vector_result<T> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		(ifh->*function_ptr)(params..., result->s.buf(), result->s.max_count());
		result->count = (int)strlen(result->s.buf()) + 1; // gross because it's assuming char  and I want a null byte
	}

	// scalar query
	// virtual EVROverlayError GetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder) = 0;
	template<typename T, typename ResultType, typename InterfaceHandle, typename FunctionPtr, typename ...Params>
	void query_scalar_rcerror(scalar_result<T, ResultType> *result, InterfaceHandle *ifh, FunctionPtr function_ptr, Params... params)
	{
		result->result_code = (ifh->*function_ptr)(params..., &result->val);
	}
#endif

}