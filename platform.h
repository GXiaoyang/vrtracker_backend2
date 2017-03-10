#pragma once 
#include <thread>
#include <chrono>
#include <cassert>
#include <cstddef>
#include <limits>

namespace plat
{
	static void sleep_ms(unsigned long ms)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}
}


template <typename T>
constexpr int size_as_int(const T &size_in) {
	assert(size_in <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
	return static_cast<int>(size_in);
}