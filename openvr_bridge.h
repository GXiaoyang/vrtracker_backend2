#pragma once

#include <openvr_broker.h>
#include "vr_settings_indexer.h"

// responsibilities
//	* export an interface
//  * bind to a configurable downstream interface
// specials:
//	* second texture downstream since sometimes you want to replay from an old file, AND view the output on real device
//  * configuration discovery.  since the bridge can see all the incoming queries, it can update the capture
//    configuration to capture things of interest

struct capture_controller;
struct vr_cursor_controller;

class openvr_bridge :
	public vr::IVRSystem,
	public vr::IVRExtendedDisplay,
	public vr::IVRTrackedCamera,
	public vr::IVRApplications,
	public vr::IVRChaperone,
	public vr::IVRChaperoneSetup,
	public vr::IVRCompositor,
	public vr::IVROverlay,
	public vr::IVRRenderModels,
	public vr::IVRNotifications,
	public vr::IVRSettings,
	public vr::IVRScreenshots,
	public vr::IVRResources
{
public:
	openvr_bridge();
	openvr_broker::open_vr_interfaces &interfaces() { return m_up_stream; };
	void set_down_stream_interface(const openvr_broker::open_vr_interfaces& interfaces);

	void set_down_stream_capture_controller(capture_controller *down_stream_capture);
	const capture_controller *get_down_stream_capture_controller() const;

	// mirror texture submits here too
	void set_aux_texture_down_stream_interface(vr::IVRCompositor *texture_down_stream);

private:
	openvr_broker::open_vr_interfaces m_up_stream;		// block of interfaces given to clients.  since
														// this class implements them, it's just 13 different pointers
														// to this instance


														
	void refresh_lockstep_capture();					// for lockstep

	openvr_broker::open_vr_interfaces m_down_stream;	// my downstream interface.  
														// e.g. could be a raw openvr interface or an interface into a
														// recorded capture

	capture_controller *m_down_stream_capture_controller;	// can be null, if present, wants to be notified about config and events to capture

	vr_cursor_controller *m_cursor_controller;				// can be null, wants to be advanced at the right time

	capture_controller *m_lockstep_capture_controller;
	openvr_broker::open_vr_interfaces m_lock_step_tracker;

	vr::IVRCompositor *m_aux_compositor; // use to submit_frames_to_real_compositor_during_playback


	bool m_lock_step_train_tracker;						// for debugging
	bool m_spy_mode;
	bool m_snapshot_playback_mode;
	bool m_snapshot_record_mode;
	bool m_events_since_last_refresh;

private:

	// these hooks apply when the downstream has a capture controller
	//	(
	//		these don't belong in the cursors, because the cursors are past references
	//		these don't belong in the settings wrappers since the settings wrappers are only
	//		used for polling queries
	//		it is therefore the responsibility of the bridge in spymode to handle discovering desired configuration
	//		over time
	//  }
	void process_poll_next_event_value(bool poll_rc, vr::VREvent_t * pEvent);
	void advance_cursor_one_frame();
	void update_capture_controller();
	void capture_vr_event(const vr::VREvent_t &event);
	void capture_vr_overlay_event(vr::VROverlayHandle_t overlay_handle, const vr::VREvent_t &event);

	void update_vr_config_near_far(float nearz, float farz);
	void update_vr_config_setting(const char *section, SettingsIndexer::SectionSettingType setting_type, const char *key);

public:
	void GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight) final;
	struct vr::HmdMatrix44_t GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ) final;
	void GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom) final;
	bool ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates) final;
	struct vr::HmdMatrix34_t GetEyeToHeadTransform(vr::EVREye eEye) final;
	bool GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter) final;
	int32_t GetD3D9AdapterIndex() final;
	void GetDXGIOutputInfo(int32_t * pnAdapterIndex) final;
	bool IsDisplayOnDesktop() final;
	bool SetDisplayVisibility(bool bIsVisibleOnDesktop) final;
	void GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) final;
	void ResetSeatedZeroPose() final;
	struct vr::HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() final;
	struct vr::HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() final;
	uint32_t GetSortedTrackedDeviceIndicesOfClass(vr::ETrackedDeviceClass eTrackedDeviceClass, vr::TrackedDeviceIndex_t * punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex) final;
	vr::EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) final;
	void ApplyTransform(struct vr::TrackedDevicePose_t * pOutputPose, const struct vr::TrackedDevicePose_t * pTrackedDevicePose, const struct vr::HmdMatrix34_t * pTransform) final;
	vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) final;
	vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) final;
	vr::ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) final;
	bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) final;
	bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) final;
	float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) final;
	int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) final;
	uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) final;
	struct vr::HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) final;
	uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError) final;
	const char * GetPropErrorNameFromEnum(vr::ETrackedPropertyError error) final;
	bool PollNextEvent(struct vr::VREvent_t * pEvent, uint32_t uncbVREvent) final;
	bool PollNextEventWithPose(vr::ETrackingUniverseOrigin eOrigin, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t * pTrackedDevicePose) final;
	const char * GetEventTypeNameFromEnum(vr::EVREventType eType) final;
	struct vr::HiddenAreaMesh_t GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type) final;
	bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize) final;
	bool GetControllerStateWithPose(vr::ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize, struct vr::TrackedDevicePose_t * pTrackedDevicePose) final;
	void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) final;
	const char * GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId) final;
	const char * GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType) final;
	bool CaptureInputFocus() final;
	void ReleaseInputFocus() final;
	bool IsInputFocusCapturedByAnotherProcess() final;
	uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize) final;
	vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) final;
	void AcknowledgeQuit_Exiting() final;
	void AcknowledgeQuit_UserPrompt() final;

	void GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) final;
	void GetEyeOutputViewport(vr::EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) final;
	void GetDXGIOutputInfo(int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex) final;

	const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) final;
	vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera) final;
	vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize) final;
	vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t * pFocalLength, vr::HmdVector2_t * pCenter) final;
	vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection) final;
	vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t * pHandle) final;
	vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) final;
	vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) final;
	vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t * pTextureBounds, uint32_t * pnWidth, uint32_t * pnHeight) final;
	vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) final;
	vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t * pglTextureId, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize) final;
	vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) final;

	vr::EVRApplicationError AddApplicationManifest(const char * pchApplicationManifestFullPath, bool bTemporary) final;
	vr::EVRApplicationError RemoveApplicationManifest(const char * pchApplicationManifestFullPath) final;
	bool IsApplicationInstalled(const char * pchAppKey) final;
	uint32_t GetApplicationCount() final;
	vr::EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) final;
	vr::EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) final;
	vr::EVRApplicationError LaunchApplication(const char * pchAppKey) final;
	vr::EVRApplicationError LaunchTemplateApplication(const char * pchTemplateAppKey, const char * pchNewAppKey, const struct vr::AppOverrideKeys_t * pKeys, uint32_t unKeys) final;
	vr::EVRApplicationError LaunchApplicationFromMimeType(const char * pchMimeType, const char * pchArgs) final;
	vr::EVRApplicationError LaunchDashboardOverlay(const char * pchAppKey) final;
	bool CancelApplicationLaunch(const char * pchAppKey) final;
	vr::EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char * pchAppKey) final;
	uint32_t GetApplicationProcessId(const char * pchAppKey) final;
	const char * GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error) final;
	uint32_t GetApplicationPropertyString(const char * pchAppKey, vr::EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, vr::EVRApplicationError * peError) final;
	bool GetApplicationPropertyBool(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) final;
	uint64_t GetApplicationPropertyUint64(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError) final;
	vr::EVRApplicationError SetApplicationAutoLaunch(const char * pchAppKey, bool bAutoLaunch) final;
	bool GetApplicationAutoLaunch(const char * pchAppKey) final;
	vr::EVRApplicationError SetDefaultApplicationForMimeType(const char * pchAppKey, const char * pchMimeType) final;
	bool GetDefaultApplicationForMimeType(const char * pchMimeType, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) final;
	bool GetApplicationSupportedMimeTypes(const char * pchAppKey, char * pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) final;
	uint32_t GetApplicationsThatSupportMimeType(const char * pchMimeType, char * pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) final;
	uint32_t GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs) final;
	vr::EVRApplicationError GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen) final;
	vr::EVRApplicationTransitionState GetTransitionState() final;
	vr::EVRApplicationError PerformApplicationPrelaunchCheck(const char * pchAppKey) final;
	const char * GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state) final;
	bool IsQuitUserPromptRequested() final;
	vr::EVRApplicationError LaunchInternalProcess(const char * pchBinaryPath, const char * pchArguments, const char * pchWorkingDirectory) final;
	uint32_t GetCurrentSceneProcessId() final;

	vr::ChaperoneCalibrationState GetCalibrationState() final;
	bool GetPlayAreaSize(float * pSizeX, float * pSizeZ) final;
	bool GetPlayAreaRect(struct vr::HmdQuad_t * rect) final;
	void ReloadInfo() final;
	void SetSceneColor(struct vr::HmdColor_t color) final;
	void GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor) final;
	bool AreBoundsVisible() final;
	void ForceBoundsVisible(bool bForce) final;

	bool CommitWorkingCopy(vr::EChaperoneConfigFile configFile) final;
	void RevertWorkingCopy() final;
	bool GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ) final;
	bool GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect) final;
	bool GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) final;
	bool GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) final;
	bool GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) final;
	bool GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose) final;
	void SetWorkingPlayAreaSize(float sizeX, float sizeZ) final;
	void SetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount) final;
	void SetWorkingSeatedZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatSeatedZeroPoseToRawTrackingPose) final;
	void SetWorkingStandingZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatStandingZeroPoseToRawTrackingPose) final;
	void ReloadFromDisk(vr::EChaperoneConfigFile configFile) final;
	bool GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose) final;
	void SetWorkingCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t unTagCount) final;
	bool GetLiveCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t * punTagCount) final;
	bool SetWorkingPhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount) final;
	bool GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount) final;
	bool ExportLiveToBuffer(char * pBuffer, uint32_t * pnBufferLength) final;
	bool ImportFromBufferToWorking(const char * pBuffer, uint32_t nImportFlags) final;

	void SetTrackingSpace(vr::ETrackingUniverseOrigin eOrigin) final;
	vr::ETrackingUniverseOrigin GetTrackingSpace() final;
	vr::EVRCompositorError WaitGetPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) final;
	vr::EVRCompositorError GetLastPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount) final;
	vr::EVRCompositorError GetLastPoseForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex, struct vr::TrackedDevicePose_t * pOutputPose, struct vr::TrackedDevicePose_t * pOutputGamePose) final;
	vr::EVRCompositorError Submit(vr::EVREye eEye, const struct vr::Texture_t * pTexture, const struct vr::VRTextureBounds_t * pBounds, vr::EVRSubmitFlags nSubmitFlags) final;
	void ClearLastSubmittedFrame() final;
	void PostPresentHandoff() final;
	bool GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo) final;
	uint32_t GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames) final;
	float GetFrameTimeRemaining() final;
	void GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes) final;
	void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground) final;
	struct vr::HmdColor_t GetCurrentFadeColor(bool bBackground) final;
	void FadeGrid(float fSeconds, bool bFadeIn) final;
	float GetCurrentGridAlpha() final;
	vr::EVRCompositorError SetSkyboxOverride(const struct vr::Texture_t * pTextures, uint32_t unTextureCount) final;
	void ClearSkyboxOverride() final;
	void CompositorBringToFront() final;
	void CompositorGoToBack() final;
	void CompositorQuit() final;
	bool IsFullscreen() final;
	uint32_t GetCurrentSceneFocusProcess() final;
	uint32_t GetLastFrameRenderer() final;
	bool CanRenderScene() final;
	void ShowMirrorWindow() final;
	void HideMirrorWindow() final;
	bool IsMirrorWindowVisible() final;
	void CompositorDumpImages() final;
	bool ShouldAppRenderWithLowResources() final;
	void ForceInterleavedReprojectionOn(bool bOverride) final;
	void ForceReconnectProcess() final;
	void SuspendRendering(bool bSuspend) final;
	vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView) final;
	void ReleaseMirrorTextureD3D11(void * pD3D11ShaderResourceView) final;
	vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t * pglTextureId, vr::glSharedTextureHandle_t * pglSharedTextureHandle) final;
	bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) final;
	void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) final;
	void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) final;
	uint32_t GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize) final;
	uint32_t GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize) final;

	vr::EVROverlayError FindOverlay(const char * pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle) final;
	vr::EVROverlayError CreateOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pOverlayHandle) final;
	vr::EVROverlayError DestroyOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError SetHighQualityOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::VROverlayHandle_t GetHighQualityOverlay() final;
	uint32_t GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) final;
	uint32_t GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError) final;
	vr::EVROverlayError GetOverlayImageData(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight) final;
	const char * GetOverlayErrorNameFromEnum(vr::EVROverlayError error) final;
	vr::EVROverlayError SetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unPID) final;
	uint32_t GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError SetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool bEnabled) final;
	vr::EVROverlayError GetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool * pbEnabled) final;
	vr::EVROverlayError SetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) final;
	vr::EVROverlayError GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue) final;
	vr::EVROverlayError SetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float fAlpha) final;
	vr::EVROverlayError GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha) final;
	vr::EVROverlayError SetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float fTexelAspect) final;
	vr::EVROverlayError GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect) final;
	vr::EVROverlayError SetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) final;
	vr::EVROverlayError GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder) final;
	vr::EVROverlayError SetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) final;
	vr::EVROverlayError GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters) final;
	vr::EVROverlayError SetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters) final;
	vr::EVROverlayError GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters) final;
	vr::EVROverlayError SetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace eTextureColorSpace) final;
	vr::EVROverlayError GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace) final;
	vr::EVROverlayError SetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VRTextureBounds_t * pOverlayTextureBounds) final;
	vr::EVROverlayError GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VRTextureBounds_t * pOverlayTextureBounds) final;
	vr::EVROverlayError GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType) final;
	vr::EVROverlayError SetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform) final;
	vr::EVROverlayError GetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin * peTrackingOrigin, struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform) final;
	vr::EVROverlayError SetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unTrackedDevice, const struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform) final;
	vr::EVROverlayError GetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punTrackedDevice, struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform) final;
	vr::EVROverlayError SetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchComponentName) final;
	vr::EVROverlayError GetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punDeviceIndex, char * pchComponentName, uint32_t unComponentNameSize) final;
	vr::EVROverlayError ShowOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError HideOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	bool IsOverlayVisible(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError GetTransformForOverlayCoordinates(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, struct vr::HmdVector2_t coordinatesInOverlay, struct vr::HmdMatrix34_t * pmatTransform) final;
	bool PollNextOverlayEvent(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent) final;
	vr::EVROverlayError GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod) final;
	vr::EVROverlayError SetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod eInputMethod) final;
	vr::EVROverlayError GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale) final;
	vr::EVROverlayError SetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::HmdVector2_t * pvecMouseScale) final;
	bool ComputeOverlayIntersection(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VROverlayIntersectionParams_t * pParams, struct vr::VROverlayIntersectionResults_t * pResults) final;
	bool HandleControllerOverlayInteractionAsMouse(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unControllerDeviceIndex) final;
	bool IsHoverTargetOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::VROverlayHandle_t GetGamepadFocusOverlay() final;
	vr::EVROverlayError SetGamepadFocusOverlay(vr::VROverlayHandle_t ulNewFocusOverlay) final;
	vr::EVROverlayError SetOverlayNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom, vr::VROverlayHandle_t ulTo) final;
	vr::EVROverlayError MoveGamepadFocusToNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom) final;
	vr::EVROverlayError SetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::Texture_t * pTexture) final;
	vr::EVROverlayError ClearOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError SetOverlayRaw(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth) final;
	vr::EVROverlayError SetOverlayFromFile(vr::VROverlayHandle_t ulOverlayHandle, const char * pchFilePath) final;
	vr::EVROverlayError GetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, void ** pNativeTextureHandle, void * pNativeTextureRef, uint32_t * pWidth, uint32_t * pHeight, uint32_t * pNativeFormat, vr::ETextureType * pAPIType, vr::EColorSpace * pColorSpace, struct vr::VRTextureBounds_t * pTextureBounds) final;
	vr::EVROverlayError ReleaseNativeOverlayHandle(vr::VROverlayHandle_t ulOverlayHandle, void * pNativeTextureHandle) final;
	vr::EVROverlayError GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight) final;
	vr::EVROverlayError CreateDashboardOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pMainHandle, vr::VROverlayHandle_t * pThumbnailHandle) final;
	bool IsDashboardVisible() final;
	bool IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle) final;
	vr::EVROverlayError SetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) final;
	vr::EVROverlayError GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId) final;
	void ShowDashboard(const char * pchOverlayToShow) final;
	vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() final;
	vr::EVROverlayError ShowKeyboard(vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue) final;
	vr::EVROverlayError ShowKeyboardForOverlay(vr::VROverlayHandle_t ulOverlayHandle, vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue) final;
	uint32_t GetKeyboardText(char * pchText, uint32_t cchText) final;
	void HideKeyboard() final;
	void SetKeyboardTransformAbsolute(vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToKeyboardTransform) final;
	void SetKeyboardPositionForOverlay(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdRect2_t avoidRect) final;
	vr::EVROverlayError SetOverlayIntersectionMask(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VROverlayIntersectionMaskPrimitive_t * pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize) final;
	vr::EVROverlayError GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags) final;
	vr::VRMessageOverlayResponse ShowMessageOverlay(const char * pchText, const char * pchCaption, const char * pchButton0Text, const char * pchButton1Text, const char * pchButton2Text, const char * pchButton3Text) final;

	vr::EVRRenderModelError LoadRenderModel_Async(const char * pchRenderModelName, struct vr::RenderModel_t ** ppRenderModel) final;
	void FreeRenderModel(struct vr::RenderModel_t * pRenderModel) final;
	vr::EVRRenderModelError LoadTexture_Async(vr::TextureID_t textureId, struct vr::RenderModel_TextureMap_t ** ppTexture) final;
	void FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture) final;
	vr::EVRRenderModelError LoadTextureD3D11_Async(vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D) final;
	vr::EVRRenderModelError LoadIntoTextureD3D11_Async(vr::TextureID_t textureId, void * pDstTexture) final;
	void FreeTextureD3D11(void * pD3D11Texture2D) final;
	uint32_t GetRenderModelName(uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen) final;
	uint32_t GetRenderModelCount() final;
	uint32_t GetComponentCount(const char * pchRenderModelName) final;
	uint32_t GetComponentName(const char * pchRenderModelName, uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen) final;
	uint64_t GetComponentButtonMask(const char * pchRenderModelName, const char * pchComponentName) final;
	uint32_t GetComponentRenderModelName(const char * pchRenderModelName, const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) final;
	bool GetComponentState(const char * pchRenderModelName, const char * pchComponentName, const vr::VRControllerState_t * pControllerState, const struct vr::RenderModel_ControllerMode_State_t * pState, struct vr::RenderModel_ComponentState_t * pComponentState) final;
	bool RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName) final;
	uint32_t GetRenderModelThumbnailURL(const char * pchRenderModelName, char * pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError) final;
	uint32_t GetRenderModelOriginalPath(const char * pchRenderModelName, char * pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError * peError) final;
	const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) final;

	vr::EVRNotificationError CreateNotification(vr::VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, vr::EVRNotificationType type, const char * pchText, vr::EVRNotificationStyle style, const struct vr::NotificationBitmap_t * pImage, vr::VRNotificationId * pNotificationId) final;
	vr::EVRNotificationError RemoveNotification(vr::VRNotificationId notificationId) final;

	const char * GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError) final;
	bool Sync(bool bForce, vr::EVRSettingsError * peError) final;
	void SetBool(const char * pchSection, const char * pchSettingsKey, bool bValue, vr::EVRSettingsError * peError) final;
	void SetInt32(const char * pchSection, const char * pchSettingsKey, int32_t nValue, vr::EVRSettingsError * peError) final;
	void SetFloat(const char * pchSection, const char * pchSettingsKey, float flValue, vr::EVRSettingsError * peError) final;
	void SetString(const char * pchSection, const char * pchSettingsKey, const char * pchValue, vr::EVRSettingsError * peError) final;
	bool GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) final;
	int32_t GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) final;
	float GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) final;
	void GetString(const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, vr::EVRSettingsError * peError) final;
	void RemoveSection(const char * pchSection, vr::EVRSettingsError * peError) final;
	void RemoveKeyInSection(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError) final;

	vr::EVRScreenshotError RequestScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, vr::EVRScreenshotType type, const char * pchPreviewFilename, const char * pchVRFilename) final;
	vr::EVRScreenshotError HookScreenshot(const vr::EVRScreenshotType * pSupportedTypes, int numTypes) final;
	vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError * pError) final;
	uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, char * pchFilename, uint32_t cchFilename, vr::EVRScreenshotError * pError) final;
	vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) final;
	vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, const char * pchPreviewFilename, const char * pchVRFilename) final;
	vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char * pchSourcePreviewFilename, const char * pchSourceVRFilename) final;

	uint32_t LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen) final;
	uint32_t GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory, char * pchPathBuffer, uint32_t unBufferLen) final;
};