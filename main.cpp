#include "slab_allocator.h"
#include "log.h"

slab my_heap(1024 * 1024);
slab my_temp_heap(1024 * 1024);


extern void base_unit_tests();
extern void time_containers_unit_tests();
extern void vr_keys_unit_tests();
extern void vr_tracker_unit_test();

extern void UPDATE_USE_CASE();
extern void TEST_SYSTEM_CURSOR();

void unit_tests()
{
	TEST_SYSTEM_CURSOR();
}

int main()
{
	base_unit_tests();
	time_containers_unit_tests();
	vr_tracker_unit_test();
	UPDATE_USE_CASE();
	log_printf("all tests done\n");
}
