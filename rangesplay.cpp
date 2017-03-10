// rangesplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <list>
#include <chrono>
#include <assert.h>
#include <forward_list>
#include <openvr.h>
#include <assert.h>
#include "result.h"
#include "history.h"
#include <deque>
#include "range.h"
//#include "concurrent_vector.h"



#if 0

template <typename T>
struct TMP
{
	typedef T value_type;
	T *s;
	T * data() { return s; }
	const T * data() const { return s; }
	
	T * first() { return s; }
	T * last() { return s + count; }

	const T * first() const { return s; }
	const T * last() const { return s + count; }


	int max_size;
	int count;
	int size() const { return count; }
	T & operator[] (int pos) { return s[pos]; }
	const T & operator[] (int pos) const { return s[pos]; }
};



// GUI wants a tree of nodes

// I want to give him the tree he wants but also not dumb down the 
// real-time, time based values
//
// for the tree view
// qt  doesn't require the data structure be anything - as long as index queries and qvariant 
// answers can be delivered.
//
// e.g QVariant get_data(int col, int row)


//
//  test the cursor functionality - given a range, can I pick a close match to the frame I want
//
template <typename HistoryT>
void test_lower_bounds()
{
	{
		// returns prev if an inexact match is found
		HistoryT a; // match at end
		a.emplace_new_entry(0, false);
		a.emplace_new_entry(20, true);
		a.emplace_new_entry(30, false);
		auto iter = a.queryLTE(21);	// make sure it chooses prev
		assert(iter->frame_number == 20);
		assert(iter->result.val == true);
	}
	{
		// returns prev if an inexact match is found
		HistoryT a; // match at end
		a.emplace_new_entry(30, false);
		auto iter = a.queryLTE(21);	// make sure it chooses prev
		assert(iter==a.end());
	}

	{
		// query an empty node. expect iterator returns end()
		HistoryT a;	
		auto iter = a.queryLTE(20);
		assert(iter == a.end());
	}
	{
		// query an exact match. expect iterator returns exact match()
		HistoryT a;
		a.emplace_new_entry(20, true);
		auto iter = a.queryLTE(20);
		assert(iter->frame_number == 20);
		assert(iter->result.val == true);
	}
	{
		HistoryT a; // match at end
		a.emplace_new_entry(0, false);
		a.emplace_new_entry(20, true);
		auto iter = a.queryLTE(20);
		assert(iter->frame_number == 20);
		assert(iter->result.val == true);
	}
	{
		HistoryT a; // match at beginning
		a.emplace_new_entry(20, true);
		a.emplace_new_entry(30, false);
		auto iter = a.queryLTE(20);
		assert(iter->frame_number == 20);
		assert(iter->result.val == true);
	}
	{
		HistoryT a; // match in middle beginning
		a.emplace_new_entry(10, false);
		a.emplace_new_entry(20, true);
		a.emplace_new_entry(30, false);
		auto iter = a.queryLTE(20);
		assert(iter->frame_number == 20);
		assert(iter->result.val == true);
	}
	{
		// match off the end
		HistoryT a; // match in middle beginning
		a.emplace_new_entry(0, true);
		a.emplace_new_entry(10, false);
		auto iter = a.queryLTE(20);	// latest info is frame 10. so expect  10,false
		assert(iter->frame_number == 10);
		assert(iter->result.val == false);
	}
}

template <typename HistoryT>
void test_intervals()
{
	{
		HistoryT a; // match in middle beginning
		a.emplace_new_entry(0, true);
		a.emplace_new_entry(10, false);
		auto range = a.interval_query(0, 5);	// expect 0 and 10
		assert(range.begin()->frame_number == 0);
		assert(range.begin()->result == true);
		assert(range.end()->frame_number == 10);
		assert(range.end()->result == false);
	}
	{
		HistoryT a; // find no events because [0-5) does not exist in the range
		a.emplace_new_entry(5, true);
		a.emplace_new_entry(10, false);
		auto range = a.interval_query(0, 5);	
		assert(range.begin() == a.end());
		assert(range.end() == a.end());
	}
	{
		HistoryT a; 
		a.emplace_new_entry(5, true);
		a.emplace_new_entry(10, false);
		auto range = a.interval_query(0, 6);	// expect 0 and 10 because the 6 will match-down to 5 and the 10 will come along
		assert(range.begin()->frame_number == 5);
		assert(range.begin()->result == true);
		assert(range.end()->frame_number == 10);
		assert(range.end()->result == false);
	}
	{
		HistoryT a;
		a.emplace_new_entry(5, true);
		a.emplace_new_entry(10, false);
		auto range = a.interval_query(10, 20);		// expect both iterators to point to end/nada
		assert(range.begin() == a.end());
		assert(range.end() == a.end());
	}
}

