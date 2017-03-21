// GUI usecase

#include "log.h"

extern void GUI_USE_CASE_TEST();

void test_tracker()
{
	GUI_USE_CASE_TEST();
}

#ifdef TEST_VR_TRACKER_MAIN
int main()
{
	test_tracker();
	log_printf("test_vr_tracker tests done\n");
}
#endif

