#pragma once
// vr_types.h	
//		map openvr specific types into types used by the rest of the system
//		* vr_result: hold the bindings between the abstract Result<> types and the openvr ones
//		* etc.

#include "vr_constants.h"
#include "vr_tmp_vector.h"
#include "time_containers.h"
#include "result.h"
#include "segmented_list.h"
#include "dynamic_bitset.hpp"
#include <openvr.h>

using VRTimestampVector = segmented_list<time_stamp_t, VR_LARGE_SEGMENT_SIZE, slab_allocator<time_stamp_t>>;

struct  VRBitset : public boost::dynamic_bitset<uint64_t, std::allocator<uint64_t>>
{
	void encode(EncodeStream &e) const
	{
		std::vector<uint64_t> tmp;
		boost::to_block_range(*this, std::back_inserter(tmp));
		int num_blocks = tmp.size();
		e.memcpy_out_to_stream(&num_blocks, sizeof(num_blocks));
		e.memcpy_out_to_stream(tmp.data(), sizeof(uint64_t) * num_blocks);
	}

	void decode(EncodeStream &e)
	{
		int num_blocks;
		e.memcpy_from_stream(&num_blocks, sizeof(num_blocks));
		std::vector<uint64_t> tmp(num_blocks);
		e.memcpy_from_stream(tmp.data(), sizeof(uint64_t) * num_blocks);
		resize(num_blocks * 64);
		boost::from_block_range(tmp.begin(), tmp.end(), *this);
	}
};

// need to support encode to serialize it
struct VREncodableEvent : vr::VREvent_t 
{
	void encode(EncodeStream &e) const
	{
		e.memcpy_out_to_stream(this, sizeof(vr::VREvent_t));
	}

	void decode(EncodeStream &e)
	{
		e.memcpy_from_stream(this, sizeof(vr::VREvent_t));
	}
};

using VREventList = time_indexed_vector<VREncodableEvent, segmented_list_1024, slab_allocator>;

struct VRKeysUpdate
{
	enum KeysUpdateType
	{
		NEW_APP_KEY
	};
	KeysUpdateType update_type;
	uint32_t iparam1;
	std::string sparam1;
	std::string sparam2;
	void encode(EncodeStream &e) const
	{
		e.memcpy_out_to_stream(&update_type, sizeof(update_type));
		e.memcpy_out_to_stream(&iparam1, sizeof(iparam1));
		e.contiguous_container_out_to_stream(sparam1);
		e.contiguous_container_out_to_stream(sparam2);
	}

	void decode(EncodeStream &e)
	{
		e.memcpy_from_stream(&update_type, sizeof(update_type));
		e.memcpy_from_stream(&iparam1, sizeof(iparam1));
		e.contiguous_container_from_stream(sparam1);
		e.contiguous_container_from_stream(sparam2);
	}
};

using VRKeysUpdateVector = time_indexed_vector<VRKeysUpdate, segmented_list_1024, slab_allocator>;
using VRUpdateVector = time_indexed_vector<VRBitset, segmented_list_1024, slab_allocator>;

namespace vr
{
	// a couple synthetic ones
	struct uint32size
	{
		uint32_t width;
		uint32_t height;
	};

	struct RGBColor
	{
		float r;
		float g;
		float b;
	};

	struct FloatRange
	{
		float min;
		float max;
	};

	struct AbsoluteTransform
	{
		vr::ETrackingUniverseOrigin tracking_origin;
		vr::HmdMatrix34_t origin2overlaytransform;
	};

	
	struct TrackedDeviceRelativeTransform
	{
		vr::TrackedDeviceIndex_t tracked_device;
		vr::HmdMatrix34_t device2overlaytransform;
	};
	
	struct Uint32Size
	{
		uint32_t width;
		uint32_t height;
	};

	
	struct WindowBounds_t
	{
		int32_t i[2];
		uint32_t u[2];
	};

	
	struct ViewPort_t
	{
		uint32_t u[4];
	};

	
	struct CameraFrameSize_t
	{
		uint32_t width;
		uint32_t height;
		uint32_t size;
	};

	struct CameraFrameIntrinsics_t
	{
		vr::HmdVector2_t focal_length;
		vr::HmdVector2_t center;
	};
	
	struct VideoStreamTextureSize_t
	{
		vr::VRTextureBounds_t texture_bounds;
		uint32_t width;
		uint32_t height;
	};
}

namespace vr_result
{
	template <typename ReturnCode = NoReturnCode>
	using Uint32Size = Result<vr::uint32size, ReturnCode>;
		