void cache_test()
{
	std::vector<int> a = { 5,10 };
	typedef std::range<std::vector<int>::iterator> range_type;
	range_type r(a.begin(), a.end());

	range_intersect_result<range_type> cache;
	auto result = cache.intersect(r, 5, 11, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });
	assert(*result.begin() == 5);
	assert(result.end() == a.end());

	result = cache.intersect(r, 5, 11, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });
	assert(*result.begin() == 5);
	assert(result.end() == a.end());

	result = cache.intersect(r, 3, 5, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });
	assert(result.begin() == a.end());
	assert(result.end() == a.end());
	
}


void range_test()
{
	std::vector<int> a = { 5,10 };
	std::range<std::vector<int>::iterator> r(a.begin(), a.end());

	auto result = range_intersect(r, 5, 11, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; }); //[5,11), does intersect [5,10]
	assert(*result.begin() == 5);
	assert(result.end() == a.end());

	result = range_intersect(r, 3, 5, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; }); //[3,5), does not intersect [5,10]
	assert(result.begin() == a.end());
	assert(result.end() == a.end());

	result = range_intersect(r, 5, 5, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; }); // [5,5) is nonsense
	assert(result.begin() == a.end());
	assert(result.end() == a.end());

	result = range_intersect(r, 10, 12, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });	//  [10,12) intersects [5,10]
	assert(*result.begin() == 10);
	assert(result.end() == a.end());

	result = range_intersect(r, 3, 5, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });	// expect no value since request is for [3,5) in [5,10]
	assert(result.begin() == a.end());
	assert(result.end() == a.end());

	result = range_intersect(r, 10, 12, [](const auto &lhs, const auto &rhs) -> bool { return lhs < rhs; });	// inputs are INCLUSIVE, so this should work:
	assert(*result.begin() == 10);
	assert(result.end() == a.end());

printf("bla");
}



// containers for VALUEs ()ie the individual values in a history node
void container_test()
{

	// what do the supported value container types look like:
	//	* require memcpyable
	//  * require size()
	//
	// array, vector, TMP<> should all return true
	static_assert(IsValueContainer<TMP<float>>(), "TMP<float> is a temporary container");
	static_assert(IsValueContainer<std::vector<int>>(), "vector<int> is a container");

	static_assert(!IsValueContainer<std::forward_list<int>>(), "forward list is not a supported containerr");
	static_assert(!IsValueContainer<float>(), "a float is not a container");
}

