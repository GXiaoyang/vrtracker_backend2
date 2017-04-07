//
// test things that traverse the schema:
//	serialization and update
//
#include "log.h"
#include "openvr_dll_client.h"
#include "openvr_bridge.h"
#include "capture.h"
#include "capture_controller.h"
#include "capture_test_context.h"
#include "vr_cursor_controller.h"
#include "assert.h"

void test_openvr_bridge()
{
	static LPCTSTR server_dll = L"openvr_api.dll";
	static LPCTSTR extra_path = L"C:\\projects\\openvr_clean\\openvr\\bin\\win32";
	uint32_t width;
	uint32_t height;
		
	openvr_dll_client openvr_dll;
	openvr_dll.open_lib(server_dll, extra_path);

	{
		//
		// USE-CASE 0 create a bridge to nowhere. and uh. make sure it doesn't crash...
		//
		openvr_bridge bridge;
		assert(bridge.interfaces().sysi == nullptr);
	}

	{
		//
		// USE-CASE 1 - create a bridge that just talks to the openvr dll
		//
		// this is just simple passthrough into the openvr dll
		openvr_bridge bridge;
		bridge.set_down_stream_interface(*openvr_dll.cpp_interfaces());
		bridge.interfaces().sysi->GetRecommendedRenderTargetSize(&width, &height); // test call using the block of interfaces
		bridge.GetRecommendedRenderTargetSize(&width, &height); // call through the object
		assert(bridge.get_down_stream_capture_controller() == nullptr); // this shouldnt be present
	}

	{
		//
		// USE-CASE 2 - create a bridge with a capture object to track modifications
		// this is passthrough with capture
		//

		//
		// create a capture container and a controller
		capture_test_context x;
		x.ForceInitAll();
		capture_controller controller;
		controller.init(x.get_capture(), x.raw_vr_interfaces());

		//
		// construct a bridge to:
		//		(a) an real openvrdll interface (downstream)
		//      (b) with a capture controller.  this thing will refresh itself anytime WaitGetPoses gets called
		openvr_bridge bridge;
		bridge.set_down_stream_interface(x.raw_vr_interfaces());  // step (a)

		// attach the capture controller
		bridge.set_down_stream_capture_controller(&controller);  // step (b);
		assert(bridge.get_down_stream_capture_controller() == &controller); 

		// check the capture has zero updates since nothing has happened yet
		assert(x.get_capture().get_num_updates() == 0);

		//
		// call waitgetposes on the BRIDGE.  the purpose of this call is because I know it should
		// trigger the capture to do a snapshot of the vr state
		//
		vr::TrackedDevicePose_t poses1[vr::k_unMaxTrackedDeviceCount];
		vr::EVRCompositorError r1 = bridge.WaitGetPoses(poses1, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

		//
		// check that after WaitGetPoses on the bridge, the capture has been updated
		//
		assert(x.get_capture().get_num_updates() == 1);

		// 
		// check that the values in the capture are correct
		//

		// construct a cursor into the capture and check that the poses are the same as the once we got across the bridge
		vr_cursor_controller cursor_controller;
		cursor_controller.init(&x.get_capture());
		vr::TrackedDevicePose_t poses2[vr::k_unMaxTrackedDeviceCount];
		vr::EVRCompositorError r2 = cursor_controller.interfaces().compi->WaitGetPoses(poses2, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

		// check that the results matched
		assert(r1 == r2);
		if (r1 == vr::VRCompositorError_None)
		{
			// we can check that the poses matched too
			assert(memcmp(poses1, poses2, sizeof(poses1) == 0));
		}
	}

	{
		//
		// USE-CASE 3 - create a bridge to inspect a capture loaded from disk
		//
		// a major main point here is openvr apis are not used at all
		//
		std::string filename(plat::make_temporary_filename());

		// step 1: make a capture file
		{
			capture_traverser t;
			capture dummy;
			t.save_capture_to_binary_file(&dummy, filename.c_str());
		}

		// step 2. load the dummy file and bridge to it
		{
			capture_traverser t;
			capture c;
			t.load_capture_from_binary_file(&c, filename.c_str());

			vr_cursor_controller cursor_controller;
			cursor_controller.init(&c);
			
			// a bridge that is into a cursor that refers to c
			openvr_bridge b;
			b.set_down_stream_interface(cursor_controller.interfaces());
			uint32_t width;
			uint32_t height;
			b.GetRecommendedRenderTargetSize(&width, &height);
		}
	}
}

