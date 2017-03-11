#include "slab_allocator.h"

slab my_heap(1024 * 1024);
slab my_temp_heap(1024 * 1024);

extern void GUI_USE_CASE();
extern void UPDATE_USE_CASE();

extern void TEST_SEGMENTED_LIST();
extern void TEST_RESULT();
extern void TEST_TIME_CONTAINERS();
void unit_tests()
{
	TEST_TIME_CONTAINERS();
	TEST_RESULT();
	TEST_SEGMENTED_LIST();
	
}

int main()
{
	unit_tests();
	GUI_USE_CASE();
	UPDATE_USE_CASE();
}
