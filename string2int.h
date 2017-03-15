#pragma once
#include <unordered_map>

// needs to be tested  - ie I don't think it really needs to go though
// the entire string to build a seed

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

#if 0
struct hash_c_string {
	void hash_combine(uint32_t& seed, char v) const {
		seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	uint32_t operator() (const char * p) const {
		uint32_t hash = 0;
		for (; *p; ++p)
			hash_combine(hash, *p);
		return hash;
	}
};
#endif
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

