#include "openvr_bridge.h"
#include "vr_types.h"
#include "openvr_softcompare.h"
#include "log.h"
#include "vr_cursor_controller.h"
#include "capture_controller.h"

openvr_bridge::openvr_bridge()
	: 
		m_down_stream_capture_controller(nullptr),
		m_cursor_controller(nullptr),
		m_lockstep_capture_controller(nullptr),
		m_lock_step_train_tracker(false),
		m_spy_mode(false),
		m_snapshot_playback_mode(false),
		m_events_since_last_refresh(false)
{
	m_up_stream.sysi = this;
	m_up_stream.appi = this;
	m_up_stream.seti = this;
	m_up_stream.chapi = this;
	m_up_stream.chapsi = this;
	m_up_stream.compi = this;
	m_up_stream.noti = this;
	m_up_stream.ovi = this;
	m_up_stream.remi = this;
	m_up_stream.exdi = this;
	m_up_stream.taci = this;
	m_up_stream.screeni = this;
	m_up_stream.resi = this;
}

void openvr_bridge::set_down_stream_interface(const openvr_broker::open_vr_interfaces& interfaces)
{
	m_down_stream = interfaces;
}

void openvr_bridge::set_down_stream_capture_controller(capture_controller *down_stream_capture)
{
	m_down_stream_capture_controller = down_stream_capture;
}
const capture_controller *openvr_bridge::get_down_stream_capture_controller() const
{
	return m_down_stream_capture_controller;
}

void openvr_bridge::set_aux_texture_down_stream_interface(vr::IVRCompositor *texture_down_stream)
{
	m_aux_compositor = texture_down_stream;
}

void openvr_bridge::update_capture_controller()
{
	if (m_down_stream_capture_controller)
	{
		m_down_stream_capture_controller->update();
	}

}
void openvr_bridge::refresh_lockstep_capture()
{
	if (m_lockstep_capture_controller)
	{
		m_down_stream_capture_controller->update();
	}
}

void openvr_bridge::capture_vr_event(const vr::VREvent_t &e)
{
	if (m_down_stream_capture_controller)
	{
		m_down_stream_capture_controller->enqueue_event(e);
	}
}

void openvr_bridge::update_vr_config_near_far(float nearz, float farz)
{
	if (m_down_stream_capture_controller)
	{
		float cur_farz = m_down_stream_capture_controller->get_model().m_keys.GetFarZ();
		float cur_nearz = m_down_stream_capture_controller->get_model().m_keys.GetNearZ();

		if (farz != cur_farz || nearz != cur_nearz)
		{
			m_down_stream_capture_controller->enqueue_new_key(VRKeysUpdate::make_modify_nearz_farz(nearz, farz));
			m_down_stream_capture_controller->update();
		}
	}
}

void openvr_bridge::update_vr_config_setting(const char *section, SettingsIndexer::SectionSettingType setting_type, const char *key)
{
	if (m_down_stream_capture_controller)
	{
		const SettingsIndexer &indexer(m_down_stream_capture_controller->get_model().m_keys.GetSettingsIndexer());
		if (!indexer.setting_exists(section, setting_type, key))
		{
			m_down_stream_capture_controller->enqueue_new_key(VRKeysUpdate::make_new_setting(section, setting_type, key));
			m_down_stream_capture_controller->update();
		}
	}
}

void openvr_bridge::advance_cursor_one_frame()
{
	if (m_cursor_controller)
	{
		m_cursor_controller->advance_one_frame();
	}
}

static void ReportFailure(const char *string, const char *file, int line)
{
	log_printf("%s %s %d\n", string, file, line);
}

#define TRAIN_TRACKER_ASSERT(cond) \
if (!(cond)) \
{ \
ReportFailure(#cond, __FILE__, __LINE__); \
} \
else\
{\
}

// utility class to memcopy data before and after a function call 
// to determine if the function call modified anything
struct TmpBuf
{
private:
	void *      m_original_data;
	void *		m_data;
	uint32_t	m_size;
public:

	TmpBuf()
		:m_data(nullptr)
	{}
	~TmpBuf()
	{
		if (m_data)
			free(m_data);
	}

	char *data()
	{
		return (char *)m_data;
	}

	void dup(void *mem, uint32_t size)
	{
		m_original_data = mem;
		m_data = malloc(size);
		m_size = size;
		memcpy(m_data, mem, size);
	}
	bool same()
	{
		return memcmp(m_original_data, m_data, m_size) == 0;
	}
};

#if 0

#include "openvr_Bridge.h"
#include "imp_interface.h"
#include <openvr_broker.h>
#include <openvr_softcompare.h>
#include <stdint.h>
#include "openvr_method_ids.h"
#include "dprintf.h"
#include <vrdelta.h>
#include <thread>
#include <chrono>

#include <InterfacesIntoOpenVR.h>
static InterfacesIntoOpenVR raw;

static bool m_snapshot_playback_mode = false;
static bool m_snapshot_record_mode = false;
static bool m_submit_frames_to_real_compositor_during_playback = false;  // what is this for exactly?

static bool m_events_since_last_refresh = false;
static bool m_spy_mode = true;


static vr_state_tracker_t tracker;
static vr_cursor_t cursor;
//openvr_broker::open_vr_interfaces tracker_source_interfaces;
openvr_broker::open_vr_interfaces m_lock_step_tracker;

static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;
static openvr_bridge openvr_bridgeInstance;

static void *upstream_interfaces_Bridge[] =
{
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
&openvr_bridgeInstance,
};




// utility class to memcopy data before and after a function call 
// to determine if the function call modified anything
struct TmpBuf
{
	void *      m_original_data;
	void *		m_data;
	uint32_t	m_size;

	TmpBuf()
		:m_data(nullptr)
	{}
	~TmpBuf()
	{
		if (m_data)
			free(m_data);
	}

	char *val()
	{
		return (char *)m_data;
	}

	void dup(void *mem, uint32_t size)
	{
		m_original_data = mem;
		m_data = malloc(size);
		m_size = size;
		memcpy(m_data, mem, size);
	}
	bool same()
	{
		return memcmp(m_original_data, m_data, m_size) == 0;
	}
};

static void InitEmptyTracker()
{
	if (tracker == 0)
	{
		TrackerConfig c;
		c.set_default();
		tracker = create_vr_state_tracker(c);
		cursor = create_cursor(tracker);
		m_lock_step_tracker = get_cursor_interfaces(cursor);
	}
}

static void InitTrackerFromFile()
{
	if (tracker == 0)
	{
		tracker = load_vrstate_from_file("C:\\vr_streams\\profiler_default.bin");
		cursor = create_cursor(tracker);
		set_cursor_to_start_frame(tracker, cursor);
		m_lock_step_tracker = get_cursor_interfaces(cursor);
	}
}

static void refresh_lockstep_capture()
{
	capture_vr_state(tracker, *raw.cpp_interfaces());
	set_cursor_to_end_frame(tracker, cursor);
}

static void SaveTracker()
{
	save_vrstate_to_file(tracker, "C:\\vr_streams\\profiler_default.bin",true);
}

static void AssignStaticInstancesToDownstream(openvr_broker::open_vr_interfaces *downstream)
{
	openvr_bridgeInstance.SetSYSI(downstream->sysi);
	openvr_bridgeInstance.SetAPPI(downstream->appi);
	openvr_bridgeInstance.SetSETI(downstream->seti);
	openvr_bridgeInstance.SetCHAPI(downstream->chapi);
	openvr_bridgeInstance.SetCHAPSI(downstream->chapsi);
	openvr_bridgeInstance.SetCOMPI(downstream->compi);
	openvr_bridgeInstance.SetNOTI(downstream->noti);
	openvr_bridgeInstance.SetOVI(downstream->ovi);
	openvr_bridgeInstance.SetREMI(downstream->remi);
	openvr_bridgeInstance.SetEXDI(downstream->exdi);
	openvr_bridgeInstance.SetTACI(downstream->taci);
	openvr_bridgeInstance.SetSCREENI(downstream->screeni);
	openvr_bridgeInstance.SetRESI(downstream->resi);
}

class CppStubImpInterface : public ImpInterface
{
public:
	virtual void Init()
	{
		static LPCTSTR extra_path = L"C:\\projects\\openvr_clean\\openvr\\bin\\win32";
		static LPCTSTR server_dll = L"openvr_api.dll";

		// initialize my interfaces object by connecting to the openvr dll
		if (!raw.is_open())
		{
			raw.open_lib(server_dll, extra_path);
		}
		
		if (m_snapshot_playback_mode)
		{
			InitTrackerFromFile();
			AssignStaticInstancesToDownstream(&m_lock_step_tracker);
		}
		else
		{
			InitEmptyTracker();
			AssignStaticInstancesToDownstream(raw.cpp_interfaces());
		}
		if (m_snapshot_record_mode)
		{
			refresh_lockstep_capture();
		}
		
	}
	virtual void Shutdown()
	{
		if (m_snapshot_record_mode)
		{
			SaveTracker();
		}
	}
	virtual bool IsHmdPresent()
	{
		return true;
	}
	virtual bool IsRuntimeInstalled()
	{
		return true;
	}
	virtual const char *RuntimePath()
	{
		return "";
	}
	
	virtual void** GetAPIInterfaceHandles()
	{
		return upstream_interfaces_Bridge;
	}
};

ImpInterface *GetBridgeInterfaces()
{ 
	static CppStubImpInterface m_lib_if;
	return &m_lib_if;
}


#endif

void openvr_bridge::GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight)
{
   LOG_ENTRY("BridgeGetRecommendedRenderTargetSize");

   m_down_stream.sysi->GetRecommendedRenderTargetSize(pnWidth, pnHeight);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t tw, th;
	   m_lock_step_tracker.sysi->GetRecommendedRenderTargetSize(&tw, &th);
	   TRAIN_TRACKER_ASSERT(tw == *pnWidth);
	   TRAIN_TRACKER_ASSERT(th == *pnHeight);
   }

   LOG_EXIT("BridgeGetRecommendedRenderTargetSize");
}

struct vr::HmdMatrix44_t openvr_bridge::GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ)
{
	LOG_ENTRY("BridgeGetProjectionMatrix");

	if (m_spy_mode)
	{
		update_vr_config_near_far(fNearZ, fFarZ);
	}

	struct vr::HmdMatrix44_t rc;
	rc = m_down_stream.sysi->GetProjectionMatrix(eEye, fNearZ, fFarZ);
   

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::HmdMatrix44_t rc2 = m_lock_step_tracker.sysi->GetProjectionMatrix(eEye, fNearZ, fFarZ);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetProjectionMatrix");
}

void openvr_bridge::GetProjectionRaw(vr::EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom)
{
   LOG_ENTRY("BridgeGetProjectionRaw");

   m_down_stream.sysi->GetProjectionRaw(eEye, pfLeft, pfRight, pfTop, pfBottom);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   float pf[4];

	   m_lock_step_tracker.sysi->GetProjectionRaw(eEye, &pf[0], &pf[1], &pf[2], &pf[3]);
	   TRAIN_TRACKER_ASSERT(pf[0] == *pfLeft);
	   TRAIN_TRACKER_ASSERT(pf[1] == *pfRight);
	   TRAIN_TRACKER_ASSERT(pf[2] == *pfTop);
	   TRAIN_TRACKER_ASSERT(pf[3] == *pfBottom);
   }

   LOG_EXIT("BridgeGetProjectionRaw");
}

bool openvr_bridge::ComputeDistortion(vr::EVREye eEye, float fU, float fV, struct vr::DistortionCoordinates_t * pDistortionCoordinates)
{
   LOG_ENTRY("BridgeComputeDistortion");

   bool rc;
   rc = m_down_stream.sysi->ComputeDistortion(eEye, fU, fV, pDistortionCoordinates);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   struct vr::DistortionCoordinates_t tracker_distortion;
	   m_lock_step_tracker.sysi->ComputeDistortion(eEye, fU, fV, &tracker_distortion);
	   if (pDistortionCoordinates)
	   {
		   TRAIN_TRACKER_ASSERT(tracker_distortion == *pDistortionCoordinates);
	   }
   }
   LOG_EXIT_RC(rc, "BridgeComputeDistortion");
}

struct vr::HmdMatrix34_t openvr_bridge::GetEyeToHeadTransform(vr::EVREye eEye)
{
   LOG_ENTRY("BridgeGetEyeToHeadTransform");

	struct vr::HmdMatrix34_t rc;
	rc = m_down_stream.sysi->GetEyeToHeadTransform(eEye);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::HmdMatrix34_t rc2 = m_lock_step_tracker.sysi->GetEyeToHeadTransform(eEye);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetEyeToHeadTransform");
}

bool openvr_bridge::GetTimeSinceLastVsync(float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter)
{
   LOG_ENTRY("BridgeGetTimeSinceLastVsync");

   bool rc;
   rc = m_down_stream.sysi->GetTimeSinceLastVsync(pfSecondsSinceLastVsync, pulFrameCounter);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   float pfSeconds;
	   uint64_t pulCounter;
	   bool rc2;
	   rc2 = m_lock_step_tracker.sysi->GetTimeSinceLastVsync(&pfSeconds, &pulCounter);
	   if (pfSecondsSinceLastVsync)
	   {
		   //TRAIN_TRACKER_ASSERT(pfSeconds == *pfSecondsSinceLastVsync);
	   }
	   if (pulCounter)
	   {
		   //TRAIN_TRACKER_ASSERT(pulCounter == *pulFrameCounter);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetTimeSinceLastVsync");
}

int32_t openvr_bridge::GetD3D9AdapterIndex()
{
   LOG_ENTRY("BridgeGetD3D9AdapterIndex");
   int32_t rc;
   rc = m_down_stream.sysi->GetD3D9AdapterIndex();
    
   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   int32_t tracker_adapter;
	   tracker_adapter = m_lock_step_tracker.sysi->GetD3D9AdapterIndex();
	   TRAIN_TRACKER_ASSERT(tracker_adapter == rc);
   }

   LOG_EXIT_RC(rc, "BridgeGetD3D9AdapterIndex");
}

void openvr_bridge::GetDXGIOutputInfo(int32_t * pnAdapterIndex)
{
   LOG_ENTRY("BridgeGetDXGIOutputInfo");

   m_down_stream.sysi->GetDXGIOutputInfo(pnAdapterIndex);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   int32_t ttracker;
	   m_lock_step_tracker.sysi->GetDXGIOutputInfo(&ttracker);
	   TRAIN_TRACKER_ASSERT(*pnAdapterIndex == ttracker);
   }
   LOG_EXIT("BridgeGetDXGIOutputInfo");
}

bool openvr_bridge::IsDisplayOnDesktop()
{
   LOG_ENTRY("BridgeIsDisplayOnDesktop");

   bool rc;
   rc = m_down_stream.sysi->IsDisplayOnDesktop();

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   bool rc2;
	   rc2 = m_lock_step_tracker.sysi->IsDisplayOnDesktop();
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeIsDisplayOnDesktop");
}

bool openvr_bridge::SetDisplayVisibility(bool bIsVisibleOnDesktop)
{
   LOG_ENTRY("BridgeSetDisplayVisibility");
   bool rc;
   rc = m_down_stream.sysi->SetDisplayVisibility(bIsVisibleOnDesktop);

   LOG_EXIT_RC(rc, "BridgeSetDisplayVisibility");
}

