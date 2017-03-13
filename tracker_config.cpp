#include "tracker_config.h"
#include "memory.h"

static const char *default_overlay_keys[] =
{
	"valve.steam.desktop"
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

	num_overlays = 1;
	overlay_keys = default_overlay_keys;

	num_resources = 0;
	resource_directories = nullptr;
	resource_filenames = nullptr;

	memset(&custom_settings, 0, sizeof(custom_settings));
	memset(&custom_tracked_device_properties, 0, sizeof(custom_tracked_device_properties));
}