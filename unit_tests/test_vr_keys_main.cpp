#include "log.h"

extern void test_app_indexer();
extern void test_texture_indexer();

void test_keys()
{
	test_texture_indexer();
	test_app_indexer();
}

#ifdef TEST_VR_KEYS_MAIN
int main()
{
	test_keys();
	log_printf("test_base_main tests done\n");
}
#endif
