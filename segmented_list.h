#pragma once 
#include <list>
#include <algorithm>
#include <atomic>
#include <thread>
#include <assert.h>
#include <cstring>
#include "log.h"
#include "platform.h"


typedef uint32_t SegmentSizeType;

template <typename T, SegmentSizeType SegmentSize, typename A = std::allocator<T>> struct segmented_list;

template <typename T, typename A = std::allocator<T>>
using segmented_list_1024 = segmented_list<T, 1024, A>;


template <typename T, typename SegmentContainerIterator, uint32_t SegmentSize, typename A = std::allocator<T>>
struct segmented_list_iterator;

template <typename T, SegmentSizeType SegmentSize, typename A>
struct segmented_list
{
public:
	
	typedef T					value_type;
	typedef T&					reference;
	typedef const value_type &	const_reference;
	
	typedef std::ptrdiff_t		difference_type;
	typedef SegmentSizeType		size_type;

	// internal:
	typedef std::list<T*, A>	segment_container_type;
	typedef std::list<const T*, A> const_segment_container_type;
	typedef typename segment_container_type::const_iterator segment_iterator_type;
	
	typedef segmented_list_iterator<T, segment_iterator_type, SegmentSize, A> iterator;
	typedef segmented_list_iterator<const T, segment_iterator_type, SegmentSize, A> const_iterator;

	bool operator == (const segmented_list &rhs) const
	{
		bool rc;
		rc = m_size == rhs.m_size;
		if (rc)
		{
			auto iter_l = m_segment_container.begin();
			auto iter_r = rhs.m_segment_container.begin();
			size_type items_left_to_compare = m_size;
			while (items_left_to_compare && rc)
			{
				size_type items_to_compare_in_this_container = std::min<size_type>(items_left_to_compare, SegmentSize);
				items_left_to_compare -= items_to_compare_in_this_container;
				rc = std::equal(*iter_l, *iter_l + items_to_compare_in_this_container, *iter_r);
				++iter_l;
				++iter_r;
			}
		}
		
		return rc;
	}

	bool operator != (segmented_list &rhs) const
	{
		return !(*this == rhs);
	}
	
	T& front()
	{
		return begin()[0];
	}

	const T&front() const
	{
		return begin()[0];
	}

	const T&back() const
	{
		return begin()[m_size - 1];		// TODO: faster way by looking at end
	}

	T&back()
	{
		return begin()[m_size - 1];		// TODO: faster way by looking at end
	}

	iterator begin() 
	{
		return{ m_segment_container.begin(), 0, 0 };
	}

	const_iterator begin() const
	{
		return{ m_segment_container.begin(), 0, 0 };
	}

	const_iterator cbegin() const { return{ m_segment_container.begin(), 0, 0 }; }
	const_iterator cend()	const { return end(); }

	T& operator[] (size_type i) 
	{
		return begin()[i];
	}

	template <typename iterator_type>
	iterator_type find_end(iterator_type begin) const
	{
		// figuring out the end without locking the writer isn't that easy
		// if i can grab the end use it. otherwise fall back to linear search
		std::atomic<size_type> size_before;
		size_before.store(m_size);
		auto the_end = m_segment_container.end();
		
		if (size_before == m_size) // <-- if this is true, then the_end I got is a good one
		{
			the_end--;
			iterator_type it2{ the_end, size_before / SegmentSize, size_before };
			return it2;
		}
		else
		{
			iterator_type it(begin);
			it.change_index(m_size);
			return it;
		}
	}

	const_iterator end() const
	{
		return find_end<const_iterator>(begin());
	}

	iterator end() 
	{
		return find_end<iterator>(begin());
	}

	explicit segmented_list(A alloc = A())
		:	m_segment_container(alloc),
			m_size(0)
	{
		add_segment();
	}
  
	explicit segmented_list(size_type count, const A& alloc = A())
		: m_segment_container(alloc)
	{
		size_type num_required_segments = count / SegmentSize + 1;
		for (size_t i = m_segment_container.size(); i < num_required_segments; i++)
		{
			add_segment();
		}
		m_size = count;
	}

	segmented_list(const segmented_list &rhs)			// "select on container copy construction" is to choose the correct allocator to use
		: m_segment_container(std::allocator_traits<A>::select_on_container_copy_construction(rhs.get_allocator()))
	{
		m_segment_container.clear();
		copy_segments(*this, rhs); // copy_segments updates m_size
	}

	explicit segmented_list(const segmented_list &rhs, const A& alloc)
		: m_segment_container(alloc)
	{
		m_segment_container.clear();
		copy_segments(*this, rhs); // copy_segments updates m_size
	}

	// If alloc is not provided, allocator is obtained by move - construction from the allocator belonging to other.
	segmented_list(segmented_list &&rhs)			// "select on container copy construction" is to choose the correct allocator to use
		: m_segment_container(std::move(rhs.m_segment_container))
	{
		size_type tmp = rhs.m_size;
		m_size = tmp;
		rhs.m_segment_container.clear();
		rhs.m_size = 0;
	}

