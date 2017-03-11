#pragma once
#include "url_named.h"

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
template <typename ResultType, template <typename, typename> class ContainerType, bool IsIterator, class Allocator>
struct node : public time_indexed_vector<ResultType, ContainerType, Allocator>::iterator
{
	node(const URL &name, const Allocator &a) {}
	URL make_url_for_child(const char *child) { return URL(); }
};

//
// State node
//
template <typename ResultType, template <typename, typename> class ContainerType, class Allocator>
struct node<ResultType, ContainerType, false, Allocator> : public time_indexed_vector<ResultType, ContainerType, Allocator>
{
	node(const URL &name, const Allocator &a)
		: time_indexed_vector<ResultType, ContainerType, Allocator>(name, a)
	{}

	URL make_url_for_child(const char *child) { return URL(); }
};