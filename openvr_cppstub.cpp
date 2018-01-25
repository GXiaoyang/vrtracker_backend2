#include "openvr_cppstub.h"
#include <openvr_broker.h>
#include <stdint.h>
#include "log.h"

static VRSystemCppStub VRSystemCppStubInstance;
static VRExtendedDisplayCppStub VRExtendedDisplayCppStubInstance;
static VRTrackedCameraCppStub VRTrackedCameraCppStubInstance;
static VRApplicationsCppStub VRApplicationsCppStubInstance;
static VRChaperoneCppStub VRChaperoneCppStubInstance;
static VRChaperoneSetupCppStub VRChaperoneSetupCppStubInstance;
static VRCompositorCppStub VRCompositorCppStubInstance;
static VROverlayCppStub VROverlayCppStubInstance;
static VRRenderModelsCppStub VRRenderModelsCppStubInstance;
static VRNotificationsCppStub VRNotificationsCppStubInstance;
static VRSettingsCppStub VRSettingsCppStubInstance;
static VRScreenshotsCppStub VRScreenshotsCppStubInstance;
static VRResourcesCppStub VRResourcesCppStubInstance;

static void *vr_interfaces_cppstub[] =
{
	&VRSystemCppStubInstance,
	&VRApplicationsCppStubInstance,
	&VRSettingsCppStubInstance,
	&VRChaperoneCppStubInstance,
	&VRChaperoneSetupCppStubInstance,
	&VRCompositorCppStubInstance,
	&VRNotificationsCppStubInstance,
	&VROverlayCppStubInstance,
	&VRRenderModelsCppStubInstance,
	&VRExtendedDisplayCppStubInstance,
	&VRTrackedCameraCppStubInstance,
	&VRScreenshotsCppStubInstance,
	&VRResourcesCppStubInstance,
};

void **GetCppStubInterfaces()
{
	return vr_interfaces_cppstub;
}

	void VRSystemCppStub::GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight)
	{
		LOG_ENTRY("CppStubGetRecommendedRenderTargetSize");
		LOG_EXIT("CppStubGetRecommendedRenderTargetSize");
	}

	struct vr::HmdMatrix44_t VRSystemCppStub::GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ)
	{
		LOG_ENTRY("CppStubGetProjectionMatrix");
		static struct vr::HmdMatrix44_t rc;
		LOG_EXIT_RC(rc, "CppStubGetProjectionMatrix");
	}

	void VRSystemCppStub::GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom)
	{
		LOG_ENTRY("CppStubGetProjectionRaw");
		LOG_EXIT("CppStubGetProjectionRaw");
	}

	bool VRSystemCppStub::ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates)
	{
		LOG_ENTRY("CppStubComputeDistortion");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubComputeDistortion");
	}

	struct vr::HmdMatrix34_t VRSystemCppStub::GetEyeToHeadTransform(vr::EVREye eEye)
	{
		LOG_ENTRY("CppStubGetEyeToHeadTransform");
		static struct vr::HmdMatrix34_t rc;
		LOG_EXIT_RC(rc, "CppStubGetEyeToHeadTransform");
	}

	bool VRSystemCppStub::GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter)
	{
		LOG_ENTRY("CppStubGetTimeSinceLastVsync");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubGetTimeSinceLastVsync");
	}

	int32_t VRSystemCppStub::GetD3D9AdapterIndex()
	{
		LOG_ENTRY("CppStubGetD3D9AdapterIndex");
		static int32_t rc = 0;
		LOG_EXIT_RC(rc, "CppStubGetD3D9AdapterIndex");
	}

	void VRSystemCppStub::GetDXGIOutputInfo(int32_t * pnAdapterIndex)
	{
		LOG_ENTRY("CppStubGetDXGIOutputInfo");
		LOG_EXIT("CppStubGetDXGIOutputInfo");
	}

	void VRSystemCppStub::GetOutputDevice(uint64_t *pnDevice, vr::ETextureType textureType, VkInstance_T *pInstance)
	{
		LOG_ENTRY("GetOutputDevice");
		if (pnDevice)
		{
			pnDevice = nullptr;
		}
		LOG_EXIT("GetOutputDevice");
	}

	bool VRSystemCppStub::IsDisplayOnDesktop()
	{
		LOG_ENTRY("CppStubIsDisplayOnDesktop");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubIsDisplayOnDesktop");
	}

	bool VRSystemCppStub::SetDisplayVisibility(bool bIsVisibleOnDesktop)
	{
		LOG_ENTRY("CppStubSetDisplayVisibility");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubSetDisplayVisibility");
	}

	void VRSystemCppStub::GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount)
	{
		LOG_ENTRY("CppStubGetDeviceToAbsoluteTrackingPose");
		LOG_EXIT("CppStubGetDeviceToAbsoluteTrackingPose");
	}

	void VRSystemCppStub::ResetSeatedZeroPose()
	{
		LOG_ENTRY("CppStubResetSeatedZeroPose");
		LOG_EXIT("CppStubResetSeatedZeroPose");
	}

	struct vr::HmdMatrix34_t VRSystemCppStub::GetSeatedZeroPoseToStandingAbsoluteTrackingPose()
	{
		LOG_ENTRY("CppStubGetSeatedZeroPoseToStandingAbsoluteTrackingPose");
		static struct vr::HmdMatrix34_t rc;
		LOG_EXIT_RC(rc, "CppStubGetSeatedZeroPoseToStandingAbsoluteTrackingPose");
	}

	struct vr::HmdMatrix34_t VRSystemCppStub::GetRawZeroPoseToStandingAbsoluteTrackingPose()
	{
		LOG_ENTRY("CppStubGetRawZeroPoseToStandingAbsoluteTrackingPose");
		static struct vr::HmdMatrix34_t rc;
		LOG_EXIT_RC(rc, "CppStubGetRawZeroPoseToStandingAbsoluteTrackingPose");
	}

	uint32_t VRSystemCppStub::GetSortedTrackedDeviceIndicesOfClass(vr::ETrackedDeviceClass eTrackedDeviceClass, vr::TrackedDeviceIndex_t * punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex)
	{
		LOG_ENTRY("CppStubGetSortedTrackedDeviceIndicesOfClass");
		static uint32_t rc = 0;
		LOG_EXIT_RC(rc, "CppStubGetSortedTrackedDeviceIndicesOfClass");
	}

	vr::EDeviceActivityLevel VRSystemCppStub::GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId)
	{
		LOG_ENTRY("CppStubGetTrackedDeviceActivityLevel");
		static vr::EDeviceActivityLevel rc;
		LOG_EXIT_RC(rc, "CppStubGetTrackedDeviceActivityLevel");
	}

	void VRSystemCppStub::ApplyTransform(struct vr::TrackedDevicePose_t * pOutputPose, const struct vr::TrackedDevicePose_t * pTrackedDevicePose, const struct vr::HmdMatrix34_t * pTransform)
	{
		LOG_ENTRY("CppStubApplyTransform");
		LOG_EXIT("CppStubApplyTransform");
	}

	vr::TrackedDeviceIndex_t VRSystemCppStub::GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType)
	{
		LOG_ENTRY("CppStubGetTrackedDeviceIndexForControllerRole");
		static vr::TrackedDeviceIndex_t rc;
		LOG_EXIT_RC(rc, "CppStubGetTrackedDeviceIndexForControllerRole");
	}

	vr::ETrackedControllerRole VRSystemCppStub::GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex)
	{
		LOG_ENTRY("CppStubGetControllerRoleForTrackedDeviceIndex");
		static vr::ETrackedControllerRole rc;
		LOG_EXIT_RC(rc, "CppStubGetControllerRoleForTrackedDeviceIndex");
	}

	vr::ETrackedDeviceClass VRSystemCppStub::GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex)
	{
		LOG_ENTRY("CppStubGetTrackedDeviceClass");
		static vr::ETrackedDeviceClass rc;
		LOG_EXIT_RC(rc, "CppStubGetTrackedDeviceClass");
	}

	bool VRSystemCppStub::IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex)
	{
		LOG_ENTRY("CppStubIsTrackedDeviceConnected");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubIsTrackedDeviceConnected");
	}

	bool VRSystemCppStub::GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetBoolTrackedDeviceProperty");
		static bool rc = true;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetBoolTrackedDeviceProperty");
	}

	float VRSystemCppStub::GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetFloatTrackedDeviceProperty");
		static float rc;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetFloatTrackedDeviceProperty");
	}

	int32_t VRSystemCppStub::GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetInt32TrackedDeviceProperty");
		static int32_t rc = 0;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetInt32TrackedDeviceProperty");
	}

	uint64_t VRSystemCppStub::GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetUint64TrackedDeviceProperty");
		static uint64_t rc = 0;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetUint64TrackedDeviceProperty");
	}

	struct vr::HmdMatrix34_t VRSystemCppStub::GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetMatrix34TrackedDeviceProperty");
		static struct vr::HmdMatrix34_t rc;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetMatrix34TrackedDeviceProperty");
	}

	uint32_t VRSystemCppStub::GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t, vr::ETrackedDeviceProperty, vr::PropertyTypeTag_t, void *, uint32_t, vr::ETrackedPropertyError *pError)
	{
		LOG_ENTRY("GetArrayTrackedDeviceProperty");
		static uint32_t rc = 0;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "GetArrayTrackedDeviceProperty");
	}

		uint32_t VRSystemCppStub::GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError)
	{
		LOG_ENTRY("CppStubGetStringTrackedDeviceProperty");
		static uint32_t rc = 0;
		if (pError) { *pError = (vr::ETrackedPropertyError)0; }
		LOG_EXIT_RC(rc, "CppStubGetStringTrackedDeviceProperty");
	}

	const char * VRSystemCppStub::GetPropErrorNameFromEnum(vr::ETrackedPropertyError error)
	{
		LOG_ENTRY("CppStubGetPropErrorNameFromEnum");
		static const char * rc = "";
		LOG_EXIT_RC(rc, "CppStubGetPropErrorNameFromEnum");
	}

	bool VRSystemCppStub::PollNextEvent(struct vr::VREvent_t * pEvent, uint32_t uncbVREvent)
	{
		LOG_ENTRY("CppStubPollNextEvent");
		static bool rc = false;
		LOG_EXIT_RC(rc, "CppStubPollNextEvent");
	}

	bool VRSystemCppStub::PollNextEventWithPose(vr::ETrackingUniverseOrigin eOrigin, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t * pTrackedDevicePose)
	{
		LOG_ENTRY("CppStubPollNextEventWithPose");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubPollNextEventWithPose");
	}

	const char * VRSystemCppStub::GetEventTypeNameFromEnum(vr::EVREventType eType)
	{
		LOG_ENTRY("CppStubGetEventTypeNameFromEnum");
		static const char * rc = "";
		LOG_EXIT_RC(rc, "CppStubGetEventTypeNameFromEnum");
	}

	struct vr::HiddenAreaMesh_t VRSystemCppStub::GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type)
	{
		LOG_ENTRY("CppStubGetHiddenAreaMesh");
		static struct vr::HiddenAreaMesh_t rc;
		LOG_EXIT_RC(rc, "CppStubGetHiddenAreaMesh");
	}

	bool VRSystemCppStub::GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize)
	{
		LOG_ENTRY("CppStubGetControllerState");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubGetControllerState");
	}

	bool VRSystemCppStub::GetControllerStateWithPose(vr::ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize, struct vr::TrackedDevicePose_t * pTrackedDevicePose)
	{
		LOG_ENTRY("CppStubGetControllerStateWithPose");
		static bool rc = true;
		LOG_EXIT_RC(rc, "CppStubGetControllerStateWithPose");
	}

	void VRSystemCppStub::TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec)
	{
		LOG_ENTRY("CppStubTriggerHapticPulse");
		LOG_EXIT("CppStubTriggerHapticPulse");
	}

	const char * VRSystemCppStub::GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId)
	{
		LOG_ENTRY("CppStubGetButtonIdNameFromEnum");
		static const char * rc = "";
		LOG_EXIT_RC(rc, "CppStubGetButtonIdNameFromEnum");
	}

	const char * VRSystemCppStub::GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType)
	{
		LOG_ENTRY("CppStubGetControllerAxisTypeNameFromEnum");
		static const char * rc = "";
		LOG_EXIT_RC(rc, "CppStubGetControllerAxisTypeNameFromEnum");
	}

	bool VRSystemCppStub::IsInputAvailable()
	{
		LOG_ENTRY("IsInputAvailable");
		static bool rc = true;
		LOG_EXIT_RC(rc, "IsInputAvailable");
	}

	bool VRSystemCppStub::IsSteamVRDrawingControllers()
	{
		LOG_ENTRY("IsSteamVRDrawingControllers");
		static bool rc = true;
		LOG_EXIT_RC(rc, "IsSteamVRDrawingControllers");
	}
	
	bool VRSystemCppStub::ShouldApplicationPause()
	{
		LOG_ENTRY("ShouldApplicationPause");
		static bool rc = true;
		LOG_EXIT_RC(rc, "ShouldApplicationPause");
	}

	bool VRSystemCppStub::ShouldApplicationReduceRenderingWork()
	{
		LOG_ENTRY("ShouldApplicationReduceRenderingWork");
		static bool rc = true;
		LOG_EXIT_RC(rc, "ShouldApplicationReduceRenderingWork");
	}

	uint32_t VRSystemCppStub::DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize)
	{
		LOG_ENTRY("CppStubDriverDebugRequest");
		static uint32_t rc = 0;
		LOG_EXIT_RC(rc, "CppStubDriverDebugRequest");
	}

	vr::EVRFirmwareError VRSystemCppStub::PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex)
	{
		LOG_ENTRY("CppStubPerformFirmwareUpdate");
		static vr::EVRFirmwareError rc;
		LOG_EXIT_RC(rc, "CppStubPerformFirmwareUpdate");
	}

	void VRSystemCppStub::AcknowledgeQuit_Exiting()
	{
		LOG_ENTRY("CppStubAcknowledgeQuit_Exiting");
		LOG_EXIT("CppStubAcknowledgeQuit_Exiting");
	}

	void VRSystemCppStub::AcknowledgeQuit_UserPrompt()
	{
		LOG_ENTRY("CppStubAcknowledgeQuit_UserPrompt");
		LOG_EXIT("CppStubAcknowledgeQuit_UserPrompt");
	}

