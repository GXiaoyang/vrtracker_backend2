#pragma once

#include <openvr.h>

// HOW to use this:
//
// openvr_broker::open_vr_interfaces vr_interfaces;
// char *error;
//	openvr_broker::open_vr_interfaces interfaces;
//	if (!openvr_broker::acquire_interfaces("raw", interfaces, &error))
//	{
//		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", error, NULL);
//	}

namespace openvr_broker
{
	struct open_vr_interfaces
	{
		vr::IVRSystem			*sysi;
		vr::IVRApplications		*appi;
		vr::IVRSettings			*seti;
		vr::IVRChaperone		*chapi;
		vr::IVRChaperoneSetup	*chapsi;
		vr::IVRCompositor		*compi;
		vr::IVRNotifications	*noti;
		vr::IVROverlay			*ovi;
		vr::IVRRenderModels		*remi;
		vr::IVRExtendedDisplay	*exdi;
		vr::IVRTrackedCamera	*taci;
		vr::IVRScreenshots		*screeni;
		vr::IVRResources		*resi;
	};
	
	struct open_vr_interfaces_void
	{
		void *sysi;
		void *appi;
		void *seti;
		void *chapi;
		void *chapsi;
		void *compi;
		void *noti;
		void *ovi;
		void *remi;
		void *exdi;
		void *taci;
		void *screeni;
		void *resi;
	};


	bool acquire_interfaces(
		const char *type,
		openvr_broker::open_vr_interfaces *interfaces, char **error_message);
	
};
