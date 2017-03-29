// 
// serialization to memory routines.  A block of memory and a counter.
//
#pragma once
#include <memory.h>
#include <assert.h>

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

	template <typename Container> 
	void contiguous_container_out_to_stream(const Container &container)
	{
		int size = container.size();	// dont' use size_t because it's different on 32 vs 64 bit
		memcpy_out_to_stream(&size, sizeof(size));
		memcpy_out_to_stream(container.data(), size * sizeof(*container.data()));
	}

	template <typename Container>
	void contiguous_container_from_stream(Container &container)
	{
		int size;
		memcpy_from_stream(&size, sizeof(size));
		container.resize(size);
		// until c++17 basic_string doesn't have a non-const data().  so use at:
		if (size > 0)
		{
			memcpy_from_stream(&container.at(0), size * sizeof(*container.data()));
		}
	}

	template <typename Container>
	void forward_container_out_to_stream(const Container &container)
	{
		int size = container.size();	// dont' use size_t because it's different on 32 vs 64 bit
		memcpy_out_to_stream(&size, sizeof(size));
		for (auto & val : container)
		{
			memcpy_out_to_stream(&val, sizeof(val));
		}
	}

	template <typename Container>
	void forward_container_from_stream(Container &container)
	{
		int size;
		memcpy_from_stream(&size, sizeof(size));
		container.clear();
		container.reserve(size);
		for (int i = 0; i < size; i++)
		{
			typename Container::value_type val;
			memcpy_from_stream(&val, sizeof(val));
			container.push_back(val);
		}
	}

	// write value to buf and advance pointer
	void memcpy_out_to_stream(const void *src, size_t s)
	{
		if (!count_only)
		{
			assert(buf_pos + (int)s < buf_size + 1); // buffer is too small to perform the copy
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
	const uint64_t buf_size;
	char *encoded_buf;
	uint64_t buf_pos;
	bool count_only;
	
};

