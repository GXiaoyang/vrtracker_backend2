#pragma once

#include "slab_allocator.h"
#include <openvr.h>

const size_t VRTMPSize = vr::k_unMaxPropertyStringSize;
using VRFinalAllocatorType = slab_allocator<char>;
using VRAllocator = slab_allocator<char>;

#include "result.h"
template <>
struct ValidReturnCode<vr::ETrackedPropertyError>
{
	static const vr::ETrackedPropertyError return_code = vr::TrackedProp_Success;
};

template <>
struct ValidReturnCode<bool>
{
	static const bool return_code = true;
};

