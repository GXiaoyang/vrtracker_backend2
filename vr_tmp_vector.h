// vr_tmp_vector
//	specializes tmp_vector in two ways:
//		* commits to the size of the temporary vectors used
//		* provides access to a global pool
//     
#pragma once
#include "vr_constants.h"
#include "tmp_vector.h"

struct vr_tmp_vector_base
{
	static tmp_vector_pool<VRTMPSize> *m_global_pool;
};

template<typename T>
struct vr_tmp_vector : tmp_vector2<T, VRTMPSize>, vr_tmp_vector_base
{
	vr_tmp_vector()
		: tmp_vector2<T, VRTMPSize>(m_global_pool)
	{}
	
	vr_tmp_vector(int)	// int is a flag to use the default constructor/create an empty object (used by vr_empty_vector below)
	{}

	vr_tmp_vector & operator = (vr_tmp_vector &&rhs)
	{
		tmp_vector2<T, VRTMPSize>::operator=(std::move(rhs));
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