// GUI usecase

#include "capture.h"
#include "openvr_string_std.h"
#include "capture_test_context.h"


// how the gui will use these data structures
// how the gui will use these data structures

void GUI_USE_CASE_TESTx()
{
	using namespace openvr_string;
	vr::TrackedDevicePose_t pose;
	memset(&pose, 0, sizeof(pose));
	to_string(pose);
}

void GUI_USE_CASE_TEST()
{
	using namespace openvr_string;
	using namespace vr;
	using namespace vr_result;
	using std::range;

	log_printf("starting gui use case\n");

	capture_test_context context;
	capture &capture = context.get_capture();

	auto &system = capture.m_state.system_node;
	auto &controllers = capture.m_state.system_node.controllers;

	//system_controller_state a_controller;
	base::URL u;
	system_controller_state statey;
	controllers.emplace_back(statey);

	DevicePose<> pose_in;
	memset(&pose_in.val, 0, sizeof(pose_in.val));
	pose_in.val.bPoseIsValid = true;
        pose_in.val.eTrackingResult = TrackingResult_Uninitialized;
	capture.m_time_stamps.push_back(123);
	controllers[0].raw_tracking_pose.emplace_back(0, pose_in);
	

	//
	// test starts here
	//

	// I0: case 0 - what if it's not present?
	DevicePose<> pose_out;
	if (!controllers.empty() && 
		!controllers[0].raw_tracking_pose.empty())
	{
		pose_out = controllers[0].raw_tracking_pose();
	}
	assert(pose_out == pose_in);

	// II: ask some meta data questions
	std::string name = controllers[0].raw_tracking_pose.get_name();
	std::string path = controllers[0].raw_tracking_pose.get_path();

	// III: return a time_indexed version
	time_indexed<DevicePose<>> a = controllers[0].raw_tracking_pose.latest();
	time_indexed<DevicePose<>> b = controllers[0].raw_tracking_pose.earliest();

	assert(a.get_value() == pose_in);
	assert(b.get_value() == pose_in);

	time_index_t t = a.get_time_index();	// returns an int
	assert(t == 0);
	time_stamp_t ts = capture.get_time_stamp(a.get_time_index()); // returns a time
	assert(ts == 123);

	// for the cursor use-case make sure I can assign iterators from system state
	vr_iterator iterators;
	iterators.system_node.recommended_target_size = capture.m_state.system_node.recommended_target_size.container.begin();



												  // IV: return a range
	auto pose_range = controllers[0].raw_tracking_pose.get_range();

	auto r2 = controllers[0].raw_tracking_pose.get_range(0, 1000);


	time_stamp_t ta = 0;
	time_stamp_t tb = 0;
	auto range_from_times = controllers[0].raw_tracking_pose.get_range(
		capture.get_closest_time_index(ta), capture.get_closest_time_index(tb));	// inputs are times

																		// V: copy a range of time_indexed_values into a vector
	std::vector<time_indexed<DevicePose<>>> a_copy(pose_range.begin(), pose_range.end());

	assert(a_copy[0].get_value() == pose_in);

	// VIII: given a vector of time_indexed values, create a vector that holds the corresponding values
	std::vector<time_stamp_t> vec;
	vec.resize(pose_range.size());
	std::transform(pose_range.begin(), pose_range.end(), vec.begin(),
		[&capture](const time_indexed<DevicePose<>> &c) { return capture.get_time_stamp(c.get_time_index()); });

	// VII: convert to a string (using return value from I)
	to_string(controllers[0].raw_tracking_pose().val);
	//to_string(pose_in.val);
}
