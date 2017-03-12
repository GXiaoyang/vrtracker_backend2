#pragma once
#include "url_named.h"
#include "result.h"
#include "time_containers.h"

template <bool is_iterator>
struct schema : public base::url_named
{
	schema(const base::URL &name)
		: url_named(name)
	{}
	base::URL make_url_for_child(const char *child) { return get_url().make_child(child); }
};

template <>
struct schema<true>
{
	schema(const base::URL &name) {}
	base::URL make_url_for_child(const char *child) { return base::URL(); }
};

//
// Iterator node
//
template <typename ResultType, template <typename, typename> class ContainerType, bool IsIterator, 
	template <typename> typename Allocator>
struct time_node : public time_indexed_vector<ResultType, ContainerType, Allocator>::iterator
{
	time_node(const base::URL &name = base::URL()) {}
	base::URL make_url_for_child(const char *child) { return base::URL(); }
};

//
// State node
//
template <typename ResultType, template <typename, typename> class ContainerType, 
	template <typename>  typename Allocator>
struct time_node<ResultType, ContainerType, false, Allocator> : public time_indexed_vector<ResultType, ContainerType, Allocator>
{
	time_node(const base::URL &name = base::URL())
		: time_indexed_vector<ResultType, ContainerType, Allocator>(name)
	{}

	base::URL make_url_for_child(const char *child) { return base::URL(); }
};