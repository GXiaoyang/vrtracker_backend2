#pragma once

#include "slab_allocator.h"
#include <openvr.h>

const size_t VRTMPSize = vr::k_unMaxPropertyStringSize;
using VRFinalAllocatorType = slab_allocator<char>;

template <typename T>
using VRAllocatorTemplate = slab_allocator<T>;


#include "result.h"
template <>
struct ValidReturnCode<vr::ETrackedPropertyError>
{
	static const vr::ETrackedPropertyError return_code = vr::TrackedProp_Success;
};

template <>
struct ValidReturnCode<vr::EVRApplicationError>
{
	static const vr::EVRApplicationError return_code = vr::VRApplicationError_None;
};

template <>
struct ValidReturnCode<vr::EVROverlayError>
{
	static const vr::EVROverlayError return_code = vr::VROverlayError_None;
};

template <>
struct ValidReturnCode<vr::EVRSettingsError>
{
	static const vr::EVRSettingsError return_code = vr::VRSettingsError_None;
};

template <>
struct ValidReturnCode<bool>
{
	static const bool return_code = true;
};