void openvr_bridge::GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin, 
	float fPredictedSecondsToPhotonsFromNow, 
	struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount)
{
   LOG_ENTRY("BridgeGetDeviceToAbsoluteTrackingPose");

   if (m_lock_step_train_tracker)
   {
	   // to make it easier to memcmp
	   memset(pTrackedDevicePoseArray, 0, unTrackedDevicePoseArrayCount * sizeof(unTrackedDevicePoseArrayCount));
   }

   m_down_stream.sysi->GetDeviceToAbsoluteTrackingPose(eOrigin, fPredictedSecondsToPhotonsFromNow, pTrackedDevicePoseArray, unTrackedDevicePoseArrayCount);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::TrackedDevicePose_t *tmp = (struct vr::TrackedDevicePose_t *)malloc(sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount);
	   memset(tmp, 0, unTrackedDevicePoseArrayCount * sizeof(unTrackedDevicePoseArrayCount));
	   m_lock_step_tracker.sysi->GetDeviceToAbsoluteTrackingPose(eOrigin, fPredictedSecondsToPhotonsFromNow, tmp, unTrackedDevicePoseArrayCount);
	   for (int i = 0; i < size_as_int(unTrackedDevicePoseArrayCount); i++)
	   {
		   TRAIN_TRACKER_ASSERT(softcompare_is_similar(tmp[i], pTrackedDevicePoseArray[i], 0.0001f));
	   }
	   free(tmp);
   }

   LOG_EXIT("BridgeGetDeviceToAbsoluteTrackingPose");
}

void openvr_bridge::ResetSeatedZeroPose()
{
   LOG_ENTRY("BridgeResetSeatedZeroPose");
   
   m_down_stream.sysi->ResetSeatedZeroPose();
   
   LOG_EXIT("BridgeResetSeatedZeroPose");
}

struct vr::HmdMatrix34_t openvr_bridge::GetSeatedZeroPoseToStandingAbsoluteTrackingPose()
{
   LOG_ENTRY("BridgeGetSeatedZeroPoseToStandingAbsoluteTrackingPose");

   struct vr::HmdMatrix34_t rc;
   rc = m_down_stream.sysi->GetSeatedZeroPoseToStandingAbsoluteTrackingPose();

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::HmdMatrix34_t rc2 = m_lock_step_tracker.sysi->GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetSeatedZeroPoseToStandingAbsoluteTrackingPose");
}

struct vr::HmdMatrix34_t openvr_bridge::GetRawZeroPoseToStandingAbsoluteTrackingPose()
{
   LOG_ENTRY("BridgeGetRawZeroPoseToStandingAbsoluteTrackingPose");

   struct vr::HmdMatrix34_t rc;
   rc = m_down_stream.sysi->GetRawZeroPoseToStandingAbsoluteTrackingPose();

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::HmdMatrix34_t rc2 = m_lock_step_tracker.sysi->GetRawZeroPoseToStandingAbsoluteTrackingPose();
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetRawZeroPoseToStandingAbsoluteTrackingPose");
}

uint32_t openvr_bridge::GetSortedTrackedDeviceIndicesOfClass(vr::ETrackedDeviceClass eTrackedDeviceClass, 
	vr::TrackedDeviceIndex_t * punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, 
	vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex)
{
   LOG_ENTRY("BridgeGetSortedTrackedDeviceIndicesOfClass");

   if (m_lock_step_train_tracker)
   {
	   memset(punTrackedDeviceIndexArray, 0, unTrackedDeviceIndexArrayCount * sizeof(vr::TrackedDeviceIndex_t));
   }
	
   uint32_t rc = m_down_stream.sysi->GetSortedTrackedDeviceIndicesOfClass(
		   eTrackedDeviceClass, punTrackedDeviceIndexArray, unTrackedDeviceIndexArrayCount, unRelativeToTrackedDeviceIndex);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::TrackedDeviceIndex_t * tmp = (vr::TrackedDeviceIndex_t *) malloc(sizeof(vr::TrackedDeviceIndex_t) * unTrackedDeviceIndexArrayCount);
	   uint32_t rc2 = m_lock_step_tracker.sysi->GetSortedTrackedDeviceIndicesOfClass(eTrackedDeviceClass,
		   tmp, unTrackedDeviceIndexArrayCount, unRelativeToTrackedDeviceIndex);																							;
   }

   LOG_EXIT_RC(rc, "BridgeGetSortedTrackedDeviceIndicesOfClass");
}

vr::EDeviceActivityLevel openvr_bridge::GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId)
{
   LOG_ENTRY("BridgeGetTrackedDeviceActivityLevel");
   vr::EDeviceActivityLevel rc;
   rc = m_down_stream.sysi->GetTrackedDeviceActivityLevel(unDeviceId);
   
   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EDeviceActivityLevel rc2 = m_lock_step_tracker.sysi->GetTrackedDeviceActivityLevel(unDeviceId);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetTrackedDeviceActivityLevel");
}

void openvr_bridge::ApplyTransform(
		struct vr::TrackedDevicePose_t * pOutputPose, 
		const struct vr::TrackedDevicePose_t * pTrackedDevicePose, 
		const struct vr::HmdMatrix34_t * pTransform)
{
   LOG_ENTRY("BridgeApplyTransform");

   if (m_lock_step_train_tracker)
   {
	   memset(pOutputPose, 0, sizeof(*pOutputPose));
   }

   m_down_stream.sysi->ApplyTransform(pOutputPose, pTrackedDevicePose, pTransform);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::TrackedDevicePose_t pose2;
	   memset(&pose2, 0, sizeof(pose2));
	   m_lock_step_tracker.sysi->ApplyTransform(&pose2, pTrackedDevicePose, pTransform);
	   TRAIN_TRACKER_ASSERT(memcmp(&pose2, pOutputPose, sizeof(pose2))==0);
   }

   LOG_EXIT("BridgeApplyTransform");
}

vr::TrackedDeviceIndex_t openvr_bridge::GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType)
{
   LOG_ENTRY("BridgeGetTrackedDeviceIndexForControllerRole");
   vr::TrackedDeviceIndex_t rc;

	rc = m_down_stream.sysi->GetTrackedDeviceIndexForControllerRole(unDeviceType);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::TrackedDeviceIndex_t tracker_rc = m_lock_step_tracker.sysi->GetTrackedDeviceIndexForControllerRole(unDeviceType);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
   }
   LOG_EXIT_RC(rc, "BridgeGetTrackedDeviceIndexForControllerRole");
}

vr::ETrackedControllerRole openvr_bridge::GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex)
{
   LOG_ENTRY("BridgeGetControllerRoleForTrackedDeviceIndex");

	vr::ETrackedControllerRole rc;
	rc = m_down_stream.sysi->GetControllerRoleForTrackedDeviceIndex(unDeviceIndex);
   

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedControllerRole tracker_rc = m_lock_step_tracker.sysi->GetControllerRoleForTrackedDeviceIndex(unDeviceIndex);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
   }

   LOG_EXIT_RC(rc, "BridgeGetControllerRoleForTrackedDeviceIndex");
}

vr::ETrackedDeviceClass openvr_bridge::GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex)
{
   LOG_ENTRY("BridgeGetTrackedDeviceClass");

   vr::ETrackedDeviceClass rc;
   rc = m_down_stream.sysi->GetTrackedDeviceClass(unDeviceIndex);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedDeviceClass tracker_rc = m_lock_step_tracker.sysi->GetTrackedDeviceClass(unDeviceIndex);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
   }

   LOG_EXIT_RC(rc, "BridgeGetTrackedDeviceClass");
}

bool openvr_bridge::IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex)
{
	LOG_ENTRY("BridgeIsTrackedDeviceConnected");

	bool rc;
	rc = m_down_stream.sysi->IsTrackedDeviceConnected(unDeviceIndex);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   bool tracker_rc = m_lock_step_tracker.sysi->IsTrackedDeviceConnected(unDeviceIndex);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
   }

   LOG_EXIT_RC(rc, "BridgeIsTrackedDeviceConnected");
}

bool openvr_bridge::GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, 
	vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetBoolTrackedDeviceProperty");

   bool rc;
	rc = m_down_stream.sysi->GetBoolTrackedDeviceProperty(unDeviceIndex, prop, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedPropertyError tracker_err;
	   bool tracker_rc = m_lock_step_tracker.sysi->GetBoolTrackedDeviceProperty(unDeviceIndex, prop, &tracker_err);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetBoolTrackedDeviceProperty");
}

float openvr_bridge::GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, 
	vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetFloatTrackedDeviceProperty");

   float rc;
   rc = m_down_stream.sysi->GetFloatTrackedDeviceProperty(unDeviceIndex, prop, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedPropertyError tracker_err;
	   float tracker_rc = m_lock_step_tracker.sysi->GetFloatTrackedDeviceProperty(unDeviceIndex, prop, &tracker_err);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetFloatTrackedDeviceProperty");
}

int32_t openvr_bridge::GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, 
	vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetInt32TrackedDeviceProperty");
   int32_t rc;
   rc = m_down_stream.sysi->GetInt32TrackedDeviceProperty(unDeviceIndex, prop, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedPropertyError tracker_err;
	   int32_t tracker_rc = m_lock_step_tracker.sysi->GetInt32TrackedDeviceProperty(unDeviceIndex, prop, &tracker_err);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetInt32TrackedDeviceProperty");
}

uint64_t openvr_bridge::GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetUint64TrackedDeviceProperty");
   uint64_t rc;
   rc = m_down_stream.sysi->GetUint64TrackedDeviceProperty(unDeviceIndex, prop, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedPropertyError tracker_err;
	   uint64_t tracker_rc = m_lock_step_tracker.sysi->GetUint64TrackedDeviceProperty(unDeviceIndex, prop, &tracker_err);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetUint64TrackedDeviceProperty");
}

struct vr::HmdMatrix34_t openvr_bridge::GetMatrix34TrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetMatrix34TrackedDeviceProperty");

	struct vr::HmdMatrix34_t rc;
	rc = m_down_stream.sysi->GetMatrix34TrackedDeviceProperty(unDeviceIndex, prop, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::ETrackedPropertyError tracker_err;
	   vr::HmdMatrix34_t tracker_rc = m_lock_step_tracker.sysi->GetMatrix34TrackedDeviceProperty(unDeviceIndex, prop, &tracker_err);
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetMatrix34TrackedDeviceProperty");
}

uint32_t openvr_bridge::GetStringTrackedDeviceProperty(
	vr::TrackedDeviceIndex_t unDeviceIndex, 
	vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError)
{
   LOG_ENTRY("BridgeGetStringTrackedDeviceProperty");

   uint32_t rc;
	rc = m_down_stream.sysi->GetStringTrackedDeviceProperty(unDeviceIndex, prop, pchValue, unBufferSize, pError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   char *tracker_buf = (char *)malloc(unBufferSize);
	   vr::ETrackedPropertyError tracker_err;
	   uint32_t tracker_rc = m_lock_step_tracker.sysi->GetStringTrackedDeviceProperty(
		   unDeviceIndex, prop, tracker_buf, unBufferSize, &tracker_err);
	   if (pchValue)
	   {
		   TRAIN_TRACKER_ASSERT(strcmp(tracker_buf, pchValue) == 0);
	   }
	   TRAIN_TRACKER_ASSERT(tracker_rc == rc);
	   if (pError)
	   {
		   TRAIN_TRACKER_ASSERT(*pError == tracker_err);
	   }
	   free(tracker_buf);
   }

   LOG_EXIT_RC(rc, "BridgeGetStringTrackedDeviceProperty");
}

const char * openvr_bridge::GetPropErrorNameFromEnum(vr::ETrackedPropertyError error)
{
   LOG_ENTRY("BridgeGetPropErrorNameFromEnum");
   const char * rc;
	rc = m_down_stream.sysi->GetPropErrorNameFromEnum(error);
   
   LOG_EXIT_RC(rc, "BridgeGetPropErrorNameFromEnum");
}

// 
void openvr_bridge::process_poll_next_event_value(bool poll_rc, vr::VREvent_t * pEvent)
{
	if (poll_rc == false)  // Empty event-queue case
	{
		if (m_events_since_last_refresh)
		{
			update_capture_controller();
			m_events_since_last_refresh = false; // clear toggle
		}
	}
	else  // non-empty event queue case
	{
		capture_vr_event(*pEvent);
		m_events_since_last_refresh = true; // mark a flag so a state capture is taken once the queue is emptied
	}
}

bool openvr_bridge::PollNextEvent(struct vr::VREvent_t * pEvent, uint32_t uncbVREvent)
{
	LOG_ENTRY("BridgePollNextEvent");

	bool rc;
	rc = m_down_stream.sysi->PollNextEvent(pEvent, uncbVREvent);
	process_poll_next_event_value(rc, pEvent);
   
	LOG_EXIT_RC(rc, "BridgePollNextEvent");
}

bool openvr_bridge::PollNextEventWithPose(vr::ETrackingUniverseOrigin eOrigin, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t * pTrackedDevicePose)
{
   LOG_ENTRY("BridgePollNextEventWithPose");
   bool rc;
	rc = m_down_stream.sysi->PollNextEventWithPose(eOrigin, pEvent, uncbVREvent, pTrackedDevicePose);

   process_poll_next_event_value(rc, pEvent);

   LOG_EXIT_RC(rc, "BridgePollNextEventWithPose");
}

const char * openvr_bridge::GetEventTypeNameFromEnum(vr::EVREventType eType)
{
   LOG_ENTRY("BridgeGetEventTypeNameFromEnum");
   const char * rc;
	rc = m_down_stream.sysi->GetEventTypeNameFromEnum(eType);
   LOG_EXIT_RC(rc, "BridgeGetEventTypeNameFromEnum");
}

struct vr::HiddenAreaMesh_t openvr_bridge::GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type)
{
   LOG_ENTRY("BridgeGetHiddenAreaMesh");
   struct vr::HiddenAreaMesh_t rc;

	rc = m_down_stream.sysi->GetHiddenAreaMesh(eEye, type);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();

	   struct vr::HiddenAreaMesh_t tracker_rc = m_lock_step_tracker.sysi->GetHiddenAreaMesh(eEye, type);
	   TRAIN_TRACKER_ASSERT(tracker_rc.unTriangleCount == rc.unTriangleCount);

	   if (type == vr::k_eHiddenAreaMesh_LineLoop)
	   {
		   TRAIN_TRACKER_ASSERT(memcmp(rc.pVertexData, tracker_rc.pVertexData, tracker_rc.unTriangleCount * sizeof(vr::HmdVector2_t)) == 0);
	   }
	   else
	   {
		   TRAIN_TRACKER_ASSERT(memcmp(rc.pVertexData, tracker_rc.pVertexData, 3 * tracker_rc.unTriangleCount * sizeof(vr::HmdVector2_t)) == 0);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetHiddenAreaMesh");
}

bool openvr_bridge::GetControllerState(
	vr::TrackedDeviceIndex_t unControllerDeviceIndex, 
	vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize)
{
   LOG_ENTRY("BridgeGetControllerState");

   bool rc;
	rc = m_down_stream.sysi->GetControllerState(unControllerDeviceIndex,
		   pControllerState, unControllerStateSize);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::VRControllerState_t tracker_state;
	   bool tracker_rc = m_lock_step_tracker.sysi->GetControllerState(unControllerDeviceIndex,
									&tracker_state, sizeof(vr::VRControllerState_t));
	   TRAIN_TRACKER_ASSERT(rc == tracker_rc);

	   int score = softcompare_controllerstates(&tracker_state, pControllerState);
	   TRAIN_TRACKER_ASSERT(score < 7);
	   // time desync, e.g the packet numbers will be different.
   }

   LOG_EXIT_RC(rc, "BridgeGetControllerState");
}

bool openvr_bridge::GetControllerStateWithPose(
	vr::ETrackingUniverseOrigin eOrigin, 
	vr::TrackedDeviceIndex_t unControllerDeviceIndex, 
	vr::VRControllerState_t * pControllerState, uint32_t unControllerStateSize, 
	struct vr::TrackedDevicePose_t * pTrackedDevicePose)
{
   LOG_ENTRY("BridgeGetControllerStateWithPose");

