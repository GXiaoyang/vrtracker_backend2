#pragma once
#include <platform.h>
#include <cstdint>
#include <memory.h>
#include <assert.h>

struct BaseStream
{
	virtual uint64_t get_pos() const = 0;
	virtual void set_pos(uint64_t) = 0;
	virtual void reset_buf_pos() = 0;
	virtual void write_to_stream(const void *src, size_t s) = 0;
	virtual void read_from_stream(void *dest, size_t s) = 0;

	template <typename Container>
	void contiguous_container_out_to_stream(const Container &container)
	{
		int size = size_as_int(container.size());	// dont' use size_t because it's different on 32 vs 64 bit
		write_to_stream(&size, sizeof(size));
		write_to_stream(container.data(), size * sizeof(*container.data()));
	}

	template <typename Container>
	void contiguous_container_from_stream(Container &container)
	{
		int size;
		read_from_stream(&size, sizeof(size));
		container.resize(size);
		// until c++17 basic_string doesn't have a non-const data().  so use at:
		if (size > 0)
		{
			read_from_stream(&container.at(0), size * sizeof(*container.data()));
		}
	}

	template <typename Container>
	void forward_container_out_to_stream(const Container &container)
	{
		int size = size_as_int(container.size());	// don't store the result in a size_t because it's different on 32 vs 64 bit
		write_to_stream(&size, sizeof(size));
		for (auto & val : container)
		{
			write_to_stream(&val, sizeof(val));
		}
	}

	template <typename Container>
	void forward_container_from_stream(Container &container)
	{
		int size;
		read_from_stream(&size, sizeof(size));
		container.clear();
		container.reserve(size);
		for (int i = 0; i < size; i++)
		{
			typename Container::value_type val;
			read_from_stream(&val, sizeof(val));
			container.push_back(val);
		}
	}
};
