#include "slab_allocator.h"
#include "log.h"

extern void test_base();
extern void test_time_containers();
extern void test_keys();
extern void test_tracker();
extern void test_traverse();

extern void TEST_SYSTEM_CURSOR();

void unit_tests()
{
	TEST_SYSTEM_CURSOR();
}

int main()
{
	test_base();
	test_time_containers();
	test_keys();
	test_tracker();
	test_traverse();
	TEST_SYSTEM_CURSOR();

	log_printf("all tests done\n");
}
