#pragma once
#include "url_named.h"
#include "result.h"
#include "time_containers.h"

template <bool is_iterator>
struct schema : public url_named
{
	schema(const URL &name)
		: url_named(name)
	{}
	URL make_url_for_child(const char *child) { return get_url().make_child(child); }
};

template <>
struct schema<true>
{
	schema(const URL &name) {}
	URL make_url_for_child(const char *child) { return URL(); }
};

//
// Iterator node
//
template <typename ResultType, template <typename, typename> class ContainerType, bool IsIterator, 
	template <typename> typename Allocator>
struct time_node : public time_indexed_vector<ResultType, ContainerType, Allocator>::iterator
{
	time_node(const URL &name = URL()) {}
	URL make_url_for_child(const char *child) { return URL(); }
};

//
// State node
//
template <typename ResultType, template <typename, typename> class ContainerType, 
	template <typename>  typename Allocator>
struct time_node<ResultType, ContainerType, false, Allocator> : public time_indexed_vector<ResultType, ContainerType, Allocator>
{
	time_node(const URL &name = URL())
		: time_indexed_vector<ResultType, ContainerType, Allocator>(name)
	{}

	URL make_url_for_child(const char *child) { return URL(); }
};