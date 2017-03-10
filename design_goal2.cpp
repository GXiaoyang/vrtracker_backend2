// rangesplay.cpp : Defines the entry point for the console application.
//

#include "time_containers.h"
#include "update_history_visitor.h"
#include "vr_system_wrapper.h"
#include "vr_schema.h"

#include "vr_tracker.h"

#include "slab_allocator.h"
#include "segmented_list.h"
#include "openvr_string_std.h"
#include <vector>

#if 0

// how the gui will use these data structures

void GUI_USE_CASE()
{
	using namespace openvr_string;
	using namespace vr;
	using namespace vr_result;
	using std::range;

	// 
	// write some placeholder data
	//
	URL root("system", "/system");
	systemx vr(root);
	vr.controllers.emplace_back(root.make_child("controllers"));
	Result<vr::TrackedDevicePose_t, NoReturnCode> p;
	vr.controllers[0].raw_tracking_pose.container.emplace_back(0, p);
	vr.m_time_stamps.emplace_back(0);

	//
	// test starts here
	//

	// I0: case 0 - what if it's not present?
	Result<vr::TrackedDevicePose_t, NoReturnCode> pose;
	if (!vr.controllers.empty() && 
		!vr.controllers[0].raw_tracking_pose.empty())
	{
		pose = vr.controllers[0].raw_tracking_pose();
	}

	// II: ask some meta data questions
	std::string name = vr.controllers[0].raw_tracking_pose.get_name();
	std::string path = vr.controllers[0].raw_tracking_pose.get_path();


	// III: return a time_indexed version
	time_indexed<DevicePose> a = vr.controllers[0].raw_tracking_pose.latest();
	time_indexed<DevicePose> b = vr.controllers[0].raw_tracking_pose.earliest();


	time_index_t t = a.get_time_index();	// returns an int
	time_stamp_t ts = vr.get_time_stamp(a.get_time_index()); // returns a time
												  // IV: return a range
	auto pose_range = vr.controllers[0].raw_tracking_pose.get_range();
	auto r2 = vr.controllers[0].raw_tracking_pose.get_range(0, 42);


	time_stamp_t ta = 0;
	time_stamp_t tb = 0;
	auto range_from_times = vr.controllers[0].raw_tracking_pose.get_range(
		vr.get_closest_time_index(ta), vr.get_closest_time_index(tb));	// inputs are times

																		// V: copy a range of time_indexed_values into a vector
	std::vector<decltype(pose_range)::value_type> a_copy(pose_range.begin(), pose_range.end());


	// VIII: given a vector of time_indexed values, create a vector that holds the corresponding values
	std::vector<time_stamp_t> vec;
	vec.resize(pose_range.size());
	std::transform(pose_range.begin(), pose_range.end(), vec.begin(),
		[&vr](auto &c) { return vr.get_time_stamp(c.get_time_index()); });

	// VII: convert to a string (using return value from I)
	to_string(vr.controllers[0].raw_tracking_pose().val);
}








template <typename ElementType, typename ReturnCode>
struct xResult
{
	xResult() {}

	template <typename ResultType2>
	xResult(const ResultType2 &rhs)
	{
		printf("hoo hoo");
	}

};


void UPDATE_USE_CASE()
{
	using namespace vr;
	using namespace vr_result;

#if 0
	typedef  std::allocator<char*> allocator_type;
	allocator_type allocator = std::allocator<char*>();
#else
	slab s(1024 * 1024);
	typedef  slab_allocator<char *> allocator_type;
	allocator_type allocator;
	allocator.m_slab = &s;
#endif

	URL root("vr", "/vr");
	vrschema<false, allocator_type>	state(root, allocator);
	vrschema<true, allocator_type>	iter(root, allocator);

	
	update_history_visitor visitor(1);
	if (state.system_node.seconds_since_last_vsync.empty())
	{
		printf("bla");
	}
	visitor.start_group_node(state.system_node.get_url(), 0);

#if 0
	TMPDeviceIndexes<allocator_type> tmp;
	SystemWrapper sys_wrap(nullptr);
	visitor.visit_node(state.system_node.hmds_sorted, sys_wrap.GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_HMD, 0, &tmp));
	visitor.end_group_node(state.system_node.get_url(), 0);
#endif


	printf("%d", state.system_node.controllers[0].activity_level.latest().get_value().val);
	printf("%d", state.system_node.controllers[0].bool_props[0].latest().get_value().val);
	
	

	




}

#endif