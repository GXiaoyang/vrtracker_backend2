#pragma once
#include "openvr_cppstub.h"
#include "vr_cursor_context.h"

class VRSystemCursor : public VRSystemCppStub
{
	CursorContext *m_context;
	vr_result::system_state &state_ref;
	vr_result::system_iterator &iter_ref;
	DevicePropertiesIndexer *m_property_indexer;

public:
	explicit VRSystemCursor(CursorContext *context);
	void SynchronizeChildVectors();

	void GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight) override;
	struct vr::HmdMatrix44_t GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ) override;
	void GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom) override;
	bool ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates) override;
	struct vr::HmdMatrix34_t GetEyeToHeadTransform(vr::EVREye eEye) override;
	bool GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter) override;
	int32_t GetD3D9AdapterIndex() override;
	void GetDXGIOutputInfo(int32_t * pnAdapterIndex) override;
	bool IsDisplayOnDesktop() override;
	void GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override;
	struct vr::HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() override;
	struct vr::HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() override;
	uint32_t GetSortedTrackedDeviceIndicesOfClass(vr::ETrackedDeviceClass eTrackedDeviceClass, vr::TrackedDeviceIndex_t * punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex) override;
	vr::EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override;
	void ApplyTransform(struct vr::TrackedDevicePose_t * pOutputPose, const struct vr::TrackedDevicePose_t * pTrackedDevicePose, const struct vr::HmdMatrix34_t * pTransform) override;
	vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) override;
	vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) override;
	vr::ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override;
	bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override;
	bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	struct vr::HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError) override;
	const char * GetPropErrorNameFromEnum(vr::ETrackedPropertyError error) override;
	bool PollNextEvent(struct vr::VREvent_t * pEvent, uint32_t uncbVREvent) override;
	bool PollNextEventWithPose(vr::ETrackingUniverseOrigin eOrigin, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t * pTrackedDevicePose) override;
	const char * GetEventTypeNameFromEnum(vr::EVREventType eType) override;
	struct vr::HiddenAreaMesh_t GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type) override;
	bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize) override;
	bool GetControllerStateWithPose(vr::ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize, struct vr::TrackedDevicePose_t * pTrackedDevicePose) override;
	const char * GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId) override;
	const char * GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType) override;
	bool IsInputFocusCapturedByAnotherProcess() override;

	bool IsValidDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex);
	bool lookup_property_index(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop_enum,
		PropertiesIndexer::PropertySettingType setting_type,
		int *property_index, vr::ETrackedPropertyError * pError);
};