void VRExtendedDisplayCppStub::GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetWindowBounds");
	LOG_EXIT("CppStubGetWindowBounds");
}

void VRExtendedDisplayCppStub::GetEyeOutputViewport(vr::EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetEyeOutputViewport");
	LOG_EXIT("CppStubGetEyeOutputViewport");
}

void VRExtendedDisplayCppStub::GetDXGIOutputInfo(int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex)
{
	LOG_ENTRY("CppStubGetDXGIOutputInfo");
	LOG_EXIT("CppStubGetDXGIOutputInfo");
}

const char * VRTrackedCameraCppStub::GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError)
{
	LOG_ENTRY("CppStubGetCameraErrorNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetCameraErrorNameFromEnum");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera)
{
	LOG_ENTRY("CppStubHasCamera");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubHasCamera");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize)
{
	LOG_ENTRY("CppStubGetCameraFrameSize");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetCameraFrameSize");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t * pFocalLength, vr::HmdVector2_t * pCenter)
{
	LOG_ENTRY("CppStubGetCameraIntrinsics");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetCameraIntrinsics");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection)
{
	LOG_ENTRY("CppStubGetCameraProjection");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetCameraProjection");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t * pHandle)
{
	LOG_ENTRY("CppStubAcquireVideoStreamingService");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubAcquireVideoStreamingService");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera)
{
	LOG_ENTRY("CppStubReleaseVideoStreamingService");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubReleaseVideoStreamingService");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
	LOG_ENTRY("CppStubGetVideoStreamFrameBuffer");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetVideoStreamFrameBuffer");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t * pTextureBounds, uint32_t * pnWidth, uint32_t * pnHeight)
{
	LOG_ENTRY("CppStubGetVideoStreamTextureSize");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetVideoStreamTextureSize");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
	LOG_ENTRY("CppStubGetVideoStreamTextureD3D11");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetVideoStreamTextureD3D11");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t * pglTextureId, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
	LOG_ENTRY("CppStubGetVideoStreamTextureGL");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubGetVideoStreamTextureGL");
}

vr::EVRTrackedCameraError VRTrackedCameraCppStub::ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId)
{
	LOG_ENTRY("CppStubReleaseVideoStreamTextureGL");
	static vr::EVRTrackedCameraError rc;
	LOG_EXIT_RC(rc, "CppStubReleaseVideoStreamTextureGL");
}

vr::EVRApplicationError VRApplicationsCppStub::AddApplicationManifest(const char * pchApplicationManifestFullPath, bool bTemporary)
{
	LOG_ENTRY("CppStubAddApplicationManifest");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubAddApplicationManifest");
}

vr::EVRApplicationError VRApplicationsCppStub::RemoveApplicationManifest(const char * pchApplicationManifestFullPath)
{
	LOG_ENTRY("CppStubRemoveApplicationManifest");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubRemoveApplicationManifest");
}

bool VRApplicationsCppStub::IsApplicationInstalled(const char * pchAppKey)
{
	LOG_ENTRY("CppStubIsApplicationInstalled");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsApplicationInstalled");
}

uint32_t VRApplicationsCppStub::GetApplicationCount()
{
	LOG_ENTRY("CppStubGetApplicationCount");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetApplicationCount");
}

vr::EVRApplicationError VRApplicationsCppStub::GetApplicationKeyByIndex(uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetApplicationKeyByIndex");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubGetApplicationKeyByIndex");
}

vr::EVRApplicationError VRApplicationsCppStub::GetApplicationKeyByProcessId(uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetApplicationKeyByProcessId");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubGetApplicationKeyByProcessId");
}

vr::EVRApplicationError VRApplicationsCppStub::LaunchApplication(const char * pchAppKey)
{
	LOG_ENTRY("CppStubLaunchApplication");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubLaunchApplication");
}

vr::EVRApplicationError VRApplicationsCppStub::LaunchTemplateApplication(const char * pchTemplateAppKey, const char * pchNewAppKey, const struct vr::AppOverrideKeys_t * pKeys, uint32_t unKeys)
{
	LOG_ENTRY("CppStubLaunchTemplateApplication");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubLaunchTemplateApplication");
}

vr::EVRApplicationError VRApplicationsCppStub::LaunchApplicationFromMimeType(const char * pchMimeType, const char * pchArgs)
{
	LOG_ENTRY("CppStubLaunchApplicationFromMimeType");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubLaunchApplicationFromMimeType");
}

vr::EVRApplicationError VRApplicationsCppStub::LaunchDashboardOverlay(const char * pchAppKey)
{
	LOG_ENTRY("CppStubLaunchDashboardOverlay");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubLaunchDashboardOverlay");
}

bool VRApplicationsCppStub::CancelApplicationLaunch(const char * pchAppKey)
{
	LOG_ENTRY("CppStubCancelApplicationLaunch");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubCancelApplicationLaunch");
}

vr::EVRApplicationError VRApplicationsCppStub::IdentifyApplication(uint32_t unProcessId, const char * pchAppKey)
{
	LOG_ENTRY("CppStubIdentifyApplication");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubIdentifyApplication");
}

uint32_t VRApplicationsCppStub::GetApplicationProcessId(const char * pchAppKey)
{
	LOG_ENTRY("CppStubGetApplicationProcessId");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetApplicationProcessId");
}

const char * VRApplicationsCppStub::GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error)
{
	LOG_ENTRY("CppStubGetApplicationsErrorNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetApplicationsErrorNameFromEnum");
}

uint32_t VRApplicationsCppStub::GetApplicationPropertyString(const char * pchAppKey, vr::EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, vr::EVRApplicationError * peError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyString");
	static uint32_t rc = 0;
	if (peError) { *peError = (vr::EVRApplicationError)0; }
	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyString");
}

bool VRApplicationsCppStub::GetApplicationPropertyBool(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyBool");
	static bool rc = true;
	if (peError) { *peError = (vr::EVRApplicationError)0; }
	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyBool");
}

uint64_t VRApplicationsCppStub::GetApplicationPropertyUint64(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError)
{
	LOG_ENTRY("CppStubGetApplicationPropertyUint64");
	static uint64_t rc = 0;
	if (peError) { *peError = (vr::EVRApplicationError)0; }
	LOG_EXIT_RC(rc, "CppStubGetApplicationPropertyUint64");
}

vr::EVRApplicationError VRApplicationsCppStub::SetApplicationAutoLaunch(const char * pchAppKey, bool bAutoLaunch)
{
	LOG_ENTRY("CppStubSetApplicationAutoLaunch");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubSetApplicationAutoLaunch");
}

bool VRApplicationsCppStub::GetApplicationAutoLaunch(const char * pchAppKey)
{
	LOG_ENTRY("CppStubGetApplicationAutoLaunch");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetApplicationAutoLaunch");
}

vr::EVRApplicationError VRApplicationsCppStub::SetDefaultApplicationForMimeType(const char * pchAppKey, const char * pchMimeType)
{
	LOG_ENTRY("CppStubSetDefaultApplicationForMimeType");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubSetDefaultApplicationForMimeType");
}

bool VRApplicationsCppStub::GetDefaultApplicationForMimeType(const char * pchMimeType, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetDefaultApplicationForMimeType");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetDefaultApplicationForMimeType");
}

bool VRApplicationsCppStub::GetApplicationSupportedMimeTypes(const char * pchAppKey, char * pchMimeTypesBuffer, uint32_t unMimeTypesBuffer)
{
	LOG_ENTRY("CppStubGetApplicationSupportedMimeTypes");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetApplicationSupportedMimeTypes");
}

uint32_t VRApplicationsCppStub::GetApplicationsThatSupportMimeType(const char * pchMimeType, char * pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer)
{
	LOG_ENTRY("CppStubGetApplicationsThatSupportMimeType");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetApplicationsThatSupportMimeType");
}

uint32_t VRApplicationsCppStub::GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs)
{
	LOG_ENTRY("CppStubGetApplicationLaunchArguments");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetApplicationLaunchArguments");
}

