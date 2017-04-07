#pragma once

// capture_controller
//
//
// api to update and modify a shared capture object
// e.g. the gui might be indexing a capture object
//
// responsible for any sequencing
//

#include "capture.h"
#include "capture_traverser.h"
#include "openvr_broker.h"

struct capture_controller
{
	capture_controller();
	
	// use an existing model
	void init(capture *c, const openvr_broker::open_vr_interfaces &interfaces);


	// apply any enqueued configuration events
	// update the model
	void update();

	const capture &get_model() { return *m_model; };

	void enqueue_new_key(const VRKeysUpdate &update);
	void enqueue_event(const vr::VREvent_t &event_in);
	void enqueue_overlay_event(vr::VROverlayHandle_t overlay_handle, const vr::VREvent_t &event_in);
	
	struct pending_controller_update;
private:
	capture *m_model;
	capture_traverser m_traverser;
	openvr_broker::open_vr_interfaces m_interfaces;

	// serializes access to update() - only one global update at a time (internally update can be multi-threaded)
	std::mutex m_update_lock;

	// serializes access to the update queue.  A single queue is used to preserve time order
	std::mutex m_queue_lock;
	std::vector<pending_controller_update *> m_pending_updates;
	
};