	bool rc = m_down_stream.sysi->GetControllerStateWithPose(
		   eOrigin, unControllerDeviceIndex, pControllerState, unControllerStateSize, pTrackedDevicePose);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::VRControllerState_t tracker_controller_state;
	   vr::TrackedDevicePose_t tracker_pose;

	   bool tracker_rc = m_lock_step_tracker.sysi->GetControllerStateWithPose(
		   eOrigin, unControllerDeviceIndex, &tracker_controller_state, unControllerStateSize, &tracker_pose);

	   TRAIN_TRACKER_ASSERT(rc == tracker_rc);
	   TRAIN_TRACKER_ASSERT(tracker_controller_state == *pControllerState); // time desync
	   TRAIN_TRACKER_ASSERT(tracker_pose == *pTrackedDevicePose);
   }

   LOG_EXIT_RC(rc, "BridgeGetControllerStateWithPose");
}

void openvr_bridge::TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec)
{
   LOG_ENTRY("BridgeTriggerHapticPulse");
	m_down_stream.sysi->TriggerHapticPulse(unControllerDeviceIndex, unAxisId, usDurationMicroSec);
   LOG_EXIT("BridgeTriggerHapticPulse");
}

const char * openvr_bridge::GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId)
{
   LOG_ENTRY("BridgeGetButtonIdNameFromEnum");
   const char *rc;
	rc = m_down_stream.sysi->GetButtonIdNameFromEnum(eButtonId);
   
   LOG_EXIT_RC(rc, "BridgeGetButtonIdNameFromEnum");
}

const char * openvr_bridge::GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType)
{
   LOG_ENTRY("BridgeGetControllerAxisTypeNameFromEnum");
   const char *rc;
	rc = m_down_stream.sysi->GetControllerAxisTypeNameFromEnum(eAxisType);

   LOG_EXIT_RC(rc, "BridgeGetControllerAxisTypeNameFromEnum");
}

bool openvr_bridge::CaptureInputFocus()
{
   LOG_ENTRY("BridgeCaptureInputFocus");
   bool rc;
	rc = m_down_stream.sysi->CaptureInputFocus();
   LOG_EXIT_RC(rc, "BridgeCaptureInputFocus");
}

void openvr_bridge::ReleaseInputFocus()
{
   LOG_ENTRY("BridgeReleaseInputFocus");
   m_down_stream.sysi->ReleaseInputFocus();
   LOG_EXIT("BridgeReleaseInputFocus");
}

bool openvr_bridge::IsInputFocusCapturedByAnotherProcess()
{
   LOG_ENTRY("BridgeIsInputFocusCapturedByAnotherProcess");
   bool rc = m_down_stream.sysi->IsInputFocusCapturedByAnotherProcess();

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   bool tracker_rc = m_lock_step_tracker.sysi->IsInputFocusCapturedByAnotherProcess();
	   TRAIN_TRACKER_ASSERT(rc == tracker_rc);
   }

   LOG_EXIT_RC(rc, "BridgeIsInputFocusCapturedByAnotherProcess");
}

uint32_t openvr_bridge::DriverDebugRequest(
				vr::TrackedDeviceIndex_t unDeviceIndex, 
				const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize)
{
   LOG_ENTRY("BridgeDriverDebugRequest");
   uint32_t rc = m_down_stream.sysi->DriverDebugRequest(unDeviceIndex,pchRequest,pchResponseBuffer,unResponseBufferSize);
   LOG_EXIT_RC(rc, "BridgeDriverDebugRequest");
}

vr::EVRFirmwareError openvr_bridge::PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex)
{
   LOG_ENTRY("BridgePerformFirmwareUpdate");
   vr::EVRFirmwareError rc = m_down_stream.sysi->PerformFirmwareUpdate(unDeviceIndex);
   LOG_EXIT_RC(rc, "BridgePerformFirmwareUpdate");
}

void openvr_bridge::AcknowledgeQuit_Exiting()
{
   LOG_ENTRY("BridgeAcknowledgeQuit_Exiting");
   m_down_stream.sysi->AcknowledgeQuit_Exiting();
   LOG_EXIT("BridgeAcknowledgeQuit_Exiting");
}

void openvr_bridge::AcknowledgeQuit_UserPrompt()
{
   LOG_ENTRY("BridgeAcknowledgeQuit_UserPrompt");
   m_down_stream.sysi->AcknowledgeQuit_UserPrompt();
   LOG_EXIT("BridgeAcknowledgeQuit_UserPrompt");
}

void openvr_bridge::GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
   LOG_ENTRY("BridgeGetWindowBounds");
   m_down_stream.exdi->GetWindowBounds(pnX,pnY,pnWidth,pnHeight);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   int32_t i[2];
	   uint32_t j[2];
	   m_lock_step_tracker.exdi->GetWindowBounds(&i[0], &i[1], &j[0], &j[1]);
	   if (pnX)
		   TRAIN_TRACKER_ASSERT(*pnX == i[0]);
	   if (pnY)
		   TRAIN_TRACKER_ASSERT(*pnY == i[1]);
	   if (pnWidth)
		   TRAIN_TRACKER_ASSERT(*pnWidth == j[0]);
	   if (pnHeight)
		   TRAIN_TRACKER_ASSERT(*pnHeight == j[1]);
   }

   LOG_EXIT("BridgeGetWindowBounds");
}

void openvr_bridge::GetEyeOutputViewport(vr::EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight)
{
   LOG_ENTRY("BridgeGetEyeOutputViewport");
   m_down_stream.exdi->GetEyeOutputViewport(eEye,pnX,pnY,pnWidth,pnHeight);
   LOG_EXIT("BridgeGetEyeOutputViewport");
}

void openvr_bridge::GetDXGIOutputInfo(int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex)
{
   LOG_ENTRY("BridgeGetDXGIOutputInfo");
   m_down_stream.exdi->GetDXGIOutputInfo(pnAdapterIndex,pnAdapterOutputIndex);
   LOG_EXIT("BridgeGetDXGIOutputInfo");
}

const char * openvr_bridge::GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError)
{
   LOG_ENTRY("BridgeGetCameraErrorNameFromEnum");
   const char * rc = m_down_stream.taci->GetCameraErrorNameFromEnum(eCameraError);
   LOG_EXIT_RC(rc, "BridgeGetCameraErrorNameFromEnum");
}

vr::EVRTrackedCameraError openvr_bridge::HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool * pHasCamera)
{
   LOG_ENTRY("BridgeHasCamera");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->HasCamera(nDeviceIndex,pHasCamera);
   LOG_EXIT_RC(rc, "BridgeHasCamera");
}

vr::EVRTrackedCameraError openvr_bridge::GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t * pnWidth, uint32_t * pnHeight, uint32_t * pnFrameBufferSize)
{
   LOG_ENTRY("BridgeGetCameraFrameSize");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetCameraFrameSize(nDeviceIndex,eFrameType,pnWidth,pnHeight,pnFrameBufferSize);
   LOG_EXIT_RC(rc, "BridgeGetCameraFrameSize");
}

vr::EVRTrackedCameraError openvr_bridge::GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t * pFocalLength, vr::HmdVector2_t * pCenter)
{
   LOG_ENTRY("BridgeGetCameraIntrinsics");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetCameraIntrinsics(nDeviceIndex,eFrameType,pFocalLength,pCenter);
   LOG_EXIT_RC(rc, "BridgeGetCameraIntrinsics");
}

vr::EVRTrackedCameraError openvr_bridge::GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t * pProjection)
{
   LOG_ENTRY("BridgeGetCameraProjection");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetCameraProjection(nDeviceIndex,eFrameType,flZNear,flZFar,pProjection);
   LOG_EXIT_RC(rc, "BridgeGetCameraProjection");
}

vr::EVRTrackedCameraError openvr_bridge::AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t * pHandle)
{
   LOG_ENTRY("BridgeAcquireVideoStreamingService");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->AcquireVideoStreamingService(nDeviceIndex,pHandle);
   LOG_EXIT_RC(rc, "BridgeAcquireVideoStreamingService");
}

vr::EVRTrackedCameraError openvr_bridge::ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera)
{
   LOG_ENTRY("BridgeReleaseVideoStreamingService");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->ReleaseVideoStreamingService(hTrackedCamera);
   LOG_EXIT_RC(rc, "BridgeReleaseVideoStreamingService");
}

vr::EVRTrackedCameraError openvr_bridge::GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
   LOG_ENTRY("BridgeGetVideoStreamFrameBuffer");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetVideoStreamFrameBuffer(hTrackedCamera,eFrameType,pFrameBuffer,nFrameBufferSize,pFrameHeader,nFrameHeaderSize);
   LOG_EXIT_RC(rc, "BridgeGetVideoStreamFrameBuffer");
}

vr::EVRTrackedCameraError openvr_bridge::GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t * pTextureBounds, uint32_t * pnWidth, uint32_t * pnHeight)
{
   LOG_ENTRY("BridgeGetVideoStreamTextureSize");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetVideoStreamTextureSize(nDeviceIndex,eFrameType,pTextureBounds,pnWidth,pnHeight);
   LOG_EXIT_RC(rc, "BridgeGetVideoStreamTextureSize");
}

vr::EVRTrackedCameraError openvr_bridge::GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
   LOG_ENTRY("BridgeGetVideoStreamTextureD3D11");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetVideoStreamTextureD3D11(hTrackedCamera,eFrameType,pD3D11DeviceOrResource,ppD3D11ShaderResourceView,pFrameHeader,nFrameHeaderSize);
   LOG_EXIT_RC(rc, "BridgeGetVideoStreamTextureD3D11");
}

vr::EVRTrackedCameraError openvr_bridge::GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t * pglTextureId, vr::CameraVideoStreamFrameHeader_t * pFrameHeader, uint32_t nFrameHeaderSize)
{
   LOG_ENTRY("BridgeGetVideoStreamTextureGL");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->GetVideoStreamTextureGL(hTrackedCamera,eFrameType,pglTextureId,pFrameHeader,nFrameHeaderSize);
   LOG_EXIT_RC(rc, "BridgeGetVideoStreamTextureGL");
}

vr::EVRTrackedCameraError openvr_bridge::ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId)
{
   LOG_ENTRY("BridgeReleaseVideoStreamTextureGL");
   vr::EVRTrackedCameraError rc = m_down_stream.taci->ReleaseVideoStreamTextureGL(hTrackedCamera,glTextureId);
   LOG_EXIT_RC(rc, "BridgeReleaseVideoStreamTextureGL");
}

vr::EVRApplicationError openvr_bridge::AddApplicationManifest(const char * pchApplicationManifestFullPath, bool bTemporary)
{
   LOG_ENTRY("BridgeAddApplicationManifest");
   vr::EVRApplicationError rc = m_down_stream.appi->AddApplicationManifest(pchApplicationManifestFullPath,bTemporary);
   LOG_EXIT_RC(rc, "BridgeAddApplicationManifest");
}

vr::EVRApplicationError openvr_bridge::RemoveApplicationManifest(const char * pchApplicationManifestFullPath)
{
   LOG_ENTRY("BridgeRemoveApplicationManifest");
   vr::EVRApplicationError rc = m_down_stream.appi->RemoveApplicationManifest(pchApplicationManifestFullPath);
   LOG_EXIT_RC(rc, "BridgeRemoveApplicationManifest");
}

