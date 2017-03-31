// GUI usecase

#include "log.h"

extern void test_tracker_class();
extern void GUI_USE_CASE_TEST();

void test_tracker()
{
	test_tracker_class();
	GUI_USE_CASE_TEST();
}

#ifdef TEST_VR_TRACKER_MAIN
int main()
{
	test_tracker();
	log_printf("test_vr_tracker tests done\n");
}
#endif

