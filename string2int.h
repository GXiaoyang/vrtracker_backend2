#pragma once
#include <unordered_map>

// genereate a hash from a c string.  this function came from some tbb sample code and beat out 
// another stack overflow one
struct hash_c_string
{
	size_t operator() (const char *s) const
	{
		size_t h = 0;
		for (; *s; ++s)
			h = (h * 17) ^ *s;
		return h;
	}
};

struct comp_c_string {
	bool operator()(const char * p1, const char * p2) const {
		return strcmp(p1, p2) == 0;
	}
};
typedef std::unordered_map<
	const char *,
	uint32_t,
	hash_c_string,
	comp_c_string
> string2int;

