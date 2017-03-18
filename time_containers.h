#pragma once

#include "url_named.h"
#include "range.h"
#include "range_algorithm.h"

template <typename T>
struct time_indexed
{
	time_indexed() {}

	template<typename... Args>
	time_indexed(time_index_t t, Args&&... args)
		: time_index(t),
		value(std::forward<Args>(args)...)
	{}

	bool operator == (const time_indexed &rhs) const
	{
		return (time_index == rhs.time_index) && (value == rhs.value);
	}

	bool operator != (const time_indexed &rhs) const
	{
		return !(*this == rhs);
	}

	const T& get_value() const { return value; }
	time_index_t get_time_index() const { return time_index; }

private:
	time_index_t time_index;
	T value;
};

// time_indexed_vector:  a container of items wrapped in time_indexed<T>
//                       a pretty thin layer
template <	typename T,
	template <typename, typename> class Container,
	template <typename> typename A>
	struct time_indexed_vector : base::url_named
{
	typedef T									value_type;
	typedef time_indexed<T>						time_indexed_type;
	typedef Container<time_indexed_type, A<time_indexed_type>>	container_type_t;
	typedef typename container_type_t::iterator iterator;

//	time_indexed_vector()
//	{}

	template<typename... Args>
	time_indexed_vector(const base::URL &url = base::URL(), Args&&... args)
		:
		url_named(url),
		container(std::forward<Args>(args)...)
	{}

	bool empty() const { return container.empty(); }
	size_t size() const { return container.size(); }

	const T& operator()() const { return container.back().get_value(); }
	const time_indexed<T>& latest() const { return container.back(); }
	const time_indexed<T>& earliest() const { return container.front(); }

	// [start and end)  (half open range)
	std::range<iterator> get_range(time_index_t a, time_index_t b)
	{
		return range_intersect(get_range(), a, b,
			[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
	}

	std::range<iterator> get_range()
	{
		return std::range<iterator>(container.begin(), container.end());
	}

	iterator end()
	{
		return container.end();
	}

	iterator last_item_less_than_or_equal_to_time(time_index_t a)
	{
		return last_item_less_than_or_equal_to(container.begin(), container.end(), a,
			[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
	}

	// check the hint iterator first before searching for it
	iterator last_item_less_than_or_equal_to_time(time_index_t a, iterator &hint_iterator)
	{
		if (hint_iterator != end())
		{
			time_index_t hint_index = hint_iterator->get_time_index();
			if (hint_index == a)
			{
				return hint_iterator;
			}
			else if (hint_index < a)
			{
				return last_item_less_than_or_equal_to(hint_iterator, container.end(), a,
					[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
			}
			else // hint index > a
			{
				return last_item_less_than_or_equal_to(container.begin(), hint_iterator, a,
					[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
			}
		}

		return last_item_less_than_or_equal_to(container.begin(), container.end(), a,
			[](const time_indexed_type &a, const time_indexed_type &b) { return a.get_time_index() < b.get_time_index(); });
	}


	template<typename... Args>
	void emplace_back(time_index_t time_index, Args&&... args)
	{
		container.emplace_back(time_index, std::forward<Args>(args)...);
	}

	container_type_t container;
};