vr::EVRApplicationError VRApplicationsCppStub::GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
	LOG_ENTRY("CppStubGetStartingApplication");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubGetStartingApplication");
}

vr::EVRApplicationTransitionState VRApplicationsCppStub::GetTransitionState()
{
	LOG_ENTRY("CppStubGetTransitionState");
	static vr::EVRApplicationTransitionState rc;
	LOG_EXIT_RC(rc, "CppStubGetTransitionState");
}

vr::EVRApplicationError VRApplicationsCppStub::PerformApplicationPrelaunchCheck(const char * pchAppKey)
{
	LOG_ENTRY("CppStubPerformApplicationPrelaunchCheck");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubPerformApplicationPrelaunchCheck");
}

const char * VRApplicationsCppStub::GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state)
{
	LOG_ENTRY("CppStubGetApplicationsTransitionStateNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetApplicationsTransitionStateNameFromEnum");
}

bool VRApplicationsCppStub::IsQuitUserPromptRequested()
{
	LOG_ENTRY("CppStubIsQuitUserPromptRequested");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsQuitUserPromptRequested");
}

vr::EVRApplicationError VRApplicationsCppStub::LaunchInternalProcess(const char * pchBinaryPath, const char * pchArguments, const char * pchWorkingDirectory)
{
	LOG_ENTRY("CppStubLaunchInternalProcess");
	static vr::EVRApplicationError rc;
	LOG_EXIT_RC(rc, "CppStubLaunchInternalProcess");
}

uint32_t VRApplicationsCppStub::GetCurrentSceneProcessId()
{
	LOG_ENTRY("CppStubGetCurrentSceneProcessId");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetCurrentSceneProcessId");
}

vr::ChaperoneCalibrationState VRChaperoneCppStub::GetCalibrationState()
{
	LOG_ENTRY("CppStubGetCalibrationState");
	static vr::ChaperoneCalibrationState rc;
	LOG_EXIT_RC(rc, "CppStubGetCalibrationState");
}

bool VRChaperoneCppStub::GetPlayAreaSize(float * pSizeX, float * pSizeZ)
{
	LOG_ENTRY("CppStubGetPlayAreaSize");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetPlayAreaSize");
}

bool VRChaperoneCppStub::GetPlayAreaRect(struct vr::HmdQuad_t * rect)
{
	LOG_ENTRY("CppStubGetPlayAreaRect");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetPlayAreaRect");
}

