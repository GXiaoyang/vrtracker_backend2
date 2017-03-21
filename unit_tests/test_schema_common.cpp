#include "schema_common.h"
#include "slab_allocator.h"
#include "segmented_list.h"
#include <assert.h>
#include <list>

template <template <typename> class Allocator>
using test_string_result = Result<
	std::basic_string<char, 
	std::char_traits<char>, Allocator<char>>, NoReturnCode>;

template <bool is_iterator>
struct test_schema : public schema<is_iterator>
{
	typedef Result<bool, NoReturnCode> bool_result;


	// define a scalar TIMENODE
	template <typename ResultType, template <typename> class Allocator>
	using TIMENODE = time_node<ResultType, std::list, is_iterator, Allocator>;

	template <typename ResultType, template <typename> class Allocator>
	using TIMENODESEG = time_node<ResultType, segmented_list_1024, is_iterator, Allocator>;

	struct sub_schema : schema<is_iterator>
	{
		sub_schema(const base::URL &name)
			: schema<is_iterator>(name)

			{}
		TIMENODE<bool_result, std::allocator>		a_bool;	// choose where the timenodes sit
		TIMENODE<bool_result, slab_allocator>		a_bool2;
		TIMENODESEG<bool_result, std::allocator>	a_bool3;
		TIMENODESEG<bool_result, slab_allocator>	a_bool4;

		TIMENODE<test_string_result<std::allocator>, std::allocator> a_string; // nodes and the string
		TIMENODE<test_string_result<slab_allocator>, slab_allocator> b_string; // nodes and the string

	};

	test_schema(const base::URL &name = base::URL())  
		: schema<is_iterator>(name),
		one(name.make_child("one"))
		{}
	
	sub_schema				one;
};

using test_state = test_schema<false>;

void TEST_SCHEMA_COMMON()
{
	slab s;
	slab_allocator_base::m_temp_slab = &s;

	// default construction
	test_state default_state;
	assert(default_state.get_name() == "");
	assert(default_state.one.a_bool.empty());
	default_state.one.a_bool.emplace_back(1, true);
	assert(!default_state.one.a_bool.empty());

	assert(default_state.one.a_bool2.empty());
	default_state.one.a_bool2.emplace_back(1, true);
	assert(!default_state.one.a_bool2.empty());
	assert(default_state.one.a_bool2.container.get_allocator().m_slab == &s);


	assert(default_state.one.a_bool3.empty());
	default_state.one.a_bool3.emplace_back(1, true);
	assert(!default_state.one.a_bool3.empty());

	assert(default_state.one.a_bool4.empty());
	default_state.one.a_bool4.emplace_back(1, true);
	assert(!default_state.one.a_bool4.empty());
	assert(default_state.one.a_bool4.container.get_allocator().m_slab == &s);

	
	default_state.one.a_string.emplace_back(1, "hi fred");
	assert(!default_state.one.a_string.empty());
	assert(default_state.one.a_string.latest().get_value().val == "hi fred");

	default_state.one.b_string.emplace_back(1, "hi fred");
	assert(!default_state.one.b_string.empty());
	assert(default_state.one.b_string.latest().get_value().val == "hi fred");

#if 0
	// slab allocator
	slab s;
	slab_allocator_base::m_temp_slab = &s;
	test_state<slab_allocator<>> state_with_allocator;
	assert(state_with_allocator.one.a_bool.container.get_allocator().m_slab == &s);
	assert(state_with_allocator.one.a_string.container.get_allocator().m_slab == &s);
	assert(state_with_allocator.get_name() == "");
	assert(state_with_allocator.one.a_bool.empty());
	state_with_allocator.one.a_bool.emplace_back(1, true);
	assert(!state_with_allocator.one.a_bool.empty());
	assert(state_with_allocator.one.a_bool.latest().get_value().val == true);
	assert(state_with_allocator.one.a_string.empty());
	state_with_allocator.one.a_string.emplace_back(1, "hi fred");
	assert(!state_with_allocator.one.a_string.empty());
	assert(state_with_allocator.one.a_string.latest().get_value().val == "hi fred");
	assert(state_with_allocator.one.a_string.latest().get_value().val.get_allocator().m_slab == &s);
#endif

}