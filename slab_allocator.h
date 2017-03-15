// allocates pages of memory.
// includes an allocator
// does not implement free

// rangesplay.cpp : Defines the entry point for the console application.
//

#pragma once
#include <assert.h>
#include <forward_list>
#include "tbb/spin_mutex.h"

struct slab
{
	static int slab_num_slabs;
	static int slab_total_slab_page_allocs;
	static int slab_total_slab_page_frees;
	int slab_num_alloc_calls = 0;
	int slab_num_dealloc_calls = 0;

	std::forward_list<char *> pages;
	int		page_size;
	int64_t current_page_pos;
	tbb::spin_mutex mutex;

	slab(int page_size_in = 1024 * 1024)
		: page_size(page_size_in)
	{
		slab_num_slabs++;
		current_page_pos = 0;
		slab_alloc(0);	// allocate the initial page
	}


	~slab()
	{
		for (auto iter = pages.begin(); iter != pages.end(); iter++)
		{
			char *mem = *iter;
			free(mem);
			slab_total_slab_page_frees += 1;
		}
		pages.clear();
		slab_num_slabs--;
	}

	void *slab_alloc(size_t size)
	{
		assert((int)size <= page_size);
		size = (size + 3) & ~0x3;
		slab_num_alloc_calls += 1;

		// critical start
		mutex.lock();
		if (pages.empty() || (int)size + current_page_pos > page_size)
		{
			current_page_pos = 0;
			slab_total_slab_page_allocs += 1;
			char *page = (char *)malloc(page_size);
			pages.push_front(page);
		}

		char *ret = pages.front();
		ret += current_page_pos;
		current_page_pos += size;
		// critical end
		mutex.unlock();
		return ret;
	}

	void slab_dealloc()
	{
		slab_num_dealloc_calls += 1;
	}
};


struct slab_allocator_base
{
	static int slab_allocators_constructed;
	static int slab_allocators_destroyed;
	static int slab_allocators_leaks;
	static slab* m_temp_slab;
};

template <class T=char>
struct slab_allocator : slab_allocator_base {
public:
	typedef T		   value_type;
	typedef T*         pointer;
	typedef const T&   const_reference;

	slab* m_slab;

	// stl likes to make these evil ones for some reason
	slab_allocator()
		: m_slab(m_temp_slab)
	{
		slab_allocators_constructed++;
	}

	slab_allocator(slab *slab)
	{
		m_slab = slab;
		slab_allocators_constructed++;
	}

	slab_allocator(const slab_allocator &rhs)
	{
		m_slab = rhs.m_slab;
		slab_allocators_constructed++;
	}

	// visual studio 2012 bug: https://connect.microsoft.com/VisualStudio/feedback/details/762094
	template<typename X, typename Y>
	struct rebind { using other = slab_allocator<T>; };

	~slab_allocator()
	{
		slab_allocators_destroyed++;
	}

	template <class U> slab_allocator(const slab_allocator<U>& other)
	{
		m_slab = other.m_slab;
		slab_allocators_constructed++;
	}

	T* allocate(std::size_t n)
	{
		T* p = (T*)m_slab->slab_alloc(n * sizeof(T));
		return (T*)p;
	}
	void deallocate(void * p, std::size_t n)
	{
		m_slab->slab_dealloc();
		slab_allocators_leaks++;
	}
};

template <class T, class U>
bool operator==(const slab_allocator<T>&, const slab_allocator<U>&)
{
	return true;
}

template <class T, class U>
bool operator!=(const slab_allocator<T>&, const slab_allocator<U>&)
{
	return false;
}