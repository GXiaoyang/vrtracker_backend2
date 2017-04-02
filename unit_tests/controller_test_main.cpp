//
// test things that traverse the schema:
//	serialization and update
//
#include "log.h"

extern void test_controller();

#ifdef TEST_CONTROLLER_MAIN
int main()
{
	log_printf("test_controller start\n");
	test_controller();
	log_printf("test_controller done\n");
}
#endif

