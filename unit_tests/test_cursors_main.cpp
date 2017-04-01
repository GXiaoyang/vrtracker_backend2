#include "log.h"
extern void TEST_SYSTEM_CURSOR();

void test_cursors()
{
	TEST_SYSTEM_CURSOR();	// just the 'system' node of vr
}

#ifdef TEST_CURSORS_MAIN
int main()
{
	test_cursors();
	log_printf("test_capture tests done\n");
}
#endif

