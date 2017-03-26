#pragma once
#include "url_named.h"
#include "result.h"
#include "time_containers.h"

// default case: schema is not an iterator
template <bool is_iterator>
struct schema : public base::url_named
{
	schema() {}
	schema(const base::URL &name, SerializableRegistry *registry)
		: url_named(name)
	{}
	base::URL make_url_for_child(const std::string &child) { return get_url().make_child(child); }
};

// specialization schema : is not an iterator
template <>
struct schema<true>
{
	schema() {}
	schema(const base::URL &name, SerializableRegistry *registry) {}
	base::URL make_url_for_child(const std::string &child) { return base::URL(); }
};

//
// Iterator node is whatever the 'iterator type' is for the time_indexed_vector
//
template <typename ResultType, template <typename, typename> class ContainerType, bool IsIterator, 
	template <typename> class Allocator>
struct time_node : public time_indexed_vector<ResultType, ContainerType, Allocator>::iterator
{
	bool initialized; // until such a time as the iterator is assigned (see operator = below), initialized is false

	time_node()
		: initialized(false)
	{}

	explicit time_node(const base::URL &name, SerializableRegistry *registry)
		: initialized(false)
	{}
	base::URL make_url_for_child(const std::string &child) { return base::URL::EMPTY_URL; }

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
// When it's a state (is_iterator_is_false), the time_node is defined as a time_indexed_vector
// the role of this truct is to makes that choice and delegate the implementation.
//
template <typename ResultType, template <typename, typename> class ContainerType, 
	template <typename>  class Allocator>
struct time_node<ResultType, ContainerType, false, Allocator> :
	public time_indexed_vector<ResultType, ContainerType, Allocator>, RegisteredSerializable
{
	time_node()
	{}

	time_node(const base::URL &name, SerializableRegistry *registry)
		: time_indexed_vector<ResultType, ContainerType, Allocator>(name)
	{		
		registry->Register(this);
	}

	virtual void encode(EncodeStream &e) const override final
	{
		time_indexed_vector<ResultType, ContainerType, Allocator>::encode(e);		
	}

	// read the value from the stream
	void decode(EncodeStream &e) override final
	{
		time_indexed_vector<ResultType, ContainerType, Allocator>::decode(e);
	}


	base::URL make_url_for_child(const std::string &child) { return base::URL(); }
};


template <typename T, class Allocator = std::allocator<T>>
struct named_vector : public base::url_named, public std::vector<T, Allocator>, public RegisteredSerializable
{
	named_vector() {}
	template<typename... Args>
	explicit named_vector(const base::URL &name, SerializableRegistry *registry, Args&&... args)
		:
		url_named(name),
		RegisteredSerializable(registry->Register(this)),
		std::vector<T, Allocator>(std::forward<Args>(args)...)
	{}
	base::URL make_url_for_child(const std::string &child) { return get_url().make_child(child); }

	void encode(EncodeStream &e) const override final
	{
		base::url_named::encode(e);
		int mysize = size();
		e.memcpy_out_to_stream(&mysize, sizeof(mysize));
	}

	void decode(EncodeStream &e) override final
	{
		base::url_named::decode(e);
		int mysize;
		e.memcpy_from_stream(&mysize, sizeof(mysize));
		resize(mysize);
	}
};