	template <typename ReturnCode = NoReturnCode>
	using Bool = Result<bool, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Uint16 = Result<uint16_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Uint32 = Result<uint32_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using FloatRange = Result<vr::FloatRange, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using AbsoluteTransform = Result<vr::AbsoluteTransform, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using TrackedDeviceRelativeTransform = Result<vr::TrackedDeviceRelativeTransform, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Int32 = Result<int32_t, ReturnCode>;
	

	template <typename ReturnCode = NoReturnCode>
	using Float = Result<float, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Uint64 = Result<uint64_t, ReturnCode>;

	// these are useful for the gui usecase which doesn't really need the detail
	template <typename ReturnCode = NoReturnCode>
	using DevicePose	 = Result<vr::TrackedDevicePose_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using ActivityLevel = Result<vr::EDeviceActivityLevel, ReturnCode>; 

	template <typename ReturnCode = NoReturnCode>
	using ControllerRole = Result<vr::ETrackedControllerRole, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using DeviceClass = Result<vr::ETrackedDeviceClass, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using ControllerState = Result<vr::VRControllerState_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdMatrix44 = Result<vr::HmdMatrix44_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdMatrix34 = Result<vr::HmdMatrix34_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdVector4 = Result<vr::HmdVector4_t, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using HmdVector2 = Result<vr::HmdVector2_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdQuad = Result<vr::HmdQuad_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdColor = Result<vr::HmdColor_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using ChaperoneCalibrationState = Result < vr::ChaperoneCalibrationState, ReturnCode >;

	template <typename ReturnCode = NoReturnCode>
	using RenderModelComponentState  = Result<vr::RenderModel_ComponentState_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using TrackingUniverseOrigin = Result<vr::ETrackingUniverseOrigin, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using CompositorFrameTiming = Result<vr::Compositor_FrameTiming, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using CompositorCumulativeStats = Result<vr::Compositor_CumulativeStats, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using ColorSpace = Result<vr::EColorSpace, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using TextureBounds = Result<vr::VRTextureBounds_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using OverlayTransformType = Result<vr::VROverlayTransformType, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using OverlayInputMethod = Result<vr::VROverlayInputMethod, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using OverlayHandle = Result<vr::VROverlayHandle_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using RGBColor = Result<vr::RGBColor, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using CameraFrameSize = Result<vr::CameraFrameSize_t, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using CameraFrameIntrinsics = Result<vr::CameraFrameIntrinsics_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using VideoStreamTextureSize = Result<vr::VideoStreamTextureSize_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using WindowBounds = Result<vr::WindowBounds_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using ViewPort = Result<vr::ViewPort_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using DeviceIndex = Result<vr::TrackedDeviceIndex_t, ReturnCode>;

	template <	typename T, 
				typename ReturnCode = NoReturnCode>
	using  ResultVector = Result<std::vector<T, VRAllocatorTemplate<T>>, ReturnCode>;


	template <typename ReturnCode = NoReturnCode>
	using String = ResultVector<char, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Int32String = ResultVector<int32_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdColorString = ResultVector<vr::HmdColor_t, ReturnCode>;

	using TMPHMDColorVectorOnly = vr_tmp_vector<vr::HmdColor_t>;
	template <typename ReturnCode = NoReturnCode>
	using TMPHMDColorString = Result<TMPHMDColorVectorOnly, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using Uint8String = ResultVector<uint8_t, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using Uint16String = ResultVector<uint16_t, ReturnCode>;
	
	template <typename ReturnCode = NoReturnCode>
	using RenderModelVertexString = ResultVector<vr::RenderModel_Vertex_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using HmdQuadString = ResultVector<vr::HmdQuad_t, ReturnCode>;

#define DEFINE_TMP_STRING(output_type, input_type) \
using output_type ## VectorOnly = vr_tmp_vector<input_type>;\
template <typename ReturnCode = NoReturnCode>\
using output_type = Result<output_type ## VectorOnly, ReturnCode>;

	DEFINE_TMP_STRING(TMPHmdQuadString, vr::HmdQuad_t);
	DEFINE_TMP_STRING(TMPUint8String, uint8_t);
	DEFINE_TMP_STRING(TMPCompositorFrameTimingString, vr::Compositor_FrameTiming);
	DEFINE_TMP_STRING(TMPInt32String, int32_t);

	template <typename ReturnCode = NoReturnCode>
	using CompositorFrameTimingString = ResultVector<vr::Compositor_FrameTiming, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using  TMPStringProperty = Result <vr_tmp_vector<char>, ReturnCode>;

	using  DeviceIndexes	= ResultVector<vr::TrackedDeviceIndex_t, NoReturnCode>;