bool openvr_bridge::IsApplicationInstalled(const char * pchAppKey)
{
   LOG_ENTRY("BridgeIsApplicationInstalled");
   bool rc = m_down_stream.appi->IsApplicationInstalled(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeIsApplicationInstalled");
}

uint32_t openvr_bridge::GetApplicationCount()
{
   LOG_ENTRY("BridgeGetApplicationCount");
   uint32_t rc = m_down_stream.appi->GetApplicationCount();
   LOG_EXIT_RC(rc, "BridgeGetApplicationCount");
}

vr::EVRApplicationError openvr_bridge::GetApplicationKeyByIndex(uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
   LOG_ENTRY("BridgeGetApplicationKeyByIndex");
   vr::EVRApplicationError rc = m_down_stream.appi->GetApplicationKeyByIndex(unApplicationIndex,pchAppKeyBuffer,unAppKeyBufferLen);
   LOG_EXIT_RC(rc, "BridgeGetApplicationKeyByIndex");
}

vr::EVRApplicationError openvr_bridge::GetApplicationKeyByProcessId(uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
   LOG_ENTRY("BridgeGetApplicationKeyByProcessId");
   vr::EVRApplicationError rc = m_down_stream.appi->GetApplicationKeyByProcessId(unProcessId,pchAppKeyBuffer,unAppKeyBufferLen);
   LOG_EXIT_RC(rc, "BridgeGetApplicationKeyByProcessId");
}

vr::EVRApplicationError openvr_bridge::LaunchApplication(const char * pchAppKey)
{
   LOG_ENTRY("BridgeLaunchApplication");
   vr::EVRApplicationError rc = m_down_stream.appi->LaunchApplication(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeLaunchApplication");
}

vr::EVRApplicationError openvr_bridge::LaunchTemplateApplication(const char * pchTemplateAppKey, const char * pchNewAppKey, const struct vr::AppOverrideKeys_t * pKeys, uint32_t unKeys)
{
   LOG_ENTRY("BridgeLaunchTemplateApplication");
   vr::EVRApplicationError rc = m_down_stream.appi->LaunchTemplateApplication(pchTemplateAppKey,pchNewAppKey,pKeys,unKeys);
   LOG_EXIT_RC(rc, "BridgeLaunchTemplateApplication");
}

vr::EVRApplicationError openvr_bridge::LaunchApplicationFromMimeType(const char * pchMimeType, const char * pchArgs)
{
   LOG_ENTRY("BridgeLaunchApplicationFromMimeType");
   vr::EVRApplicationError rc = m_down_stream.appi->LaunchApplicationFromMimeType(pchMimeType,pchArgs);
   LOG_EXIT_RC(rc, "BridgeLaunchApplicationFromMimeType");
}

vr::EVRApplicationError openvr_bridge::LaunchDashboardOverlay(const char * pchAppKey)
{
   LOG_ENTRY("BridgeLaunchDashboardOverlay");
   vr::EVRApplicationError rc = m_down_stream.appi->LaunchDashboardOverlay(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeLaunchDashboardOverlay");
}

bool openvr_bridge::CancelApplicationLaunch(const char * pchAppKey)
{
   LOG_ENTRY("BridgeCancelApplicationLaunch");
   bool rc = m_down_stream.appi->CancelApplicationLaunch(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeCancelApplicationLaunch");
}

vr::EVRApplicationError openvr_bridge::IdentifyApplication(uint32_t unProcessId, const char * pchAppKey)
{
   LOG_ENTRY("BridgeIdentifyApplication");
   vr::EVRApplicationError rc = m_down_stream.appi->IdentifyApplication(unProcessId,pchAppKey);
   LOG_EXIT_RC(rc, "BridgeIdentifyApplication");
}

uint32_t openvr_bridge::GetApplicationProcessId(const char * pchAppKey)
{
   LOG_ENTRY("BridgeGetApplicationProcessId");
   uint32_t rc = m_down_stream.appi->GetApplicationProcessId(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeGetApplicationProcessId");
}

const char * openvr_bridge::GetApplicationsErrorNameFromEnum(vr::EVRApplicationError error)
{
   LOG_ENTRY("BridgeGetApplicationsErrorNameFromEnum");
   const char * rc = m_down_stream.appi->GetApplicationsErrorNameFromEnum(error);
   LOG_EXIT_RC(rc, "BridgeGetApplicationsErrorNameFromEnum");
}

uint32_t openvr_bridge::GetApplicationPropertyString(const char * pchAppKey, vr::EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, vr::EVRApplicationError * peError)
{
   LOG_ENTRY("BridgeGetApplicationPropertyString");
   uint32_t rc = m_down_stream.appi->GetApplicationPropertyString(pchAppKey,eProperty,pchPropertyValueBuffer,unPropertyValueBufferLen,peError);
   LOG_EXIT_RC(rc, "BridgeGetApplicationPropertyString");
}

bool openvr_bridge::GetApplicationPropertyBool(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError)
{
   LOG_ENTRY("BridgeGetApplicationPropertyBool");
   bool rc = m_down_stream.appi->GetApplicationPropertyBool(pchAppKey,eProperty,peError);
   LOG_EXIT_RC(rc, "BridgeGetApplicationPropertyBool");
}

uint64_t openvr_bridge::GetApplicationPropertyUint64(const char * pchAppKey, vr::EVRApplicationProperty eProperty, vr::EVRApplicationError * peError)
{
   LOG_ENTRY("BridgeGetApplicationPropertyUint64");
   uint64_t rc = m_down_stream.appi->GetApplicationPropertyUint64(pchAppKey,eProperty,peError);
   LOG_EXIT_RC(rc, "BridgeGetApplicationPropertyUint64");
}

vr::EVRApplicationError openvr_bridge::SetApplicationAutoLaunch(const char * pchAppKey, bool bAutoLaunch)
{
   LOG_ENTRY("BridgeSetApplicationAutoLaunch");
   vr::EVRApplicationError rc = m_down_stream.appi->SetApplicationAutoLaunch(pchAppKey,bAutoLaunch);
   LOG_EXIT_RC(rc, "BridgeSetApplicationAutoLaunch");
}

bool openvr_bridge::GetApplicationAutoLaunch(const char * pchAppKey)
{
   LOG_ENTRY("BridgeGetApplicationAutoLaunch");
   bool rc = m_down_stream.appi->GetApplicationAutoLaunch(pchAppKey);
   LOG_EXIT_RC(rc, "BridgeGetApplicationAutoLaunch");
}

vr::EVRApplicationError openvr_bridge::SetDefaultApplicationForMimeType(const char * pchAppKey, const char * pchMimeType)
{
   LOG_ENTRY("BridgeSetDefaultApplicationForMimeType");
   vr::EVRApplicationError rc = m_down_stream.appi->SetDefaultApplicationForMimeType(pchAppKey,pchMimeType);
   LOG_EXIT_RC(rc, "BridgeSetDefaultApplicationForMimeType");
}

bool openvr_bridge::GetDefaultApplicationForMimeType(const char * pchMimeType, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
   LOG_ENTRY("BridgeGetDefaultApplicationForMimeType");
   bool rc = m_down_stream.appi->GetDefaultApplicationForMimeType(pchMimeType,pchAppKeyBuffer,unAppKeyBufferLen);
   LOG_EXIT_RC(rc, "BridgeGetDefaultApplicationForMimeType");
}

bool openvr_bridge::GetApplicationSupportedMimeTypes(const char * pchAppKey, char * pchMimeTypesBuffer, uint32_t unMimeTypesBuffer)
{
   LOG_ENTRY("BridgeGetApplicationSupportedMimeTypes");
   bool rc = m_down_stream.appi->GetApplicationSupportedMimeTypes(pchAppKey,pchMimeTypesBuffer,unMimeTypesBuffer);
   LOG_EXIT_RC(rc, "BridgeGetApplicationSupportedMimeTypes");
}

uint32_t openvr_bridge::GetApplicationsThatSupportMimeType(const char * pchMimeType, char * pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer)
{
   LOG_ENTRY("BridgeGetApplicationsThatSupportMimeType");
   uint32_t rc = m_down_stream.appi->GetApplicationsThatSupportMimeType(pchMimeType,pchAppKeysThatSupportBuffer,unAppKeysThatSupportBuffer);
   LOG_EXIT_RC(rc, "BridgeGetApplicationsThatSupportMimeType");
}

uint32_t openvr_bridge::GetApplicationLaunchArguments(uint32_t unHandle, char * pchArgs, uint32_t unArgs)
{
   LOG_ENTRY("BridgeGetApplicationLaunchArguments");
   uint32_t rc = m_down_stream.appi->GetApplicationLaunchArguments(unHandle,pchArgs,unArgs);
   LOG_EXIT_RC(rc, "BridgeGetApplicationLaunchArguments");
}

vr::EVRApplicationError openvr_bridge::GetStartingApplication(char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen)
{
   LOG_ENTRY("BridgeGetStartingApplication");
   vr::EVRApplicationError rc = m_down_stream.appi->GetStartingApplication(pchAppKeyBuffer,unAppKeyBufferLen);
   LOG_EXIT_RC(rc, "BridgeGetStartingApplication");
}

vr::EVRApplicationTransitionState openvr_bridge::GetTransitionState()
{
   LOG_ENTRY("BridgeGetTransitionState");
   vr::EVRApplicationTransitionState rc = m_down_stream.appi->GetTransitionState();
   LOG_EXIT_RC(rc, "BridgeGetTransitionState");
}

vr::EVRApplicationError openvr_bridge::PerformApplicationPrelaunchCheck(const char * pchAppKey)
{
   LOG_ENTRY("BridgePerformApplicationPrelaunchCheck");
   vr::EVRApplicationError rc = m_down_stream.appi->PerformApplicationPrelaunchCheck(pchAppKey);
   LOG_EXIT_RC(rc, "BridgePerformApplicationPrelaunchCheck");
}

const char * openvr_bridge::GetApplicationsTransitionStateNameFromEnum(vr::EVRApplicationTransitionState state)
{
   LOG_ENTRY("BridgeGetApplicationsTransitionStateNameFromEnum");
   const char * rc = m_down_stream.appi->GetApplicationsTransitionStateNameFromEnum(state);
   LOG_EXIT_RC(rc, "BridgeGetApplicationsTransitionStateNameFromEnum");
}

bool openvr_bridge::IsQuitUserPromptRequested()
{
   LOG_ENTRY("BridgeIsQuitUserPromptRequested");
   bool rc = m_down_stream.appi->IsQuitUserPromptRequested();
   LOG_EXIT_RC(rc, "BridgeIsQuitUserPromptRequested");
}

vr::EVRApplicationError openvr_bridge::LaunchInternalProcess(const char * pchBinaryPath, const char * pchArguments, const char * pchWorkingDirectory)
{
   LOG_ENTRY("BridgeLaunchInternalProcess");
   vr::EVRApplicationError rc = m_down_stream.appi->LaunchInternalProcess(pchBinaryPath,pchArguments,pchWorkingDirectory);
   LOG_EXIT_RC(rc, "BridgeLaunchInternalProcess");
}

uint32_t openvr_bridge::GetCurrentSceneProcessId()
{
   LOG_ENTRY("BridgeGetCurrentSceneProcessId");
   uint32_t rc = m_down_stream.appi->GetCurrentSceneProcessId();
   LOG_EXIT_RC(rc, "BridgeGetCurrentSceneProcessId");
}

vr::ChaperoneCalibrationState openvr_bridge::GetCalibrationState()
{
   LOG_ENTRY("BridgeGetCalibrationState");
   vr::ChaperoneCalibrationState rc = m_down_stream.chapi->GetCalibrationState();
   LOG_EXIT_RC(rc, "BridgeGetCalibrationState");
}

bool openvr_bridge::GetPlayAreaSize(float * pSizeX, float * pSizeZ)
{
   LOG_ENTRY("BridgeGetPlayAreaSize");
   bool rc = m_down_stream.chapi->GetPlayAreaSize(pSizeX,pSizeZ);
   LOG_EXIT_RC(rc, "BridgeGetPlayAreaSize");
}

bool openvr_bridge::GetPlayAreaRect(struct vr::HmdQuad_t * rect)
{
   LOG_ENTRY("BridgeGetPlayAreaRect");
   bool rc = m_down_stream.chapi->GetPlayAreaRect(rect);
   LOG_EXIT_RC(rc, "BridgeGetPlayAreaRect");
}

void openvr_bridge::ReloadInfo()
{
   LOG_ENTRY("BridgeReloadInfo");
   m_down_stream.chapi->ReloadInfo();
   LOG_EXIT("BridgeReloadInfo");
}

void openvr_bridge::SetSceneColor(struct vr::HmdColor_t color)
{
   LOG_ENTRY("BridgeSetSceneColor");
   m_down_stream.chapi->SetSceneColor(color);
   LOG_EXIT("BridgeSetSceneColor");
}

void openvr_bridge::GetBoundsColor(struct vr::HmdColor_t * pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, struct vr::HmdColor_t * pOutputCameraColor)
{
   LOG_ENTRY("BridgeGetBoundsColor");
   m_down_stream.chapi->GetBoundsColor(pOutputColorArray,nNumOutputColors,flCollisionBoundsFadeDistance,pOutputCameraColor);
   LOG_EXIT("BridgeGetBoundsColor");
}

bool openvr_bridge::AreBoundsVisible()
{
   LOG_ENTRY("BridgeAreBoundsVisible");
   bool rc = m_down_stream.chapi->AreBoundsVisible();
   LOG_EXIT_RC(rc, "BridgeAreBoundsVisible");
}

void openvr_bridge::ForceBoundsVisible(bool bForce)
{
   LOG_ENTRY("BridgeForceBoundsVisible");
   m_down_stream.chapi->ForceBoundsVisible(bForce);
   LOG_EXIT("BridgeForceBoundsVisible");
}

bool openvr_bridge::CommitWorkingCopy(vr::EChaperoneConfigFile configFile)
{
   LOG_ENTRY("BridgeCommitWorkingCopy");
   bool rc = m_down_stream.chapsi->CommitWorkingCopy(configFile);
   LOG_EXIT_RC(rc, "BridgeCommitWorkingCopy");
}

void openvr_bridge::RevertWorkingCopy()
{
   LOG_ENTRY("BridgeRevertWorkingCopy");
   m_down_stream.chapsi->RevertWorkingCopy();
   LOG_EXIT("BridgeRevertWorkingCopy");
}

bool openvr_bridge::GetWorkingPlayAreaSize(float * pSizeX, float * pSizeZ)
{
   LOG_ENTRY("BridgeGetWorkingPlayAreaSize");
   bool rc = m_down_stream.chapsi->GetWorkingPlayAreaSize(pSizeX,pSizeZ);
   LOG_EXIT_RC(rc, "BridgeGetWorkingPlayAreaSize");
}

bool openvr_bridge::GetWorkingPlayAreaRect(struct vr::HmdQuad_t * rect)
{
   LOG_ENTRY("BridgeGetWorkingPlayAreaRect");
   bool rc = m_down_stream.chapsi->GetWorkingPlayAreaRect(rect);
   LOG_EXIT_RC(rc, "BridgeGetWorkingPlayAreaRect");
}

bool openvr_bridge::GetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
   LOG_ENTRY("BridgeGetWorkingCollisionBoundsInfo");
   bool rc = m_down_stream.chapsi->GetWorkingCollisionBoundsInfo(pQuadsBuffer,punQuadsCount);
   LOG_EXIT_RC(rc, "BridgeGetWorkingCollisionBoundsInfo");
}

bool openvr_bridge::GetLiveCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
   LOG_ENTRY("BridgeGetLiveCollisionBoundsInfo");
   bool rc = m_down_stream.chapsi->GetLiveCollisionBoundsInfo(pQuadsBuffer,punQuadsCount);
   LOG_EXIT_RC(rc, "BridgeGetLiveCollisionBoundsInfo");
}

bool openvr_bridge::GetWorkingSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose)
{
   LOG_ENTRY("BridgeGetWorkingSeatedZeroPoseToRawTrackingPose");
   bool rc = m_down_stream.chapsi->GetWorkingSeatedZeroPoseToRawTrackingPose(pmatSeatedZeroPoseToRawTrackingPose);
   LOG_EXIT_RC(rc, "BridgeGetWorkingSeatedZeroPoseToRawTrackingPose");
}

bool openvr_bridge::GetWorkingStandingZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose)
{
   LOG_ENTRY("BridgeGetWorkingStandingZeroPoseToRawTrackingPose");
   bool rc = m_down_stream.chapsi->GetWorkingStandingZeroPoseToRawTrackingPose(pmatStandingZeroPoseToRawTrackingPose);
   LOG_EXIT_RC(rc, "BridgeGetWorkingStandingZeroPoseToRawTrackingPose");
}

void openvr_bridge::SetWorkingPlayAreaSize(float sizeX, float sizeZ)
{
   LOG_ENTRY("BridgeSetWorkingPlayAreaSize");
   m_down_stream.chapsi->SetWorkingPlayAreaSize(sizeX,sizeZ);
   LOG_EXIT("BridgeSetWorkingPlayAreaSize");
}

void openvr_bridge::SetWorkingCollisionBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount)
{
   LOG_ENTRY("BridgeSetWorkingCollisionBoundsInfo");
   m_down_stream.chapsi->SetWorkingCollisionBoundsInfo(pQuadsBuffer,unQuadsCount);
   LOG_EXIT("BridgeSetWorkingCollisionBoundsInfo");
}

void openvr_bridge::SetWorkingSeatedZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatSeatedZeroPoseToRawTrackingPose)
{
   LOG_ENTRY("BridgeSetWorkingSeatedZeroPoseToRawTrackingPose");
   m_down_stream.chapsi->SetWorkingSeatedZeroPoseToRawTrackingPose(pMatSeatedZeroPoseToRawTrackingPose);
   LOG_EXIT("BridgeSetWorkingSeatedZeroPoseToRawTrackingPose");
}

void openvr_bridge::SetWorkingStandingZeroPoseToRawTrackingPose(const struct vr::HmdMatrix34_t * pMatStandingZeroPoseToRawTrackingPose)
{
   LOG_ENTRY("BridgeSetWorkingStandingZeroPoseToRawTrackingPose");
   m_down_stream.chapsi->SetWorkingStandingZeroPoseToRawTrackingPose(pMatStandingZeroPoseToRawTrackingPose);
   LOG_EXIT("BridgeSetWorkingStandingZeroPoseToRawTrackingPose");
}

void openvr_bridge::ReloadFromDisk(vr::EChaperoneConfigFile configFile)
{
   LOG_ENTRY("BridgeReloadFromDisk");
   m_down_stream.chapsi->ReloadFromDisk(configFile);
   LOG_EXIT("BridgeReloadFromDisk");
}

bool openvr_bridge::GetLiveSeatedZeroPoseToRawTrackingPose(struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose)
{
   LOG_ENTRY("BridgeGetLiveSeatedZeroPoseToRawTrackingPose");
   bool rc = m_down_stream.chapsi->GetLiveSeatedZeroPoseToRawTrackingPose(pmatSeatedZeroPoseToRawTrackingPose);
   LOG_EXIT_RC(rc, "BridgeGetLiveSeatedZeroPoseToRawTrackingPose");
}

void openvr_bridge::SetWorkingCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t unTagCount)
{
   LOG_ENTRY("BridgeSetWorkingCollisionBoundsTagsInfo");
   m_down_stream.chapsi->SetWorkingCollisionBoundsTagsInfo(pTagsBuffer,unTagCount);
   LOG_EXIT("BridgeSetWorkingCollisionBoundsTagsInfo");
}

bool openvr_bridge::GetLiveCollisionBoundsTagsInfo(uint8_t * pTagsBuffer, uint32_t * punTagCount)
{
   LOG_ENTRY("BridgeGetLiveCollisionBoundsTagsInfo");
   bool rc = m_down_stream.chapsi->GetLiveCollisionBoundsTagsInfo(pTagsBuffer,punTagCount);
   LOG_EXIT_RC(rc, "BridgeGetLiveCollisionBoundsTagsInfo");
}

