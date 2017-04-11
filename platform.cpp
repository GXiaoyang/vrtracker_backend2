#include "platform.h"
#include <thread>

void plat::sleep_ms(unsigned long ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::string plat::make_temporary_filename(const std::string &key)
{
#ifdef _WIN32
	return std::string("c:\\temp\\" + key);
#else
	return std::string("/tmp/" + key);
#endif
};

std::string plat::make_temporary_filename()
{
	return(std::tmpnam(nullptr));
}
