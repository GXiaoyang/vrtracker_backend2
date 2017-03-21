#include "log.h"

extern void TEST_INDEXERS();

void vr_keys_unit_tests()
{
	TEST_INDEXERS();
}

#ifdef TEST_VR_KEYS_MAIN
int main()
{
	vr_keys_unit_tests();
	log_printf("test_base_main tests done\n");
}
#endif
