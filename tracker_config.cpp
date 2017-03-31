#include "platform.h"
#include "tracker_config.h"
#include "memory.h"

static const char *default_overlay_keys[] =
{
	"valve.steam.desktop",  // (Desktop),
	"valve.steam.desktop.thumb",  // Desktop(Thumb)
	"valve.steam.bigpicture",  // Steam
	"valve.steam.bigpicture.thumb",  // Steam(Thumb)
	"system.vrdashboard",  // VR Dashboard
	"system.powermenu",  // Power Menu
	"system.powermenu.thumb",  // Power Menu(Thumb)
	"system.notificationhistory",  // History
	"system.notificationhistory.thumb",  // History(Thumb)
	"system.settings",  // Settings
	"system.settings.thumb",  // Settings(Thumb)
	"system.keyboard",  // Keyboard
	"system.messageoverlay",  // Message Overlay
	"system.messageoverlay.thumb",  // Message Overlay(Thumb)
	"system.IPDValue",  // IPD Setting
	"system.ScreenshotProgress",  // Screenshot Progress
	"system.CameraSurfaceUI",  // Camera Surface UI
	"system.DefaultTransition",  // Default Transition
	"system.controllerstatus.1 ",  // system.controllerstatus.1
	"system.controllerstatus.2 ",  // system.controllerstatus.2
	"system.dashboardfade",  // Fades the world for the dashboard
	"system.dashboard.controls.background",  // Sits behind dashboard controls
	"system.dashboard.active.background",  // Sits behind active dashboard
};

void TrackerConfig::set_default()
{
	nearz = 0.02f;
	farz = 100.0f;
	distortion_sample_width = 43;
	distortion_sample_height = 43;
	predicted_seconds_to_photon = 0.0f;
	num_bounds_colors = 10;
	collision_bounds_fade_distance = 2.0f;
	frame_timing_frames_ago = 0;
	frame_timings_num_frames = 10;

	num_overlays = TBL_SIZE(default_overlay_keys);
	overlay_keys = default_overlay_keys;

	num_resources = 0;
	resource_directories = nullptr;
	resource_filenames = nullptr;

	memset(&custom_settings, 0, sizeof(custom_settings));
	memset(&custom_tracked_device_properties, 0, sizeof(custom_tracked_device_properties));
}