bool openvr_bridge::SetWorkingPhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount)
{
   LOG_ENTRY("BridgeSetWorkingPhysicalBoundsInfo");
   bool rc = m_down_stream.chapsi->SetWorkingPhysicalBoundsInfo(pQuadsBuffer,unQuadsCount);
   LOG_EXIT_RC(rc, "BridgeSetWorkingPhysicalBoundsInfo");
}

bool openvr_bridge::GetLivePhysicalBoundsInfo(struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount)
{
   LOG_ENTRY("BridgeGetLivePhysicalBoundsInfo");
   bool rc = m_down_stream.chapsi->GetLivePhysicalBoundsInfo(pQuadsBuffer,punQuadsCount);
   LOG_EXIT_RC(rc, "BridgeGetLivePhysicalBoundsInfo");
}

bool openvr_bridge::ExportLiveToBuffer(char * pBuffer, uint32_t * pnBufferLength)
{
   LOG_ENTRY("BridgeExportLiveToBuffer");
   bool rc = m_down_stream.chapsi->ExportLiveToBuffer(pBuffer,pnBufferLength);
   LOG_EXIT_RC(rc, "BridgeExportLiveToBuffer");
}

bool openvr_bridge::ImportFromBufferToWorking(const char * pBuffer, uint32_t nImportFlags)
{
   LOG_ENTRY("BridgeImportFromBufferToWorking");
   bool rc = m_down_stream.chapsi->ImportFromBufferToWorking(pBuffer,nImportFlags);
   LOG_EXIT_RC(rc, "BridgeImportFromBufferToWorking");
}

void openvr_bridge::SetTrackingSpace(vr::ETrackingUniverseOrigin eOrigin)
{
   LOG_ENTRY("BridgeSetTrackingSpace");
   m_down_stream.compi->SetTrackingSpace(eOrigin);
   LOG_EXIT("BridgeSetTrackingSpace");
}

vr::ETrackingUniverseOrigin openvr_bridge::GetTrackingSpace()
{
   LOG_ENTRY("BridgeGetTrackingSpace");
   vr::ETrackingUniverseOrigin rc = m_down_stream.compi->GetTrackingSpace();
   LOG_EXIT_RC(rc, "BridgeGetTrackingSpace");
}

vr::EVRCompositorError openvr_bridge::WaitGetPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
   LOG_ENTRY("BridgeWaitGetPoses");

   vr::EVRCompositorError rc = m_down_stream.compi->WaitGetPoses(pRenderPoseArray,unRenderPoseArrayCount,pGamePoseArray,unGamePoseArrayCount);

   if (m_snapshot_playback_mode)
   {
	   using namespace std::chrono_literals;
	   std::this_thread::sleep_for(16ms);
	   advance_cursor_one_frame();
   }

   update_capture_controller();

   LOG_EXIT_RC(rc, "BridgeWaitGetPoses");
}

vr::EVRCompositorError openvr_bridge::GetLastPoses(struct vr::TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct vr::TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount)
{
   LOG_ENTRY("BridgeGetLastPoses");
   vr::EVRCompositorError rc = m_down_stream.compi->GetLastPoses(pRenderPoseArray,unRenderPoseArrayCount,pGamePoseArray,unGamePoseArrayCount);
   LOG_EXIT_RC(rc, "BridgeGetLastPoses");
}

vr::EVRCompositorError openvr_bridge::GetLastPoseForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex, struct vr::TrackedDevicePose_t * pOutputPose, struct vr::TrackedDevicePose_t * pOutputGamePose)
{
   LOG_ENTRY("BridgeGetLastPoseForTrackedDeviceIndex");
   vr::EVRCompositorError rc = m_down_stream.compi->GetLastPoseForTrackedDeviceIndex(unDeviceIndex,pOutputPose,pOutputGamePose);
   LOG_EXIT_RC(rc, "BridgeGetLastPoseForTrackedDeviceIndex");
}

vr::EVRCompositorError openvr_bridge::Submit(
		vr::EVREye eEye, 
		const struct vr::Texture_t * pTexture, 
		const struct vr::VRTextureBounds_t * pBounds, vr::EVRSubmitFlags nSubmitFlags)
{
   LOG_ENTRY("BridgeSubmit");
   vr::EVRCompositorError rc = m_down_stream.compi->Submit(eEye,pTexture,pBounds,nSubmitFlags);

   if (m_aux_compositor)
   {
	   vr::EVRCompositorError err = m_aux_compositor->Submit(eEye, pTexture, pBounds, nSubmitFlags);
	   if (rc == vr::VRCompositorError_None && err != vr::VRCompositorError_None)
	   {
		   rc = err;
	   }
   }

   LOG_EXIT_RC(rc, "BridgeSubmit");
}

void openvr_bridge::ClearLastSubmittedFrame()
{
   LOG_ENTRY("BridgeClearLastSubmittedFrame");
   m_down_stream.compi->ClearLastSubmittedFrame();
   LOG_EXIT("BridgeClearLastSubmittedFrame");
}

void openvr_bridge::PostPresentHandoff()
{
   LOG_ENTRY("BridgePostPresentHandoff");
   m_down_stream.compi->PostPresentHandoff();
   LOG_EXIT("BridgePostPresentHandoff");
}

bool openvr_bridge::GetFrameTiming(struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo)
{
   LOG_ENTRY("BridgeGetFrameTiming");
   bool rc = m_down_stream.compi->GetFrameTiming(pTiming,unFramesAgo);
   LOG_EXIT_RC(rc, "BridgeGetFrameTiming");
}

uint32_t openvr_bridge::GetFrameTimings(struct vr::Compositor_FrameTiming * pTiming, uint32_t nFrames)
{
   LOG_ENTRY("BridgeGetFrameTimings");
   uint32_t rc = m_down_stream.compi->GetFrameTimings(pTiming,nFrames);
   LOG_EXIT_RC(rc, "BridgeGetFrameTimings");
}

float openvr_bridge::GetFrameTimeRemaining()
{
   LOG_ENTRY("BridgeGetFrameTimeRemaining");
   float rc = m_down_stream.compi->GetFrameTimeRemaining();
   LOG_EXIT_RC(rc, "BridgeGetFrameTimeRemaining");
}

void openvr_bridge::GetCumulativeStats(struct vr::Compositor_CumulativeStats * pStats, uint32_t nStatsSizeInBytes)
{
   LOG_ENTRY("BridgeGetCumulativeStats");
   m_down_stream.compi->GetCumulativeStats(pStats,nStatsSizeInBytes);
   LOG_EXIT("BridgeGetCumulativeStats");
}

void openvr_bridge::FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground)
{
   LOG_ENTRY("BridgeFadeToColor");
   m_down_stream.compi->FadeToColor(fSeconds,fRed,fGreen,fBlue,fAlpha,bBackground);
   LOG_EXIT("BridgeFadeToColor");
}

struct vr::HmdColor_t openvr_bridge::GetCurrentFadeColor(bool bBackground)
{
   LOG_ENTRY("BridgeGetCurrentFadeColor");
   struct vr::HmdColor_t rc = m_down_stream.compi->GetCurrentFadeColor(bBackground);
   LOG_EXIT_RC(rc, "BridgeGetCurrentFadeColor");
}

void openvr_bridge::FadeGrid(float fSeconds, bool bFadeIn)
{
   LOG_ENTRY("BridgeFadeGrid");
   m_down_stream.compi->FadeGrid(fSeconds,bFadeIn);
   LOG_EXIT("BridgeFadeGrid");
}

float openvr_bridge::GetCurrentGridAlpha()
{
   LOG_ENTRY("BridgeGetCurrentGridAlpha");
   float rc = m_down_stream.compi->GetCurrentGridAlpha();
   LOG_EXIT_RC(rc, "BridgeGetCurrentGridAlpha");
}

vr::EVRCompositorError openvr_bridge::SetSkyboxOverride(const struct vr::Texture_t * pTextures, uint32_t unTextureCount)
{
   LOG_ENTRY("BridgeSetSkyboxOverride");
   vr::EVRCompositorError rc = m_down_stream.compi->SetSkyboxOverride(pTextures,unTextureCount);
   LOG_EXIT_RC(rc, "BridgeSetSkyboxOverride");
}

void openvr_bridge::ClearSkyboxOverride()
{
   LOG_ENTRY("BridgeClearSkyboxOverride");
   m_down_stream.compi->ClearSkyboxOverride();
   LOG_EXIT("BridgeClearSkyboxOverride");
}

void openvr_bridge::CompositorBringToFront()
{
   LOG_ENTRY("BridgeCompositorBringToFront");
   m_down_stream.compi->CompositorBringToFront();
   LOG_EXIT("BridgeCompositorBringToFront");
}

void openvr_bridge::CompositorGoToBack()
{
   LOG_ENTRY("BridgeCompositorGoToBack");
   m_down_stream.compi->CompositorGoToBack();
   LOG_EXIT("BridgeCompositorGoToBack");
}

void openvr_bridge::CompositorQuit()
{
   LOG_ENTRY("BridgeCompositorQuit");
   m_down_stream.compi->CompositorQuit();
   LOG_EXIT("BridgeCompositorQuit");
}

bool openvr_bridge::IsFullscreen()
{
   LOG_ENTRY("BridgeIsFullscreen");
   bool rc = m_down_stream.compi->IsFullscreen();
   LOG_EXIT_RC(rc, "BridgeIsFullscreen");
}

uint32_t openvr_bridge::GetCurrentSceneFocusProcess()
{
   LOG_ENTRY("BridgeGetCurrentSceneFocusProcess");
   uint32_t rc = m_down_stream.compi->GetCurrentSceneFocusProcess();
   LOG_EXIT_RC(rc, "BridgeGetCurrentSceneFocusProcess");
}

uint32_t openvr_bridge::GetLastFrameRenderer()
{
   LOG_ENTRY("BridgeGetLastFrameRenderer");
   uint32_t rc = m_down_stream.compi->GetLastFrameRenderer();
   LOG_EXIT_RC(rc, "BridgeGetLastFrameRenderer");
}

bool openvr_bridge::CanRenderScene()
{
   LOG_ENTRY("BridgeCanRenderScene");
   bool rc = m_down_stream.compi->CanRenderScene();
   LOG_EXIT_RC(rc, "BridgeCanRenderScene");
}

void openvr_bridge::ShowMirrorWindow()
{
   LOG_ENTRY("BridgeShowMirrorWindow");
   m_down_stream.compi->ShowMirrorWindow();
   LOG_EXIT("BridgeShowMirrorWindow");
}

void openvr_bridge::HideMirrorWindow()
{
   LOG_ENTRY("BridgeHideMirrorWindow");
   m_down_stream.compi->HideMirrorWindow();
   LOG_EXIT("BridgeHideMirrorWindow");
}

bool openvr_bridge::IsMirrorWindowVisible()
{
   LOG_ENTRY("BridgeIsMirrorWindowVisible");
   bool rc = m_down_stream.compi->IsMirrorWindowVisible();
   LOG_EXIT_RC(rc, "BridgeIsMirrorWindowVisible");
}

void openvr_bridge::CompositorDumpImages()
{
   LOG_ENTRY("BridgeCompositorDumpImages");
   m_down_stream.compi->CompositorDumpImages();
   LOG_EXIT("BridgeCompositorDumpImages");
}

bool openvr_bridge::ShouldAppRenderWithLowResources()
{
   LOG_ENTRY("BridgeShouldAppRenderWithLowResources");
   bool rc = m_down_stream.compi->ShouldAppRenderWithLowResources();
   LOG_EXIT_RC(rc, "BridgeShouldAppRenderWithLowResources");
}

void openvr_bridge::ForceInterleavedReprojectionOn(bool bOverride)
{
   LOG_ENTRY("BridgeForceInterleavedReprojectionOn");
   m_down_stream.compi->ForceInterleavedReprojectionOn(bOverride);
   LOG_EXIT("BridgeForceInterleavedReprojectionOn");
}

void openvr_bridge::ForceReconnectProcess()
{
   LOG_ENTRY("BridgeForceReconnectProcess");
   m_down_stream.compi->ForceReconnectProcess();
   LOG_EXIT("BridgeForceReconnectProcess");
}

void openvr_bridge::SuspendRendering(bool bSuspend)
{
   LOG_ENTRY("BridgeSuspendRendering");
   m_down_stream.compi->SuspendRendering(bSuspend);
   LOG_EXIT("BridgeSuspendRendering");
}

vr::EVRCompositorError openvr_bridge::GetMirrorTextureD3D11(vr::EVREye eEye, void * pD3D11DeviceOrResource, void ** ppD3D11ShaderResourceView)
{
   LOG_ENTRY("BridgeGetMirrorTextureD3D11");
   vr::EVRCompositorError rc = m_down_stream.compi->GetMirrorTextureD3D11(eEye,pD3D11DeviceOrResource,ppD3D11ShaderResourceView);
   LOG_EXIT_RC(rc, "BridgeGetMirrorTextureD3D11");
}

void openvr_bridge::ReleaseMirrorTextureD3D11(void * pD3D11ShaderResourceView)
{
   LOG_ENTRY("BridgeReleaseMirrorTextureD3D11");
   m_down_stream.compi->ReleaseMirrorTextureD3D11(pD3D11ShaderResourceView);
   LOG_EXIT("BridgeReleaseMirrorTextureD3D11");
}

vr::EVRCompositorError openvr_bridge::GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t * pglTextureId, vr::glSharedTextureHandle_t * pglSharedTextureHandle)
{
   LOG_ENTRY("BridgeGetMirrorTextureGL");
   vr::EVRCompositorError rc = m_down_stream.compi->GetMirrorTextureGL(eEye,pglTextureId,pglSharedTextureHandle);
   LOG_EXIT_RC(rc, "BridgeGetMirrorTextureGL");
}

bool openvr_bridge::ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle)
{
   LOG_ENTRY("BridgeReleaseSharedGLTexture");
   bool rc = m_down_stream.compi->ReleaseSharedGLTexture(glTextureId,glSharedTextureHandle);
   LOG_EXIT_RC(rc, "BridgeReleaseSharedGLTexture");
}

void openvr_bridge::LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle)
{
   LOG_ENTRY("BridgeLockGLSharedTextureForAccess");
   m_down_stream.compi->LockGLSharedTextureForAccess(glSharedTextureHandle);
   LOG_EXIT("BridgeLockGLSharedTextureForAccess");
}

void openvr_bridge::UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle)
{
   LOG_ENTRY("BridgeUnlockGLSharedTextureForAccess");
   m_down_stream.compi->UnlockGLSharedTextureForAccess(glSharedTextureHandle);
   LOG_EXIT("BridgeUnlockGLSharedTextureForAccess");
}

uint32_t openvr_bridge::GetVulkanInstanceExtensionsRequired(char * pchValue, uint32_t unBufferSize)
{
   LOG_ENTRY("BridgeGetVulkanInstanceExtensionsRequired");
   uint32_t rc = m_down_stream.compi->GetVulkanInstanceExtensionsRequired(pchValue,unBufferSize);
   //uint32_t rc = 1;
   //if (unBufferSize)
	//   *pchValue = 0;
   LOG_EXIT_RC(rc, "BridgeGetVulkanInstanceExtensionsRequired");
}

uint32_t openvr_bridge::GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T * pPhysicalDevice, char * pchValue, uint32_t unBufferSize)
{
   LOG_ENTRY("BridgeGetVulkanDeviceExtensionsRequired");
   uint32_t rc = m_down_stream.compi->GetVulkanDeviceExtensionsRequired(pPhysicalDevice,pchValue,unBufferSize);
   //uint32_t rc = 1;
   //if (unBufferSize)
//	   *pchValue = 0;
   LOG_EXIT_RC(rc, "BridgeGetVulkanDeviceExtensionsRequired");
}

