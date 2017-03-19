//
// system wide definitions and typedefs
//
#pragma once 
#include <thread>
#include <chrono>
#include <cassert>
#include <cstddef>
#include <limits>
#include <intrin.h>

namespace plat
{
	static void sleep_ms(unsigned long ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}

typedef uint64_t time_stamp_t;
typedef int time_index_t;


template <typename T>
constexpr int size_as_int(const T &size_in) {
	return static_cast<int>(size_in);
}

inline uint64_t rdtsc() {
	return __rdtsc();
}

#define TBL_SIZE(t) (sizeof(t)/sizeof(t[0]))

template <typename T>
inline T CLAMP(T min, T max, T sample)
{
	if (sample < min)
		return min;
	if (sample > max)
		return max;
	return sample;
}

