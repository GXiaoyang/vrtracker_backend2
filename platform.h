#pragma once 
#include <thread>
#include <chrono>

namespace plat
{
	static void sleep_ms(unsigned long ms)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}
}
