#include "capture_controller.h"
#include <chrono>

capture_controller::capture_controller()
{
}

bool capture_controller::init(const CaptureConfig &c)
{
	m_model.m_keys.Init(c);
	char *error;
	bool acquired = openvr_broker::acquire_interfaces("raw", &m_interfaces, &error);
	if (!acquired)
	{
		log_printf("error! %s", error);
		return false;
	}
	return true;
}


using us = std::chrono::duration<int64_t, std::micro>;

struct capture_controller::pending_controller_update
{
	virtual void apply(capture *) = 0;
};

struct pending_event_update : public capture_controller::pending_controller_update
{
	pending_event_update(time_point_t a, const vr::VREvent_t& b)
		: m_time(a), m_event(b)
	{}

	void apply(capture *target) override
	{
		target->m_vr_events.emplace_back(target->get_last_updated_frame() + 1, m_event);

		us frame_time = std::chrono::duration_cast<std::chrono::microseconds>(m_time - target->m_start);
		target->m_time_stamps.emplace_back(frame_time.count());
		target->increment_last_updated_frame();
	}

	time_point_t m_time;
	VREncodableEvent m_event;
};

struct pending_overlay_event_update : public capture_controller::pending_controller_update
{
	pending_overlay_event_update(time_point_t a, vr::VROverlayHandle_t b, const vr::VREvent_t& c)
		: m_time(a), m_handle(b), m_event(c)
	{}

	void apply(capture *target) override
	{
		// TODO(sean): finish overlay events
		assert(0); // the key has to do it's work
		// find the overlay for this id. add this event to the overlay

		us frame_time = std::chrono::duration_cast<std::chrono::microseconds>(m_time - target->m_start);
		target->m_time_stamps.emplace_back(frame_time.count());
		target->increment_last_updated_frame();
	}

	time_point_t m_time;
	vr::VROverlayHandle_t m_handle;
	vr::VREvent_t m_event;
};

struct pending_key_update : public capture_controller::pending_controller_update
{
	pending_key_update(time_point_t a, const VRKeysUpdate& b)
		: m_time(a), m_key_update(b)
	{}

	void apply(capture *target) override
	{
		switch (m_key_update.update_type)
		{
			case VRKeysUpdate::NEW_APP_KEY:
				target->m_keys.GetApplicationsIndexer().add_app_key(m_key_update.sparam1.c_str());
				break;
			
			case VRKeysUpdate::NEW_SETTING:
				target->m_keys.GetSettingsIndexer().AddCustomSetting(
					m_key_update.sparam1.c_str(), 
					static_cast<SettingsIndexer::SectionSettingType>(m_key_update.iparam1), 
					m_key_update.sparam2.c_str());
				break;
			case VRKeysUpdate::NEW_DEVICE_PROPERTY:
				target->m_keys.GetDevicePropertiesIndexer().AddCustomProperty(
					static_cast<PropertiesIndexer::PropertySettingType>(m_key_update.iparam1),
					m_key_update.sparam1.c_str(),
					m_key_update.iparam2);
				break;
			case VRKeysUpdate::NEW_RESOURCE:
				target->m_keys.GetResourcesIndexer().add_resource(m_key_update.sparam1.c_str(), m_key_update.sparam2.c_str());
				break;
			case VRKeysUpdate::NEW_OVERLAY:
				target->m_keys.GetOverlayIndexer().add_overlay_key(m_key_update.sparam1.c_str());
				break;
			
			default:
				assert(0); // the key has to do it's work
							// assert 0
		}

		target->m_keys_updates.emplace_back(target->get_last_updated_frame() + 1, m_key_update);
		us frame_time = std::chrono::duration_cast<std::chrono::microseconds>(m_time - target->m_start);
		target->m_time_stamps.emplace_back(frame_time.count());
		target->increment_last_updated_frame();
	}

	time_point_t m_time;
	VRKeysUpdate m_key_update;
};

void capture_controller::update()
{
	// apply queued events to the capture
	m_queue_lock.lock();

	for (auto pending_update : m_pending_updates)
	{
		pending_update->apply(&m_model);
		delete pending_update;
	}
	m_pending_updates.clear();
	
	m_queue_lock.unlock();

	m_update_lock.lock();
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	if (m_model.get_num_updates() == 0)
	{
		m_model.m_start = start;
	}
	using us = std::chrono::duration<int64_t, std::micro>;
	us frame_time = std::chrono::duration_cast<std::chrono::microseconds>(start - m_model.m_start);
	m_traverser.update_capture_sequential(&m_model, &m_interfaces, frame_time.count());
	m_update_lock.unlock();
}

void capture_controller::enqueue_new_key(const VRKeysUpdate &update)
{
	m_queue_lock.lock();
	auto key_update = new pending_key_update(std::chrono::steady_clock::now(), update);
	m_pending_updates.push_back(key_update);
	m_queue_lock.unlock();
}

void capture_controller::enqueue_event(const vr::VREvent_t &event_in)
{
	m_queue_lock.lock();
	auto event_update = new pending_event_update(std::chrono::steady_clock::now(), event_in);
	m_pending_updates.push_back(event_update);
	m_queue_lock.unlock();
}

void capture_controller::enqueue_overlay_event(vr::VROverlayHandle_t overlay_handle, const vr::VREvent_t &event_in)
{
	m_queue_lock.lock();
	auto event_update = new pending_overlay_event_update(std::chrono::steady_clock::now(), overlay_handle, event_in);
	m_pending_updates.push_back(event_update);
	m_queue_lock.unlock();
}