void VRChaperoneCppStub::ReloadInfo()
{
	LOG_ENTRY("CppStubReloadInfo");
	LOG_EXIT("CppStubReloadInfo");
}

void VRChaperoneCppStub::SetSceneColor(struct vr::HmdColor_t color)
{
	LOG_ENTRY("CppStubSetSceneColor");
	LOG_EXIT("CppStubSetSceneColor");
}

void VRChaperoneCppStub::GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor)
{
	LOG_ENTRY("CppStubGetBoundsColor");
	LOG_EXIT("CppStubGetBoundsColor");
}

bool VRChaperoneCppStub::AreBoundsVisible()
{
	LOG_ENTRY("CppStubAreBoundsVisible");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubAreBoundsVisible");
}

void VRChaperoneCppStub::ForceBoundsVisible(bool bForce)
{
	LOG_ENTRY("CppStubForceBoundsVisible");
	LOG_EXIT("CppStubForceBoundsVisible");
}

bool VRChaperoneSetupCppStub::CommitWorkingCopy(vr::EChaperoneConfigFile configFile)
{
	LOG_ENTRY("CppStubCommitWorkingCopy");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubCommitWorkingCopy");
}

void VRChaperoneSetupCppStub::RevertWorkingCopy()
{
	LOG_ENTRY("CppStubRevertWorkingCopy");
	LOG_EXIT("CppStubRevertWorkingCopy");
}

bool VRChaperoneSetupCppStub::GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ)
{
	LOG_ENTRY("CppStubGetWorkingPlayAreaSize");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetWorkingPlayAreaSize");
}

bool VRChaperoneSetupCppStub::GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect)
{
	LOG_ENTRY("CppStubGetWorkingPlayAreaRect");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetWorkingPlayAreaRect");
}

bool VRChaperoneSetupCppStub::GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
	LOG_ENTRY("CppStubGetWorkingCollisionBoundsInfo");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetWorkingCollisionBoundsInfo");
}

bool VRChaperoneSetupCppStub::GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
	LOG_ENTRY("CppStubGetLiveCollisionBoundsInfo");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetLiveCollisionBoundsInfo");
}

bool VRChaperoneSetupCppStub::GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose)
{
	LOG_ENTRY("CppStubGetWorkingSeatedZeroPoseToRawTrackingPose");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetWorkingSeatedZeroPoseToRawTrackingPose");
}

bool VRChaperoneSetupCppStub::GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose)
{
	LOG_ENTRY("CppStubGetWorkingStandingZeroPoseToRawTrackingPose");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetWorkingStandingZeroPoseToRawTrackingPose");
}

void VRChaperoneSetupCppStub::SetWorkingPlayAreaSize(float sizeX, float sizeZ)
{
	LOG_ENTRY("CppStubSetWorkingPlayAreaSize");
	LOG_EXIT("CppStubSetWorkingPlayAreaSize");
}

void VRChaperoneSetupCppStub::SetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount)
{
	LOG_ENTRY("CppStubSetWorkingCollisionBoundsInfo");
	LOG_EXIT("CppStubSetWorkingCollisionBoundsInfo");
}

void VRChaperoneSetupCppStub::SetWorkingSeatedZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatSeatedZeroPoseToRawTrackingPose)
{
	LOG_ENTRY("CppStubSetWorkingSeatedZeroPoseToRawTrackingPose");
	LOG_EXIT("CppStubSetWorkingSeatedZeroPoseToRawTrackingPose");
}

void VRChaperoneSetupCppStub::SetWorkingStandingZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatStandingZeroPoseToRawTrackingPose)
{
	LOG_ENTRY("CppStubSetWorkingStandingZeroPoseToRawTrackingPose");
	LOG_EXIT("CppStubSetWorkingStandingZeroPoseToRawTrackingPose");
}

void VRChaperoneSetupCppStub::ReloadFromDisk(vr::EChaperoneConfigFile configFile)
{
	LOG_ENTRY("CppStubReloadFromDisk");
	LOG_EXIT("CppStubReloadFromDisk");
}

bool VRChaperoneSetupCppStub::GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose)
{
	LOG_ENTRY("CppStubGetLiveSeatedZeroPoseToRawTrackingPose");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetLiveSeatedZeroPoseToRawTrackingPose");
}

void VRChaperoneSetupCppStub::SetWorkingCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t unTagCount)
{
	LOG_ENTRY("CppStubSetWorkingCollisionBoundsTagsInfo");
	LOG_EXIT("CppStubSetWorkingCollisionBoundsTagsInfo");
}

bool VRChaperoneSetupCppStub::GetLiveCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t * punTagCount)
{
	LOG_ENTRY("CppStubGetLiveCollisionBoundsTagsInfo");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetLiveCollisionBoundsTagsInfo");
}

bool VRChaperoneSetupCppStub::SetWorkingPhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount)
{
	LOG_ENTRY("CppStubSetWorkingPhysicalBoundsInfo");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubSetWorkingPhysicalBoundsInfo");
}

bool VRChaperoneSetupCppStub::GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
	LOG_ENTRY("CppStubGetLivePhysicalBoundsInfo");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetLivePhysicalBoundsInfo");
}

bool VRChaperoneSetupCppStub::ExportLiveToBuffer(char * pBuffer, uint32_t * pnBufferLength)
{
	LOG_ENTRY("CppStubExportLiveToBuffer");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubExportLiveToBuffer");
}

bool VRChaperoneSetupCppStub::ImportFromBufferToWorking(const char * pBuffer, uint32_t nImportFlags)
{
	LOG_ENTRY("CppStubImportFromBufferToWorking");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubImportFromBufferToWorking");
}

void VRCompositorCppStub::SetTrackingSpace(vr::ETrackingUniverseOrigin eOrigin)
{
	LOG_ENTRY("CppStubSetTrackingSpace");
	LOG_EXIT("CppStubSetTrackingSpace");
}

vr::ETrackingUniverseOrigin VRCompositorCppStub::GetTrackingSpace()
{
	LOG_ENTRY("CppStubGetTrackingSpace");
	static vr::ETrackingUniverseOrigin rc;
	LOG_EXIT_RC(rc, "CppStubGetTrackingSpace");
}

vr::EVRCompositorError VRCompositorCppStub::WaitGetPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
	LOG_ENTRY("CppStubWaitGetPoses");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubWaitGetPoses");
}

vr::EVRCompositorError VRCompositorCppStub::GetLastPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
	LOG_ENTRY("CppStubGetLastPoses");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubGetLastPoses");
}

vr::EVRCompositorError VRCompositorCppStub::GetLastPoseForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex, struct vr::TrackedDevicePose_t * pOutputPose, struct vr::TrackedDevicePose_t * pOutputGamePose)
{
	LOG_ENTRY("CppStubGetLastPoseForTrackedDeviceIndex");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubGetLastPoseForTrackedDeviceIndex");
}

vr::EVRCompositorError VRCompositorCppStub::Submit(vr::EVREye eEye, const struct vr::Texture_t * pTexture, const struct vr::VRTextureBounds_t * pBounds, vr::EVRSubmitFlags nSubmitFlags)
{
	LOG_ENTRY("CppStubSubmit");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubSubmit");
}

void VRCompositorCppStub::ClearLastSubmittedFrame()
{
	LOG_ENTRY("CppStubClearLastSubmittedFrame");
	LOG_EXIT("CppStubClearLastSubmittedFrame");
}

void VRCompositorCppStub::PostPresentHandoff()
{
	LOG_ENTRY("CppStubPostPresentHandoff");
	LOG_EXIT("CppStubPostPresentHandoff");
}

bool VRCompositorCppStub::GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo)
{
	LOG_ENTRY("CppStubGetFrameTiming");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetFrameTiming");
}

uint32_t VRCompositorCppStub::GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames)
{
	LOG_ENTRY("CppStubGetFrameTimings");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetFrameTimings");
}

float VRCompositorCppStub::GetFrameTimeRemaining()
{
	LOG_ENTRY("CppStubGetFrameTimeRemaining");
	static float rc;
	LOG_EXIT_RC(rc, "CppStubGetFrameTimeRemaining");
}

void VRCompositorCppStub::GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes)
{
	LOG_ENTRY("CppStubGetCumulativeStats");
	LOG_EXIT("CppStubGetCumulativeStats");
}

