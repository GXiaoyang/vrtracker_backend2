// test_result
// * unit test for result
//

#include "segmented_list.h"
#include "result.h"
#include <assert.h>
#include <deque>
#include <gsl.h>

static void test_type_checks()
{
	int a[11];
	gsl::span<int> ii{ a };
	gsl::span<int> ii2(a);

	gsl::span<int> b((int *)malloc(sizeof(int) * 1000), 1000);

	assert(b.size() == 1000);
	free(b.data());

	tmp_vector_pool<1024> my_pool;
	typedef tmp_vector<char, std::allocator<char>, 1024> tmp_string;

	tmp_string v(&my_pool, std::allocator<char>());
	static_assert(detail::SupportsData<tmp_string>::value, "has data");
	static_assert(Result<tmp_string, NoReturnCode>::value_is_container, "Result knows tmp_strings are containers");
	static_assert(Result<gsl::span<int>, NoReturnCode>::value_is_container, "Result knows that spans are containers");
	static_assert(Result<std::vector<int>, NoReturnCode>::value_is_container, "Result knows that vectors are containers");

	// uncomment to test
	//	static_assert(!Result<segmented_list<int, 1024>, NoReturnCode>::value_is_container, 
	//							"Result knows that segmented_lists are not contiguous containers");

	// uncomment to test
	//	static_assert(!Result<std::deque<int>, NoReturnCode>::value_is_container,
	//		"Result knows that deque are not contiguous containers");

	// uncomment to test 
	//	Result<std::deque<int>, NoReturnCode> x;

	Result<std::string, NoReturnCode> x;

	static_assert(!std::is_pod<std::string>::value, "bla");
	static_assert(!Result<int[11], NoReturnCode>::value_is_container, "Result knows arrays are NOT containers");

}

// test copies of similar result types
void test_copies()
{
	tmp_vector_pool<1024> my_pool;
	typedef tmp_vector<int, std::allocator<int>, 1024> tmp_ints;

	{
		Result<std::vector<int>, NoReturnCode> a;
		a.val = { 1,2,3 };

		tmp_ints tmp(&my_pool, std::allocator<char>());
		Result<tmp_ints, NoReturnCode> b(tmp_ints(&my_pool, std::allocator<char>()));
		assert(b != a);

		b = a;

		assert(b.val[0] == 1);
		assert(b.val[1] == 2);
		assert(a.val.size() == b.val.size());
		assert(a == b);
	
	}

	{
		Result<tmp_ints, NoReturnCode> a(tmp_ints(&my_pool, std::allocator<char>()));
		a.val.resize(3);
		a.val[0] = 4;
		a.val[1] = 5;
		a.val[2] = 6;
		Result<std::vector<int>, NoReturnCode> b(a);
		assert(b.val[0] == 4);
		assert(b.val[1] == 5);
		assert(b.val[2] == 6);
	}
}
void TEST_RESULT()
{
	test_copies();
	test_type_checks();
}

