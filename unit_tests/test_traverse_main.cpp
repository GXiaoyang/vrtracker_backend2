//
// test things that traverse the schema:
//	serialization and update
//
#include "log.h"

extern void UPDATE_USE_CASE();
extern void test_capture_serialization();

void test_traverse()
{
	test_capture_serialization();
	UPDATE_USE_CASE();
}

#ifdef TEST_TRAVERSE_MAIN
int main()
{
	test_traverse();
	log_printf("test_capture tests done\n");
}
#endif

