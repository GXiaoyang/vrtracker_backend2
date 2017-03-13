#pragma once 
#include <assert.h>
#include <mutex>
#include <vector>

template <size_t FixedSizeBytes>
struct tmp_vector_pool
{
	static const size_t FixedSizeBytes = FixedSizeBytes;

	tmp_vector_pool()
		: 
		m_alloc_one_calls(0),
		m_free_one_calls(0)
	{}

	~tmp_vector_pool()
	{
		for (auto ptr : string_pool)
		{
			free(ptr);
		}
	}

	char *AllocOne()
	{
		char *ret;
		pool_mutex.lock();
		m_alloc_one_calls++;

		if (string_pool.empty())
		{
			ret = (char *)malloc(FixedSizeBytes);
			assert(ret);
		}
		else
		{
			ret = string_pool.back();
			string_pool.pop_back();
		}
		pool_mutex.unlock();

		return ret;
	}

	void FreeOne(char *s)
	{
		pool_mutex.lock();
		m_free_one_calls++;
		for (int i = 0; i < (int)string_pool.size(); i++)
		{
			// make sure duplicates don't appear back in the string pool
			assert(string_pool[i] != (char *)s);
		}
		string_pool.push_back((char *)s);

		pool_mutex.unlock();
	}

	int get_num_alloc_one_calls() const {
		return m_alloc_one_calls;
	}
	int get_num_free_one_calls() const {
		return m_free_one_calls;
	}
private:
	int m_alloc_one_calls;
	int m_free_one_calls;
	std::mutex pool_mutex;
	std::vector<char *> string_pool;
};

template <typename T, typename FinalAllocatorType, size_t FixedSizeBytes>
struct tmp_vector
{
	typedef T value_type;
	typedef size_t size_type;
private:
	T *m_s;
	tmp_vector_pool<FixedSizeBytes> *m_pool;
	size_type m_count;
	FinalAllocatorType m_final_allocator;		// when converting to vectors - where to allocate the memory - see cast below


public:
	tmp_vector()
		:	m_pool(nullptr),
			m_count(0),
			m_s(0)
	{
	}

	tmp_vector(tmp_vector_pool<FixedSizeBytes> *pool, const FinalAllocatorType &final_allocator)
		:
		m_pool(pool),
		m_count(0),
		m_final_allocator(final_allocator)
	{
		m_s = (T*)m_pool->AllocOne();
	}
	~tmp_vector()
	{
		if (m_s)
		{
			m_pool->FreeOne((char *)m_s);
		}
	}

	tmp_vector(tmp_vector &&rhs)
		:
		m_s(rhs.m_s),
		m_pool(rhs.m_pool),
		m_count(rhs.m_count),
		m_final_allocator(rhs.m_final_allocator)
	{
		rhs.m_s = nullptr;
	}

	tmp_vector & operator = (tmp_vector &&rhs)
	{
		m_s		= rhs.m_s;
		m_pool	= rhs.m_pool;
		m_count = rhs.m_count;
		m_final_allocator = rhs.m_final_allocator;
		
		rhs.m_s = nullptr;
		
		return *this;
	}

	tmp_vector(const tmp_vector &rhs) = delete;// dangerous - do I want to have copies of tempoaries
	tmp_vector& operator=(const tmp_vector&) = delete;

	T * data() { return m_s; }
	const T * data() const { return m_s; }

	const T * begin() const { return m_s; }
	const T * end() const { return m_s+m_count; }

	T * first() { return m_s; }
	T * last() { return m_s + m_count; }

	const T * first() const { return m_s; }
	const T * last() const { return m_s + m_count; }

	size_type max_size() const { return FixedSizeBytes / sizeof(T); }
	size_type size() const { return m_count; }
	void resize(size_type count) { m_count = count;  }
	void clear() { m_count = 0; }

	T & operator[] (int pos) { return m_s[pos]; }
	const T & operator[] (int pos) const { return m_s[pos]; }

	void push_back(const T&ref)
	{
		m_s[m_count++] = ref;
	}

#if 0
	template <typename A> 
	explicit operator std::vector<T,A>() const {
		return std::vector<T,A>(begin(), end(), m_final_allocator);
	}
#endif
};


