#include "MemoryStream.h"
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

void test_serialization()
{
	{
		// time indexed vector
		typedef Result<std::vector<int>, NoReturnCode> int_result;
		time_indexed<int_result> a;
		a.set_time_index(11);

		char buf[1024];
		MemoryStream stream(buf, sizeof(buf), false);

		a.encode(stream);
		stream.reset_buf_pos();

		time_indexed<int_result> b;
		b.decode(stream);
		assert(a.get_time_index() == b.get_time_index());
		assert(a.get_value() == b.get_value());


		{
			time_indexed_vector<int_result, std::vector, std::allocator> veca;
			veca.push_back(a);
			stream.reset_buf_pos();
			veca.encode(stream);
			time_indexed_vector<int_result, std::vector, std::allocator> vecb;
			stream.reset_buf_pos();
			vecb.decode(stream);
			assert(veca == vecb);
		}
		{
			time_indexed_vector<int_result, segmented_list_1024, std::allocator> veca;

			veca.push_back(a);
			stream.reset_buf_pos();
			veca.encode(stream);
			time_indexed_vector<int_result, segmented_list_1024, std::allocator> vecb;
			stream.reset_buf_pos();
			vecb.decode(stream);
			assert(veca == vecb);
		}

		// add a name
		{
			time_indexed_vector<int_result, segmented_list_1024, std::allocator> veca(base::URL("foo", "/root/foo"));

			veca.push_back(a);
			stream.reset_buf_pos();
			veca.encode(stream);
			time_indexed_vector<int_result, segmented_list_1024, std::allocator> vecb;
			stream.reset_buf_pos();
			vecb.decode(stream);
			assert(veca == vecb);
		}
	}
}

void test_move_operators()
{
	time_indexed<std::string> a(time_indexed<std::string>(3, "ha"));
	time_indexed<std::string> b(a);
	const time_indexed<std::string> &c = b;
	time_indexed<std::string> d(c);

	const time_indexed <Result<int, bool>> e;
	time_indexed <Result<int, bool>> f(e);

	std::vector <time_indexed<Result<int, bool>>> s;
	s.reserve(22);

	//time_indexed < Result<int, bool>> g(e, );

//	const time_indexed <Result<int, bool>> e;

}

void TEST_TIME_CONTAINERS()
{
	test_move_operators();

	test_serialization();

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
