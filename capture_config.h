#pragma once


// need to pipe in callers areas of interest:
// caller assets: resources, screenshot handles, overlays, 
// caller specific queries: nearz, farz, seconds to photon queries, etc:
// unless he tells me, then I need to "spy" it out
//
// its meant to be more of a hint than part of the database - ie don't add dependencies to this
struct CaptureConfig	// aka Hints
{
	float nearz;					// intercept: GetProjectionMatrix
	float farz;

	int  distortion_sample_width;
	int  distortion_sample_height;

	float predicted_seconds_to_photon; // intercept: GetDeviceToAbsoluteTrackingPose

	int num_bounds_colors;					// intercept: GetBoundsColor
	float collision_bounds_fade_distance;	// ""

	unsigned int frame_timing_frames_ago;		// intercept GetFrameTiming
	unsigned int frame_timings_num_frames;		// intercept GetFrameTimings

	int num_overlays;				// intercept 30-50 overlay functions
	const char **overlay_keys;

	int num_resources;			// intercept GetResourceFullPath and GetResourceFullPath
	const char **resource_directories;
	const char **resource_filenames;

	// custom settings
	struct {
		int num_bool_settings;
		const char **bool_sections;
		const char **bool_names;

		int num_int32_settings;
		const char **int32_sections;
		const char **int32_names;

		int num_string_settings;
		const char **string_sections;
		const char **string_names;

		int num_float_settings;
		const char **float_sections;
		const char **float_names;
	} custom_settings;

	// custom tracked device properties
	struct {
		int num_bool_properties;
		const char **bool_names;
		int *bool_values;

		int num_string_properties;
		const char **string_names;
		int *string_values;

		int num_uint64_properties;
		const char **uint64_names;
		int *uint64_values;

		int num_int32_properties;
		const char **int32_names;
		int *int32_values;

		int num_mat34_properties;
		const char **mat34_names;
		int *mat34_values;

		int num_float_properties;
		const char **float_names;
		int *float_values;
	} custom_tracked_device_properties;

	void set_default();

};