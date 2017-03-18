#include "time_containers.h"
#include "segmented_list.h"
#include "result.h"

template <template <typename, typename> class Container>
void test_ops_on_container()
{
	time_indexed_vector<Result<int, bool>, Container, std::allocator> v;

	v.emplace_back(3, 10, true);
	assert(v.latest().get_time_index() == 3);
	assert(v().val == 10);
	assert(v.latest().get_value().val == 10);
	assert(v.latest().get_value().return_code == true);
	assert(v.earliest().get_value().val == 10);
	assert(v.earliest().get_value().return_code == true);
	v.emplace_back(4, 20, false);
	assert(v.earliest().get_time_index() == 3);
	assert(v.earliest().get_value().val == 10);
	assert(v.earliest().get_value().return_code == true);
	assert(v.latest().get_time_index() == 4);
	assert(v.latest().get_value().val == 20);
	assert(v.latest().get_value().return_code == false);

	auto range = v.get_range();
	assert(range.begin()->get_time_index() == 3);
	assert(range.front().get_time_index() == 3);

	assert((range.end() - 1)->get_time_index() == 4);
	assert(range.back().get_time_index() == 4);
	for (int i = 0; i < 1024; i++)
	{
		v.emplace_back(4 + i, 10000 + i, true);
	}
	assert(v.size() == 1024 + 2);

}

template <template <typename, typename> class Container>
void test_last_item_query_cache()
{
	// see that cached results return the same result as uncached results
	time_indexed_vector<Result<int, bool>, Container, std::allocator> v;
	for (time_index_t time_index = 0; time_index < 1000; ++time_index)
	{
		v.emplace_back(time_index, 1,true);
	}

	std::vector<time_index_t> tests = { 0,1,1000,1001, 1000, 1, 0 };
	for (int i = 0; i < 50; i++)
	{
		tests.push_back(rand() % 1001);
	}

	auto cached_iter = v.end();
	for (time_index_t test : tests)
	{
		auto uncached = v.last_item_less_than_or_equal_to_time(test);
		cached_iter = v.last_item_less_than_or_equal_to_time(test, cached_iter);
		assert(uncached->get_time_index() == cached_iter->get_time_index());
	}

}

void TEST_TIME_CONTAINERS()
{
	time_indexed<std::string> a(3, "ha");
	time_indexed<std::string> b;
	b = a;
	assert(a == b);
	time_indexed<Result<int, bool>> r;

	test_ops_on_container<std::vector>();
	test_ops_on_container<segmented_list_1024>();

	test_last_item_query_cache<std::vector>();
	test_last_item_query_cache<segmented_list_1024>();

}
