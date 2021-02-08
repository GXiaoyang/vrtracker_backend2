#include "openvr_broker.h"
#include <stdio.h>
#include <string.h>
#include "openvr_string_std.h"

static char s_error_message[1024];

#ifdef HAVE_OPEN_VR_RAW
static bool acquire_raw_interfaces(openvr_broker::open_vr_interfaces *interfaces,
	char **error_message);
#endif

static bool acquire_null_interfaces(openvr_broker::open_vr_interfaces *interfaces,
	char **error_message);
static bool acquire_stub_interfaces(openvr_broker::open_vr_interfaces *interfaces,
	char **error_message);

bool openvr_broker::acquire_interfaces(const char *interface_type,
	openvr_broker::open_vr_interfaces *interfaces, char **error_message)
{
#ifdef HAVE_OPEN_VR_RAW
	if (strcmp(interface_type, "raw") == 0)
	{
		return acquire_raw_interfaces(interfaces, error_message);
	}
#endif
	if (strcmp(interface_type, "null") == 0)
	{
		return acquire_null_interfaces(interfaces, error_message);
	}
	if (strcmp(interface_type, "stub") == 0)
	{
		return acquire_stub_interfaces(interfaces, error_message);
	}
	else
	{
		snprintf(s_error_message, sizeof(s_error_message), "broker error unrecognized interface type %s", interface_type);
		*error_message = s_error_message;
		return false;
	}
}

template <size_t count>
void strcpy_safe(char(&s)[count], const char* pSrc)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	strncpy(s, pSrc, count);
#pragma warning(pop)
	// Ensure null-termination.
	s[count - 1] = 0;
}


#ifdef HAVE_OPEN_VR_RAW
static bool acquire_raw_interfaces(openvr_broker::open_vr_interfaces *interfaces, char **error_message)
{
	vr::EVRInitError eError = vr::VRInitError_None;

#if 0
	if (!vr::VR_IsHmdPresent())
	{
		strcpy_safe(s_error_message, "no hmd present");
		*error_message = s_error_message;
		return false;
	}
#endif

	extern int hack_vr_init_called; 
	if (!hack_vr_init_called)
	{
		VR_Init(&eError, vr::VRApplication_Scene);
		hack_vr_init_called = 1;
	}
	else
	{
		eError = vr::VRInitError_None;
	}
	
	if (eError != vr::VRInitError_None)
	{
		snprintf(s_error_message, sizeof(s_error_message), "Unable to init VR runtime: %s",
			openvr_string::to_string(eError).c_str());
		*error_message = s_error_message;
		return false;
	}
	interfaces->sysi = vr::VRSystem();
	interfaces->appi = vr::VRApplications();
	interfaces->seti = vr::VRSettings();
	interfaces->chapi = vr::VRChaperone();
	interfaces->chapsi = vr::VRChaperoneSetup();
	interfaces->compi = vr::VRCompositor();
	interfaces->ovi = vr::VROverlay();
	interfaces->remi = vr::VRRenderModels();
	interfaces->exdi = vr::VRExtendedDisplay();
	interfaces->taci = vr::VRTrackedCamera();
	interfaces->screeni = vr::VRScreenshots();
	interfaces->noti = nullptr;  // from openvr.h: This current interface is not yet implemented. Do not use yet.
	interfaces->resi = vr::VRResources();
	interfaces->drivi = vr::VRDriverManager();
	return true;
}
#endif

void openvr_broker::shutdown()
{
#ifdef HAVE_OPEN_VR_RAW
	vr::VR_Shutdown();
#endif
}


static bool acquire_null_interfaces(openvr_broker::open_vr_interfaces *interfaces,
	char **error_message)
{
	interfaces->sysi =		nullptr;
	interfaces->appi =		nullptr;
	interfaces->seti =		nullptr;
	interfaces->chapi =		nullptr;
	interfaces->chapsi =	nullptr;
	interfaces->compi =		nullptr;
	interfaces->ovi =		nullptr;
	interfaces->remi =		nullptr;
	interfaces->exdi =		nullptr;
	interfaces->taci =		nullptr;
	interfaces->screeni = nullptr;
	interfaces->noti = nullptr;  // from openvr.h: This current interface is not yet implemented. Do not use yet.
	interfaces->resi = nullptr;
	interfaces->drivi = nullptr;
	return true;
}

static bool acquire_stub_interfaces(openvr_broker::open_vr_interfaces *interfaces,
	char **error_message)
{
	interfaces->sysi = nullptr;
	interfaces->appi = nullptr;
	interfaces->seti = nullptr;
	interfaces->chapi = nullptr;
	interfaces->chapsi = nullptr;
	interfaces->compi = nullptr;
	interfaces->ovi = nullptr;
	interfaces->remi = nullptr;
	interfaces->exdi = nullptr;
	interfaces->taci = nullptr;
	interfaces->screeni = nullptr;
	interfaces->noti = nullptr;  // from openvr.h: This current interface is not yet implemented. Do not use yet.
	interfaces->resi = nullptr;
	interfaces->drivi = nullptr;
	return true;
}
