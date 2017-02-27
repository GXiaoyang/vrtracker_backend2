// rangesplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <chrono>
#include <assert.h>
#include <forward_list>
#include <openvr.h>

// serialization
// temporary allocation
// instantiate for iterators vs values
// 

// cached_iterators
// tree nodes

template<typename T>
struct has_const_iterator
{
private:
	typedef char                      yes;
	typedef struct { char array[2]; } no;

	template<typename C> static yes test(typename C::const_iterator*);
	template<typename C> static no  test(...);
public:
	static const bool value = sizeof(test<T>(0)) == sizeof(yes);
	typedef T type;
};



template <typename T>
struct has_begin_end
{
	template<typename C> static char(&f(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::begin)),
		typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char(&f(...))[2];

	template<typename C> static char(&g(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::end)),
		typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char(&g(...))[2];

	static bool const beg_value = sizeof(f<T>(0)) == 1;
	static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T>
struct is_container_check : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value>
{ };

template <typename T, bool b = is_container_check<T>::value>
struct get_value_type
{
	typedef typename T::value_type type;
};

template <typename T>
struct get_value_type<typename T, false>
{
	typedef T type;
};


template <typename T>
struct Foo
{
	static const bool value_is_container = is_container_check<T>::value;
	typedef typename get_value_type<T>::type value_type;

	T val;
};


template <typename T>
void do_stuff(T& f, typename std::enable_if<std::is_class<T>::value, T>::type* = 0) {
	// an implementation for class types
}


template <typename T>
void sean_do_stuff(T& f, typename std::enable_if<T::value_is_container, T>::type* = 0) {
	// an implementation for class types
}

template <typename T>
void sean_do_stuff(T& f, typename std::enable_if<!T::value_is_container, T>::type* = 0) {
	// an implementation for class types
}




class Moo
{

};

void test()
{
	Moo m;
	Foo<float> foof;
	Foo<std::forward_list<int>> voof;
	do_stuff(m);
	
	sean_do_stuff(foof);
	sean_do_stuff(voof);
}

template <typename T>
struct TMP
{
	typedef T value_type;
	T *s;
	int max_size;
	int count;
	int size() const { return count; }
	T & operator[] (int pos) const { return s[pos]; }
};

template <typename T>
struct ReturnCodeForValidValue
{
};

template <> struct ReturnCodeForValidValue <bool>
{
	static const bool return_code = true;
};

struct NoReturnCode
{
};

template <typename ReturnCode>
struct WithReturnCode { 
	bool is_present() const { return (ReturnCodeForValidValue<ReturnCode>::return_code == return_code); }
	ReturnCode return_code; 
};
template <>
struct WithReturnCode<NoReturnCode>
{ 
	constexpr bool is_present() const { return true; }
};

// RESULT
template <typename ResultType, typename ReturnCode> 
struct Result : WithReturnCode<ReturnCode>
{
	static const bool value_is_container = is_container_check<ResultType>::value;
	//typedef typename get_value_type<ResultType>::type value_type;
	// todo - answer is_container for my TMP variables


	ResultType val;
};

template <typename ResultType, typename ReturnCode, typename ResultType2>
bool return_code_not_equals(const Result<ResultType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<!std::is_same<ReturnCode,NoReturnCode>::value, int>::type* = 0)
{
	return a.return_code != b.return_code;
}

template <typename ResultType, typename ReturnCode, typename ResultType2>
bool constexpr return_code_not_equals(const Result<ResultType, NoReturnCode> &a, const Result<ResultType2, NoReturnCode> &b,
	typename std::enable_if<std::is_same<ReturnCode, NoReturnCode>::value, int>::type* = 0)
{
	return false;
}

template <typename ResultType, typename ReturnCode, typename ResultType2>
static bool not_equals(const Result<ResultType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<!Result<ResultType, ReturnCode>::value_is_container, int>::type* = 0) 
{
	if (return_code_not_equals<ResultType,ReturnCode,ResultType2>(a,b))
		return true;
	if (a.val != b.val)
		return true;
	return false;
}

template <typename ResultType, typename ReturnCode, typename ResultType2>
static bool not_equals(const Result<ResultType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<Result<ResultType, ReturnCode>::value_is_container, int>::type* = 0) 
{
	static_assert(std::is_same<ResultType::value_type, ResultType2::value_type>::value, "ha");
	static_assert(std::is_pod<ResultType::value_type>::value, "values are not pods.  the memcmp prob won't work");

	if (return_code_not_equals<ResultType, ReturnCode, ResultType2>(a, b))
		return true;
	if (a.val.size() != b.val.size())
		return true;
	if (a.val.size() == 0)
		return true;
	if (memcmp(&a.val[0], &b.val[0], b.val.size() * sizeof(a.val[0])) != 0)
		return true;
	return false;
}

void sfinaetest()
{
	Result<float, bool> r1;
	Result<float, bool> r2;
	not_equals(r1, r2);

	Result<std::vector<float>, bool> v1;
	Result<std::vector<float>, bool> v2;
	not_equals(v1, v2);

	Result<std::vector<float>, bool> v3;
	Result<TMP<float>, bool> v4;
	bool wa = Result<TMP<float>, bool>::value_is_container;
	not_equals(v3, v4);

	Result<std::vector<std::vector<int>>, bool> v5;
	Result<std::vector<std::vector<int>>, bool> v6;
	//not_equals(v5, v6);
	

}


template <typename T>
struct HistoryEntry  
{
	HistoryEntry(int frame_number_in,  const T &result_in)
		:	frame_number(frame_number_in),
			result(result_in)
	{}

