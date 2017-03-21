#include "log.h"

extern void TEST_TIME_CONTAINERS();
extern void TEST_SCHEMA_COMMON();

void test_time_containers()
{
	TEST_TIME_CONTAINERS();
	TEST_SCHEMA_COMMON();
}

#ifdef TEST_TIME_CONTAINERS_MAIN
int main()
{
	test_time_containers();
	log_printf("test_time_containers tests done\n");
}
#endif
