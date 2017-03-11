#pragma once

//
// Utility functions to deal with ranges
//

template <typename I, typename T>
I last_item_less_than_or_equal_to(I begin, I end, const T& val)
{
	I it = upper_bound(begin, end, val);

	if (it != begin) {
		it--; // not at end of array so rewind to previous item
	}
	else {
		it = end; // no items before this point, so return end()
	}
	return it;
}

template <typename I, typename T, class _Pr>
I last_item_less_than_or_equal_to(I begin, I end, const T& val, _Pr pr)
{
	I it = upper_bound(begin, end, val, pr);

	if (it != begin) {
		it--; // not at end of array so rewind to previous item
	}
	else {
		it = end; // no items before this point, so return end()
	}
	return it;
}

template <typename I, typename T, class _Pr>
I first_item_greater_than_or_equal_to(I begin, I end, T val, _Pr pr)
{
	I it = upper_bound(begin, end, val, pr);

	if (it != begin) {
		T found = *(it - 1);
		if (found == val)
			it--;
	}
	return it;
}

template <typename R, typename T, class _Pr>
R range_intersect(const R &r, const T& min_val, const T&max_val, _Pr pr)  // [min_val and max_val) are not inclusive
{
	if (r.empty() || min_val >= max_val)		// check 1: is the range empty
		return R(r.end(), r.end());
	if (min_val > r.back())
		return R(r.end(), r.end()); // check 2: minimum is outside the values of the range
	if (max_val < r.front())
		return R(r.end(), r.end()); // check 3: maximum is outside the values of the range

	auto first = first_item_greater_than_or_equal_to(r.begin(), r.end(), min_val, pr);
	assert(first != r.end());	// can't happen because of checks 2 and 3
	if (*first >= max_val)
		return R(r.end(), r.end());

	auto second = upper_bound(first, r.end(), max_val, pr);
	return R(first, second);
}

template <typename R>
struct range_intersect_result
{
private:
	int query_min_val;
	int query_max_val;
	R query_range;
	R result_range;
public:
	range_intersect_result()
		: query_max_val(-1)
	{}

	bool different_query(const R &r_in, int min_val_in, int max_val_in)
	{
		if (query_min_val != min_val_in)
			return true;
		if (query_max_val != max_val_in)
			return true;
		if (query_range.begin() != r_in.begin())
			return true;
		if (query_range.end() != r_in.end())
			return true;
		return false;
	}
	template <class _Pr>
	R intersect(const R &r, int min_val, int max_val, _Pr pr)
	{
		if (different_query(r, min_val, max_val))
		{
			query_min_val = min_val;
			query_max_val = max_val;
			query_range = r;
			result_range = range_intersect(r, min_val, max_val, pr);
		}
		return result_range;
	}
};

