// 
// serialization to memory routines.  A block of memory and a counter.
//
#pragma once
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

