#pragma once
#include "url_named.h"
#include "result.h"
#include "time_containers.h"

template <bool is_iterator>
struct schema : public base::url_named
{
	schema() {}
	schema(const base::URL &name)
		: url_named(name)
	{}
	base::URL make_url_for_child(const char *child) { return get_url().make_child(child); }
};

template <>
struct schema<true>
{
	schema() {}
	schema(const base::URL &name) {}
	base::URL make_url_for_child(const char *child) { return base::URL(); }
};

//
// Iterator node is whatever the 'iterator type' is for the time_indexed_vector
//
template <typename ResultType, template <typename, typename> class ContainerType, bool IsIterator, 
	template <typename> class Allocator>
struct time_node : public time_indexed_vector<ResultType, ContainerType, Allocator>::iterator
{
	bool initialized;
	explicit time_node(const base::URL &name = base::URL()) 
		: initialized(false)
	{}
	base::URL make_url_for_child(const char *child) { return base::URL::EMPTY_URL; }

	time_node(const time_node &rhs) 
		: 
		initialized(rhs.initialized),
		time_indexed_vector<ResultType, ContainerType, Allocator>::iterator(rhs)
	{
	}

	time_node &operator =(const time_node &rhs)
	{
		time_indexed_vector<ResultType, ContainerType, Allocator>::iterator::operator=(rhs);
		initialized = rhs.initialized;
		return *this;
	}

	// important - let the node in a tree be assigned to it's base iterator.
	time_node &operator =(const typename time_indexed_vector<ResultType, ContainerType, Allocator>::iterator &rhs)
	{
		time_indexed_vector<ResultType, ContainerType, Allocator>::iterator::operator=(rhs);
		initialized = true;
		return *this;
	}
};

//
// State node
//
template <typename ResultType, template <typename, typename> class ContainerType, 
	template <typename>  class Allocator>
struct time_node<ResultType, ContainerType, false, Allocator> : public time_indexed_vector<ResultType, ContainerType, Allocator>
{
	time_node(const base::URL &name = base::URL())
		: time_indexed_vector<ResultType, ContainerType, Allocator>(name)
	{}

	base::URL make_url_for_child(const char *child) { return base::URL(); }
};