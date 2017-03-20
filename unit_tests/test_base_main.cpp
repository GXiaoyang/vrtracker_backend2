#include "slab_allocator.h"
#include "log.h"

extern void TEST_SEGMENTED_LIST();
extern void TEST_RESULT();
extern void TEST_SLAB_ALLOCATOR();

void base_unit_tests()
{
	TEST_RESULT();
	TEST_SLAB_ALLOCATOR();
	TEST_SEGMENTED_LIST();
}

#ifdef TEST_BASE_MAIN
int main()
{
	base_unit_tests();
	log_printf("test_base_main tests done\n");
}
#endif
