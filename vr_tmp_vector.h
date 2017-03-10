// vr_tmp_vector
//	does two types of thing
//
// Commits to types
//		* commits to the size of the temporary vectors used
//      * commits to the final storage type
// Provides access to a global pool
//     
#pragma once
#include "vr_constants.h"
#include "tmp_vector.h"

struct vr_tmp_vector_base
{
	static tmp_vector_pool<VRTMPSize> *m_global_pool;
};

template<typename T>
struct vr_tmp_vector : tmp_vector<T, VRFinalAllocatorType, VRTMPSize>, vr_tmp_vector_base
{
	vr_tmp_vector()
		: tmp_vector<T, VRFinalAllocatorType, VRTMPSize>(m_global_pool, VRFinalAllocatorType())
	{}
	
};
