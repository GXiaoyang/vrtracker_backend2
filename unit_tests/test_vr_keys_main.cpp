#include "log.h"

extern void TEST_INDEXERS();

void test_keys()
{
	TEST_INDEXERS();
}

#ifdef TEST_VR_KEYS_MAIN
int main()
{
	test_keys();
	log_printf("test_base_main tests done\n");
}
#endif