void VRCompositorCppStub::FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground)
{
	LOG_ENTRY("CppStubFadeToColor");
	LOG_EXIT("CppStubFadeToColor");
}

struct vr::HmdColor_t VRCompositorCppStub::GetCurrentFadeColor(bool bBackground)
{
	LOG_ENTRY("CppStubGetCurrentFadeColor");
	static struct vr::HmdColor_t rc;
	LOG_EXIT_RC(rc, "CppStubGetCurrentFadeColor");
}

void VRCompositorCppStub::FadeGrid(float fSeconds, bool bFadeIn)
{
	LOG_ENTRY("CppStubFadeGrid");
	LOG_EXIT("CppStubFadeGrid");
}

float VRCompositorCppStub::GetCurrentGridAlpha()
{
	LOG_ENTRY("CppStubGetCurrentGridAlpha");
	static float rc;
	LOG_EXIT_RC(rc, "CppStubGetCurrentGridAlpha");
}

vr::EVRCompositorError VRCompositorCppStub::SetSkyboxOverride(const struct vr::Texture_t * pTextures, uint32_t unTextureCount)
{
	LOG_ENTRY("CppStubSetSkyboxOverride");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubSetSkyboxOverride");
}

void VRCompositorCppStub::ClearSkyboxOverride()
{
	LOG_ENTRY("CppStubClearSkyboxOverride");
	LOG_EXIT("CppStubClearSkyboxOverride");
}

void VRCompositorCppStub::CompositorBringToFront()
{
	LOG_ENTRY("CppStubCompositorBringToFront");
	LOG_EXIT("CppStubCompositorBringToFront");
}

void VRCompositorCppStub::CompositorGoToBack()
{
	LOG_ENTRY("CppStubCompositorGoToBack");
	LOG_EXIT("CppStubCompositorGoToBack");
}

void VRCompositorCppStub::CompositorQuit()
{
	LOG_ENTRY("CppStubCompositorQuit");
	LOG_EXIT("CppStubCompositorQuit");
}

bool VRCompositorCppStub::IsFullscreen()
{
	LOG_ENTRY("CppStubIsFullscreen");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsFullscreen");
}

uint32_t VRCompositorCppStub::GetCurrentSceneFocusProcess()
{
	LOG_ENTRY("CppStubGetCurrentSceneFocusProcess");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetCurrentSceneFocusProcess");
}

uint32_t VRCompositorCppStub::GetLastFrameRenderer()
{
	LOG_ENTRY("CppStubGetLastFrameRenderer");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetLastFrameRenderer");
}

bool VRCompositorCppStub::CanRenderScene()
{
	LOG_ENTRY("CppStubCanRenderScene");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubCanRenderScene");
}

void VRCompositorCppStub::ShowMirrorWindow()
{
	LOG_ENTRY("CppStubShowMirrorWindow");
	LOG_EXIT("CppStubShowMirrorWindow");
}

void VRCompositorCppStub::HideMirrorWindow()
{
	LOG_ENTRY("CppStubHideMirrorWindow");
	LOG_EXIT("CppStubHideMirrorWindow");
}

bool VRCompositorCppStub::IsMirrorWindowVisible()
{
	LOG_ENTRY("CppStubIsMirrorWindowVisible");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsMirrorWindowVisible");
}

void VRCompositorCppStub::CompositorDumpImages()
{
	LOG_ENTRY("CppStubCompositorDumpImages");
	LOG_EXIT("CppStubCompositorDumpImages");
}

bool VRCompositorCppStub::ShouldAppRenderWithLowResources()
{
	LOG_ENTRY("CppStubShouldAppRenderWithLowResources");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubShouldAppRenderWithLowResources");
}

void VRCompositorCppStub::ForceInterleavedReprojectionOn(bool bOverride)
{
	LOG_ENTRY("CppStubForceInterleavedReprojectionOn");
	LOG_EXIT("CppStubForceInterleavedReprojectionOn");
}

void VRCompositorCppStub::ForceReconnectProcess()
{
	LOG_ENTRY("CppStubForceReconnectProcess");
	LOG_EXIT("CppStubForceReconnectProcess");
}

void VRCompositorCppStub::SuspendRendering(bool bSuspend)
{
	LOG_ENTRY("CppStubSuspendRendering");
	LOG_EXIT("CppStubSuspendRendering");
}

vr::EVRCompositorError VRCompositorCppStub::GetMirrorTextureD3D11(vr::EVREye eEye, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView)
{
	LOG_ENTRY("CppStubGetMirrorTextureD3D11");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubGetMirrorTextureD3D11");
}

void VRCompositorCppStub::ReleaseMirrorTextureD3D11(void * pD3D11ShaderResourceView)
{
	LOG_ENTRY("CppStubReleaseMirrorTextureD3D11");
	LOG_EXIT("CppStubReleaseMirrorTextureD3D11");
}

vr::EVRCompositorError VRCompositorCppStub::GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t * pglTextureId, vr::glSharedTextureHandle_t * pglSharedTextureHandle)
{
	LOG_ENTRY("CppStubGetMirrorTextureGL");
	static vr::EVRCompositorError rc;
	LOG_EXIT_RC(rc, "CppStubGetMirrorTextureGL");
}

bool VRCompositorCppStub::ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle)
{
	LOG_ENTRY("CppStubReleaseSharedGLTexture");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubReleaseSharedGLTexture");
}

void VRCompositorCppStub::LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle)
{
	LOG_ENTRY("CppStubLockGLSharedTextureForAccess");
	LOG_EXIT("CppStubLockGLSharedTextureForAccess");
}

void VRCompositorCppStub::UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle)
{
	LOG_ENTRY("CppStubUnlockGLSharedTextureForAccess");
	LOG_EXIT("CppStubUnlockGLSharedTextureForAccess");
}

uint32_t VRCompositorCppStub::GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CppStubGetVulkanInstanceExtensionsRequired");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetVulkanInstanceExtensionsRequired");
}

uint32_t VRCompositorCppStub::GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CppStubGetVulkanDeviceExtensionsRequired");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetVulkanDeviceExtensionsRequired");
}

void VRCompositorCppStub::SetExplicitTimingMode(vr::EVRCompositorTimingMode eTimingMode)
{
	LOG_ENTRY("CppStubSetExplicitTimingMode");
	LOG_EXIT("CppStubSetExplicitTimingMode");
}

vr::EVRCompositorError VRCompositorCppStub::SubmitExplicitTimingData()
{
	LOG_ENTRY("CppStubSubmitExplicitTimingData");
	static vr::EVRCompositorError rc = vr::VRCompositorError_None;
	LOG_EXIT_RC(rc, "CppStubSubmitExplicitTimingData");
}

vr::EVROverlayError VROverlayCppStub::FindOverlay(const char * pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle)
{
	LOG_ENTRY("CppStubFindOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubFindOverlay");
}

vr::EVROverlayError VROverlayCppStub::CreateOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pOverlayHandle)
{
	LOG_ENTRY("CppStubCreateOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubCreateOverlay");
}

vr::EVROverlayError VROverlayCppStub::DestroyOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubDestroyOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubDestroyOverlay");
}

vr::EVROverlayError VROverlayCppStub::SetHighQualityOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubSetHighQualityOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetHighQualityOverlay");
}

vr::VROverlayHandle_t VROverlayCppStub::GetHighQualityOverlay()
{
	LOG_ENTRY("CppStubGetHighQualityOverlay");
	static vr::VROverlayHandle_t rc;
	LOG_EXIT_RC(rc, "CppStubGetHighQualityOverlay");
}

uint32_t VROverlayCppStub::GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
	LOG_ENTRY("CppStubGetOverlayKey");
	static uint32_t rc = 0;
	if (pError) { *pError = (vr::EVROverlayError)0; }
	LOG_EXIT_RC(rc, "CppStubGetOverlayKey");
}

uint32_t VROverlayCppStub::GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
	LOG_ENTRY("CppStubGetOverlayName");
	static uint32_t rc = 0;
	if (pError) { *pError = (vr::EVROverlayError)0; }
	LOG_EXIT_RC(rc, "CppStubGetOverlayName");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, const char *pchName)
{
	LOG_ENTRY("SetOverlayName");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "SetOverlayName");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayImageData(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight)
{
	LOG_ENTRY("CppStubGetOverlayImageData");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayImageData");
}

