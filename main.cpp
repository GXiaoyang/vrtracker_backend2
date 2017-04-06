#include "slab_allocator.h"
#include "log.h"

extern void test_base();
extern void test_time_containers();
extern void test_keys();
extern void test_capture();
extern void test_traverse();
extern void test_cursors();
extern void test_controller();
extern void test_monitor();

int main()
{
	test_monitor();

	test_controller();
	test_base();
	test_time_containers();
	test_keys();
	test_capture();
	test_traverse();
	test_cursors();
	log_printf("all tests done\n");
}
