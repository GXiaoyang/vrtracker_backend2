#include "schema_common.h"
#include "slab_allocator.h"
#include "segmented_list.h"
#include "MemoryStream.h"
#include <assert.h>
#include <list>

template <template <typename> class Allocator>
using test_string_result = Result<
	std::basic_string<char, 
	std::char_traits<char>, Allocator<char>>, NoReturnCode>;


typedef Result<bool, NoReturnCode> bool_result;

template <bool is_iterator>
struct test_schema : public schema<is_iterator>
{

	// define a scalar TIMENODE
	template <typename ResultType, template <typename> class Allocator>
	using TIMENODE = time_node<ResultType, std::vector, is_iterator, Allocator>;

	template <typename ResultType, template <typename> class Allocator>
	using TIMENODESEG = time_node<ResultType, segmented_list_1024, is_iterator, Allocator>;

#define INIT(var_name)			var_name(schema<is_iterator>::make_url_for_child( #var_name ), registry)

	struct sub_schema : schema<is_iterator>
	{
		sub_schema(const base::URL &name, SerializableRegistry *registry)
			: schema<is_iterator>(name, registry),
			INIT(a_bool),
			INIT(a_bool2),
			INIT(a_bool3),
			INIT(a_bool4),
			INIT(a_string),
			INIT(b_string)
			{}
		TIMENODE<bool_result, std::allocator>		a_bool;	// choose where the timenodes sit
		TIMENODE<bool_result, slab_allocator>		a_bool2;
		TIMENODESEG<bool_result, std::allocator>	a_bool3;
		TIMENODESEG<bool_result, slab_allocator>	a_bool4;

		TIMENODE<test_string_result<std::allocator>, std::allocator> a_string; // nodes and the string
		TIMENODE<test_string_result<slab_allocator>, slab_allocator> b_string; // nodes and the string

	};

	test_schema(const base::URL &name, SerializableRegistry *r)
		: schema<is_iterator>(name, r),
		one(name.make_child("one"), r)
		{}
	
	sub_schema				one;
};

using test_state = test_schema<false>;

template <typename a_bool_typenode_type>
void exercise_a_bool_timenode(a_bool_typenode_type &ref, const char *expected_name)
{
	assert(ref.get_name() == expected_name); // initialized with the expected name?

	assert(ref.empty());	// initialized empty?
	ref.emplace_back(1, true);
	assert(!ref.empty());	// adding a value makes it non empty?

	a_bool_typenode_type copy_a_bool(ref);	// copies works?
	assert(copy_a_bool == ref);
	a_bool_typenode_type copy1_a_bool = ref;
	assert(copy1_a_bool == ref);
	a_bool_typenode_type copy2_a_bool = ref;
	assert(copy2_a_bool == ref);
	a_bool_typenode_type copy3_a_bool = ref;
	assert(copy3_a_bool == ref);

	// change a serialization index	to see that != works
	assert(ref == copy_a_bool);
	copy_a_bool.set_serialization_index(999);
	assert(ref != copy_a_bool);

	// change a value to see that != works
	assert(ref == copy1_a_bool);
	copy1_a_bool.emplace_back(2, false);
	assert(ref != copy1_a_bool);

	// encode and decode to check that it seems to load ok
	char buf[1024];
	MemoryStream e(buf, sizeof(buf), false);
	ref.encode(e);
	a_bool_typenode_type decoded_a_bool;
	e.reset_buf_pos();
	assert(decoded_a_bool != ref); // doesn't match to start
	decoded_a_bool.decode(e);	// but after decoding
	assert(decoded_a_bool == ref); // now matches!
}


void TEST_SCHEMA_COMMON()
{
	slab s;
	slab_allocator_base::m_temp_slab = &s;

	// default construction
	SerializableRegistry registry;
	test_state default_state(base::URL(), &registry);
	assert(default_state.get_name() == "");	// because default URL is ""
	registry.dump();

	exercise_a_bool_timenode(default_state.one.a_bool, "a_bool");
	exercise_a_bool_timenode(default_state.one.a_bool2, "a_bool2");
	exercise_a_bool_timenode(default_state.one.a_bool3, "a_bool3");
	exercise_a_bool_timenode(default_state.one.a_bool4, "a_bool4");
	
	assert(default_state.one.a_bool2.container.get_allocator().m_slab == &s);

	default_state.one.a_string.emplace_back(1, "hi fred");
	assert(!default_state.one.a_string.empty());
	assert(default_state.one.a_string.latest().get_value().val == "hi fred");

	default_state.one.b_string.emplace_back(1, "hi fred");
	assert(!default_state.one.b_string.empty());
	assert(default_state.one.b_string.latest().get_value().val == "hi fred");

	slab_allocator_base::m_temp_slab = nullptr;

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