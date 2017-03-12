#include "time_containers.h"
#include "segmented_list.h"

template <template <typename, typename> class Container>
void test_ops_on_container()
{
	time_indexed_vector<Result<int, bool>, Container, std::allocator> v;
#if 0
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
#endif
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
}