	template <typename U, typename V>
	HistoryEntry(int frame_number_in, const Result<TMP<U>, V> &result_in) 
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


#if 0 
template <typename ResultType, typename ReturnCode>
struct compare_result
{
	static bool not_equals(const Result<ResultType, ReturnCode> &a, const Result<ResultType, ReturnCode> &b)
	{
		if (a.return_code != b.return_code)
			return true;
		if (a.val != b.val)
			return true;
		return false;
	}

	template <typename U>
	static bool not_equals(const Result<ResultType, ReturnCode> &a, const Result<TMP<U>, ReturnCode> &b)
	{
		static_assert(std::is_trivially_copyable<U>::value, "ha");

		if (a.return_code != b.return_code)
			return true;
		if (a.val.size() != b.val.count)
			return true;
		if (memcmp(&a.val[0], b.val.s, b.val.count * sizeof(a.val[0])) != 0)
			return true;
		return false;
	}
};

template <typename ResultType>
struct compare_result <ResultType, void>
{
	static bool not_equals(const Result<ResultType, void> &a, const Result<ResultType, void> &b)
	{
		if (a.val != b.val)
			return true;
		return false;
	}

	template <typename U>
	static bool not_equals(const Result<ResultType, void> &a, const Result<TMP<U>, void> &b)
	{
		static_assert(std::is_trivially_copyable<U>::value, "ha");
		if (a.val.size() != b.val.count)
			return true;
		if (memcmp(&a.val[0], b.val.s, b.val.count * sizeof(a.val[0])) != 0)
			return true;
		return false;
	}
};
#endif



class UpdateVisitor
{
public:
	UpdateVisitor()
	{}

	int m_frame_number;

	static constexpr bool visit_source_interfaces()  { return true; }
	static constexpr bool reload_render_models()  { return false; }

	template <typename ResultType, typename ReturnCode, typename ResultTypeMaybeTMP>
	void visit_node(History<HistoryEntry<Result<ResultType, ReturnCode>>> &node, 
		const Result<ResultTypeMaybeTMP, ReturnCode> &latest_result)
	{
		if (node.empty() || not_equals(node.latest().result,latest_result))
		{
			node.emplace_front(m_frame_number, latest_result);
		}
	}

	template <typename ResultType, typename ReturnCode>
	void visit_node(History<HistoryEntry<Result<ResultType, ReturnCode>>> &node)
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
	History<HistoryEntry<Result<Uint32Size, NoReturnCode>>> target_size;
	History<HistoryEntry<Result<Uint32Size, bool>>> target_size2;
	History < HistoryEntry < Result<std::vector<int>, bool>>> ints;
	History < HistoryEntry < Result<std::vector<int>, NoReturnCode>>> ints2;
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


int main()
{
	test();
	visitortest();
	sfinaetest();

	//
	// scalar test
	//
	Result<int, bool> a;
	a.return_code = true;
	a.val = false;
	HistoryEntry <Result<int, bool>> a_node(1000, a);

	Result<vr::HmdMatrix34_t, NoReturnCode> b;
	b.val.m[0][0] = 1.0f;
	HistoryEntry < Result<vr::HmdMatrix34_t, NoReturnCode> > bh(1000, b);

	// prebake
	Result<TMP<int>, bool> c1;
	c1.val.s = (int *)malloc(10000);
	c1.val.count = 1;
	HistoryEntry < Result<std::vector<int>, bool> > ch1(1000, c1);

	// vector test : baked
	Result<std::vector<int>, bool> c2;
	HistoryEntry < Result<std::vector<int>, bool> > ch2(1000, c2);


	// history of scalar test
	History<HistoryEntry<Result<int, bool>>> ha;
	ha.emplace_front(a_node);

	// two steps
	// history of tmp vector->baked vector in two steps
	// build the node and then emplace it
	Result<TMP<int>, bool> cx1;
	cx1.val.s = (int *)malloc(10000);
	cx1.val.count = 1;
	History<HistoryEntry < Result<std::vector<int>, bool> >> history_of_vectors;

	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		//HistoryEntry < Result<std::vector<int>, bool> > step1(1000, cx1);
		
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
		History<HistoryEntry < Result<std::vector<int>, bool> >> history_of_vectors;
		history_of_vectors.emplace_front(1000,cx1);
	}


}

#if 0

// represents a value and a result type
template <typename T, typename ResultType>
struct scalar_result
{
	scalar_result() = default;

