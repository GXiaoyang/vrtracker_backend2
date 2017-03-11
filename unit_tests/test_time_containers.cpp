#include "time_containers.h"

void TEST_TIME_CONTAINERS()
{
	time_indexed<std::string> a(3, "ha");
	time_indexed<std::string> b;
	b = a;
	assert(a == b);


	
}