vr::EVROverlayError openvr_bridge::FindOverlay(const char * pchOverlayKey, vr::VROverlayHandle_t * pOverlayHandle)
{
   LOG_ENTRY("BridgeFindOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->FindOverlay(pchOverlayKey,pOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeFindOverlay");
}

vr::EVROverlayError openvr_bridge::CreateOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pOverlayHandle)
{
   LOG_ENTRY("BridgeCreateOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->CreateOverlay(pchOverlayKey,pchOverlayFriendlyName,pOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeCreateOverlay");
}

vr::EVROverlayError openvr_bridge::DestroyOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeDestroyOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->DestroyOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeDestroyOverlay");
}

vr::EVROverlayError openvr_bridge::SetHighQualityOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeSetHighQualityOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->SetHighQualityOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeSetHighQualityOverlay");
}

vr::VROverlayHandle_t openvr_bridge::GetHighQualityOverlay()
{
   LOG_ENTRY("BridgeGetHighQualityOverlay");
   vr::VROverlayHandle_t rc = m_down_stream.ovi->GetHighQualityOverlay();
   LOG_EXIT_RC(rc, "BridgeGetHighQualityOverlay");
}

uint32_t openvr_bridge::GetOverlayKey(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
   LOG_ENTRY("BridgeGetOverlayKey");
   uint32_t rc = m_down_stream.ovi->GetOverlayKey(ulOverlayHandle,pchValue,unBufferSize,pError);
   LOG_EXIT_RC(rc, "BridgeGetOverlayKey");
}

uint32_t openvr_bridge::GetOverlayName(vr::VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, vr::EVROverlayError * pError)
{
   LOG_ENTRY("BridgeGetOverlayName");
   uint32_t rc = m_down_stream.ovi->GetOverlayName(ulOverlayHandle,pchValue,unBufferSize,pError);
   LOG_EXIT_RC(rc, "BridgeGetOverlayName");
}

vr::EVROverlayError openvr_bridge::GetOverlayImageData(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight)
{
   LOG_ENTRY("BridgeGetOverlayImageData");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayImageData(ulOverlayHandle,pvBuffer,unBufferSize,punWidth,punHeight);
   LOG_EXIT_RC(rc, "BridgeGetOverlayImageData");
}

const char * openvr_bridge::GetOverlayErrorNameFromEnum(vr::EVROverlayError error)
{
   LOG_ENTRY("BridgeGetOverlayErrorNameFromEnum");
   const char * rc = m_down_stream.ovi->GetOverlayErrorNameFromEnum(error);
   LOG_EXIT_RC(rc, "BridgeGetOverlayErrorNameFromEnum");
}

vr::EVROverlayError openvr_bridge::SetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unPID)
{
   LOG_ENTRY("BridgeSetOverlayRenderingPid");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayRenderingPid(ulOverlayHandle,unPID);
   LOG_EXIT_RC(rc, "BridgeSetOverlayRenderingPid");
}

uint32_t openvr_bridge::GetOverlayRenderingPid(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeGetOverlayRenderingPid");
   uint32_t rc = m_down_stream.ovi->GetOverlayRenderingPid(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeGetOverlayRenderingPid");
}

vr::EVROverlayError openvr_bridge::SetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool bEnabled)
{
   LOG_ENTRY("BridgeSetOverlayFlag");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayFlag(ulOverlayHandle,eOverlayFlag,bEnabled);
   LOG_EXIT_RC(rc, "BridgeSetOverlayFlag");
}

vr::EVROverlayError openvr_bridge::GetOverlayFlag(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayFlags eOverlayFlag, bool * pbEnabled)
{
   LOG_ENTRY("BridgeGetOverlayFlag");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayFlag(ulOverlayHandle,eOverlayFlag,pbEnabled);
   LOG_EXIT_RC(rc, "BridgeGetOverlayFlag");
}

vr::EVROverlayError openvr_bridge::SetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue)
{
   LOG_ENTRY("BridgeSetOverlayColor");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayColor(ulOverlayHandle,fRed,fGreen,fBlue);
   LOG_EXIT_RC(rc, "BridgeSetOverlayColor");
}

vr::EVROverlayError openvr_bridge::GetOverlayColor(vr::VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue)
{
   LOG_ENTRY("BridgeGetOverlayColor");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayColor(ulOverlayHandle,pfRed,pfGreen,pfBlue);
   LOG_EXIT_RC(rc, "BridgeGetOverlayColor");
}

vr::EVROverlayError openvr_bridge::SetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float fAlpha)
{
   LOG_ENTRY("BridgeSetOverlayAlpha");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayAlpha(ulOverlayHandle,fAlpha);
   LOG_EXIT_RC(rc, "BridgeSetOverlayAlpha");
}

vr::EVROverlayError openvr_bridge::GetOverlayAlpha(vr::VROverlayHandle_t ulOverlayHandle, float * pfAlpha)
{
   LOG_ENTRY("BridgeGetOverlayAlpha");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayAlpha(ulOverlayHandle,pfAlpha);
   LOG_EXIT_RC(rc, "BridgeGetOverlayAlpha");
}

vr::EVROverlayError openvr_bridge::SetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float fTexelAspect)
{
   LOG_ENTRY("BridgeSetOverlayTexelAspect");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTexelAspect(ulOverlayHandle,fTexelAspect);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTexelAspect");
}

vr::EVROverlayError openvr_bridge::GetOverlayTexelAspect(vr::VROverlayHandle_t ulOverlayHandle, float * pfTexelAspect)
{
   LOG_ENTRY("BridgeGetOverlayTexelAspect");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTexelAspect(ulOverlayHandle,pfTexelAspect);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTexelAspect");
}

vr::EVROverlayError openvr_bridge::SetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder)
{
   LOG_ENTRY("BridgeSetOverlaySortOrder");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlaySortOrder(ulOverlayHandle,unSortOrder);
   LOG_EXIT_RC(rc, "BridgeSetOverlaySortOrder");
}

vr::EVROverlayError openvr_bridge::GetOverlaySortOrder(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punSortOrder)
{
   LOG_ENTRY("BridgeGetOverlaySortOrder");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlaySortOrder(ulOverlayHandle,punSortOrder);
   LOG_EXIT_RC(rc, "BridgeGetOverlaySortOrder");
}

vr::EVROverlayError openvr_bridge::SetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fWidthInMeters)
{
   LOG_ENTRY("BridgeSetOverlayWidthInMeters");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayWidthInMeters(ulOverlayHandle,fWidthInMeters);
   LOG_EXIT_RC(rc, "BridgeSetOverlayWidthInMeters");
}

vr::EVROverlayError openvr_bridge::GetOverlayWidthInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters)
{
   LOG_ENTRY("BridgeGetOverlayWidthInMeters");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayWidthInMeters(ulOverlayHandle,pfWidthInMeters);
   LOG_EXIT_RC(rc, "BridgeGetOverlayWidthInMeters");
}

vr::EVROverlayError openvr_bridge::SetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters)
{
   LOG_ENTRY("BridgeSetOverlayAutoCurveDistanceRangeInMeters");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayAutoCurveDistanceRangeInMeters(ulOverlayHandle,fMinDistanceInMeters,fMaxDistanceInMeters);
   LOG_EXIT_RC(rc, "BridgeSetOverlayAutoCurveDistanceRangeInMeters");
}

vr::EVROverlayError openvr_bridge::GetOverlayAutoCurveDistanceRangeInMeters(vr::VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters)
{
   LOG_ENTRY("BridgeGetOverlayAutoCurveDistanceRangeInMeters");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayAutoCurveDistanceRangeInMeters(ulOverlayHandle,pfMinDistanceInMeters,pfMaxDistanceInMeters);
   LOG_EXIT_RC(rc, "BridgeGetOverlayAutoCurveDistanceRangeInMeters");
}

vr::EVROverlayError openvr_bridge::SetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace eTextureColorSpace)
{
   LOG_ENTRY("BridgeSetOverlayTextureColorSpace");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTextureColorSpace(ulOverlayHandle,eTextureColorSpace);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTextureColorSpace");
}

vr::EVROverlayError openvr_bridge::GetOverlayTextureColorSpace(vr::VROverlayHandle_t ulOverlayHandle, vr::EColorSpace * peTextureColorSpace)
{
   LOG_ENTRY("BridgeGetOverlayTextureColorSpace");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTextureColorSpace(ulOverlayHandle,peTextureColorSpace);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTextureColorSpace");
}

vr::EVROverlayError openvr_bridge::SetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VRTextureBounds_t * pOverlayTextureBounds)
{
   LOG_ENTRY("BridgeSetOverlayTextureBounds");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTextureBounds(ulOverlayHandle,pOverlayTextureBounds);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTextureBounds");
}

vr::EVROverlayError openvr_bridge::GetOverlayTextureBounds(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VRTextureBounds_t * pOverlayTextureBounds)
{
   LOG_ENTRY("BridgeGetOverlayTextureBounds");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTextureBounds(ulOverlayHandle,pOverlayTextureBounds);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTextureBounds");
}

vr::EVROverlayError openvr_bridge::GetOverlayTransformType(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayTransformType * peTransformType)
{
   LOG_ENTRY("BridgeGetOverlayTransformType");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTransformType(ulOverlayHandle,peTransformType);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTransformType");
}

vr::EVROverlayError openvr_bridge::SetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform)
{
   LOG_ENTRY("BridgeSetOverlayTransformAbsolute");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTransformAbsolute(ulOverlayHandle,eTrackingOrigin,pmatTrackingOriginToOverlayTransform);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTransformAbsolute");
}

vr::EVROverlayError openvr_bridge::GetOverlayTransformAbsolute(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin * peTrackingOrigin, struct vr::HmdMatrix34_t * pmatTrackingOriginToOverlayTransform)
{
   LOG_ENTRY("BridgeGetOverlayTransformAbsolute");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTransformAbsolute(ulOverlayHandle,peTrackingOrigin,pmatTrackingOriginToOverlayTransform);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTransformAbsolute");
}

vr::EVROverlayError openvr_bridge::SetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unTrackedDevice, const struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform)
{
   LOG_ENTRY("BridgeSetOverlayTransformTrackedDeviceRelative");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTransformTrackedDeviceRelative(ulOverlayHandle,unTrackedDevice,pmatTrackedDeviceToOverlayTransform);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTransformTrackedDeviceRelative");
}

vr::EVROverlayError openvr_bridge::GetOverlayTransformTrackedDeviceRelative(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punTrackedDevice, struct vr::HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform)
{
   LOG_ENTRY("BridgeGetOverlayTransformTrackedDeviceRelative");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTransformTrackedDeviceRelative(ulOverlayHandle,punTrackedDevice,pmatTrackedDeviceToOverlayTransform);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTransformTrackedDeviceRelative");
}

vr::EVROverlayError openvr_bridge::SetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unDeviceIndex, const char * pchComponentName)
{
   LOG_ENTRY("BridgeSetOverlayTransformTrackedDeviceComponent");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTransformTrackedDeviceComponent(ulOverlayHandle,unDeviceIndex,pchComponentName);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTransformTrackedDeviceComponent");
}

vr::EVROverlayError openvr_bridge::GetOverlayTransformTrackedDeviceComponent(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t * punDeviceIndex, char * pchComponentName, uint32_t unComponentNameSize)
{
   LOG_ENTRY("BridgeGetOverlayTransformTrackedDeviceComponent");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTransformTrackedDeviceComponent(ulOverlayHandle,punDeviceIndex,pchComponentName,unComponentNameSize);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTransformTrackedDeviceComponent");
}

vr::EVROverlayError openvr_bridge::ShowOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeShowOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->ShowOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeShowOverlay");
}

vr::EVROverlayError openvr_bridge::HideOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeHideOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->HideOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeHideOverlay");
}

bool openvr_bridge::IsOverlayVisible(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeIsOverlayVisible");
   bool rc = m_down_stream.ovi->IsOverlayVisible(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeIsOverlayVisible");
}

vr::EVROverlayError openvr_bridge::GetTransformForOverlayCoordinates(vr::VROverlayHandle_t ulOverlayHandle, vr::ETrackingUniverseOrigin eTrackingOrigin, struct vr::HmdVector2_t coordinatesInOverlay, struct vr::HmdMatrix34_t * pmatTransform)
{
   LOG_ENTRY("BridgeGetTransformForOverlayCoordinates");
   vr::EVROverlayError rc = m_down_stream.ovi->GetTransformForOverlayCoordinates(ulOverlayHandle,eTrackingOrigin,coordinatesInOverlay,pmatTransform);
   LOG_EXIT_RC(rc, "BridgeGetTransformForOverlayCoordinates");
}

bool openvr_bridge::PollNextOverlayEvent(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VREvent_t * pEvent, uint32_t uncbVREvent)
{
   LOG_ENTRY("BridgePollNextOverlayEvent");
   bool rc = m_down_stream.ovi->PollNextOverlayEvent(ulOverlayHandle,pEvent,uncbVREvent);
   LOG_EXIT_RC(rc, "BridgePollNextOverlayEvent");
}

vr::EVROverlayError openvr_bridge::GetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod * peInputMethod)
{
   LOG_ENTRY("BridgeGetOverlayInputMethod");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayInputMethod(ulOverlayHandle,peInputMethod);
   LOG_EXIT_RC(rc, "BridgeGetOverlayInputMethod");
}

vr::EVROverlayError openvr_bridge::SetOverlayInputMethod(vr::VROverlayHandle_t ulOverlayHandle, vr::VROverlayInputMethod eInputMethod)
{
   LOG_ENTRY("BridgeSetOverlayInputMethod");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayInputMethod(ulOverlayHandle,eInputMethod);
   LOG_EXIT_RC(rc, "BridgeSetOverlayInputMethod");
}

vr::EVROverlayError openvr_bridge::GetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdVector2_t * pvecMouseScale)
{
   LOG_ENTRY("BridgeGetOverlayMouseScale");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayMouseScale(ulOverlayHandle,pvecMouseScale);
   LOG_EXIT_RC(rc, "BridgeGetOverlayMouseScale");
}

vr::EVROverlayError openvr_bridge::SetOverlayMouseScale(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::HmdVector2_t * pvecMouseScale)
{
   LOG_ENTRY("BridgeSetOverlayMouseScale");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayMouseScale(ulOverlayHandle,pvecMouseScale);
   LOG_EXIT_RC(rc, "BridgeSetOverlayMouseScale");
}

bool openvr_bridge::ComputeOverlayIntersection(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::VROverlayIntersectionParams_t * pParams, struct vr::VROverlayIntersectionResults_t * pResults)
{
   LOG_ENTRY("BridgeComputeOverlayIntersection");
   bool rc = m_down_stream.ovi->ComputeOverlayIntersection(ulOverlayHandle,pParams,pResults);
   LOG_EXIT_RC(rc, "BridgeComputeOverlayIntersection");
}

bool openvr_bridge::HandleControllerOverlayInteractionAsMouse(vr::VROverlayHandle_t ulOverlayHandle, vr::TrackedDeviceIndex_t unControllerDeviceIndex)
{
   LOG_ENTRY("BridgeHandleControllerOverlayInteractionAsMouse");
   bool rc = m_down_stream.ovi->HandleControllerOverlayInteractionAsMouse(ulOverlayHandle,unControllerDeviceIndex);
   LOG_EXIT_RC(rc, "BridgeHandleControllerOverlayInteractionAsMouse");
}

bool openvr_bridge::IsHoverTargetOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeIsHoverTargetOverlay");
   bool rc = m_down_stream.ovi->IsHoverTargetOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeIsHoverTargetOverlay");
}

vr::VROverlayHandle_t openvr_bridge::GetGamepadFocusOverlay()
{
   LOG_ENTRY("BridgeGetGamepadFocusOverlay");
   vr::VROverlayHandle_t rc = m_down_stream.ovi->GetGamepadFocusOverlay();
   LOG_EXIT_RC(rc, "BridgeGetGamepadFocusOverlay");
}

