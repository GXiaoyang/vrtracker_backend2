// tracker_class

#include "vr_tracker.h"
#include "openvr_string_std.h"
#include "tracker_test_context.h"


void test_tracker_class()
{
	using namespace openvr_string;
	using namespace vr;
	using namespace vr_result;
	using std::range;

	log_printf("starting test_tracker_class\n");
	
	vr_tracker a;
	vr_tracker b(a);
	vr_tracker c;
	c = a;
	assert(a == b);
	assert(a == c);
}
