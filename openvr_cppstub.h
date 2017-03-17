#pragma once
#include <openvr.h>
class VRSystemCppStub : public vr::IVRSystem
{
public:
  void GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight) override;
  struct vr::HmdMatrix44_t GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ) override;
  void GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom) override;
  bool ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates) override;
  struct vr::HmdMatrix34_t GetEyeToHeadTransform(vr::EVREye eEye) override;
  bool GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter) override;
  int32_t GetD3D9AdapterIndex() override;
  void GetDXGIOutputInfo(int32_t * pnAdapterIndex) override;
  bool IsDisplayOnDesktop() override;
  bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override;
  void GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override;
  void ResetSeatedZeroPose() override;
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
  void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) override;
  const char * GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId) override;
  const char * GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType) override;
  bool CaptureInputFocus() override;
  void ReleaseInputFocus() override;
  bool IsInputFocusCapturedByAnotherProcess() override;
  uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize) override;
  vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override;
  void AcknowledgeQuit_Exiting() override;
  void AcknowledgeQuit_UserPrompt() override;
};
class VRExtendedDisplayCppStub : public vr::IVRExtendedDisplay
{
public:
  void GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
  void GetEyeOutputViewport(vr::EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
  void GetDXGIOutputInfo(int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex) override;
};
class VRTrackedCameraCppStub : public vr::IVRTrackedCamera
{
public:
  const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) override;
  vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera) override;
  vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize) override;
  vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t * pFocalLength, vr::HmdVector2_t * pCenter) override;
  vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection) override;
  vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t * pHandle) override;
  vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) override;
  vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) override;
  vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t * pTextureBounds, uint32_t * pnWidth, uint32_t * pnHeight) override;
  vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) override;
  vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t * pglTextureId, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) override;
  vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) override;
};
class VRApplicationsCppStub : public vr::IVRApplications
{
public:
  vr::EVRApplicationError AddApplicationManifest(const char * pchApplicationManifestFullPath, bool bTemporary) override;
  vr::EVRApplicationError RemoveApplicationManifest(const char * pchApplicationManifestFullPath) override;
  bool IsApplicationInstalled(const char * pchAppKey) override;
  uint32_t GetApplicationCount() override;
  vr::EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
  vr::EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
  vr::EVRApplicationError LaunchApplication(const char * pchAppKey) override;
  vr::EVRApplicationError LaunchTemplateApplication(const char * pchTemplateAppKey, const char * pchNewAppKey, const struct vr::AppOverrideKeys_t * pKeys, uint32_t unKeys) override;
  vr::EVRApplicationError LaunchApplicationFromMimeType(const char * pchMimeType, const char * pchArgs) override;
  vr::EVRApplicationError LaunchDashboardOverlay(const char * pchAppKey) override;
  bool CancelApplicationLaunch(const char * pchAppKey) override;
  vr::EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char * pchAppKey) override;
  uint32_t GetApplicationProcessId(const char * pchAppKey) override;
  const char * GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error) override;
  uint32_t GetApplicationPropertyString(const char * pchAppKey, vr::EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, vr::EVRApplicationError * peError) override;
  bool GetApplicationPropertyBool(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) override;
  uint64_t GetApplicationPropertyUint64(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) override;
  vr::EVRApplicationError SetApplicationAutoLaunch(const char * pchAppKey, bool bAutoLaunch) override;
  bool GetApplicationAutoLaunch(const char * pchAppKey) override;
  vr::EVRApplicationError SetDefaultApplicationForMimeType(const char * pchAppKey, const char * pchMimeType) override;
  bool GetDefaultApplicationForMimeType(const char * pchMimeType, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
  bool GetApplicationSupportedMimeTypes(const char * pchAppKey, char * pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override;
  uint32_t GetApplicationsThatSupportMimeType(const char * pchMimeType, char * pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override;
  uint32_t GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs) override;
  vr::EVRApplicationError GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override;
  vr::EVRApplicationTransitionState GetTransitionState() override;
  vr::EVRApplicationError PerformApplicationPrelaunchCheck(const char * pchAppKey) override;
  const char * GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state) override;
  bool IsQuitUserPromptRequested() override;
  vr::EVRApplicationError LaunchInternalProcess(const char * pchBinaryPath, const char * pchArguments, const char * pchWorkingDirectory) override;
  uint32_t GetCurrentSceneProcessId() override;
};
class VRChaperoneCppStub : public vr::IVRChaperone
{
public:
  vr::ChaperoneCalibrationState GetCalibrationState() override;
  bool GetPlayAreaSize(float * pSizeX, float * pSizeZ) override;
  bool GetPlayAreaRect(struct vr::HmdQuad_t * rect) override;
  void ReloadInfo() override;
  void SetSceneColor(struct vr::HmdColor_t color) override;
  void GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor) override;
  bool AreBoundsVisible() override;
  void ForceBoundsVisible(bool bForce) override;
};
class VRChaperoneSetupCppStub : public vr::IVRChaperoneSetup
{
public:
  bool CommitWorkingCopy(vr::EChaperoneConfigFile configFile) override;
  void RevertWorkingCopy() override;
  bool GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ) override;
  bool GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect) override;
  bool GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;
  bool GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;
  bool GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) override;
  bool GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose) override;
  void SetWorkingPlayAreaSize(float sizeX, float sizeZ) override;
  void SetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount) override;
  void SetWorkingSeatedZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatSeatedZeroPoseToRawTrackingPose) override;
  void SetWorkingStandingZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatStandingZeroPoseToRawTrackingPose) override;
  void ReloadFromDisk(vr::EChaperoneConfigFile configFile) override;
  bool GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) override;
  void SetWorkingCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t unTagCount) override;
  bool GetLiveCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t * punTagCount) override;
  bool SetWorkingPhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount) override;
  bool GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) override;
  bool ExportLiveToBuffer(char * pBuffer, uint32_t * pnBufferLength) override;
  bool ImportFromBufferToWorking(const char * pBuffer, uint32_t nImportFlags) override;
};
class VRCompositorCppStub : public vr::IVRCompositor
{
public:
  void SetTrackingSpace(vr::ETrackingUniverseOrigin eOrigin) override;
  vr::ETrackingUniverseOrigin GetTrackingSpace() override;
  vr::EVRCompositorError WaitGetPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) override;
  vr::EVRCompositorError GetLastPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) override;
  vr::EVRCompositorError GetLastPoseForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex, struct vr::TrackedDevicePose_t * pOutputPose, struct vr::TrackedDevicePose_t * pOutputGamePose) override;
  vr::EVRCompositorError Submit(vr::EVREye eEye, const struct vr::Texture_t * pTexture, const struct vr::VRTextureBounds_t * pBounds, vr::EVRSubmitFlags nSubmitFlags) override;
  void ClearLastSubmittedFrame() override;
  void PostPresentHandoff() override;
  bool GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo) override;
  uint32_t GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames) override;
  float GetFrameTimeRemaining() override;
  void GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes) override;
  void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground) override;
  struct vr::HmdColor_t GetCurrentFadeColor(bool bBackground) override;
  void FadeGrid(float fSeconds, bool bFadeIn) override;
  float GetCurrentGridAlpha() override;
  vr::EVRCompositorError SetSkyboxOverride(const struct vr::Texture_t * pTextures, uint32_t unTextureCount) override;
  void ClearSkyboxOverride() override;
  void CompositorBringToFront() override;
  void CompositorGoToBack() override;
  void CompositorQuit() override;
  bool IsFullscreen() override;
  uint32_t GetCurrentSceneFocusProcess() override;
  uint32_t GetLastFrameRenderer() override;
  bool CanRenderScene() override;
  void ShowMirrorWindow() override;
  void HideMirrorWindow() override;
  bool IsMirrorWindowVisible() override;
  void CompositorDumpImages() override;
  bool ShouldAppRenderWithLowResources() override;
  void ForceInterleavedReprojectionOn(bool bOverride) override;
  void ForceReconnectProcess() override;
  void SuspendRendering(bool bSuspend) override;
  vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView) override;
  void ReleaseMirrorTextureD3D11(void * pD3D11ShaderResourceView) override;
  vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t * pglTextureId, vr::glSharedTextureHandle_t * pglSharedTextureHandle) override;
  bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) override;
  void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override;
  void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override;
  uint32_t GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize) override;
  uint32_t GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize) override;
};
class VROverlayCppStub : public vr::IVROverlay
{
public:
  vr::EVROverlayError FindOverlay(const char * pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle) override;
  vr::EVROverlayError CreateOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pOverlayHandle) override;
  vr::EVROverlayError DestroyOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError SetHighQualityOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::VROverlayHandle_t GetHighQualityOverlay() override;
  uint32_t GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) override;
  uint32_t GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) override;
  vr::EVROverlayError GetOverlayImageData(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight) override;
  const char * GetOverlayErrorNameFromEnum(vr::EVROverlayError error) override;
  vr::EVROverlayError SetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unPID) override;
  uint32_t GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError SetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool bEnabled) override;
  vr::EVROverlayError GetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool * pbEnabled) override;
  vr::EVROverlayError SetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) override;
  vr::EVROverlayError GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue) override;
  vr::EVROverlayError SetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float fAlpha) override;
  vr::EVROverlayError GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha) override;
  vr::EVROverlayError SetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float fTexelAspect) override;
  vr::EVROverlayError GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect) override;
  vr::EVROverlayError SetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) override;
  vr::EVROverlayError GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder) override;
  vr::EVROverlayError SetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) override;
  vr::EVROverlayError GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters) override;
  vr::EVROverlayError SetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters) override;
  vr::EVROverlayError GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters) override;
  vr::EVROverlayError SetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace eTextureColorSpace) override;
  vr::EVROverlayError GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace) override;
  vr::EVROverlayError SetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VRTextureBounds_t * pOverlayTextureBounds) override;
  vr::EVROverlayError GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VRTextureBounds_t * pOverlayTextureBounds) override;
  vr::EVROverlayError GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType) override;
  vr::EVROverlayError SetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform) override;
  vr::EVROverlayError GetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin * peTrackingOrigin, struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform) override;
  vr::EVROverlayError SetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unTrackedDevice, const struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform) override;
  vr::EVROverlayError GetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punTrackedDevice, struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform) override;
  vr::EVROverlayError SetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchComponentName) override;
  vr::EVROverlayError GetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punDeviceIndex, char * pchComponentName, uint32_t unComponentNameSize) override;
  vr::EVROverlayError ShowOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError HideOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  bool IsOverlayVisible(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError GetTransformForOverlayCoordinates(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, struct vr::HmdVector2_t coordinatesInOverlay, struct vr::HmdMatrix34_t * pmatTransform) override;
  bool PollNextOverlayEvent(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent) override;
  vr::EVROverlayError GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod) override;
  vr::EVROverlayError SetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod eInputMethod) override;
  vr::EVROverlayError GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale) override;
  vr::EVROverlayError SetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::HmdVector2_t * pvecMouseScale) override;
  bool ComputeOverlayIntersection(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VROverlayIntersectionParams_t * pParams, struct vr::VROverlayIntersectionResults_t * pResults) override;
  bool HandleControllerOverlayInteractionAsMouse(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unControllerDeviceIndex) override;
  bool IsHoverTargetOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::VROverlayHandle_t GetGamepadFocusOverlay() override;
  vr::EVROverlayError SetGamepadFocusOverlay(vr::VROverlayHandle_t ulNewFocusOverlay) override;
  vr::EVROverlayError SetOverlayNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom, vr::VROverlayHandle_t ulTo) override;
  vr::EVROverlayError MoveGamepadFocusToNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom) override;
  vr::EVROverlayError SetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::Texture_t * pTexture) override;
  vr::EVROverlayError ClearOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError SetOverlayRaw(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth) override;
  vr::EVROverlayError SetOverlayFromFile(vr::VROverlayHandle_t ulOverlayHandle, const char * pchFilePath) override;
  vr::EVROverlayError GetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, void ** pNativeTextureHandle, void * pNativeTextureRef, uint32_t * pWidth, uint32_t * pHeight, uint32_t * pNativeFormat, vr::ETextureType * pAPIType, vr::EColorSpace * pColorSpace, struct vr::VRTextureBounds_t * pTextureBounds) override;
  vr::EVROverlayError ReleaseNativeOverlayHandle(vr::VROverlayHandle_t ulOverlayHandle, void * pNativeTextureHandle) override;
  vr::EVROverlayError GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight) override;
  vr::EVROverlayError CreateDashboardOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pMainHandle, vr::VROverlayHandle_t * pThumbnailHandle) override;
  bool IsDashboardVisible() override;
  bool IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle) override;
  vr::EVROverlayError SetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) override;
  vr::EVROverlayError GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId) override;
  void ShowDashboard(const char * pchOverlayToShow) override;
  vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override;
  vr::EVROverlayError ShowKeyboard(vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue) override;
  vr::EVROverlayError ShowKeyboardForOverlay(vr::VROverlayHandle_t ulOverlayHandle, vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue) override;
  uint32_t GetKeyboardText(char * pchText, uint32_t cchText) override;
  void HideKeyboard() override;
  void SetKeyboardTransformAbsolute(vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToKeyboardTransform) override;
  void SetKeyboardPositionForOverlay(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdRect2_t avoidRect) override;
  vr::EVROverlayError SetOverlayIntersectionMask(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VROverlayIntersectionMaskPrimitive_t * pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize) override;
  vr::EVROverlayError GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags) override;
  vr::VRMessageOverlayResponse ShowMessageOverlay(const char * pchText, const char * pchCaption, const char * pchButton0Text, const char * pchButton1Text, const char * pchButton2Text, const char * pchButton3Text) override;
};
class VRRenderModelsCppStub : public vr::IVRRenderModels
{
public:
  vr::EVRRenderModelError LoadRenderModel_Async(const char * pchRenderModelName, struct vr::RenderModel_t ** ppRenderModel) override;
  void FreeRenderModel(struct vr::RenderModel_t * pRenderModel) override;
  vr::EVRRenderModelError LoadTexture_Async(vr::TextureID_t textureId, struct vr::RenderModel_TextureMap_t ** ppTexture) override;
  void FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture) override;
  vr::EVRRenderModelError LoadTextureD3D11_Async(vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D) override;
  vr::EVRRenderModelError LoadIntoTextureD3D11_Async(vr::TextureID_t textureId, void * pDstTexture) override;
  void FreeTextureD3D11(void * pD3D11Texture2D) override;
  uint32_t GetRenderModelName(uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen) override;
  uint32_t GetRenderModelCount() override;
  uint32_t GetComponentCount(const char * pchRenderModelName) override;
  uint32_t GetComponentName(const char * pchRenderModelName, uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen) override;
  uint64_t GetComponentButtonMask(const char * pchRenderModelName, const char * pchComponentName) override;
  uint32_t GetComponentRenderModelName(const char * pchRenderModelName, const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override;
  bool GetComponentState(const char * pchRenderModelName, const char * pchComponentName, const vr::VRControllerState_t * pControllerState, const struct vr::RenderModel_ControllerMode_State_t * pState, struct vr::RenderModel_ComponentState_t * pComponentState) override;
  bool RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName) override;
  uint32_t GetRenderModelThumbnailURL(const char * pchRenderModelName, char * pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError) override;
  uint32_t GetRenderModelOriginalPath(const char * pchRenderModelName, char * pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError * peError) override;
  const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override;
};
class VRNotificationsCppStub : public vr::IVRNotifications
{
public:
  vr::EVRNotificationError CreateNotification(vr::VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, vr::EVRNotificationType type, const char * pchText, vr::EVRNotificationStyle style, const struct vr::NotificationBitmap_t * pImage, vr::VRNotificationId * pNotificationId) override;
  vr::EVRNotificationError RemoveNotification(vr::VRNotificationId notificationId) override;
};
class VRSettingsCppStub : public vr::IVRSettings
{
public:
  const char * GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError) override;
  bool Sync(bool bForce, vr::EVRSettingsError * peError) override;
  void SetBool(const char * pchSection, const char * pchSettingsKey, bool bValue, vr::EVRSettingsError * peError) override;
  void SetInt32(const char * pchSection, const char * pchSettingsKey, int32_t nValue, vr::EVRSettingsError * peError) override;
  void SetFloat(const char * pchSection, const char * pchSettingsKey, float flValue, vr::EVRSettingsError * peError) override;
  void SetString(const char * pchSection, const char * pchSettingsKey, const char * pchValue, vr::EVRSettingsError * peError) override;
  bool GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
  int32_t GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
  float GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
  void GetString(const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, vr::EVRSettingsError * peError) override;
  void RemoveSection(const char * pchSection, vr::EVRSettingsError * peError) override;
  void RemoveKeyInSection(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) override;
};
class VRScreenshotsCppStub : public vr::IVRScreenshots
{
public:
  vr::EVRScreenshotError RequestScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, vr::EVRScreenshotType type, const char * pchPreviewFilename, const char * pchVRFilename) override;
  vr::EVRScreenshotError HookScreenshot(const vr::EVRScreenshotType * pSupportedTypes, int numTypes) override;
  vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError * pError) override;
  uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, char * pchFilename, uint32_t cchFilename, vr::EVRScreenshotError * pError) override;
  vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) override;
  vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, const char * pchPreviewFilename, const char * pchVRFilename) override;
  vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char * pchSourcePreviewFilename, const char * pchSourceVRFilename) override;
};
class VRResourcesCppStub : public vr::IVRResources
{
public:
  uint32_t LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen) override;
  uint32_t GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory, char * pchPathBuffer, uint32_t unBufferLen) override;
};
