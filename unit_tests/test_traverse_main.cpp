//
// test things that traverse the schema:
//	serialization and update
//
#include "log.h"

extern void UPDATE_USE_CASE();
extern void test_tracker_serialization();

void test_traverse()
{
	UPDATE_USE_CASE();
	test_tracker_serialization();
}

#ifdef TEST_TRAVERSE_MAIN
int main()
{
	test_traverse();
	log_printf("test_vr_tracker tests done\n");
}
#endif

