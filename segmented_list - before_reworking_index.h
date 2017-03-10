#pragma once 
#include <list>
#include <algorithm>

// i prefer it to be contiguous on reload. so thats what the copy constructor does

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
		size_type left_container_size = m_segment_container.size();
		size_type right_container_size = rhs.m_segment_container.size();
		bool rc = left_container_size == right_container_size;
		if (rc)
		{
			rc = m_num_elements_in_last_segment == rhs.m_num_elements_in_last_segment;
			if (rc)
			{
				auto iter_l = m_segment_container.begin();
				auto iter_r = rhs.m_segment_container.begin();
				for (size_type i = 0; rc && i < left_container_size; i++)
				{
					size_type num_values_to_compare;
					if (i == left_container_size - 1)	// last container, compare only remaining elements
					{
						num_values_to_compare = m_num_elements_in_last_segment;
					}
					else
					{
						num_values_to_compare = SegmentSize;
					}
					rc = std::equal(*iter_l, *iter_l + num_values_to_compare, *iter_r);
				}
			}
		}
		return rc;
	}

	bool operator != (segmented_list &rhs) const
	{
		return !(*this == rhs);
	}
	

	const_iterator cbegin() { return begin(); }
	const_iterator cend()	{ return end(); }

	
	iterator begin()
	{
		return { m_segment_container.begin(), 0, m_segment_container.front() };
	}

	iterator end()
	{
		T* ptr = m_segment_container.back() + m_num_elements_in_last_segment;
		auto last_iter = m_segment_container.end();
		iterator ret(--last_iter, m_segment_container.size()-1, ptr);
		return ret;
	}

	segmented_list()
		: m_num_elements_in_last_segment(0)
	{
		add_segment();
	}


	// count   numelements
	//  0       0
	// 1024     
	explicit segmented_list(size_type count, const A& alloc = std::allocator<T>())
		: m_segment_container(alloc)
	{
		size_type num_required_segments = count / SegmentSize + 1;
		T *buf = get_allocator().allocate(num_required_segments * SegmentSize);
		for (size_t i = 0; i < num_required_segments; i++)
		{
			m_segment_container.push_back(buf);
			buf += SegmentSize;
		}
		m_num_elements_in_last_segment = count % SegmentSize;
	}

	segmented_list(segmented_list &rhs)
		:	m_segment_container(std::allocator_traits<A>::select_on_container_copy_construction(rhs.get_allocator()))
	{
		m_segment_container.clear();
		copy_segments(*this, rhs);
	}

	void clear()
	{
		// erase everything but one
		auto erase_start = m_segment_container.begin();
		erase_start++;
		m_segment_container.erase(erase_start, m_segment_container.end());
		m_num_elements_in_last_segment = 0;
	}

	void copy_segments(segmented_list& lhs, const segmented_list& rhs)
	{
		lhs.m_num_elements_in_last_segment = rhs.m_num_elements_in_last_segment;
		size_type required_size = rhs.m_segment_container.size() * SegmentSize * sizeof(value_type);
		T *buf = get_allocator().allocate(required_size);
		
		for (auto segment = rhs.m_segment_container.begin(); segment != rhs.m_segment_container.end(); segment++)
		{
			const value_type *dest = *segment;
			memcpy(buf, dest, SegmentSize * sizeof(value_type));	// copy data from rhs segment to lhs buf
			lhs.m_segment_container.push_back(buf);						// add data onto lhs container
			buf += SegmentSize;
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
		std::swap(m_num_elements_in_last_segment, rhs.m_num_elements_in_last_segment);
		m_segment_container.swap(rhs.m_segment_container);
	}

	bool empty() const
	{
		return m_segment_container.size() == 1 && m_num_elements_in_last_segment == 0;
	}

	size_t size() const
	{
		if (m_num_elements_in_last_segment == SegmentSize)
		{
			return (m_segment_container.size() - 2) * SegmentSize + m_num_elements_in_last_segment;
		}
		else
		{
			return (m_segment_container.size() - 1 )* SegmentSize + m_num_elements_in_last_segment;
		}
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

	template<typename... Args> 
	void emplace_back(Args&&... args)
	{
		T* buf = &m_segment_container.back()[m_num_elements_in_last_segment];
		new(buf) T(std::forward<Args>(args)...);
		m_num_elements_in_last_segment++;
		if (m_num_elements_in_last_segment == SegmentSize)
		{
			add_segment();
			m_num_elements_in_last_segment = 0;
		}
	}

private:
	segment_container_type	m_segment_container;
	int						m_num_elements_in_last_segment;
};

// difference type - a type that can hold the distance between two iterators
template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
struct segmented_list_iterator : std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t>
{
	typedef size_t size_type;
	typedef typename segmented_list<T, SegmentSize, A>::segment_iterator_type segment_iterator_type;

	segment_iterator_type	_segment;
	size_type				_index;
	

	segmented_list_iterator(const segment_iterator_type &segment, size_type segment_index, T*ptr)
		: _segment(segment), _segment_index(segment_index), _ptr(ptr)
	{}

	size_type offset_in_segment() const
	{
		return _ptr - *_segment;
	}
	bool is_last_element_in_segment() const
	{
		return (offset_in_segment() == SegmentSize - 1);
	}
	bool is_first_element_in_segment() const { return _ptr - *_segment == 0; }

	void move_to_next_segment()
	{
		_segment++;
		_segment_index++;
	}
	void move_to_prev_segment()
	{
		_segment--;
		_segment_index--;
	}

	
	difference_type operator-(const &segmented_list_iterator) const
	{
		// maybe just keep a real index.
	}

	segmented_list_iterator operator+(difference_type n) const
	{
		segmented_list_iterator tmp(*this);
		tmp += n;
		return tmp;
	}

	segmented_list_iterator &operator+=(difference_type n)
	{
		if (n < 0)
		{
			return *this -= (-n);
		}
		size_type current_offset = offset_in_segment();
		size_type next_offset = n + current_offset;
		while (next_offset > SegmentSize-1)
		{
			move_to_next_segment();
			next_offset -= SegmentSize;
		}
		_ptr = *_segment + next_offset;
		return *this;
	}

	segmented_list_iterator &operator++()
	{
		if (is_last_element_in_segment())
		{
			move_to_next_segment();
			_ptr = *_segment;
		}
		else
		{
			_ptr++;
		}
		return *this;
	}

	segmented_list_iterator operator++(int)
	{
		segmented_list_iterator tmp(*this);
		if (is_last_element_in_segment())
		{
			move_to_next_segment();
			_ptr = *_segment;
		}
		else
		{
			_ptr++;
		}
		return tmp;
	}

	segmented_list_iterator &operator-=(difference_type n)
	{
		if (n < 0)
		{
			return *this += (-n);
		}
		size_type current_offset = offset_in_segment();
		while (size_type(n) > current_offset)
		{
			move_to_prev_segment();
			current_offset += SegmentSize;
		}
		_ptr = *_segment + current_offset - n;

		return *this;
	}

	segmented_list_iterator operator-(difference_type n) const
	{
		segmented_list_iterator tmp(*this);
		tmp -= n;
		return tmp;
	}


	segmented_list_iterator operator--(int)
	{
		segmented_list_iterator tmp(*this);
		if (is_first_element_in_segment())
		{
			move_to_prev_segment();
			_ptr = *_segment + SegmentSize - 1;
		}
		else
		{
			_ptr--;
		}
		return tmp;
	}

	segmented_list_iterator operator--()
	{
		if (is_first_element_in_segment())
		{
			move_to_prev_segment();
			_ptr = *_segment + SegmentSize - 1;
		}
		else
		{
			_ptr--;
		}
	}

	T & operator *()
	{
		return *_ptr;
	}

	bool operator != (segmented_list_iterator &rhs) const
	{
		return _ptr != rhs._ptr;
	}

	reference operator[](difference_type n) const
	{	
		return (*(*this + n));
	}

	bool operator < (const segmented_list_iterator &rhs) const
	{
		if (_segment_index == rhs._segment_index)
			return _ptr < rhs._ptr;
		else
			return _segment_index < rhs._segment_index;
	}
	
	bool operator > (const segmented_list_iterator &rhs) const
	{
		return !(*this < rhs);
	}

	bool operator >= (const segmented_list_iterator &rhs) const
	{
		return (*this > rhs) || (*this == rhs);
	}
};


template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator==(const segmented_list_iterator<T,SegmentSize,A>& a,
	const  segmented_list_iterator<T, SegmentSize, A>& b)
{
	return a._ptr == b._ptr;
}

template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
inline bool operator!=(const segmented_list_iterator<T, SegmentSize, A>& a,
	const  segmented_list_iterator<T, SegmentSize, A>& b)
{
	return a._ptr != b._ptr;
}

template <typename T, uint32_t SegmentSize, typename A = std::allocator<T>>
void swap(segmented_list<T, SegmentSize, A> &a, segmented_list<T, SegmentSize, A> &b)
{
	a.swap(b);
}