void result_test()
{
	Result<float, bool> a1;
	a1.val = 0;
	a1.return_code = true;

	Result<float, bool> a2;
	a2 = a1;
	assert(a2 == a1);

	Result<std::vector<float>, bool> av1;
	Result<std::vector<float>, bool> av2;
	av1 = av2;

	int v[12] = { 1,2,3,4,5,6,7,8,9,1,1,2 };
	
	std::vector<void *> vv;
	vv.reserve(100000);
	// test how much this costs

	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	
	for (int i = 0; i < 100000; i++)
	{
		Result<TMP<int>, bool> av3;
		av3.val.count = 12;
		av3.val.s = v;
		vv.push_back(new Result<std::vector<int>, bool>(av3));
	}

	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
	printf("v1 took %lld us\n", std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

	getchar();
//	av1 = av3;

	//av3 = av1;



	Result<float, bool> r1;
	Result<float, bool> r2;
	r1.val = 0;
	r2.val = 0;
	r1.return_code = true;
	r2.return_code = true;
	assert(r1 == r2);
	assert(!(r1 != r2));

	r1.val = 1;
	assert(r1 != r2);
	assert(!(r1 == r2));


	



	Result<std::vector<float>, bool> v1;
	Result<std::vector<float>, bool> v2;
	not_equals(v1, v2);

	Result<std::vector<float>, bool> v3;
	Result<TMP<float>, bool> v4;
	bool wa = Result<TMP<float>, bool>::value_is_container;


}

#if 0

template <typename T>
struct FrameData::Record  
{
	FrameData::Record(int frame_number_in,  const T &result_in)
		:	frame_number(frame_number_in),
			result(result_in)
	{}

	template <typename U, typename V>
	FrameData::Record(int frame_number_in, const Result<TMP<U>, V> &result_in) 
		: 
		frame_number(frame_number_in)
		{ 
			result.val.resize(result_in.val.count);
			memcpy(&result.val[0], result_in.val.s, result_in.val.count * sizeof(result.val[0]));
		}
	
	int	frame_number;
	T	result;
};

template <typename T>
struct History
{
	History() : name(nullptr)
	{}

	bool empty() const { return values.empty(); }

	const T & latest() const { return values.front(); }

	template<typename... Args>
	void emplace_front(Args&&... args)
	{
		values.emplace_front(std::forward<Args>(args)...);
	}

	const char *name;
	std::forward_list<T> values;
};


class UpdateVisitor
{
public:
	UpdateVisitor()
	{}

	int m_frame_number;

	static constexpr bool visit_source_interfaces()  { return true; }
	static constexpr bool reload_render_models()  { return false; }

	template <typename ResultType, typename ReturnCode, typename ResultTypeMaybeTMP>
	void visit_node(History<FrameData::Record<Result<ResultType, ReturnCode>>> &node, 
		const Result<ResultTypeMaybeTMP, ReturnCode> &latest_result)
	{
		if (node.empty() || not_equals(node.latest().result,latest_result))
		{
			node.emplace_front(m_frame_number, latest_result);
		}
	}

	template <typename ResultType, typename ReturnCode>
	void visit_node(History<FrameData::Record<Result<ResultType, ReturnCode>>> &node)
	{
	}
};

#define MEMCMP_OPERATOR_EQ(my_typename)\
inline bool operator == (const my_typename &lhs, const my_typename &rhs)\
{\
	return (memcmp(&lhs, &rhs, sizeof(lhs)) == 0);\
}\
inline bool operator != (const my_typename &lhs, const my_typename &rhs)\
{\
	return !(lhs == rhs);\
}
struct Uint32Size
{
	uint32_t width;
	uint32_t height;
};

MEMCMP_OPERATOR_EQ(Uint32Size)


struct dummywrapper
{
	inline Result<Uint32Size,NoReturnCode> GetRecommendedRenderTargetSize()
	{
		Result<Uint32Size, NoReturnCode> s;
		s.val.width = 12;
		s.val.height = 11;
		return s;
	}

	inline Result<Uint32Size, bool> GetRecommendedRenderTargetSize2()
	{
		Result<Uint32Size, bool> s;
		s.val.width = 12;
		s.val.height = 11;
		s.return_code = false;
		return s;
	}
};


struct statehistory
{
	History<FrameData::Record<Result<Uint32Size, NoReturnCode>>> target_size;
	History<FrameData::Record<Result<Uint32Size, bool>>> target_size2;
	History < FrameData::Record < Result<std::vector<int>, bool>>> ints;
	History < FrameData::Record < Result<std::vector<int>, NoReturnCode>>> ints2;
};

void visitortest()
{
	UpdateVisitor v;
	dummywrapper w;
	statehistory s;
	Result<Uint32Size, NoReturnCode> r = w.GetRecommendedRenderTargetSize();
	v.visit_node(s.target_size, r); 
	v.visit_node(s.target_size, r);
	v.visit_node(s.target_size);
	v.visit_node(s.target_size);

	Result<Uint32Size, bool> r2 = w.GetRecommendedRenderTargetSize2();
	v.visit_node(s.target_size2, r2); 
	v.visit_node(s.target_size2, r2);
	v.visit_node(s.target_size2,r2);
	v.visit_node(s.target_size2,r2);

	Result<TMP<int>, bool> cx1;
	cx1.val.s = (int *)malloc(10000);
	cx1.val.count = 1;
	v.visit_node(s.ints, cx1);
	v.visit_node(s.ints, cx1);

	Result<TMP<int>, NoReturnCode> cx2;
	cx2.val.s = (int *)malloc(10000);
	cx2.val.count = 1;
	v.visit_node(s.ints2, cx2);
	v.visit_node(s.ints2, cx2);
}
#endif

int main()
{
	typedef Result<bool, NoReturnCode> ResultType;
	typedef History2<ResultType, std::vector> HistoryNodeType;

	HistoryNodeType a;

	test_lower_bounds<HistoryNodeType>();
	a.emplace_new_entry(0, false);



	//test_range();
	//history_test();
	//result_test();
#if 0
	visitortest();
	

	//
	// scalar test
	//
	Result<int, bool> a;
	a.return_code = true;
	a.val = false;
	FrameData::Record <Result<int, bool>> a_node(1000, a);

	Result<vr::HmdMatrix34_t, NoReturnCode> b;
	b.val.m[0][0] = 1.0f;
	FrameData::Record < Result<vr::HmdMatrix34_t, NoReturnCode> > bh(1000, b);

	// prebake
	Result<TMP<int>, bool> c1;
	c1.val.s = (int *)malloc(10000);
	c1.val.count = 1;
	FrameData::Record < Result<std::vector<int>, bool> > ch1(1000, c1);

	// vector test : baked
	Result<std::vector<int>, bool> c2;
	FrameData::Record < Result<std::vector<int>, bool> > ch2(1000, c2);


	// history of scalar test
	History<FrameData::Record<Result<int, bool>>> ha;
	ha.emplace_front(a_node);

	// two steps
	// history of tmp vector->baked vector in two steps
	// build the node and then emplace ift
	Result<TMP<int>, bool> cx1;
	cx1.val.s = (int *)malloc(10000);
	cx1.val.count = 1;
	History<FrameData::Record < Result<std::vector<int>, bool> >> history_of_vectors;

	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		//FrameData::Record < Result<std::vector<int>, bool> > step1(1000, cx1);
		
		//history_of_vectors.emplace_front(step1);
		history_of_vectors.emplace_front(1000, cx1);
	}

	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
	printf("v1 took %lld us\n", std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

	// one step - way faster
	{
		Result<TMP<int>, bool> cx1;
		cx1.val.s = (int *)malloc(10000);
		cx1.val.count = 1;
		History<FrameData::Record < Result<std::vector<int>, bool> >> history_of_vectors;
		history_of_vectors.emplace_front(1000,cx1);
	}

#endif
}

#endif