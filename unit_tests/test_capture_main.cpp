// GUI usecase

#include "log.h"

extern void test_capture_class();
extern void GUI_USE_CASE_TEST();

void test_capture()
{
	test_capture_class();
	GUI_USE_CASE_TEST();
}

#ifdef TEST_capture_MAIN
int main()
{
	test_capture();
	log_printf("test_capture tests done\n");
}
#endif

