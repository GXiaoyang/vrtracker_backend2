// 
// serialization to memory routines.  A block of memory and a counter.
//
#pragma once
#include "BaseStream.h"
#include <cstdint>

struct MemoryStream : public BaseStream
{
	MemoryStream(char *buf, uint64_t buf_size, bool count_only_in)
		:
		buf_size(buf_size),
		encoded_buf(buf),
		buf_pos(0),
		count_only(count_only_in)
	{}

	MemoryStream(MemoryStream &) = delete;

	uint64_t get_pos() const override
	{
		return buf_pos;
	}

	void set_pos(uint64_t new_pos) override
	{
		buf_pos = new_pos;
	}

	void reset_buf_pos() override
	{
		buf_pos = 0;
	}

	// write value to buf and advance pointer
	void write_to_stream(const void *src, size_t s) override
	{
		if (!count_only)
		{
			assert(buf_pos + static_cast<int>(s) < buf_size + 1); // buffer is too small to perform the copy
			::memcpy(&encoded_buf[buf_pos], src, (int)s);
		}
		buf_pos += (int) s;
	}

	// write internal value out to stream and advance pointer
	void read_from_stream(void *dest, size_t s) override
	{
		assert(buf_pos + (int)s < buf_size + 1);	 // buf_pos can refer to the element after the last one
		::memcpy(dest, &encoded_buf[buf_pos], (int)s);
		buf_pos += (int)s;
	}
	const uint64_t buf_size;
	char *encoded_buf;
	uint64_t buf_pos;
	bool count_only;
};