	using DeviceIndexesVectorOnly = vr_tmp_vector<vr::TrackedDeviceIndex_t>;
	using TMPDeviceIndexes = Result<DeviceIndexesVectorOnly, NoReturnCode>;

	using TMPStringVectorOnly = vr_tmp_vector<char>;
	template <typename ReturnCode = NoReturnCode>
	using TMPString = Result<TMPStringVectorOnly, ReturnCode>;

	using  HmdVector2s = ResultVector <vr::HmdVector2_t, NoReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using DistortionCoord = Result <vr::DistortionCoordinates_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using DistortionCoordinates = ResultVector<vr::DistortionCoordinates_t, ReturnCode>;

	template <typename ReturnCode = NoReturnCode>
	using ApplicationTransitionState = Result<vr::EVRApplicationTransitionState, ReturnCode>;

};

#define MEMCMP_OPERATOR_EQ(my_typename)\
\
namespace vr {\
inline bool operator == (const my_typename &lhs, const my_typename &rhs)\
{\
    if (&lhs == &rhs) return true; \
	return (memcmp(&lhs, &rhs, sizeof(lhs)) == 0);\
}\
inline bool operator != (const my_typename &lhs, const my_typename &rhs)\
{\
	return !(lhs == rhs);\
}\
};

MEMCMP_OPERATOR_EQ(vr::TrackedDeviceRelativeTransform)
MEMCMP_OPERATOR_EQ(vr::AbsoluteTransform)
MEMCMP_OPERATOR_EQ(vr::FloatRange)
MEMCMP_OPERATOR_EQ(vr::RGBColor)
MEMCMP_OPERATOR_EQ(vr::VideoStreamTextureSize_t)
MEMCMP_OPERATOR_EQ(vr::CameraFrameIntrinsics_t)
MEMCMP_OPERATOR_EQ(vr::CameraFrameSize_t)
MEMCMP_OPERATOR_EQ(vr::WindowBounds_t)
MEMCMP_OPERATOR_EQ(vr::ViewPort_t)
MEMCMP_OPERATOR_EQ(vr::uint32size)
MEMCMP_OPERATOR_EQ(vr::HmdVector2_t)
MEMCMP_OPERATOR_EQ(vr::HmdVector3_t)
MEMCMP_OPERATOR_EQ(vr::HmdVector4_t)
MEMCMP_OPERATOR_EQ(vr::DistortionCoordinates_t)
MEMCMP_OPERATOR_EQ(vr::HmdMatrix34_t)
MEMCMP_OPERATOR_EQ(vr::HmdMatrix44_t)
MEMCMP_OPERATOR_EQ(vr::VRControllerState_t)
MEMCMP_OPERATOR_EQ(vr::HmdQuad_t)
MEMCMP_OPERATOR_EQ(vr::Compositor_FrameTiming)
MEMCMP_OPERATOR_EQ(vr::Compositor_CumulativeStats)
MEMCMP_OPERATOR_EQ(vr::HmdColor_t)
MEMCMP_OPERATOR_EQ(vr::RenderModel_ComponentState_t)
MEMCMP_OPERATOR_EQ(vr::RenderModel_Vertex_t)
MEMCMP_OPERATOR_EQ(vr::VRTextureBounds_t)

namespace vr
{
	inline bool operator == (const vr::TrackedDevicePose_t &lhs, const vr::TrackedDevicePose_t &rhs)
	{
		if (&lhs == &rhs)
			return true;
		if (lhs.bPoseIsValid == false && rhs.bPoseIsValid == false)
			return true;
		if (lhs.bPoseIsValid != rhs.bPoseIsValid)
			return false;
		// otherwise both poses are valid - go big:
		return (memcmp(&lhs, &rhs, sizeof(lhs)) == 0);
	}
	inline bool operator != (const vr::TrackedDevicePose_t &lhs, const vr::TrackedDevicePose_t &rhs)
	{
		return !(lhs == rhs);
	}

	inline bool operator == (const vr::HiddenAreaMesh_t &lhs, const vr::HiddenAreaMesh_t &rhs)
	{
		if (&lhs == &rhs)
			return true;
		if (lhs.unTriangleCount != rhs.unTriangleCount)
			return false;
		if (lhs.pVertexData == rhs.pVertexData)
			return true;
		else
		{
			return (memcmp(lhs.pVertexData, rhs.pVertexData, lhs.unTriangleCount * 3 * sizeof(lhs.pVertexData[0])) == 0);
		}
	}
	inline bool operator != (const vr::HiddenAreaMesh_t &lhs, const vr::HiddenAreaMesh_t &rhs)
	{
		return !(lhs == rhs);
	}
};