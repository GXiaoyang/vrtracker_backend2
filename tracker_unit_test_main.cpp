// GUI usecase

#include "log.h"

extern void GUI_USE_CASE_TEST();

void vr_tracker_unit_test()
{
	GUI_USE_CASE_TEST();
}

#ifdef TEST_VR_TRACKER_MAIN
int main()
{
	vr_tracker_unit_test();
	log_printf("test_vr_tracker tests done\n");
}
#endif

