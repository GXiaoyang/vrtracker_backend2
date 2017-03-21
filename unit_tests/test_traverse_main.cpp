// GUI usecase

#include "log.h"

extern void UPDATE_USE_CASE();

void test_traverse()
{
	UPDATE_USE_CASE();
}

#ifdef TEST_TRAVERSE_MAIN
int main()
{
	test_traverse();
	log_printf("test_vr_tracker tests done\n");
}
#endif

