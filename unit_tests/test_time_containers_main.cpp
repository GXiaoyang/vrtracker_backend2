#include "log.h"

extern void TEST_TIME_CONTAINERS();
extern void TEST_SCHEMA_COMMON();

void time_containers_unit_tests()
{
	TEST_TIME_CONTAINERS();
	TEST_SCHEMA_COMMON();
}

#ifdef TEST_TIME_CONTAINERS_MAIN
int main()
{
	time_containers_unit_tests();
	log_printf("test_time_containers tests done\n");
}
#endif
