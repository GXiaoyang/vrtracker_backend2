// capture_class

#include "capture.h"
#include "openvr_string_std.h"
#include "capture_test_context.h"


void test_capture_class()
{
	using namespace openvr_string;
	using namespace vr;
	using namespace vr_result;
	using std::range;

	log_printf("starting test_capture_class\n");
	
	capture a;
	capture b(a);
	capture c;
	c = a;
	assert(a == b);
	assert(a == c);
}
