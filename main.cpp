#include "slab_allocator.h"
#include "log.h"

slab my_heap(1024 * 1024);
slab my_temp_heap(1024 * 1024);


extern void base_unit_tests();
extern void time_containers_unit_tests();

extern void GUI_USE_CASE();
extern void UPDATE_USE_CASE();


extern void TEST_INDEXERS();
extern void TEST_SYSTEM_CURSOR();

void unit_tests()
{
	TEST_SYSTEM_CURSOR();
	TEST_INDEXERS();
}

int main()
{
	base_unit_tests();
	time_containers_unit_tests();
	unit_tests();
	GUI_USE_CASE();
	UPDATE_USE_CASE();
	log_printf("all tests done\n");
}
