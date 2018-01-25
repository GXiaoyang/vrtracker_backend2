//
// system wide definitions and typedefs
//
#pragma once 
#include <string>
#include <chrono>
#include <cassert>
#include <cstddef>
#include <limits>

namespace plat
{
	void sleep_ms(unsigned long ms);
	std::string make_temporary_filename(const std::string &key);
	std::string make_temporary_filename();
	
};

using time_point_t = std::chrono::steady_clock::time_point;

typedef uint64_t time_stamp_t;
typedef int time_index_t;
using serialization_id = uint16_t;

template <typename T>
constexpr int size_as_int(const T &size_in) {
	return static_cast<int>(size_in);
}

template <typename T>
constexpr int size_as_uint32(const T &size_in) {
	return static_cast<uint32_t>(size_in);
}

constexpr int ptrdiff_as_int(const ptrdiff_t &size_in) {
	return static_cast<int>(size_in);
}

template <typename T>
constexpr int size_as_serialization_id(const T &size_in) {
	return static_cast<serialization_id>(size_in);
}

#include <stdint.h>

//  Windows
#ifdef _WIN32
#include <intrin.h>
inline uint64_t rdtsc() {
	return __rdtsc();
}
//  Linux/GCC
#else
inline uint64_t rdtsc() {
	unsigned int lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
}

#define vsprintf_s  vsprintf
#define fopen_s(fd, path, mode) int((*fd = fopen(path, mode)) != nullptr)
#define _vsnprintf_s(buffer, size, fmt, ap)  vsnprintf(buffer, size, fmt, ap)

#endif  

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

