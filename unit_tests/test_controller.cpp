#include "capture_controller.h"
#include "capture_config.h"
#include "capture.h"
#include "openvr.h"

void test_controller()
{
	{
		capture_controller controller;
		CaptureConfig config;
		controller.init(config);
		
		// zero updates:
		// check that number of frames collected is zero
		const capture& model(controller.get_model());
		assert(model.get_num_updates() == 0);

		// first update:
		// check that after a single update, the number of updates has changed
		controller.update();
		assert(model.get_num_updates() == 1);
		assert(model.get_last_updated_frame() == 0);	// first frame is zero and timestamp should be zero
		assert(model.get_time_stamp(0) == 0);
		assert(model.get_closest_time_index(0) == 0);

		// second update
		controller.update();
		assert(model.get_num_updates() == 2);
		assert(model.get_last_updated_frame() == 1);	
		time_stamp_t last_time = model.get_time_stamp(1);
		assert(last_time > 0);
		assert(model.get_closest_time_index(last_time) == 1);

		// enque a new event
		vr::VREvent_t e;
		controller.enqueue_event(e);
		// since update has not been called make sure it hasn't been applied to the model prior to update
		assert(model.m_vr_events.size() == 0);
		controller.update();
		// make sure it was applied
		assert(model.m_vr_events.size() == 1);

		// add an overlay key
		int num_key_updates_before = model.m_keys_updates.size();

		const char *test_key = "test_overlay";
		assert(model.m_keys.GetOverlayIndexer().get_index_for_key(test_key) == -1);
		VRKeysUpdate new_overlay = VRKeysUpdate::make_new_overlay(test_key);
		controller.enqueue_new_key(new_overlay);

		// check it hasn't taken effect
		assert(model.m_keys_updates.size() == num_key_updates_before);
		assert(model.m_keys.GetOverlayIndexer().get_index_for_key(test_key) == -1);

		controller.update();

		// check the overlay key took effect after update
		assert(model.m_keys.GetOverlayIndexer().get_index_for_key(test_key) != -1);


		void enqueue_key_configuration(const VRKeysUpdate &update);

	}
}