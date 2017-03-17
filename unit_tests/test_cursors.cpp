#include "vr_system_cursor.h"
#include "tracker_test_context.h"
#include "vr_cursor_context.h"

// test that the cursor never reads data from a future timeframe
static void test_overread()
{
	tracker_test_context tracker;
	CursorContext cursor(&tracker.get_tracker());
	VRSystemCursor system(&cursor);
	cursor.ChangeFrame(0);
	uint32_t width;
	uint32_t height;
//	system.GetRecommendedRenderTargetSize(&width, &height);
}


// test that multiple cursors can run simultaneously
// test that seek time is linear or better
void TEST_SYSTEM_CURSOR()
{
	test_overread();
	

}
