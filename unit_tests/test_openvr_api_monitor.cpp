//
// test things that traverse the schema:
//	serialization and update
//
#include "log.h"

extern void test_dll_client();
extern void test_openvr_bridge();

void test_monitor()
{
	test_openvr_bridge();
	test_dll_client();
}

#ifdef TEST_MONITOR_MAIN
int main()
{
	
	log_printf("test_monitor tests done\n");
}
#endif

