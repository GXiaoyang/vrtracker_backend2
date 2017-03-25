// rangesplay.cpp : Defines the entry point for the console application.
//

#include "vr_tracker_traverse.h"
#include "log.h"
#include "tracker_test_context.h"

using namespace vr;

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