	segmented_list(segmented_list &&rhs, const A& alloc)
		:
		m_segment_container(rhs.m_segment_container, alloc)  
	{
		log_printf("the one i wanted to test");
		size_type tmp = rhs.m_size;
		m_size = tmp;
		rhs.m_segment_container.clear();
		rhs.m_size = 0;
	}

	template< class InputIt >
	segmented_list(InputIt first, InputIt last, const A& alloc = A())
		: m_segment_container(alloc),
			m_size(0)
	{
		add_segment();
		while (first != last)
		{
			emplace_back(*first);
			first++;
		}
	}

	segmented_list(std::initializer_list<T> l, const A& alloc = A())
		: segmented_list(l.begin(), l.end(), alloc)
	{}

	~segmented_list()
	{
		for (auto iter = m_segment_container.begin();
			iter != m_segment_container.end();
			iter++)
		{
			get_allocator().deallocate(*iter, SegmentSize);
		}
	}

	void clear()
	{
		// erase everything but one
		auto erase_start = m_segment_container.begin();
		erase_start++;
		for (auto iter = erase_start; iter != m_segment_container.end(); iter++)
		{
			get_allocator().deallocate(*iter, SegmentSize);
		}
		m_segment_container.erase(erase_start, m_segment_container.end());
		m_size = 0;
	}

	void copy_segments(segmented_list& lhs, const segmented_list& rhs)
	{
		lhs.m_size.store(rhs.m_size);

		for (auto segment = rhs.m_segment_container.begin(); segment != rhs.m_segment_container.end(); segment++)
		{
			const value_type *dest = *segment;
			T *buf = get_allocator().allocate(SegmentSize);
			memcpy(buf, dest, SegmentSize * sizeof(value_type));	// copy data from rhs segment to lhs buf
			lhs.m_segment_container.push_back(buf);						// add data onto lhs container
		}
	}

	segmented_list& operator=(const segmented_list& rhs) 
	{
		if (this != &rhs) { 
			m_segment_container.clear();
			copy_segments(*this, rhs);
		}
		return *this;
	}

	segmented_list& operator=(segmented_list&& rhs)
	{
		if (this != &rhs) {
			m_segment_container = std::move(rhs.m_segment_container);
			size_type tmp = m_size;
			m_size.store(rhs.m_size);
			rhs.m_segment_container.clear();
			rhs.m_size = 0;
		}
		return *this;
	}

	void swap(segmented_list& rhs)
	{
		size_type tmp = m_size;
		m_size.store(rhs.m_size);
		rhs.m_size.store(tmp);
		m_segment_container.swap(rhs.m_segment_container);
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_type size() const 
	{
		return m_size;
	}

	size_type max_size() const
	{
		return std::min(
			std::numeric_limits<size_type>::max() / sizeof(value_type),	// memory size
			m_segment_container.max_size() * SegmentSize); // max number of segments limited by segment container
	}

	A get_allocator() const
	{
		return m_segment_container.get_allocator();
	}

	void add_segment()
	{
		T* segment = get_allocator().allocate(SegmentSize);
		if (!segment)
		{
			ABORT("alloc failed");
		}
		m_segment_container.emplace_back(segment);
	}

	void grow_if_necessary()
	{
		if ((m_size + 1) % SegmentSize == 0)
		{
			add_segment();
		}
	}

	
	

	void reserve(size_type new_cap)
	{
		// todo: support reserve.  can't do it because
		// some code (like emplace back and pushback below)
		// assume indices into the last container
	}

	template<typename... Args> 
	void emplace_back(Args&&... args)
	{
		T* buf = &m_segment_container.back()[m_size % SegmentSize];
		new(buf) T(std::forward<Args>(args)...);
		grow_if_necessary();
		m_size++;
	}

	void push_back(const T& value)
	{
		T* buf = &m_segment_container.back()[m_size % SegmentSize];
		new(buf) T(value);
		//*buf = value;
		grow_if_necessary();
		m_size++;
	}

private:
	segment_container_type		m_segment_container;
	std::atomic<size_type>		m_size;

};

// difference type - a type that can hold the distance between two iterators
template <typename T, typename SegmentContainerIterator, uint32_t SegmentSize, typename A>
struct segmented_list_iterator : std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t>
{
	typedef size_t			size_type;
	typedef std::ptrdiff_t		difference_type;
	typedef T&				reference;

	SegmentContainerIterator _segment;			// pointer to current segment
	size_type				 _segment_index;		// 0..n-1 segment index
	size_type				 _listwide_index;				// index into the entire list

	segmented_list_iterator()
	{}

