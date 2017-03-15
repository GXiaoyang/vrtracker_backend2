#pragma once
#include "openvr.h"
#include "base_serialization.h"
// don't define this or pay the price of crazy (implicit) encodings!
template <typename T>
static void encode(T v, EncodeStream &e);

#define ENCODE(type_name) template <> \
void encode<type_name>(type_name v, EncodeStream &e) { e.memcpy_out_to_stream(&v, sizeof(v)); }


ENCODE(unsigned int);
ENCODE(int);
ENCODE(vr::EVRApplicationError);
ENCODE(vr::EVRRenderModelError);
ENCODE(bool);
ENCODE(vr::EVRTrackedCameraError);
ENCODE(vr::EVRCompositorError);
ENCODE(vr::ETrackedPropertyError);
ENCODE(vr::EVRSettingsError);
ENCODE(uint64_t);
ENCODE(vr::EVROverlayError);




template <>
void encode<const char *>(const char *v, EncodeStream &e)
{
	int size = (int)strlen(v) + 1;
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

inline void decode_str(char *str, EncodeStream &e)
{
	// read the size of the string
	int size;
	decode(size, e);

	// read it into the string
	e.memcpy_from_stream(str, size);
}


inline void decode(vr::HmdMatrix34_t &v, EncodeStream &e)
{
	e.memcpy_from_stream(&v, sizeof(v));
}

template <typename StringVectorType>
inline void write_string_vector_to_stream(EncodeStream &s, StringVectorType &v)
{
	uint32_t count = (uint32_t)v.size();
	encode(count, s);

	for (int i = 0; i < (int)count; i++)
	{
		encode(v[i].c_str(), s);
	}
}

template <typename StringVectorType>
inline void read_string_vector_from_stream(EncodeStream &s, StringVectorType &v)
{
	uint32_t count;
	decode(count, s);
	v.reserve(count);
	for (int i = 0; i < (int)count; i++)
	{
		char szBuf[256];
		decode_str(szBuf, s);
		v.emplace_back(szBuf);
	}
}

inline void write_int_vector_to_stream(EncodeStream &s, std::vector<int> &v)
{
	uint32_t count = (uint32_t)v.size();
	encode(count, s);

	for (int i = 0; i < (int)count; i++)
	{
		encode(v[i], s);
	}
}

inline void read_int_vector_from_stream(EncodeStream &s, std::vector<int> &v)
{
	uint32_t count;
	decode(count, s);
	v.reserve(count);
	for (int i = 0; i < (int)count; i++)
	{
		int x;
		decode(x, s);
		v.emplace_back(x);
	}
}

