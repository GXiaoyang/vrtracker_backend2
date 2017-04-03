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
#include "vr_settings_indexer.h"
#include "vr_properties_indexer.h"
#include <openvr.h>

using VRTimestampVector = segmented_list<time_stamp_t, VR_LARGE_SEGMENT_SIZE, VRAllocatorTemplate<time_stamp_t>>;

struct  VRBitset : public boost::dynamic_bitset<uint64_t, std::allocator<uint64_t>>
{
	void encode(BaseStream &e) const
	{
		serialization_id num_bits;
		num_bits = static_cast<serialization_id>(size());
		e.write_to_stream(&num_bits, sizeof(num_bits));

		std::vector<uint64_t> tmp;
		boost::to_block_range(*this, std::back_inserter(tmp));	// this will fill tmp with the bit vector
		e.contiguous_container_out_to_stream(tmp);
	}

	void decode(BaseStream &e)
	{
		serialization_id num_bits;
		e.read_from_stream(&num_bits, sizeof(num_bits));
		resize(num_bits); // resize container to be able to store whatever is in the vector

		std::vector<uint64_t> tmp(num_bits);
		e.contiguous_container_from_stream(tmp);
		boost::from_block_range(tmp.begin(), tmp.end(), *this);
	}
};

// need to support encode to serialize it
struct VREncodableEvent : vr::VREvent_t 
{
	VREncodableEvent() = default;

	VREncodableEvent(const vr::VREvent_t &rhs)
		: vr::VREvent_t(rhs)
	{
	}
	void encode(BaseStream &e) const
	{
		e.write_to_stream(this, sizeof(vr::VREvent_t));
	}

	void decode(BaseStream &e)
	{
		e.read_from_stream(this, sizeof(vr::VREvent_t));
	}
};

using VREventList = time_indexed_vector<VREncodableEvent, segmented_list_1024, VRAllocatorTemplate>;

struct VRKeysUpdate
{
	enum KeysUpdateType
	{
		NEW_APP_KEY,			// sparam1: app_key
		NEW_SETTING,			//sparam1: name, sparam2: section, iparam: setting type
		NEW_DEVICE_PROPERTY,	// ditto
		NEW_RESOURCE,			// sparam1: name sparam2: directory
		NEW_OVERLAY,			// sparam1: overlay_name
		MODIFY_NEARZ_FARZ,		// fparm0, fparam1
	};

	VRKeysUpdate()
		: iparam1(0), iparam2(0), fparam1(0), fparam2(0)
	{}

	VRKeysUpdate(KeysUpdateType update_type_in, const std::string &sparam1_in)
		: update_type(update_type_in), iparam1(0), iparam2(0), fparam1(0), fparam2(0), sparam1(sparam1_in)
	{}
	
	VRKeysUpdate(KeysUpdateType update_type_in, const std::string &sparam1_in, const std::string &sparam2_in)
		: update_type(update_type_in), iparam1(0), iparam2(0), fparam1(0), fparam2(0), sparam1(sparam1_in), sparam2(sparam2_in)
	{}

	VRKeysUpdate(KeysUpdateType update_type_in, const std::string &sparam1_in, int iparam_in, const std::string &sparam2_in)
		: update_type(update_type_in), iparam1(iparam_in), iparam2(0), fparam1(0), fparam2(0), sparam1(sparam1_in), sparam2(sparam2_in)
	{}

	VRKeysUpdate(KeysUpdateType update_type_in, int iparam1_in, const std::string &sparam1_in, int iparam2_in)
		: update_type(update_type_in), iparam1(iparam1_in), iparam2(iparam2_in), fparam1(0), fparam2(0), sparam1(sparam1_in)
	{}

	VRKeysUpdate(KeysUpdateType update_type_in, float fparam1_in, float fparam2_in)
		: update_type(update_type_in), iparam1(0), iparam2(0), fparam1(fparam1_in), fparam2(fparam2_in)
	{}

	bool operator==(const VRKeysUpdate &rhs) const
	{
		if (update_type != rhs.update_type)
			return false;
		if (iparam1 != rhs.iparam1)
			return false;
		if (iparam2 != rhs.iparam2)
			return false;
		if (fparam1 != rhs.fparam1)
			return false;
		if (fparam2 != rhs.fparam2)
			return false;
		if (sparam1 != rhs.sparam1)
			return false;
		if (sparam2 != rhs.sparam2)
			return false;
		return true;
	}

	bool operator!=(const VRKeysUpdate &rhs) const
	{
		return !(*this == rhs);
	}

	static VRKeysUpdate make_new_app(const std::string& app_key) { return VRKeysUpdate(NEW_APP_KEY, app_key); }
	static VRKeysUpdate make_new_overlay(const std::string& overlay_name) { return VRKeysUpdate(NEW_OVERLAY, overlay_name); }
	
	static VRKeysUpdate make_new_setting(const std::string& section_name, SettingsIndexer::SectionSettingType setting_type, 
						const std::string&setting_name) 
	{
		return VRKeysUpdate(NEW_SETTING, section_name, static_cast<int>(setting_type), setting_name);
	}

	static VRKeysUpdate make_new_device_property(PropertiesIndexer::PropertySettingType setting_type, const std::string&setting_name, int val)
	{
		return VRKeysUpdate(NEW_DEVICE_PROPERTY, static_cast<int>(setting_type), setting_name, val);
	}

	static VRKeysUpdate make_new_resource(const std::string &name, const std::string directory)
	{
		return VRKeysUpdate(NEW_RESOURCE, name, directory);
	}

	static VRKeysUpdate make_modify_nearz_farz(float nearz, float farz)
	{
		return VRKeysUpdate(MODIFY_NEARZ_FARZ, nearz, farz);
	}


	KeysUpdateType update_type;
	int iparam1;
	int iparam2;
	float fparam1;
	float fparam2;
	std::string sparam1;
	std::string sparam2;
	void encode(BaseStream &e) const
	{
		e.write_to_stream(&update_type, sizeof(update_type));
		e.write_to_stream(&iparam1, sizeof(iparam1));
		e.write_to_stream(&iparam2, sizeof(iparam2));
		e.write_to_stream(&fparam1, sizeof(fparam1));
		e.write_to_stream(&fparam2, sizeof(fparam2));
		e.contiguous_container_out_to_stream(sparam1);
		e.contiguous_container_out_to_stream(sparam2);
	}

	void decode(BaseStream &e)
	{
		e.read_from_stream(&update_type, sizeof(update_type));
		e.read_from_stream(&iparam1, sizeof(iparam1));
		e.read_from_stream(&iparam2, sizeof(iparam2));
		e.read_from_stream(&fparam1, sizeof(fparam1));
		e.read_from_stream(&fparam2, sizeof(fparam2));
		e.contiguous_container_from_stream(sparam1);
		e.contiguous_container_from_stream(sparam2);
	}
};

using VRKeysUpdateVector = time_indexed_vector<VRKeysUpdate, segmented_list_1024, VRAllocatorTemplate>;
using VRUpdateVector = time_indexed_vector<VRBitset, segmented_list_1024, VRAllocatorTemplate>;

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