	segmented_list_iterator(const SegmentContainerIterator &segment, size_type segment_index, size_type listwide_index)
		: _segment(segment), _segment_index(segment_index), _listwide_index(listwide_index)
	{}

	segmented_list_iterator(const segmented_list_iterator &rhs)
		: _segment(rhs._segment), _segment_index(rhs._segment_index), _listwide_index(rhs._listwide_index)
	{
	}

	segmented_list_iterator& operator=(const segmented_list_iterator &rhs)
	{
		_segment = rhs._segment;
		_segment_index = rhs._segment_index;
		_listwide_index = rhs._listwide_index;
		return *this;
	}

	segmented_list_iterator& operator=(segmented_list_iterator &&rhs)
	{
		_segment = rhs._segment;
		_segment_index = rhs._segment_index;
		_listwide_index = rhs._listwide_index;
		return *this;
	}

	// adjust segment for the current index value
	void change_index(size_type listwide_index)
	{
		_listwide_index = listwide_index;
		size_type correct_segment = listwide_index / SegmentSize;
		while (_segment_index < correct_segment)
		{
			_segment++;
			_segment_index++;
		}
		while (_segment_index > correct_segment)
		{
			_segment--;
			_segment_index--;
		}
	}
	

	segmented_list_iterator operator+(difference_type n) const
	{
		segmented_list_iterator tmp(*this);
		tmp.change_index(tmp._listwide_index + n);
		return tmp;
	}

	friend segmented_list_iterator operator+(segmented_list_iterator lhs, const segmented_list_iterator& rhs)
	{
		lhs += rhs; // reuse compound assignment
		return lhs; // return the result by value (uses move constructor)
	}


	segmented_list_iterator &operator+=(difference_type n)
	{
		change_index(_listwide_index + n);
		return *this;
	}

	segmented_list_iterator &operator++()
	{
		change_index(_listwide_index + 1);
		return *this;
	}

	segmented_list_iterator operator++(int)
	{
		segmented_list_iterator tmp(*this);
		change_index(_listwide_index + 1);
		return tmp;
	}

	segmented_list_iterator &operator-=(difference_type n)
	{
		change_index(_listwide_index - n);
		return *this;
	}

	difference_type operator-(const segmented_list_iterator &rhs) const
	{
		difference_type ret = _listwide_index - rhs._listwide_index;
		return ret;
	}

	segmented_list_iterator operator-(difference_type n) const
	{
		segmented_list_iterator tmp(*this);
		tmp.change_index(tmp._listwide_index - n);
		return tmp;
	}


	segmented_list_iterator operator--(int)
	{
		segmented_list_iterator tmp(*this);
		change_index(_listwide_index - 1);
		return tmp;
	}

	segmented_list_iterator& operator--()
	{
		change_index(_listwide_index - 1);
		return *this;
	}

	T * operator->()
	{
		T* base_addr = *_segment;
		return base_addr + _listwide_index % SegmentSize;
	}

	const T * operator->() const
	{
		T* base_addr = *_segment;
		return base_addr + _listwide_index % SegmentSize;
	}

	T & operator *()
	{
		T* base_addr = *_segment;
		return *(base_addr + _listwide_index % SegmentSize);
	}

	const T & operator *() const
	{
		const T* base_addr = *_segment;
		return *(base_addr + _listwide_index % SegmentSize);
	}

	bool operator != (segmented_list_iterator &rhs) const
	{
		return _listwide_index != rhs._listwide_index;
	}

	reference operator[](difference_type n) const
	{	
		return (*(*this + n));
	}

	bool operator < (const segmented_list_iterator &rhs) const
	{
		return _listwide_index < rhs._listwide_index;
	}
	
	bool operator > (const segmented_list_iterator &rhs) const
	{
		return _listwide_index > rhs._listwide_index;
	}

	bool operator >= (const segmented_list_iterator &rhs) const
	{
		return _listwide_index >= rhs._listwide_index;
	}

	bool operator <= (const segmented_list_iterator &rhs) const
	{
		return _listwide_index <= rhs._listwide_index;
	}
};



template <typename T, typename SegmentContainerIterator, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator==(const segmented_list_iterator<T, SegmentContainerIterator,SegmentSize,A>& a,
	const  segmented_list_iterator<T, SegmentContainerIterator, SegmentSize, A>& b)
{
	return a._listwide_index == b._listwide_index;
}

template <typename T, typename SegmentContainerIterator, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator!=(const segmented_list_iterator<T, SegmentContainerIterator, SegmentSize, A>& a,
	const  segmented_list_iterator<T, SegmentContainerIterator, SegmentSize, A>& b)
{
	return a._listwide_index != b._listwide_index;
}

template <typename T,uint32_t SegmentSize, typename A = std::allocator<T>>
void swap(segmented_list<T, SegmentSize, A> &a, 
	segmented_list<T, SegmentSize, A> &b)
{
	a.swap(b);
}
