#include "slab_allocator.h"
#include "log.h"

extern void TEST_SEGMENTED_LIST();
extern void TEST_RESULT();
extern void TEST_SLAB_ALLOCATOR();

void test_base()
{
	TEST_RESULT();
	TEST_SLAB_ALLOCATOR();
	TEST_SEGMENTED_LIST();
}

#ifdef TEST_BASE_MAIN
int main()
{
	test_base();
	log_printf("test_base_main tests done\n");
}
#endif
