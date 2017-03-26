#pragma once
#include "platform.h"
#include "openvr.h"
#include "base_serialization.h"
#include <string.h>
#include <vector>

template <typename T>
static void encode(T v, EncodeStream &e);

#define ENCODE(type_name) template <> \
void encode<type_name>(type_name v, EncodeStream &e) { e.memcpy_out_to_stream(&v, sizeof(v)); }

ENCODE(unsigned int);
ENCODE(int);
ENCODE(vr::VREvent_t);

template <>
void encode<const char *>(const char *v, EncodeStream &e)
{
	int size = size_as_int(strlen(v)) + 1;
	encode(size, e);
	e.memcpy_out_to_stream(v, size);
}

template <typename T>
void decode(T &v, EncodeStream &e)
{
	e.memcpy_from_stream(&v, sizeof(v));
}

template <typename U>
void decode(const char *&str, EncodeStream &e, U allocator)
{
	// read the size of the string
	int size;
	decode(size, e);

	// make space for it
	char *buf = allocator.allocate(size);
	// read it into the string
	e.memcpy_from_stream(buf, size);
	str = buf;
}


inline void decode(vr::HmdMatrix34_t &v, EncodeStream &e)
{
	e.memcpy_from_stream(&v, sizeof(v));
}

template <typename StringVectorType>
inline void write_string_vector_to_stream(EncodeStream &s, StringVectorType &v)
{
	int count = size_as_int(v.size());
	encode(count, s);

	for (int i = 0; i < count; i++)
	{
		encode(v[i].c_str(), s);
	}
}

template <typename StringVectorType>
inline void read_string_vector_from_stream(EncodeStream &s, StringVectorType &v)
{
	int count;
	decode(count, s);
	v.reserve(count);

	std::string tmp;
	for (int i = 0; i < count; i++)
	{
		s.contiguous_container_from_stream(tmp);
		v.emplace_back(tmp);
	}
}

inline void write_int_vector_to_stream(EncodeStream &s, std::vector<int> &v)
{
	int count = size_as_int(v.size());
	encode(count, s);

	for (int i = 0; i < count; i++)
	{
		encode(v[i], s);
	}
}

inline void read_int_vector_from_stream(EncodeStream &s, std::vector<int> &v)
{
	int count;
	decode(count, s);
	v.reserve(count);
	for (int i = 0; i < count; i++)
	{
		int x;
		decode(x, s);
		v.emplace_back(x);
	}
}

