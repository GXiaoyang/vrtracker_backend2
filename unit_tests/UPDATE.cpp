// rangesplay.cpp : Defines the entry point for the console application.
//

#include "capture_traverser.h"
#include "log.h"
#include "capture_test_context.h"

using namespace vr;

void UPDATE_USE_CASE()
{
	using namespace vr;
	using namespace vr_result;
	using std::range;

	log_printf("start of UPDATE_USE_CASE\n");

	capture_test_context context;

	// 
	// Sequential visit 
	//
	capture_traverser u;
	log_printf("traversing sequential\n");
	u.update_capture_sequential(&context.get_capture(), &context.raw_vr_interfaces(), 0);
	log_printf("traversing sequential\n");
	u.update_capture_sequential(&context.get_capture(), &context.raw_vr_interfaces(), 1);
	log_printf("traversing parallel\n");
	u.update_capture_parallel(&context.get_capture(), &context.raw_vr_interfaces(), 2);

	for (int i = 0; i < 10; i++)
	{
		u.update_capture_parallel(&context.get_capture(), &context.raw_vr_interfaces(), 3+i);
	}
	log_printf("done UPDATE_USE_CASE\n");
}

