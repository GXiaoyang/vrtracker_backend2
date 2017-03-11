#pragma once

#include "url_named.h"
#include "range.h"
#include "range_algorithm.h"
#include "result.h"

typedef uint64_t time_stamp_t;
typedef int time_index_t;

template <typename T>
struct time_indexed
{
	time_indexed() {}

	template<typename... Args>
	time_indexed(time_index_t t, Args&&... args)
		: time_index(t),
		value(std::forward<Args>(args)...)
	{}

	const T& get_value() const { return value; }
	time_index_t get_time_index() const { return time_index; }

private:
	time_index_t time_index;
	T value;
};

template <typename T>
bool operator<(const time_indexed<T> &a, const time_indexed<T> &b)
{
	return a.time_index < b.time_index;
}

template <typename T>
bool operator>(const time_indexed<T> &a, const time_indexed<T> &b)
{
	return a.time_index > b.time_index;
}


template <typename T>
bool operator>=(const time_indexed<T> &a, const time_indexed<T> &b)
{
	return a.time_index >= b.time_index;
}

template <typename T>
bool operator==(const time_indexed<T> &a, const time_indexed<T> &b)
{
	return a.time_index == b.time_index; // TODO this was put here to get ranges to work, but I think it's not a good idea to define
	// this operator without considerint the entire object
}

template <	typename T,
	template <typename, typename> class Container,
	typename A = std::allocator<T>>
	struct time_indexed_vector : url_named
{
	typedef T									value_type;
	typedef time_indexed<T>						time_indexed_type;
	typedef Container<time_indexed_type, A>		container_type_t;
	typedef typename container_type_t::iterator iterator;

	template<typename... Args>
	time_indexed_vector(const URL &url, Args&&... args)
		:
		url_named(url),
		container(std::forward<Args>(args)...)
	{}

	bool empty() const { return container.empty(); }
	const T& operator()() const { return container.back().get_value(); }
	const time_indexed<T>& latest() const { return container.back(); }
	const time_indexed<T>& earliest() const { return container.back(); }

	// [start and end)  (IE ARE NOT inclusive of end)
	std::range<iterator> get_range(time_index_t a, time_index_t b)
	{
		time_indexed_type dummy_a(a);
		
		time_indexed_type dummy_b;
		dummy_b.time_index = b;

		return range_intersect(get_range(), dummy_a, dummy_b,
			[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
	}

	std::range<iterator> get_range()
	{
		return std::range<iterator>(container.begin(), container.end());
	}

	template<typename... Args>
	void emplace_back(time_index_t time_index, Args&&... args)
	{
		container.emplace_back(time_index, std::forward<Args>(args)...);
	}

	container_type_t container;
};
