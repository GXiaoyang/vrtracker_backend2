// rangesplay.cpp : Defines the entry point for the console application.
//

#include "vr_tracker_traverse.h"
#include "url_named.h"
#include "log.h"
#include <thread>
#include "tracker_test_context.h"

using namespace vr;

void load_one_overlay_image(const char *overlay_name, vr::IVROverlay *ovi)
{
	vr::VROverlayHandle_t handle;
	vr::EVROverlayError err = ovi->FindOverlay(overlay_name, &handle);
	if (err == vr::VROverlayError_None)
	{
		uint32_t width;
		uint32_t height;
		EVROverlayError err2 = ovi->GetOverlayImageData(handle, nullptr, 0, &width, &height);
		if (err2 == VROverlayError_ArrayTooSmall)
		{
			size_t required_size = width * height * 4;
			void *buf = malloc(required_size);
			if (buf)
			{
				EVROverlayError err3 = ovi->GetOverlayImageData(handle, buf, required_size, &width, &height);
				if (err3 == VROverlayError_None)
				{
					log_printf("success\n");
				}
				else
				{
					log_printf("failed\n");
				}
				free(buf);
			}
		}
	}
}

void parallel_overlay_test(vr::IVROverlay *ovi)
{
	for (int i = 0; i < 10; i++)
	{
		load_one_overlay_image("valve.steam.desktop", ovi);
		load_one_overlay_image("valve.steam.bigpicture", ovi);
		load_one_overlay_image("system.vrdashboard", ovi);
	}
	
	std::vector<std::thread *> threads;
	for (int i = 0; i < 100; i++)
	{
		threads.push_back(new std::thread(load_one_overlay_image, "valve.steam.bigpicture", ovi));
		threads.push_back(new std::thread(load_one_overlay_image, "system.vrdashboard", ovi));
	}

	for (auto thread : threads)
	{
		thread->join();
	}
}

void UPDATE_USE_CASE()
{
	using namespace vr;
	using namespace vr_result;
	using std::range;

	tracker_test_context context;

	// 
	// Sequential visit 
	//
	vr_tracker_traverse u;
	u.update_tracker_sequential(&context.get_tracker(), &context.raw_vr_interfaces());
	u.update_tracker_sequential(&context.get_tracker(), &context.raw_vr_interfaces());
	u.update_tracker_parallel(&context.get_tracker(), &context.raw_vr_interfaces());

	for (int i = 0; i < 10; i++)
	{
		u.update_tracker_parallel(&context.get_tracker(), &context.raw_vr_interfaces());
	}
}

