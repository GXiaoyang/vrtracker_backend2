#pragma once
#include "openvr.h"
#include <memory.h>
#include <assert.h>
#include <vector>

struct EncodeStream
{
	EncodeStream(char *buf, uint64_t buf_size, bool count_only_in)
		:
		buf_size(buf_size),
		encoded_buf(buf),
		buf_pos(0),
		count_only(count_only_in)
	{}

	EncodeStream(EncodeStream &) = delete;

	void reset_buf_pos()
	{
		buf_pos = 0;
	}

	// write value to buf and advance pointer
	void memcpy_out_to_stream(const void *src, size_t s)
	{
		if (!count_only)
		{
			assert(buf_pos + (int)s < buf_size + 1);
			::memcpy(&encoded_buf[buf_pos], src, (int)s);
		}
		buf_pos += (int) s;
	}

	// write internal value out to stream and advance pointer
	void memcpy_from_stream(void *dest, size_t s)
	{
		assert(buf_pos + (int)s < buf_size + 1);	 // buf_pos can refer to the element after the last one
		::memcpy(dest, &encoded_buf[buf_pos], (int)s);
		buf_pos += (int)s;
	}
	uint64_t buf_size;
	char *encoded_buf;
	uint64_t buf_pos;
	bool count_only;
	
};

// don't define this or pay the price of crazy (implicit) encodings!
template <typename T>
static void encode(T v, EncodeStream &e);

#define ENCODE(type_name) template <> void encode<type_name>(type_name v, EncodeStream &e) { e.memcpy_out_to_stream(&v, sizeof(v)); }

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

inline void write_string_vector_to_stream(EncodeStream &s, std::vector<std::string> &v)
{
	uint32_t count = (uint32_t)v.size();
	encode(count, s);

	for (int i = 0; i < (int)count; i++)
	{
		encode(v[i].c_str(), s);
	}
}

inline void read_string_vector_from_stream(EncodeStream &s, std::vector<std::string> &v)
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

