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
	
	vr_tmp_vector(int)	// int is a flag to indicate not to create from a pool
	{}

	vr_tmp_vector & operator = (vr_tmp_vector &&rhs)
	{
		tmp_vector<T, VRFinalAllocatorType, VRTMPSize>::operator=(std::move(rhs));
		return *this;
	}

	vr_tmp_vector &operator =(const vr_tmp_vector &rhs) = delete;

};

//
// vr_empty_vector allows a a temporary to be created without memory allocated
//
// the assignment operator will allow it to then be promoted to something with memory allocated
// 
template<typename T>
struct vr_empty_vector : vr_tmp_vector<T>
{
	vr_empty_vector()
		: vr_tmp_vector<T>(0)
	{}

	vr_empty_vector & operator = (vr_tmp_vector<T> &&rhs)
	{
		vr_tmp_vector<T>::operator=(std::move(rhs));
		return *this;
	}
};