const char * VROverlayCppStub::GetOverlayErrorNameFromEnum(vr::EVROverlayError error)
{
	LOG_ENTRY("CppStubGetOverlayErrorNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetOverlayErrorNameFromEnum");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unPID)
{
	LOG_ENTRY("CppStubSetOverlayRenderingPid");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayRenderingPid");
}

uint32_t VROverlayCppStub::GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubGetOverlayRenderingPid");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetOverlayRenderingPid");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool bEnabled)
{
	LOG_ENTRY("CppStubSetOverlayFlag");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayFlag");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool * pbEnabled)
{
	LOG_ENTRY("CppStubGetOverlayFlag");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayFlag");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue)
{
	LOG_ENTRY("CppStubSetOverlayColor");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayColor");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue)
{
	LOG_ENTRY("CppStubGetOverlayColor");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayColor");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float fAlpha)
{
	LOG_ENTRY("CppStubSetOverlayAlpha");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayAlpha");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha)
{
	LOG_ENTRY("CppStubGetOverlayAlpha");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayAlpha");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float fTexelAspect)
{
	LOG_ENTRY("CppStubSetOverlayTexelAspect");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTexelAspect");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect)
{
	LOG_ENTRY("CppStubGetOverlayTexelAspect");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTexelAspect");
}

vr::EVROverlayError VROverlayCppStub::SetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder)
{
	LOG_ENTRY("CppStubSetOverlaySortOrder");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlaySortOrder");
}

vr::EVROverlayError VROverlayCppStub::GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder)
{
	LOG_ENTRY("CppStubGetOverlaySortOrder");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlaySortOrder");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fWidthInMeters)
{
	LOG_ENTRY("CppStubSetOverlayWidthInMeters");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayWidthInMeters");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters)
{
	LOG_ENTRY("CppStubGetOverlayWidthInMeters");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayWidthInMeters");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters)
{
	LOG_ENTRY("CppStubSetOverlayAutoCurveDistanceRangeInMeters");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayAutoCurveDistanceRangeInMeters");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters)
{
	LOG_ENTRY("CppStubGetOverlayAutoCurveDistanceRangeInMeters");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayAutoCurveDistanceRangeInMeters");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace eTextureColorSpace)
{
	LOG_ENTRY("CppStubSetOverlayTextureColorSpace");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTextureColorSpace");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace)
{
	LOG_ENTRY("CppStubGetOverlayTextureColorSpace");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureColorSpace");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VRTextureBounds_t * pOverlayTextureBounds)
{
	LOG_ENTRY("CppStubSetOverlayTextureBounds");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTextureBounds");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VRTextureBounds_t * pOverlayTextureBounds)
{
	LOG_ENTRY("CppStubGetOverlayTextureBounds");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureBounds");
}

uint32_t VROverlayCppStub::GetOverlayRenderModel(vr::VROverlayHandle_t ulOverlayHandle, char *pchValue, uint32_t unBufferSize, vr::HmdColor_t *pColor, vr::EVROverlayError *pError)
{
	LOG_ENTRY("CppStubGetOverlayRenderModel");
	static uint32_t rc;
	LOG_EXIT_RC(rc, "CppGetOverlayRenderModel");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayRenderModel(vr::VROverlayHandle_t ulOverlayHandle, const char *pchRenderModel, const vr::HmdColor_t *pColor)
{
	LOG_ENTRY("CppStubSetOverlayRenderModel");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppSetOverlayRenderModel");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType)
{
	LOG_ENTRY("CppStubGetOverlayTransformType");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformType");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform)
{
	LOG_ENTRY("CppStubSetOverlayTransformAbsolute");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTransformAbsolute");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin * peTrackingOrigin, struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform)
{
	LOG_ENTRY("CppStubGetOverlayTransformAbsolute");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformAbsolute");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unTrackedDevice, const struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform)
{
	LOG_ENTRY("CppStubSetOverlayTransformTrackedDeviceRelative");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTransformTrackedDeviceRelative");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punTrackedDevice, struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform)
{
	LOG_ENTRY("CppStubGetOverlayTransformTrackedDeviceRelative");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformTrackedDeviceRelative");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchComponentName)
{
	LOG_ENTRY("CppStubSetOverlayTransformTrackedDeviceComponent");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTransformTrackedDeviceComponent");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punDeviceIndex, char * pchComponentName, uint32_t unComponentNameSize)
{
	LOG_ENTRY("CppStubGetOverlayTransformTrackedDeviceComponent");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformTrackedDeviceComponent");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTransformOverlayRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayHandle_t *ulOverlayHandleParent, vr::HmdMatrix34_t *pmatParentOverlayToOverlayTransform)
{
	LOG_ENTRY("CppStubGetOverlayTransformOverlayRelative");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTransformOverlayRelative");
}
vr::EVROverlayError VROverlayCppStub::SetOverlayTransformOverlayRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayHandle_t ulOverlayHandleParent, const vr::HmdMatrix34_t *pmatParentOverlayToOverlayTransform)
{
	LOG_ENTRY("CppStubSetOverlayTransformOverlayRelative");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTransformOverlayRelative");
}

vr::EVROverlayError VROverlayCppStub::ShowOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubShowOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubShowOverlay");
}

vr::EVROverlayError VROverlayCppStub::HideOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubHideOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubHideOverlay");
}

bool VROverlayCppStub::IsOverlayVisible(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubIsOverlayVisible");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsOverlayVisible");
}

vr::EVROverlayError VROverlayCppStub::GetTransformForOverlayCoordinates(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, struct vr::HmdVector2_t coordinatesInOverlay, struct vr::HmdMatrix34_t * pmatTransform)
{
	LOG_ENTRY("CppStubGetTransformForOverlayCoordinates");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetTransformForOverlayCoordinates");
}

bool VROverlayCppStub::PollNextOverlayEvent(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent)
{
	LOG_ENTRY("CppStubPollNextOverlayEvent");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubPollNextOverlayEvent");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod)
{
	LOG_ENTRY("CppStubGetOverlayInputMethod");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayInputMethod");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod eInputMethod)
{
	LOG_ENTRY("CppStubSetOverlayInputMethod");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayInputMethod");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale)
{
	LOG_ENTRY("CppStubGetOverlayMouseScale");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayMouseScale");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::HmdVector2_t * pvecMouseScale)
{
	LOG_ENTRY("CppStubSetOverlayMouseScale");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayMouseScale");
}

bool VROverlayCppStub::ComputeOverlayIntersection(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VROverlayIntersectionParams_t * pParams, struct vr::VROverlayIntersectionResults_t * pResults)
{
	LOG_ENTRY("CppStubComputeOverlayIntersection");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubComputeOverlayIntersection");
}

bool VROverlayCppStub::IsHoverTargetOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubIsHoverTargetOverlay");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsHoverTargetOverlay");
}

vr::VROverlayHandle_t VROverlayCppStub::GetGamepadFocusOverlay()
{
	LOG_ENTRY("CppStubGetGamepadFocusOverlay");
	static vr::VROverlayHandle_t rc;
	LOG_EXIT_RC(rc, "CppStubGetGamepadFocusOverlay");
}

vr::EVROverlayError VROverlayCppStub::SetGamepadFocusOverlay(vr::VROverlayHandle_t ulNewFocusOverlay)
{
	LOG_ENTRY("CppStubSetGamepadFocusOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetGamepadFocusOverlay");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom, vr::VROverlayHandle_t ulTo)
{
	LOG_ENTRY("CppStubSetOverlayNeighbor");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayNeighbor");
}

vr::EVROverlayError VROverlayCppStub::MoveGamepadFocusToNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom)
{
	LOG_ENTRY("CppStubMoveGamepadFocusToNeighbor");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubMoveGamepadFocusToNeighbor");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayDualAnalogTransform(vr::VROverlayHandle_t ulOverlay, vr::EDualAnalogWhich eWhich, const vr::HmdVector2_t & vCenter, float fRadius)
{
	LOG_ENTRY("SetOverlayDualAnalogTransform");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "SetOverlayDualAnalogTransform");
}
vr::EVROverlayError VROverlayCppStub::GetOverlayDualAnalogTransform(vr::VROverlayHandle_t ulOverlay, vr::EDualAnalogWhich eWhich, vr::HmdVector2_t *pvCenter, float *pfRadius)
{
	LOG_ENTRY("GetOverlayDualAnalogTransform");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "GetOverlayDualAnalogTransform");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::Texture_t * pTexture)
{
	LOG_ENTRY("CppStubSetOverlayTexture");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayTexture");
}

vr::EVROverlayError VROverlayCppStub::ClearOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubClearOverlayTexture");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubClearOverlayTexture");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayRaw(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth)
{
	LOG_ENTRY("CppStubSetOverlayRaw");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayRaw");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayFromFile(vr::VROverlayHandle_t ulOverlayHandle, const char * pchFilePath)
{
	LOG_ENTRY("CppStubSetOverlayFromFile");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayFromFile");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, void ** pNativeTextureHandle, void * pNativeTextureRef, uint32_t * pWidth, uint32_t * pHeight, uint32_t * pNativeFormat, vr::ETextureType * pAPIType, vr::EColorSpace * pColorSpace, struct vr::VRTextureBounds_t * pTextureBounds)
{
	LOG_ENTRY("CppStubGetOverlayTexture");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTexture");
}

vr::EVROverlayError VROverlayCppStub::ReleaseNativeOverlayHandle(vr::VROverlayHandle_t ulOverlayHandle, void * pNativeTextureHandle)
{
	LOG_ENTRY("CppStubReleaseNativeOverlayHandle");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubReleaseNativeOverlayHandle");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight)
{
	LOG_ENTRY("CppStubGetOverlayTextureSize");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayTextureSize");
}

vr::EVROverlayError VROverlayCppStub::CreateDashboardOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pMainHandle, vr::VROverlayHandle_t * pThumbnailHandle)
{
	LOG_ENTRY("CppStubCreateDashboardOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubCreateDashboardOverlay");
}

bool VROverlayCppStub::IsDashboardVisible()
{
	LOG_ENTRY("CppStubIsDashboardVisible");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsDashboardVisible");
}

bool VROverlayCppStub::IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
	LOG_ENTRY("CppStubIsActiveDashboardOverlay");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubIsActiveDashboardOverlay");
}

vr::EVROverlayError VROverlayCppStub::SetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId)
{
	LOG_ENTRY("CppStubSetDashboardOverlaySceneProcess");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetDashboardOverlaySceneProcess");
}

vr::EVROverlayError VROverlayCppStub::GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId)
{
	LOG_ENTRY("CppStubGetDashboardOverlaySceneProcess");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetDashboardOverlaySceneProcess");
}

void VROverlayCppStub::ShowDashboard(const char * pchOverlayToShow)
{
	LOG_ENTRY("CppStubShowDashboard");
	LOG_EXIT("CppStubShowDashboard");
}

vr::TrackedDeviceIndex_t VROverlayCppStub::GetPrimaryDashboardDevice()
{
	LOG_ENTRY("CppStubGetPrimaryDashboardDevice");
	static vr::TrackedDeviceIndex_t rc;
	LOG_EXIT_RC(rc, "CppStubGetPrimaryDashboardDevice");
}

vr::EVROverlayError VROverlayCppStub::ShowKeyboard(vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue)
{
	LOG_ENTRY("CppStubShowKeyboard");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubShowKeyboard");
}

vr::EVROverlayError VROverlayCppStub::ShowKeyboardForOverlay(vr::VROverlayHandle_t ulOverlayHandle, vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue)
{
	LOG_ENTRY("CppStubShowKeyboardForOverlay");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubShowKeyboardForOverlay");
}

uint32_t VROverlayCppStub::GetKeyboardText(char * pchText, uint32_t cchText)
{
	LOG_ENTRY("CppStubGetKeyboardText");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetKeyboardText");
}

void VROverlayCppStub::HideKeyboard()
{
	LOG_ENTRY("CppStubHideKeyboard");
	LOG_EXIT("CppStubHideKeyboard");
}

void VROverlayCppStub::SetKeyboardTransformAbsolute(vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToKeyboardTransform)
{
	LOG_ENTRY("CppStubSetKeyboardTransformAbsolute");
	LOG_EXIT("CppStubSetKeyboardTransformAbsolute");
}

void VROverlayCppStub::SetKeyboardPositionForOverlay(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdRect2_t avoidRect)
{
	LOG_ENTRY("CppStubSetKeyboardPositionForOverlay");
	LOG_EXIT("CppStubSetKeyboardPositionForOverlay");
}

vr::EVROverlayError VROverlayCppStub::SetOverlayIntersectionMask(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VROverlayIntersectionMaskPrimitive_t * pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize)
{
	LOG_ENTRY("CppStubSetOverlayIntersectionMask");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubSetOverlayIntersectionMask");
}

vr::EVROverlayError VROverlayCppStub::GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags)
{
	LOG_ENTRY("CppStubGetOverlayFlags");
	static vr::EVROverlayError rc;
	LOG_EXIT_RC(rc, "CppStubGetOverlayFlags");
}

vr::VRMessageOverlayResponse VROverlayCppStub::ShowMessageOverlay(const char * pchText, const char * pchCaption, const char * pchButton0Text, const char * pchButton1Text, const char * pchButton2Text, const char * pchButton3Text)
{
	LOG_ENTRY("CppStubShowMessageOverlay");
	static vr::VRMessageOverlayResponse rc;
	LOG_EXIT_RC(rc, "CppStubShowMessageOverlay");
}

void VROverlayCppStub::CloseMessageOverlay()
{
	LOG_ENTRY("CloseMessageOverlay");
	LOG_EXIT("CloseMessageOverlay");
}

vr::EVRRenderModelError VRRenderModelsCppStub::LoadRenderModel_Async(const char * pchRenderModelName, struct vr::RenderModel_t ** ppRenderModel)
{
	LOG_ENTRY("CppStubLoadRenderModel_Async");
	static vr::EVRRenderModelError rc;
	LOG_EXIT_RC(rc, "CppStubLoadRenderModel_Async");
}

void VRRenderModelsCppStub::FreeRenderModel(struct vr::RenderModel_t * pRenderModel)
{
	LOG_ENTRY("CppStubFreeRenderModel");
	LOG_EXIT("CppStubFreeRenderModel");
}

vr::EVRRenderModelError VRRenderModelsCppStub::LoadTexture_Async(vr::TextureID_t textureId, struct vr::RenderModel_TextureMap_t ** ppTexture)
{
	LOG_ENTRY("CppStubLoadTexture_Async");
	static vr::EVRRenderModelError rc;
	LOG_EXIT_RC(rc, "CppStubLoadTexture_Async");
}

void VRRenderModelsCppStub::FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture)
{
	LOG_ENTRY("CppStubFreeTexture");
	LOG_EXIT("CppStubFreeTexture");
}

vr::EVRRenderModelError VRRenderModelsCppStub::LoadTextureD3D11_Async(vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D)
{
	LOG_ENTRY("CppStubLoadTextureD3D11_Async");
	static vr::EVRRenderModelError rc;
	LOG_EXIT_RC(rc, "CppStubLoadTextureD3D11_Async");
}

vr::EVRRenderModelError VRRenderModelsCppStub::LoadIntoTextureD3D11_Async(vr::TextureID_t textureId, void * pDstTexture)
{
	LOG_ENTRY("CppStubLoadIntoTextureD3D11_Async");
	static vr::EVRRenderModelError rc;
	LOG_EXIT_RC(rc, "CppStubLoadIntoTextureD3D11_Async");
}

void VRRenderModelsCppStub::FreeTextureD3D11(void * pD3D11Texture2D)
{
	LOG_ENTRY("CppStubFreeTextureD3D11");
	LOG_EXIT("CppStubFreeTextureD3D11");
}

uint32_t VRRenderModelsCppStub::GetRenderModelName(uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen)
{
	LOG_ENTRY("CppStubGetRenderModelName");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetRenderModelName");
}

uint32_t VRRenderModelsCppStub::GetRenderModelCount()
{
	LOG_ENTRY("CppStubGetRenderModelCount");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetRenderModelCount");
}

uint32_t VRRenderModelsCppStub::GetComponentCount(const char * pchRenderModelName)
{
	LOG_ENTRY("CppStubGetComponentCount");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetComponentCount");
}

uint32_t VRRenderModelsCppStub::GetComponentName(const char * pchRenderModelName, uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen)
{
	LOG_ENTRY("CppStubGetComponentName");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetComponentName");
}

uint64_t VRRenderModelsCppStub::GetComponentButtonMask(const char * pchRenderModelName, const char * pchComponentName)
{
	LOG_ENTRY("CppStubGetComponentButtonMask");
	static uint64_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetComponentButtonMask");
}

uint32_t VRRenderModelsCppStub::GetComponentRenderModelName(const char * pchRenderModelName, const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen)
{
	LOG_ENTRY("CppStubGetComponentRenderModelName");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetComponentRenderModelName");
}

bool VRRenderModelsCppStub::GetComponentState(const char * pchRenderModelName, const char * pchComponentName, const vr::VRControllerState_t * pControllerState, const struct vr::RenderModel_ControllerMode_State_t * pState, struct vr::RenderModel_ComponentState_t * pComponentState)
{
	LOG_ENTRY("CppStubGetComponentState");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubGetComponentState");
}

bool VRRenderModelsCppStub::RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName)
{
	LOG_ENTRY("CppStubRenderModelHasComponent");
	static bool rc = true;
	LOG_EXIT_RC(rc, "CppStubRenderModelHasComponent");
}

uint32_t VRRenderModelsCppStub::GetRenderModelThumbnailURL(const char * pchRenderModelName, char * pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError)
{
	LOG_ENTRY("CppStubGetRenderModelThumbnailURL");
	static uint32_t rc = 0;
	if (peError) { *peError = (vr::EVRRenderModelError)0; }
	LOG_EXIT_RC(rc, "CppStubGetRenderModelThumbnailURL");
}

uint32_t VRRenderModelsCppStub::GetRenderModelOriginalPath(const char * pchRenderModelName, char * pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError * peError)
{
	LOG_ENTRY("CppStubGetRenderModelOriginalPath");
	static uint32_t rc = 0;
	if (peError) { *peError = (vr::EVRRenderModelError)0; }
	LOG_EXIT_RC(rc, "CppStubGetRenderModelOriginalPath");
}

const char * VRRenderModelsCppStub::GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error)
{
	LOG_ENTRY("CppStubGetRenderModelErrorNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetRenderModelErrorNameFromEnum");
}

vr::EVRNotificationError VRNotificationsCppStub::CreateNotification(vr::VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, vr::EVRNotificationType type, const char * pchText, vr::EVRNotificationStyle style, const struct vr::NotificationBitmap_t * pImage, vr::VRNotificationId * pNotificationId)
{
	LOG_ENTRY("CppStubCreateNotification");
	static vr::EVRNotificationError rc;
	LOG_EXIT_RC(rc, "CppStubCreateNotification");
}

vr::EVRNotificationError VRNotificationsCppStub::RemoveNotification(vr::VRNotificationId notificationId)
{
	LOG_ENTRY("CppStubRemoveNotification");
	static vr::EVRNotificationError rc;
	LOG_EXIT_RC(rc, "CppStubRemoveNotification");
}

const char * VRSettingsCppStub::GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError)
{
	LOG_ENTRY("CppStubGetSettingsErrorNameFromEnum");
	static const char * rc = "";
	LOG_EXIT_RC(rc, "CppStubGetSettingsErrorNameFromEnum");
}

bool VRSettingsCppStub::Sync(bool bForce, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubSync");
	static bool rc = true;
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT_RC(rc, "CppStubSync");
}

void VRSettingsCppStub::SetBool(const char * pchSection, const char * pchSettingsKey, bool bValue, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubSetBool");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubSetBool");
}

void VRSettingsCppStub::SetInt32(const char * pchSection, const char * pchSettingsKey, int32_t nValue, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubSetInt32");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubSetInt32");
}

void VRSettingsCppStub::SetFloat(const char * pchSection, const char * pchSettingsKey, float flValue, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubSetFloat");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubSetFloat");
}

void VRSettingsCppStub::SetString(const char * pchSection, const char * pchSettingsKey, const char * pchValue, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubSetString");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubSetString");
}

bool VRSettingsCppStub::GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubGetBool");
	static bool rc = true;
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT_RC(rc, "CppStubGetBool");
}

int32_t VRSettingsCppStub::GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubGetInt32");
	static int32_t rc = 0;
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT_RC(rc, "CppStubGetInt32");
}

float VRSettingsCppStub::GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubGetFloat");
	static float rc;
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT_RC(rc, "CppStubGetFloat");
}

void VRSettingsCppStub::GetString(const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubGetString");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubGetString");
}

void VRSettingsCppStub::RemoveSection(const char * pchSection, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubRemoveSection");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubRemoveSection");
}

void VRSettingsCppStub::RemoveKeyInSection(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
	LOG_ENTRY("CppStubRemoveKeyInSection");
	if (peError) { *peError = (vr::EVRSettingsError)0; }
	LOG_EXIT("CppStubRemoveKeyInSection");
}

vr::EVRScreenshotError VRScreenshotsCppStub::RequestScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, vr::EVRScreenshotType type, const char * pchPreviewFilename, const char * pchVRFilename)
{
	LOG_ENTRY("CppStubRequestScreenshot");
	static vr::EVRScreenshotError rc;
	LOG_EXIT_RC(rc, "CppStubRequestScreenshot");
}

vr::EVRScreenshotError VRScreenshotsCppStub::HookScreenshot(const vr::EVRScreenshotType * pSupportedTypes, int numTypes)
{
	LOG_ENTRY("CppStubHookScreenshot");
	static vr::EVRScreenshotError rc;
	LOG_EXIT_RC(rc, "CppStubHookScreenshot");
}

vr::EVRScreenshotType VRScreenshotsCppStub::GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError * pError)
{
	LOG_ENTRY("CppStubGetScreenshotPropertyType");
	static vr::EVRScreenshotType rc;
	if (pError) { *pError = (vr::EVRScreenshotError)0; }
	LOG_EXIT_RC(rc, "CppStubGetScreenshotPropertyType");
}

