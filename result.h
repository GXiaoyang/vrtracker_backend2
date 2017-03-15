// result.h
//
// * hold the element value and the return code in a single structure
// * knows how to memcpy vector type results between two different containers
//
//
#pragma once

#include "typehelper.h"

// ResultValue
// IsValueContainer()
//
// Is the result type a valid, supported value container.
// Really I just want to say can I memcmpy and memcmp it. but I can't.  so instead 
// I check for a data() method for containers and ispod for non-containers

// uses macros in typehelper.h
namespace detail
{
	TYPE_SUPPORTS(SupportsData, T().data())
}
template<typename T>
bool constexpr IsValueContainer()
{
	return detail::SupportsData<T>::value;
}

struct NoReturnCode {};


template <typename ReturnCode>
struct ValidReturnCode
{
	static const ReturnCode return_code;
};

template <typename ElementType> 
struct ResultBase
{
	static const bool value_is_pod = std::is_pod<ElementType>::value;
	static const bool value_is_container = IsValueContainer<ElementType>();
	static_assert(value_is_pod || value_is_container, "Result only supports pods and contiguous containers as values");
};

// Result
//
// A Result has a value for the result and may also have a Return Code
template <typename ElementType, typename ReturnCode>
struct Result : ResultBase<ElementType>
{
	static const bool has_return_code = true;
	bool is_present() const { return (ValidReturnCode<ReturnCode>::return_code == return_code); }

	Result()
	{}

	Result(ElementType e, ReturnCode r)
		: return_code(r),
		val(e)
	{}

	Result(const ElementType &&rhs)
		:	return_code(rhs.return_code),
			val(std::move(rhs))
	{}

	// perf 12,11,15
	template <typename E, typename R>
	explicit Result(Result<E, R> &rhs)
	{
		static_assert(
			(value_is_pod && std::is_pod<E>::value) || (value_is_container && IsValueContainer<E>()),
			"source and target are not similar enough");
		assign(*this, rhs);
	}

	Result& operator = (const Result &rhs)
	{
		assign(*this, rhs);
		return *this;
	}

	template <typename ResultType2>
	Result& operator = (const Result<ResultType2, ReturnCode> &rhs)
	{
		static_assert(value_is_pod && std::is_pod<ResultType2>::value);
		assign(*this, rhs);
		return *this;
	}
	
	ReturnCode return_code;
	ElementType val;
};

// Result with no return code
//
// A specialization for Result that do not have ReturnCodes

template <typename ElementType>
struct Result<ElementType, NoReturnCode> : ResultBase<ElementType>
{
	static const bool has_return_code = false;
	bool is_present() const { return true; }

	Result()
	{}

	Result(const ElementType &rhs)
		: val(rhs)
	{}

	Result(ElementType &&rhs)
		: val(std::move(rhs))
	{}

	template <typename E, typename R>
	explicit Result(Result<E, R> &rhs)
	{
		static_assert(
			(value_is_pod && std::is_pod<E>::value) || (value_is_container && IsValueContainer<E>()),
			"source and target are not similar enough");

		assign(*this, rhs);
	}

	template <typename E, typename R>
	Result& operator =(const Result<E, R> &rhs)
	{
		static_assert(
			(value_is_pod && std::is_pod<E>::value) || (value_is_container && IsValueContainer<E>()),
			"source and target are not similar enough");

		assign(*this, rhs);
		return *this;
	}

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
static bool not_equals(
	const Result<ElementType, ReturnCode> &a, 
	const Result<ResultType2, ReturnCode> &b,
	typename std::enable_if<Result<ElementType, ReturnCode>::value_is_container, int>::type* = 0) 
{
	static_assert(std::is_same<std::remove_cv<std::remove_reference<decltype(a.val[0])>::type>::type, 
							   std::remove_cv<std::remove_reference<decltype(b.val[0])>::type>::type>::value, "a and b are not the same type");
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
	assert(a.val.size() == b.val.size());
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