	scalar_result(const T&val_in, const ResultType &result_code_in)
		: val(val_in), result_code(result_code_in)
	{}

	~scalar_result()
	{}

	scalar_result(scalar_result &rhs)
	{
		val = rhs.val;
		result_code = rhs.result_code;
	}

	scalar_result &operator = (const scalar_result &rhs)
	{
		val = rhs.val;
		result_code = rhs.result_code;
		return *this;
	}

	scalar_result &operator = (scalar_result &&rhs)
	{
		val = std::move(rhs.val);
		result_code = rhs.result_code;
		return *this;
	}

	scalar_result(scalar_result &&rhs)
		: val(std::move(rhs.val))
	{
		result_code = rhs.result_code;
	}

	inline bool is_present() { return (PresenceValue<ResultType>::is_present == result_code); }

	T val;
	ResultType result_code;
};

template <typename T, typename P>
struct history_entry
{
	history_entry() = default;

	template<typename... Args>
	history_entry(int frame_number_in, P presence_in, Args&&... args)
		:
		frame_number(frame_number_in),
		presence(presence_in),
		val(std::forward<Args>(args)...)
	{
	}
	history_entry(int frame_number_in, P presence_in, const T& val_in)
		:
		frame_number(frame_number_in),
		presence(presence_in),
		val(val_in)
	{}

	bool is_present() const
	{
		return (PresenceValue<P>::is_present == presence);
	}
}


template <typename T, typename P>
struct history_vector
{
	typedef T val_type;
	using history_entry = history_entry_base<T, P>;

	typedef typename std::forward_list<history_entry, AllocatorT>::iterator iter_type;

	history_base(const AllocatorT& alloc) : name(nullptr), values(alloc)
	{}

	history_base(const char* name_in, const AllocatorT& alloc)
		: name(name_in), values(alloc)
	{}

	bool empty() const { return values.empty(); }

	bool more_than_2_values() const
	{
		int count = 0;
		for (auto iter = values.cbegin(); iter != values.end(); iter++)
		{
			count++;
			if (count > 1)
			{
				return true;
			}
		}
		return false;
	}
	const history_entry & front() const { return values.front(); }
	const T& latest() const
	{
		return values.front().val;
	}

	template<typename... Args>
	void base_emplace_front(int frame_number, Args&&... args)
	{
		values.emplace_front(frame_number, std::forward<Args>(args)...);
	}

	void dump(std::ostream &ofs, int indent_level)
	{
		indented_output(ofs, "history " + std::string(name) + "\n", indent_level);
		for (auto iter = values.begin(); iter != values.end(); iter++)
		{
			iter->dump(ofs, indent_level);
		}
		ofs << "\n";
	}
	const char *name;
	std::forward_list<history_entry, AllocatorT> values;
};





#define BOOST_NO_EXCEPTIONS
#include <list>
#include <boost/any.hpp>


#ifdef BOOST_NO_EXCEPTIONS
extern void __cdecl boost::throw_exception(class std::exception const &)
{
	printf("howdy");
}
#endif


using boost::any_cast;
typedef std::list<boost::any> many;

void append_int(many & values, int value)
{
	boost::any to_append = value;
	values.push_back(to_append);
}

void append_string(many & values, const std::string & value)
{
	values.push_back(value);
}

bool is_int(const boost::any & operand)
{
	return operand.type() == typeid(int);
}

struct mat34
{
	float v[3][4]; // [4];
};

int main()
{
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	int count = 0;

	for (int i = 0; i < 1000000; i++)
	{
		many l;
		boost::any ii = 11;
		mat34 f;
		boost::any ss = &f;

		mat34* mm = any_cast<mat34 *>(ss);
	//	int x = any_cast<int>(ss);

		append_int(l, 42);
		append_string(l, "hi");
		bool b = is_int(l.front());
		count += any_cast<int>(l.front());
	}

	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
	
	printf("%d Audit took %lld us\n", count, std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

	getchar();
    return 0;
}

#endif