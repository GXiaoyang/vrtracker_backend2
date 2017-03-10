#pragma once

#include "tracker_config.h"
#include "vr_device_properties_indexer.h"
#include "vr_overlay_indexer.h"
#include "vr_applications_indexer.h"
#include "vr_applications_properties_indexer.h"
#include "vr_resources_indexer.h"
#include "vr_settings_indexer.h"
#include "vr_mime_types_indexer.h"

// vr_keys
//
// resources outside of the bare openvr interfaces that need to be 
// tracked 
//
struct vr_keys
{
	vr_keys()
	{}

	DevicePropertiesIndexer &GetDevicePropertiesIndexer() { return m_device_properties_indexer; }
	OverlayIndexer	&GetOverlayIndexer() { return m_overlay_indexer; }
	ApplicationsIndexer &GetApplicationsIndexer() { return m_applications_indexer; }
	ApplicationsPropertiesIndexer &GetApplicationsPropertiesIndexer() { return m_applications_properties_indexer; }
	ResourcesIndexer &GetResourcesIndexer() { return m_resources_indexer; }
	SettingsIndexer &GetSettingsIndexer() { return m_settings_indexer; }
	
	MimeTypesIndexer &GetMimeTypesIndexer() { return m_mime_types_indexer; }

	void Init(const TrackerConfig &c)
	{
		m_overlay_indexer.Init(c.overlay_keys, c.num_overlays);
		m_resources_indexer.Init(c.resource_filenames, c.resource_directories, c.num_resources);
		m_settings_indexer.Init(
			c.custom_settings.num_bool_settings, c.custom_settings.bool_sections, c.custom_settings.bool_names,
			c.custom_settings.num_int32_settings, c.custom_settings.int32_sections, c.custom_settings.int32_names,
			c.custom_settings.num_string_settings, c.custom_settings.string_sections, c.custom_settings.string_names,
			c.custom_settings.num_float_settings, c.custom_settings.float_sections, c.custom_settings.float_names
		);
		m_applications_properties_indexer.Init();
		auto *props = &c.custom_tracked_device_properties;
		m_device_properties_indexer.Init(
			props->num_bool_properties, props->bool_names, props->bool_values,
			props->num_string_properties, props->string_names, props->string_values,
			props->num_uint64_properties, props->uint64_names, props->uint64_values,
			props->num_int32_properties, props->int32_names, props->int32_values,
			props->num_mat34_properties, props->mat34_names, props->mat34_values,
			props->num_float_properties, props->float_names, props->float_values);
		m_data.nearz = c.nearz;
		m_data.farz = c.farz;
		m_data.distortion_sample_height = c.distortion_sample_height;
		m_data.distortion_sample_width = c.distortion_sample_width;
		m_data.predicted_seconds_to_photon = c.predicted_seconds_to_photon;
		m_data.num_bounds_colors = c.num_bounds_colors;
		m_data.collision_bounds_fade_distance = c.collision_bounds_fade_distance;
		m_data.frame_timing_frames_ago = c.frame_timing_frames_ago;
		m_data.frame_timings_num_frames = c.frame_timings_num_frames;
	}

	void UpdateNearFar(float fnear, float ffar)
	{
		m_data.nearz = fnear;
		m_data.farz = ffar;
	}

	void write_to_stream(EncodeStream &stream)
	{
		stream.memcpy_out_to_stream(&m_data, sizeof(m_data));
		m_overlay_indexer.WriteToStream(stream);
		m_applications_indexer.WriteToStream(stream);
		m_applications_properties_indexer.WriteToStream(stream);
		m_device_properties_indexer.WriteToStream(stream);
		m_resources_indexer.WriteToStream(stream);
		m_settings_indexer.WriteToStream(stream);
	}

	void read_from_stream(EncodeStream &stream)
	{
		stream.memcpy_from_stream(&m_data, sizeof(m_data));
		m_overlay_indexer.ReadFromStream(stream);
		m_applications_indexer.ReadFromStream(stream);
		m_applications_properties_indexer.ReadFromStream(stream);
		m_device_properties_indexer.ReadFromStream(stream);
		m_resources_indexer.ReadFromStream(stream);
		m_settings_indexer.ReadFromStream(stream);
	}

	uint64_t GetEncodedSize()
	{
		EncodeStream counter(nullptr, 0, true);
		write_to_stream(counter);
		return counter.buf_pos;
	}

	void Encode(char *buf, uint64_t buf_size)
	{
		EncodeStream encoder(buf, buf_size, false);
		write_to_stream(encoder);
	}

	void Decode(char *buf, uint64_t buf_size)
	{
		EncodeStream decoder(buf, buf_size, false);
		read_from_stream(decoder);
	}

	float GetNearZ() const { return m_data.nearz; }
	float GetFarZ() const { return m_data.farz; }
	int GetDistortionSampleWidth() const { return m_data.distortion_sample_width; }
	int GetDistortionSampleHeight() const { return m_data.distortion_sample_height; }
	float GetPredictedSecondsToPhoton() const { return m_data.predicted_seconds_to_photon; }
	int GetNumBoundsColors() const { return m_data.num_bounds_colors; }
	float GetCollisionBoundsFadeDistance() const { return m_data.collision_bounds_fade_distance; }
	uint32_t GetFrameTimingFramesAgo() const { return m_data.frame_timing_frames_ago; }
	uint32_t GetFrameTimingsNumFrames() const { return m_data.frame_timings_num_frames; }

private:
	struct {
		float nearz;
		float farz;
		int distortion_sample_width;
		int distortion_sample_height;
		float predicted_seconds_to_photon;
		int num_bounds_colors;
		float collision_bounds_fade_distance;
		uint32_t frame_timing_frames_ago;
		uint32_t frame_timings_num_frames;
	} m_data;

	OverlayIndexer m_overlay_indexer;
	ApplicationsIndexer m_applications_indexer;
	ApplicationsPropertiesIndexer m_applications_properties_indexer;
	ResourcesIndexer m_resources_indexer;
	SettingsIndexer m_settings_indexer;
	DevicePropertiesIndexer m_device_properties_indexer;
	MimeTypesIndexer m_mime_types_indexer;
};
