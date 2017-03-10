// result.h
//
// * hold the element value and the return code in a single structure
// * knows how to memcpy vector type results between two different containers
//
// TODO: this was written before I wrote the cast->vector operator in the TMP
//       vector and probably could be simplified now to only allow LHS and RHS to be
//       identical
//
#pragma once

#include "tmp_vector.h"
#include "typehelper.h"

// ResultValue
// IsValueContainer()
//
// Is the result type a valid, supported value container.
// really I just want to say can I memcmpy and memcmp it. but I can't.  so the
// best is a couple of reasonable checks.  
//
// * does it have a size() operator
// * does it have a [] operator
// * does it have a value_type member
//

// uses macros in typehelper.h
namespace detail
{
	TYPE_SUPPORTS(SupportsSize, T().size())
	TYPE_SUPPORTS(SupportsIndexOperator, T().operator[](0))
}
template<typename T>
bool constexpr IsValueContainer()
{
	return detail::SupportsSize<T>::value && detail::SupportsIndexOperator<T>::value && has_value_type<T>::value;
}

struct NoReturnCode {};


template <typename ReturnCode>
struct ValidReturnCode
{
	static const ReturnCode return_code;
};


// Result
//
// A Result has a value for the result and may also have a Return Code
template <typename ElementType, typename ReturnCode>
struct Result 
{
	static const bool value_is_container = IsValueContainer<ElementType>();
	static const bool has_return_code = true;

	Result()
	{}
#if 0
	template <size_t FixedSizeBytes, typename FinalAllocatorType>
	Result(tmp_vector_pool<FixedSizeBytes> *pool, const FinalAllocatorType &final_allocator)
		: val(pool, final_allocator)
	{

	}
#endif

	Result(ElementType e, ReturnCode r)
		: return_code(r),
		val(e)
	{

	}

	// perf 12,11,15
	template <typename ResultType2>
	Result(const ResultType2 &rhs)          // e.g. rhs is a std::vector and *this is a segmented list
	{
		assign(*this, rhs);
	}

	Result& operator = (const Result &rhs)
	{
		assign(*this, rhs);
		return *this;
	}
//#if 0	// See TODO note above.  leaving it commented out here until I get wrappers working
	template <typename ResultType2>
	Result& operator = (const Result<ResultType2, ReturnCode> &rhs)
	{
		assign(*this, rhs);
		return *this;
	}
//#endif

	bool is_present() const { return (ValidReturnCode<ReturnCode>::return_code == return_code); }
	ReturnCode return_code;
	ElementType val;
};

// Result with no return code
//
// A specialization for Result that do not have ReturnCodes

template <typename ElementType>
struct Result<ElementType, NoReturnCode>
{
	Result()
	{}

#if 0	
	template <typename PoolType, typename FinalAllocatorType>
	Result(PoolType *pool, const FinalAllocatorType &final_allocator)
		: val(pool, final_allocator)
	{

	}
#endif

	Result(const ElementType &rhs)
		: val(rhs)
	{}

//#if 0
	template <typename E, typename R>
	explicit Result(Result<E,R> &rhs)
	{
		assign(*this, rhs);
		//val = rhs.val;
	}
//#endif

	static const bool value_is_container = IsValueContainer<ElementType>();
	static const bool has_return_code = false;

	bool is_present() const { return true; }
	ElementType val;
};

// Compares only the returncode part of the result
template <typename Result, typename Result2>
bool return_code_not_equals(const Result &a, const Result2 &b,
	typename std::enable_if<Result::has_return_code, int>::type* = 0)
{
	return a.return_code != b.return_code;
}

template <typename Result, typename Result2>
bool return_code_not_equals(const Result &a, const Result2 &b,
	typename std::enable_if<!Result::has_return_code, int>::type* = 0)
{
	return false;
}

// if it's not a container, evaluate the values
template <typename ElementType, typename ReturnCode, typename ResultType2>
static bool not_equals(const Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<!Result<ElementType, ReturnCode>::value_is_container, int>::type* = 0)
{
	if (return_code_not_equals(a,b))
		return true;
	if (a.val != b.val)
		return true;
	return false;
}

// if it is a container, evaluate the sizes before the values
template <typename ElementType, typename ReturnCode, typename ResultType2>
static bool not_equals(const Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<Result<ElementType, ReturnCode>::value_is_container, int>::type* = 0) 
{
	static_assert(std::is_same<typename ElementType::value_type, typename ResultType2::value_type>::value, "ha");
	static_assert(std::is_pod<typename ElementType::value_type>::value, "values are not pods.  the memcmp prob won't work");

	if (return_code_not_equals(a, b))
		return true;
	if (a.val.size() != b.val.size())
		return true;
	if (a.val.size() == 0)
		return true;
	if (memcmp(&a.val[0], &b.val[0], b.val.size() * sizeof(a.val[0])) != 0)
		return true;
	return false;
}

template <typename ElementType, typename ReturnCode, typename ResultType2>
static bool operator != (const Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b)
{
	return not_equals(a, b);
}

template <typename ElementType, typename ReturnCode, typename ResultType2>
static bool operator == (const Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b)
{
	return !not_equals(a, b);
}

template <typename Result, typename Result2>
void assign_return_code(Result &a, const Result2 &b,
	typename std::enable_if<Result::has_return_code, int>::type* = 0)
{
	a.return_code = b.return_code;
}

template <typename Result, typename Result2>
void assign_return_code(const Result &a, const Result2 &b,
	typename std::enable_if<!Result::has_return_code, int>::type* = 0)
{
}

template <typename ElementType, typename ReturnCode, typename ResultType2>
static void assign(Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<!Result<ElementType, ReturnCode>::value_is_container, int>::type* = 0)
{
	assign_return_code(a, b);
	a.val = b.val;
}

template <typename ElementType, typename ReturnCode, typename ResultType2>
static void assign(Result<ElementType, ReturnCode> &a, const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<Result<ElementType, ReturnCode>::value_is_container, int>::type* = 0)
{
	assign_return_code(a, b);
	a.val.resize(b.val.size());
	memcpy(a.val.data(), b.val.data(), b.val.size() * sizeof(a.val[0]));
}

// utilities
template <typename T, typename R>
Result<T, R>  make_result(T val, R result) {
	return Result<T, R>(val, result);
}

template <typename T>
Result<T, NoReturnCode>  make_result(T val) {
	return Result < T, NoReturnCode>( val );
}