uint32_t VRScreenshotsCppStub::GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, char * pchFilename, uint32_t cchFilename, vr::EVRScreenshotError * pError)
{
	LOG_ENTRY("CppStubGetScreenshotPropertyFilename");
	static uint32_t rc = 0;
	if (pError) { *pError = (vr::EVRScreenshotError)0; }
	LOG_EXIT_RC(rc, "CppStubGetScreenshotPropertyFilename");
}

vr::EVRScreenshotError VRScreenshotsCppStub::UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress)
{
	LOG_ENTRY("CppStubUpdateScreenshotProgress");
	static vr::EVRScreenshotError rc;
	LOG_EXIT_RC(rc, "CppStubUpdateScreenshotProgress");
}

vr::EVRScreenshotError VRScreenshotsCppStub::TakeStereoScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, const char * pchPreviewFilename, const char * pchVRFilename)
{
	LOG_ENTRY("CppStubTakeStereoScreenshot");
	static vr::EVRScreenshotError rc;
	LOG_EXIT_RC(rc, "CppStubTakeStereoScreenshot");
}

vr::EVRScreenshotError VRScreenshotsCppStub::SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char * pchSourcePreviewFilename, const char * pchSourceVRFilename)
{
	LOG_ENTRY("CppStubSubmitScreenshot");
	static vr::EVRScreenshotError rc;
	LOG_EXIT_RC(rc, "CppStubSubmitScreenshot");
}

uint32_t VRResourcesCppStub::LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen)
{
	LOG_ENTRY("CppStubLoadSharedResource");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubLoadSharedResource");
}

uint32_t VRResourcesCppStub::GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory, char * pchPathBuffer, uint32_t unBufferLen)
{
	LOG_ENTRY("CppStubGetResourceFullPath");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetResourceFullPath");
}

uint32_t VRDriverManagerCppStub::GetDriverCount() const
{
	LOG_ENTRY("CppStubGetDriverCount");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetDriverCount");
}

uint32_t VRDriverManagerCppStub::GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CppStubGetDriverName");
	static uint32_t rc = 0;
	LOG_EXIT_RC(rc, "CppStubGetDriverName");
}