vr::EVROverlayError openvr_bridge::SetGamepadFocusOverlay(vr::VROverlayHandle_t ulNewFocusOverlay)
{
   LOG_ENTRY("BridgeSetGamepadFocusOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->SetGamepadFocusOverlay(ulNewFocusOverlay);
   LOG_EXIT_RC(rc, "BridgeSetGamepadFocusOverlay");
}

vr::EVROverlayError openvr_bridge::SetOverlayNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom, vr::VROverlayHandle_t ulTo)
{
   LOG_ENTRY("BridgeSetOverlayNeighbor");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayNeighbor(eDirection,ulFrom,ulTo);
   LOG_EXIT_RC(rc, "BridgeSetOverlayNeighbor");
}

vr::EVROverlayError openvr_bridge::MoveGamepadFocusToNeighbor(vr::EOverlayDirection eDirection, vr::VROverlayHandle_t ulFrom)
{
   LOG_ENTRY("BridgeMoveGamepadFocusToNeighbor");
   vr::EVROverlayError rc = m_down_stream.ovi->MoveGamepadFocusToNeighbor(eDirection,ulFrom);
   LOG_EXIT_RC(rc, "BridgeMoveGamepadFocusToNeighbor");
}

vr::EVROverlayError openvr_bridge::SetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, const struct vr::Texture_t * pTexture)
{
   LOG_ENTRY("BridgeSetOverlayTexture");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayTexture(ulOverlayHandle,pTexture);
   LOG_EXIT_RC(rc, "BridgeSetOverlayTexture");
}

vr::EVROverlayError openvr_bridge::ClearOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeClearOverlayTexture");
   vr::EVROverlayError rc = m_down_stream.ovi->ClearOverlayTexture(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeClearOverlayTexture");
}

vr::EVROverlayError openvr_bridge::SetOverlayRaw(vr::VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth)
{
   LOG_ENTRY("BridgeSetOverlayRaw");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayRaw(ulOverlayHandle,pvBuffer,unWidth,unHeight,unDepth);
   LOG_EXIT_RC(rc, "BridgeSetOverlayRaw");
}

vr::EVROverlayError openvr_bridge::SetOverlayFromFile(vr::VROverlayHandle_t ulOverlayHandle, const char * pchFilePath)
{
   LOG_ENTRY("BridgeSetOverlayFromFile");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayFromFile(ulOverlayHandle,pchFilePath);
   LOG_EXIT_RC(rc, "BridgeSetOverlayFromFile");
}

vr::EVROverlayError openvr_bridge::GetOverlayTexture(vr::VROverlayHandle_t ulOverlayHandle, void ** pNativeTextureHandle, void * pNativeTextureRef, uint32_t * pWidth, uint32_t * pHeight, uint32_t * pNativeFormat, vr::ETextureType * pAPIType, vr::EColorSpace * pColorSpace, struct vr::VRTextureBounds_t * pTextureBounds)
{
   LOG_ENTRY("BridgeGetOverlayTexture");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTexture(ulOverlayHandle,pNativeTextureHandle,pNativeTextureRef,pWidth,pHeight,pNativeFormat,pAPIType,pColorSpace,pTextureBounds);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTexture");
}

vr::EVROverlayError openvr_bridge::ReleaseNativeOverlayHandle(vr::VROverlayHandle_t ulOverlayHandle, void * pNativeTextureHandle)
{
   LOG_ENTRY("BridgeReleaseNativeOverlayHandle");
   vr::EVROverlayError rc = m_down_stream.ovi->ReleaseNativeOverlayHandle(ulOverlayHandle,pNativeTextureHandle);
   LOG_EXIT_RC(rc, "BridgeReleaseNativeOverlayHandle");
}

vr::EVROverlayError openvr_bridge::GetOverlayTextureSize(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pWidth, uint32_t * pHeight)
{
   LOG_ENTRY("BridgeGetOverlayTextureSize");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayTextureSize(ulOverlayHandle,pWidth,pHeight);
   LOG_EXIT_RC(rc, "BridgeGetOverlayTextureSize");
}

vr::EVROverlayError openvr_bridge::CreateDashboardOverlay(const char * pchOverlayKey, const char * pchOverlayFriendlyName, vr::VROverlayHandle_t * pMainHandle, vr::VROverlayHandle_t * pThumbnailHandle)
{
   LOG_ENTRY("BridgeCreateDashboardOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->CreateDashboardOverlay(pchOverlayKey,pchOverlayFriendlyName,pMainHandle,pThumbnailHandle);
   LOG_EXIT_RC(rc, "BridgeCreateDashboardOverlay");
}

bool openvr_bridge::IsDashboardVisible()
{
   LOG_ENTRY("BridgeIsDashboardVisible");
   bool rc = m_down_stream.ovi->IsDashboardVisible();
   LOG_EXIT_RC(rc, "BridgeIsDashboardVisible");
}

bool openvr_bridge::IsActiveDashboardOverlay(vr::VROverlayHandle_t ulOverlayHandle)
{
   LOG_ENTRY("BridgeIsActiveDashboardOverlay");
   bool rc = m_down_stream.ovi->IsActiveDashboardOverlay(ulOverlayHandle);
   LOG_EXIT_RC(rc, "BridgeIsActiveDashboardOverlay");
}

vr::EVROverlayError openvr_bridge::SetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId)
{
   LOG_ENTRY("BridgeSetDashboardOverlaySceneProcess");
   vr::EVROverlayError rc = m_down_stream.ovi->SetDashboardOverlaySceneProcess(ulOverlayHandle,unProcessId);
   LOG_EXIT_RC(rc, "BridgeSetDashboardOverlaySceneProcess");
}

vr::EVROverlayError openvr_bridge::GetDashboardOverlaySceneProcess(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId)
{
   LOG_ENTRY("BridgeGetDashboardOverlaySceneProcess");
   vr::EVROverlayError rc = m_down_stream.ovi->GetDashboardOverlaySceneProcess(ulOverlayHandle,punProcessId);
   LOG_EXIT_RC(rc, "BridgeGetDashboardOverlaySceneProcess");
}

void openvr_bridge::ShowDashboard(const char * pchOverlayToShow)
{
   LOG_ENTRY("BridgeShowDashboard");
   m_down_stream.ovi->ShowDashboard(pchOverlayToShow);
   LOG_EXIT("BridgeShowDashboard");
}

vr::TrackedDeviceIndex_t openvr_bridge::GetPrimaryDashboardDevice()
{
   LOG_ENTRY("BridgeGetPrimaryDashboardDevice");
   vr::TrackedDeviceIndex_t rc = m_down_stream.ovi->GetPrimaryDashboardDevice();
   LOG_EXIT_RC(rc, "BridgeGetPrimaryDashboardDevice");
}

vr::EVROverlayError openvr_bridge::ShowKeyboard(vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue)
{
   LOG_ENTRY("BridgeShowKeyboard");
   vr::EVROverlayError rc = m_down_stream.ovi->ShowKeyboard(eInputMode,eLineInputMode,pchDescription,unCharMax,pchExistingText,bUseMinimalMode,uUserValue);
   LOG_EXIT_RC(rc, "BridgeShowKeyboard");
}

vr::EVROverlayError openvr_bridge::ShowKeyboardForOverlay(vr::VROverlayHandle_t ulOverlayHandle, vr::EGamepadTextInputMode eInputMode, vr::EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue)
{
   LOG_ENTRY("BridgeShowKeyboardForOverlay");
   vr::EVROverlayError rc = m_down_stream.ovi->ShowKeyboardForOverlay(ulOverlayHandle,eInputMode,eLineInputMode,pchDescription,unCharMax,pchExistingText,bUseMinimalMode,uUserValue);
   LOG_EXIT_RC(rc, "BridgeShowKeyboardForOverlay");
}

uint32_t openvr_bridge::GetKeyboardText(char * pchText, uint32_t cchText)
{
   LOG_ENTRY("BridgeGetKeyboardText");
   uint32_t rc = m_down_stream.ovi->GetKeyboardText(pchText,cchText);
   LOG_EXIT_RC(rc, "BridgeGetKeyboardText");
}

void openvr_bridge::HideKeyboard()
{
   LOG_ENTRY("BridgeHideKeyboard");
   m_down_stream.ovi->HideKeyboard();
   LOG_EXIT("BridgeHideKeyboard");
}

void openvr_bridge::SetKeyboardTransformAbsolute(vr::ETrackingUniverseOrigin eTrackingOrigin, const struct vr::HmdMatrix34_t * pmatTrackingOriginToKeyboardTransform)
{
   LOG_ENTRY("BridgeSetKeyboardTransformAbsolute");
   m_down_stream.ovi->SetKeyboardTransformAbsolute(eTrackingOrigin,pmatTrackingOriginToKeyboardTransform);
   LOG_EXIT("BridgeSetKeyboardTransformAbsolute");
}

void openvr_bridge::SetKeyboardPositionForOverlay(vr::VROverlayHandle_t ulOverlayHandle, struct vr::HmdRect2_t avoidRect)
{
   LOG_ENTRY("BridgeSetKeyboardPositionForOverlay");
   m_down_stream.ovi->SetKeyboardPositionForOverlay(ulOverlayHandle,avoidRect);
   LOG_EXIT("BridgeSetKeyboardPositionForOverlay");
}

vr::EVROverlayError openvr_bridge::SetOverlayIntersectionMask(vr::VROverlayHandle_t ulOverlayHandle, struct vr::VROverlayIntersectionMaskPrimitive_t * pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize)
{
   LOG_ENTRY("BridgeSetOverlayIntersectionMask");
   vr::EVROverlayError rc = m_down_stream.ovi->SetOverlayIntersectionMask(ulOverlayHandle,pMaskPrimitives,unNumMaskPrimitives,unPrimitiveSize);
   LOG_EXIT_RC(rc, "BridgeSetOverlayIntersectionMask");
}

vr::EVROverlayError openvr_bridge::GetOverlayFlags(vr::VROverlayHandle_t ulOverlayHandle, uint32_t * pFlags)
{
   LOG_ENTRY("BridgeGetOverlayFlags");
   vr::EVROverlayError rc = m_down_stream.ovi->GetOverlayFlags(ulOverlayHandle,pFlags);
   LOG_EXIT_RC(rc, "BridgeGetOverlayFlags");
}

vr::VRMessageOverlayResponse openvr_bridge::ShowMessageOverlay(const char * pchText, const char * pchCaption, const char * pchButton0Text, const char * pchButton1Text, const char * pchButton2Text, const char * pchButton3Text)
{
   LOG_ENTRY("BridgeShowMessageOverlay");
   vr::VRMessageOverlayResponse rc = m_down_stream.ovi->ShowMessageOverlay(pchText,pchCaption,pchButton0Text,pchButton1Text,pchButton2Text,pchButton3Text);
   LOG_EXIT_RC(rc, "BridgeShowMessageOverlay");
}

vr::EVRRenderModelError openvr_bridge::LoadRenderModel_Async(const char * pchRenderModelName, struct vr::RenderModel_t ** ppRenderModel)
{
   LOG_ENTRY("BridgeLoadRenderModel_Async");
   vr::EVRRenderModelError rc = m_down_stream.remi->LoadRenderModel_Async(pchRenderModelName,ppRenderModel);
   LOG_EXIT_RC(rc, "BridgeLoadRenderModel_Async");
}

void openvr_bridge::FreeRenderModel(struct vr::RenderModel_t * pRenderModel)
{
   LOG_ENTRY("BridgeFreeRenderModel");
   m_down_stream.remi->FreeRenderModel(pRenderModel);
   LOG_EXIT("BridgeFreeRenderModel");
}

vr::EVRRenderModelError openvr_bridge::LoadTexture_Async(vr::TextureID_t textureId, struct vr::RenderModel_TextureMap_t ** ppTexture)
{
   LOG_ENTRY("BridgeLoadTexture_Async");
   vr::EVRRenderModelError rc = m_down_stream.remi->LoadTexture_Async(textureId,ppTexture);
   LOG_EXIT_RC(rc, "BridgeLoadTexture_Async");
}

void openvr_bridge::FreeTexture(struct vr::RenderModel_TextureMap_t * pTexture)
{
   LOG_ENTRY("BridgeFreeTexture");
   m_down_stream.remi->FreeTexture(pTexture);
   LOG_EXIT("BridgeFreeTexture");
}

vr::EVRRenderModelError openvr_bridge::LoadTextureD3D11_Async(vr::TextureID_t textureId, void * pD3D11Device, void ** ppD3D11Texture2D)
{
   LOG_ENTRY("BridgeLoadTextureD3D11_Async");
   vr::EVRRenderModelError rc = m_down_stream.remi->LoadTextureD3D11_Async(textureId,pD3D11Device,ppD3D11Texture2D);
   LOG_EXIT_RC(rc, "BridgeLoadTextureD3D11_Async");
}

vr::EVRRenderModelError openvr_bridge::LoadIntoTextureD3D11_Async(vr::TextureID_t textureId, void * pDstTexture)
{
   LOG_ENTRY("BridgeLoadIntoTextureD3D11_Async");
   vr::EVRRenderModelError rc = m_down_stream.remi->LoadIntoTextureD3D11_Async(textureId,pDstTexture);
   LOG_EXIT_RC(rc, "BridgeLoadIntoTextureD3D11_Async");
}

void openvr_bridge::FreeTextureD3D11(void * pD3D11Texture2D)
{
   LOG_ENTRY("BridgeFreeTextureD3D11");
   m_down_stream.remi->FreeTextureD3D11(pD3D11Texture2D);
   LOG_EXIT("BridgeFreeTextureD3D11");
}



uint32_t openvr_bridge::GetRenderModelName(uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen)
{
   LOG_ENTRY("BridgeGetRenderModelName");

   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   buf.dup(pchRenderModelName, unRenderModelNameLen);
   }

   uint32_t rc = m_down_stream.remi->GetRenderModelName(unRenderModelIndex, pchRenderModelName, unRenderModelNameLen);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t rc2 = m_lock_step_tracker.remi->GetRenderModelName(unRenderModelIndex, buf.data(), unRenderModelNameLen);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   TRAIN_TRACKER_ASSERT(buf.same());	
   }

   LOG_EXIT_RC(rc, "BridgeGetRenderModelName");
}

uint32_t openvr_bridge::GetRenderModelCount()
{
   LOG_ENTRY("BridgeGetRenderModelCount");

   uint32_t rc = m_down_stream.remi->GetRenderModelCount();

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t rc2 = m_lock_step_tracker.remi->GetRenderModelCount();
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetRenderModelCount");
}

uint32_t openvr_bridge::GetComponentCount(const char * pchRenderModelName)
{
   LOG_ENTRY("BridgeGetComponentCount");
   uint32_t rc = m_down_stream.remi->GetComponentCount(pchRenderModelName);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t rc2 = m_lock_step_tracker.remi->GetComponentCount(pchRenderModelName);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
   }

   LOG_EXIT_RC(rc, "BridgeGetComponentCount");
}

uint32_t openvr_bridge::GetComponentName(
		const char * pchRenderModelName, 
		uint32_t unComponentIndex, 
		char * pchComponentName, uint32_t unComponentNameLen)
{
   LOG_ENTRY("BridgeGetComponentName");

   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   buf.dup(pchComponentName, unComponentNameLen);
   }

   uint32_t rc = m_down_stream.remi->GetComponentName(pchRenderModelName,unComponentIndex,pchComponentName,unComponentNameLen);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t rc2 = m_lock_step_tracker.remi->GetComponentName(pchRenderModelName, unComponentIndex, buf.data(), unComponentNameLen);
	   TRAIN_TRACKER_ASSERT(rc2 == rc);
	   TRAIN_TRACKER_ASSERT(buf.same());
   }

   LOG_EXIT_RC(rc, "BridgeGetComponentName");
}

