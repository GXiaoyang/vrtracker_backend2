#pragma once 
#include <list>
#include <algorithm>
#include <atomic>
#include <thread>
#include <assert.h>

// i prefer it to be contiguous on reload. so thats what the copy constructor does

class spin_lock {
	std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
	void lock() {
		while (locked.test_and_set(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}
	}
	void unlock() {
		locked.clear(std::memory_order_release);
	}
};



template <typename T, uint32_t SegmentSize, typename A>
struct segmented_list_iterator;

template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
struct segmented_list
{
	
	typedef T					value_type;
	typedef T&					reference;
	typedef const value_type &	const_reference;
	
	typedef ptrdiff_t			difference_type;
	typedef size_t				size_type;

	

	typedef segmented_list_iterator<T, SegmentSize, A> iterator;
	typedef const segmented_list_iterator<T, SegmentSize, A> const_iterator;

	// internal:
	typedef std::list<T*, A>	segment_container_type;
	typedef typename segment_container_type::iterator segment_iterator_type;


	bool operator == (const segmented_list &rhs) const
	{
		bool rc;
		rc = m_size == rhs.m_size;
		if (rc)
		{
			auto iter_l = m_segment_container.begin();
			auto iter_r = rhs.m_segment_container.begin();
			size_t left_container_size = m_size;
			for (size_type i = 0; rc && i < left_container_size; i++)
			{
				size_type num_values_to_compare;
				if (i == left_container_size - 1)	// last container, compare only remaining elements
				{
					num_values_to_compare = (m_size-1) % SegmentSize + 1;
				}
				else
				{
					num_values_to_compare = SegmentSize;
				}
				rc = std::equal(*iter_l, *iter_l + num_values_to_compare, *iter_r);
			}
		}
		
		return rc;
	}

	bool operator != (segmented_list &rhs) const
	{
		return !(*this == rhs);
	}
	

	const_iterator cbegin() const { return{ m_segment_container.begin(), 0, 0 }; }
	const_iterator cend()	const { return end(); }

	
	iterator begin() 
	{
		return{ m_segment_container.begin(), 0, 0 };
	}

	T& operator[] (size_type i) 
	{
		return begin()[i];
	}

	

	iterator end()
	{
		// figuring out the end without locking the writer isn't that easy
		// if i can grab the end use it. otherwise fall back to linear search
		std::atomic<int> size_before;
		size_before.store(m_size);
		auto the_end = m_segment_container.end();
		if (size_before == m_size) // <-- if this is true, then the_end I got is a good one
		{
			the_end--;
			iterator it2{ the_end, size_before/SegmentSize, size_before  };
			return it2;
		}
		else
		{
			iterator it(begin());
			it.change_index(m_size);
			return it;
		}
	}

	segmented_list()
		: m_size(0)
	{
		add_segment();
	}

	explicit segmented_list(const A& alloc)
		:	m_segment_container(alloc),
			m_size(0)
	{
		add_segment();
	}
  
	explicit segmented_list(size_type count, const A& alloc = std::allocator<T>())
		: m_segment_container(alloc)
	{
		size_type num_required_segments = count / SegmentSize + 1;
		for (size_t i = 0; i < num_required_segments; i++)
		{
			T *buf = get_allocator().allocate(SegmentSize);
			m_segment_container.push_back(buf);
		}
		m_size = count;
	}

	segmented_list(segmented_list &rhs)
		: m_segment_container(std::allocator_traits<A>::select_on_container_copy_construction(rhs.get_allocator()))
	{
		m_segment_container.clear();
		copy_segments(*this, rhs);
	}

	template< class InputIt >
	segmented_list(InputIt first, InputIt last, const A& alloc = std::allocator<T>())
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

	segmented_list(std::initializer_list<T> l)
		: segmented_list(l.begin(), l.end())
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

	size_t size() const
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
		m_segment_container.emplace_back(get_allocator().allocate(SegmentSize));
	}

	void grow_if_necessary()
	{
		if ((m_size + 1) % SegmentSize == 0)
		{
			add_segment();
		}
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
		*buf = value;
		grow_if_necessary();
		m_size++;
	}

private:
	segment_container_type		m_segment_container;
	std::atomic<size_type>		m_size;
};

// difference type - a type that can hold the distance between two iterators
template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
struct segmented_list_iterator : std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t>
{
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;
	typedef T&				reference;

	typedef typename segmented_list<T, SegmentSize, A>::segment_iterator_type segment_iterator_type;

	segment_iterator_type	_segment;			// pointer to current segment
	size_type				_segment_index;		// 0..n-1 segment index
	size_type				_listwide_index;				// index into the entire list

	segmented_list_iterator(const segment_iterator_type &segment, size_type segment_index, size_type listwide_index)
		: _segment(segment), _segment_index(segment_index), _listwide_index(listwide_index)
	{}

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

	T & operator *()
	{
		T* base_addr = *_segment;
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


template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator==(const segmented_list_iterator<T,SegmentSize,A>& a,
	const  segmented_list_iterator<T, SegmentSize, A>& b)
{
	return a._listwide_index == b._listwide_index;
}

template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator!=(const segmented_list_iterator<T, SegmentSize, A>& a,
	const  segmented_list_iterator<T, SegmentSize, A>& b)
{
	return a._listwide_index != b._listwide_index;
}

template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
void swap(segmented_list<T, SegmentSize, A> &a, segmented_list<T, SegmentSize, A> &b)
{
	a.swap(b);
}