uint64_t openvr_bridge::GetComponentButtonMask(const char * pchRenderModelName, const char * pchComponentName)
{
   LOG_ENTRY("BridgeGetComponentButtonMask");

   uint64_t rc = m_down_stream.remi->GetComponentButtonMask(pchRenderModelName,pchComponentName);
   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint64_t rc2 = m_lock_step_tracker.remi->GetComponentButtonMask(pchRenderModelName, pchComponentName);
	   TRAIN_TRACKER_ASSERT(rc2 == rc);
   }

   LOG_EXIT_RC(rc, "BridgeGetComponentButtonMask");
}

uint32_t openvr_bridge::GetComponentRenderModelName(const char * pchRenderModelName, const char * pchComponentName, 
	char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen)
{
   LOG_ENTRY("BridgeGetComponentRenderModelName");
  
   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   buf.dup(pchComponentRenderModelName, unComponentRenderModelNameLen);
   }

   uint32_t rc = m_down_stream.remi->GetComponentRenderModelName(
							pchRenderModelName,pchComponentName,pchComponentRenderModelName,unComponentRenderModelNameLen);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   uint32_t rc2 = m_lock_step_tracker.remi->GetComponentRenderModelName(
		   pchRenderModelName, pchComponentName, buf.data(), unComponentRenderModelNameLen);

	   TRAIN_TRACKER_ASSERT(rc2 == rc);
	   TRAIN_TRACKER_ASSERT(buf.same());
   }

   LOG_EXIT_RC(rc, "BridgeGetComponentRenderModelName");
}

bool openvr_bridge::GetComponentState(
	const char * pchRenderModelName, 
	const char * pchComponentName, 
	const vr::VRControllerState_t * pControllerState, 
	const struct vr::RenderModel_ControllerMode_State_t * pState, 
	struct vr::RenderModel_ComponentState_t * pComponentState)
{
   LOG_ENTRY("BridgeGetComponentState");

   bool rc = m_down_stream.remi->GetComponentState(pchRenderModelName,pchComponentName,pControllerState,pState,pComponentState);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::RenderModel_ComponentState_t component_state2;
	   bool rc2 = m_lock_step_tracker.remi->GetComponentState(
		   pchRenderModelName, pchComponentName, pControllerState, pState, &component_state2);

	   TRAIN_TRACKER_ASSERT(rc2 == rc);
	   if (pComponentState)
	   {
		   TRAIN_TRACKER_ASSERT(*pComponentState == component_state2);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetComponentState");
}

bool openvr_bridge::RenderModelHasComponent(const char * pchRenderModelName, const char * pchComponentName)
{
   LOG_ENTRY("BridgeRenderModelHasComponent");

   bool rc = m_down_stream.remi->RenderModelHasComponent(pchRenderModelName,pchComponentName);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   bool rc2 = m_lock_step_tracker.remi->RenderModelHasComponent(
													pchRenderModelName, pchComponentName);

	   TRAIN_TRACKER_ASSERT(rc2 == rc);
   }

   LOG_EXIT_RC(rc, "BridgeRenderModelHasComponent");
}

uint32_t openvr_bridge::GetRenderModelThumbnailURL(
		const char * pchRenderModelName, 
		char * pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError * peError)
{
   LOG_ENTRY("BridgeGetRenderModelThumbnailURL");

   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   buf.dup(pchThumbnailURL, unThumbnailURLLen);
   }

   uint32_t rc = m_down_stream.remi->GetRenderModelThumbnailURL(pchRenderModelName,pchThumbnailURL,unThumbnailURLLen,peError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EVRRenderModelError error2;
	   uint32_t rc2 = m_lock_step_tracker.remi->GetRenderModelThumbnailURL(
		   pchRenderModelName, buf.data(), unThumbnailURLLen, &error2);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   TRAIN_TRACKER_ASSERT(buf.same());
	   if (peError)
	   {
		   TRAIN_TRACKER_ASSERT(*peError == error2);
	   }
   }
   LOG_EXIT_RC(rc, "BridgeGetRenderModelThumbnailURL");
}

uint32_t openvr_bridge::GetRenderModelOriginalPath(
	const char * pchRenderModelName, 
	char * pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError * peError)
{
   LOG_ENTRY("BridgeGetRenderModelOriginalPath");

   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   buf.dup(pchOriginalPath, unOriginalPathLen);
   }

   uint32_t rc = m_down_stream.remi->GetRenderModelOriginalPath(pchRenderModelName,pchOriginalPath,unOriginalPathLen,peError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EVRRenderModelError error2;
	   uint32_t rc2 = m_lock_step_tracker.remi->GetRenderModelOriginalPath(
		   pchRenderModelName, buf.data(), unOriginalPathLen, &error2);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   TRAIN_TRACKER_ASSERT(buf.same());
   }

   LOG_EXIT_RC(rc, "BridgeGetRenderModelOriginalPath");
}

const char * openvr_bridge::GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error)
{
   LOG_ENTRY("BridgeGetRenderModelErrorNameFromEnum");
   const char * rc = m_down_stream.remi->GetRenderModelErrorNameFromEnum(error);
   LOG_EXIT_RC(rc, "BridgeGetRenderModelErrorNameFromEnum");
}

vr::EVRNotificationError openvr_bridge::CreateNotification(vr::VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, vr::EVRNotificationType type, const char * pchText, vr::EVRNotificationStyle style, const struct vr::NotificationBitmap_t * pImage, vr::VRNotificationId * pNotificationId)
{
   LOG_ENTRY("BridgeCreateNotification");
   vr::EVRNotificationError rc = m_down_stream.noti->CreateNotification(ulOverlayHandle,ulUserValue,type,pchText,style,pImage,pNotificationId);
   LOG_EXIT_RC(rc, "BridgeCreateNotification");
}

vr::EVRNotificationError openvr_bridge::RemoveNotification(vr::VRNotificationId notificationId)
{
   LOG_ENTRY("BridgeRemoveNotification");
   vr::EVRNotificationError rc = m_down_stream.noti->RemoveNotification(notificationId);
   LOG_EXIT_RC(rc, "BridgeRemoveNotification");
}

const char * openvr_bridge::GetSettingsErrorNameFromEnum(vr::EVRSettingsError eError)
{
   LOG_ENTRY("BridgeGetSettingsErrorNameFromEnum");
   const char * rc = m_down_stream.seti->GetSettingsErrorNameFromEnum(eError);
   LOG_EXIT_RC(rc, "BridgeGetSettingsErrorNameFromEnum");
}

bool openvr_bridge::Sync(bool bForce, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeSync");
   bool rc = m_down_stream.seti->Sync(bForce,peError);
   LOG_EXIT_RC(rc, "BridgeSync");
}

void openvr_bridge::SetBool(const char * pchSection, const char * pchSettingsKey, bool bValue, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeSetBool");

   m_down_stream.seti->SetBool(pchSection, pchSettingsKey, bValue, peError);

   // spy for any new settings. todo: add this to getbool and all the other sets and gets
   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_BOOL, pchSettingsKey);
   } 

   LOG_EXIT("BridgeSetBool");
}

void openvr_bridge::SetInt32(const char * pchSection, const char * pchSettingsKey, int32_t nValue, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeSetInt32");

   m_down_stream.seti->SetInt32(pchSection, pchSettingsKey, nValue, peError);

   // spy for any new settings. todo: add this to getbool and all the other sets and gets
   if (m_spy_mode)
   {
		update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_INT32, pchSettingsKey);
   }

   
   LOG_EXIT("BridgeSetInt32");
}

void openvr_bridge::SetFloat(const char * pchSection, const char * pchSettingsKey, float flValue, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeSetFloat");

   m_down_stream.seti->SetFloat(pchSection, pchSettingsKey, flValue, peError);

   if (m_spy_mode)
   {
	   if (m_spy_mode)
	   {
		   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_FLOAT, pchSettingsKey);
	   }
   }
   LOG_EXIT("BridgeSetFloat");
}

void openvr_bridge::SetString(const char * pchSection, const char * pchSettingsKey, const char * pchValue, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeSetString");

   m_down_stream.seti->SetString(pchSection,pchSettingsKey,pchValue,peError);

   // make sure if it's being set that I'm capturing it
   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_STRING, pchSettingsKey);
   }

   LOG_EXIT("BridgeSetString");
}

bool openvr_bridge::GetBool(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeGetBool");
   
   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_BOOL, pchSettingsKey);
   }

   bool rc = m_down_stream.seti->GetBool(pchSection,pchSettingsKey,peError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EVRSettingsError tracker_error;
	   bool rc2 = m_lock_step_tracker.seti->GetBool(pchSection, pchSettingsKey, &tracker_error);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   if (peError)
	   {
		   TRAIN_TRACKER_ASSERT(tracker_error == *peError);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetBool");
}

int32_t openvr_bridge::GetInt32(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeGetInt32");

   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_INT32, pchSettingsKey);
   }

   int32_t rc = m_down_stream.seti->GetInt32(pchSection,pchSettingsKey,peError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EVRSettingsError tracker_error;
	   int32_t rc2 = m_lock_step_tracker.seti->GetInt32(pchSection, pchSettingsKey, &tracker_error);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   if (peError)
	   {
		   TRAIN_TRACKER_ASSERT(tracker_error == *peError);
	   }
   }


   LOG_EXIT_RC(rc, "BridgeGetInt32");
}

float openvr_bridge::GetFloat(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeGetFloat");
   
   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_FLOAT, pchSettingsKey);
   }

   float rc = m_down_stream.seti->GetFloat(pchSection,pchSettingsKey,peError);

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();
	   vr::EVRSettingsError tracker_error;
	   float rc2 = m_lock_step_tracker.seti->GetFloat(pchSection, pchSettingsKey, &tracker_error);
	   TRAIN_TRACKER_ASSERT(rc == rc2);
	   if (peError)
	   {
		   TRAIN_TRACKER_ASSERT(tracker_error == *peError);
	   }
   }

   LOG_EXIT_RC(rc, "BridgeGetFloat");
}

void openvr_bridge::GetString(const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeGetString");

   if (m_spy_mode)
   {
	   update_vr_config_setting(pchSection, SettingsIndexer::SETTING_TYPE_STRING, pchSettingsKey);
   }

   TmpBuf buf;
   if (m_lock_step_train_tracker)
   {
	   if (unValueLen > 0)
	   {
		   buf.dup(pchValue, unValueLen);
	   }
   }

   m_down_stream.seti->GetString(pchSection,pchSettingsKey,pchValue,unValueLen,peError); // <--- read pchValue here

   if (m_lock_step_train_tracker)
   {
	   refresh_lockstep_capture();   
	   vr::EVRSettingsError tracker_error;
	   m_lock_step_tracker.seti->GetString(pchSection, pchSettingsKey, buf.data(), unValueLen, &tracker_error);
	   if (peError)
	   {
		   TRAIN_TRACKER_ASSERT(tracker_error == *peError);
	   }
	   TRAIN_TRACKER_ASSERT(memcmp(buf.data(), pchValue, unValueLen) == 0);  // <--- check the same value was found in the tracker here
   }

   LOG_EXIT("BridgeGetString");
}

void openvr_bridge::RemoveSection(const char * pchSection, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeRemoveSection");
   m_down_stream.seti->RemoveSection(pchSection,peError);
   LOG_EXIT("BridgeRemoveSection");
}

void openvr_bridge::RemoveKeyInSection(const char * pchSection, const char * pchSettingsKey, vr::EVRSettingsError * peError)
{
   LOG_ENTRY("BridgeRemoveKeyInSection");
   m_down_stream.seti->RemoveKeyInSection(pchSection,pchSettingsKey,peError);
   LOG_EXIT("BridgeRemoveKeyInSection");
}

vr::EVRScreenshotError openvr_bridge::RequestScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, vr::EVRScreenshotType type, const char * pchPreviewFilename, const char * pchVRFilename)
{
   LOG_ENTRY("BridgeRequestScreenshot");
   vr::EVRScreenshotError rc = m_down_stream.screeni->RequestScreenshot(pOutScreenshotHandle,type,pchPreviewFilename,pchVRFilename);
   LOG_EXIT_RC(rc, "BridgeRequestScreenshot");
}

vr::EVRScreenshotError openvr_bridge::HookScreenshot(const vr::EVRScreenshotType * pSupportedTypes, int numTypes)
{
   LOG_ENTRY("BridgeHookScreenshot");
   vr::EVRScreenshotError rc = m_down_stream.screeni->HookScreenshot(pSupportedTypes,numTypes);
   LOG_EXIT_RC(rc, "BridgeHookScreenshot");
}

vr::EVRScreenshotType openvr_bridge::GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError * pError)
{
   LOG_ENTRY("BridgeGetScreenshotPropertyType");
   vr::EVRScreenshotType rc = m_down_stream.screeni->GetScreenshotPropertyType(screenshotHandle,pError);
   LOG_EXIT_RC(rc, "BridgeGetScreenshotPropertyType");
}

uint32_t openvr_bridge::GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, char * pchFilename, uint32_t cchFilename, vr::EVRScreenshotError * pError)
{
   LOG_ENTRY("BridgeGetScreenshotPropertyFilename");
   uint32_t rc = m_down_stream.screeni->GetScreenshotPropertyFilename(screenshotHandle,filenameType,pchFilename,cchFilename,pError);
   LOG_EXIT_RC(rc, "BridgeGetScreenshotPropertyFilename");
}

vr::EVRScreenshotError openvr_bridge::UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress)
{
   LOG_ENTRY("BridgeUpdateScreenshotProgress");
   vr::EVRScreenshotError rc = m_down_stream.screeni->UpdateScreenshotProgress(screenshotHandle,flProgress);
   LOG_EXIT_RC(rc, "BridgeUpdateScreenshotProgress");
}

vr::EVRScreenshotError openvr_bridge::TakeStereoScreenshot(vr::ScreenshotHandle_t * pOutScreenshotHandle, const char * pchPreviewFilename, const char * pchVRFilename)
{
   LOG_ENTRY("BridgeTakeStereoScreenshot");
   vr::EVRScreenshotError rc = m_down_stream.screeni->TakeStereoScreenshot(pOutScreenshotHandle,pchPreviewFilename,pchVRFilename);
   LOG_EXIT_RC(rc, "BridgeTakeStereoScreenshot");
}

vr::EVRScreenshotError openvr_bridge::SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char * pchSourcePreviewFilename, const char * pchSourceVRFilename)
{
   LOG_ENTRY("BridgeSubmitScreenshot");
   vr::EVRScreenshotError rc = m_down_stream.screeni->SubmitScreenshot(screenshotHandle,type,pchSourcePreviewFilename,pchSourceVRFilename);
   LOG_EXIT_RC(rc, "BridgeSubmitScreenshot");
}

uint32_t openvr_bridge::LoadSharedResource(const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen)
{
   LOG_ENTRY("BridgeLoadSharedResource");
   uint32_t rc = m_down_stream.resi->LoadSharedResource(pchResourceName,pchBuffer,unBufferLen);
   LOG_EXIT_RC(rc, "BridgeLoadSharedResource");
}

uint32_t openvr_bridge::GetResourceFullPath(const char * pchResourceName, const char * pchResourceTypeDirectory, char * pchPathBuffer, uint32_t unBufferLen)
{
   LOG_ENTRY("BridgeGetResourceFullPath");
   uint32_t rc = m_down_stream.resi->GetResourceFullPath(pchResourceName,pchResourceTypeDirectory,pchPathBuffer,unBufferLen);
   LOG_EXIT_RC(rc, "BridgeGetResourceFullPath");
}

