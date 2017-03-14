// test_segmented_list
// * unit test for segmented_list
//

#include "segmented_list.h"
#include "slab_allocator.h"
#include <assert.h>

// segmented list
#include <thread>
#include <list>
#include <vector>
#include <numeric>
#include <iterator>
#include <deque>
#include <chrono>
#include <thread>

// fixed size allocator with memory filled with a known value
// used by the threading test to detect for uninitialized memory read
template <class T>
struct SimpleAllocator {
	typedef T value_type;
	SimpleAllocator(/*ctor args*/)
	{}

	template <class U> SimpleAllocator(const SimpleAllocator<U>& other)
	{
	}

	T* allocate(std::size_t n)
	{
		T* ret = (T*)malloc(sizeof(T) * n);
		memset(ret, 7, sizeof(T) * n);
		return ret;
	}
	void deallocate(T* p, std::size_t n)
	{
		free(p);
	}
};



template <typename ListT>
void reader(int max_values, int expected_value, int termination_value, 
	ListT *seg)
{
	int num_unique_reads = 0;
	while (seg->size() == 0)
		;

	while (1)
	{
		for (auto iter = seg->begin(); iter != seg->end(); iter++)
		{
			int value = *iter;
			assert(value == expected_value || value == termination_value);
			if (value == termination_value)
			{
				size_t container_size = seg->size();
				assert(container_size == max_values);
				return;
			}
		}
	}
}


template <typename ListT>
void writer(int fast, int max_values, int expected_value, int termination_value, 
	ListT *seg)
{
	using namespace std::chrono_literals;
	for (int i = 0; i < max_values; i++)
	{
		if (i == max_values - 1)
			seg->emplace_back(termination_value);
		else
			seg->emplace_back(expected_value);
		if (fast)
		{
			//std::this_thread::sleep_for(1ms);
		}
		else
		{
			std::this_thread::sleep_for(50us);
		}

	}
}

// sets up a shared list with a single writer (fast or slow) and a set of
// readers (1..10) and verifies no reader reads too far ahead and that the
// threads get terminated (ie, the readers eventually catch up).
//
// caller can set the SegmentSize to test different segment sizes
template <size_t SegmentSize>
void read_ordering_test_int()
{
	static const int NUM_WRITES = 102;
	static const int expected_value = 7;
	static const int termination_value = 8;

	for (int fast_writer = 0; fast_writer < 2; fast_writer++)
	{
		for (int num_readers = 1; num_readers < 10; num_readers++)
		{
			SimpleAllocator<int> ss;
			typedef segmented_list<int, SegmentSize, SimpleAllocator<int>> list_t;
			//typedef std::list<int, SimpleAllocator<int>> list_t;
			list_t shared_list(ss);
			std::vector<std::thread*> threads;
			printf("threading test. SegmentSize: %d starting with fast: %d num_readers %d\n", SegmentSize, fast_writer, num_readers);
			for (int i = 0; i < num_readers; i++)
			{
				std::thread *t = new std::thread(reader<list_t>, NUM_WRITES, expected_value, termination_value, &shared_list);
				threads.push_back(t);
			}
			std::thread *w = new std::thread(writer<list_t>, fast_writer, NUM_WRITES, expected_value, termination_value, &shared_list);
			threads.emplace_back(w);
			int num_joined = 0;
			for (auto iter = threads.begin(); iter != threads.end(); iter++)
			{
				(*iter)->join();
				num_joined++;
				printf("joined: %d\n", num_joined);
				delete *iter;
			}
		}	
	}
}



template <size_t N>
void initializer_test_iter_constructor()
{
	{
		std::vector<std::string> ref({});
		segmented_list<std::string, N> segmented_list0(ref.begin(), ref.end());
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	{
		std::vector<std::string> ref({ "a", "b" });
		segmented_list<std::string, N> segmented_list0(ref.begin(), ref.end());
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}

	{
		std::vector<std::string> ref({ "a", "b", "c" });
		segmented_list<std::string, N> segmented_list0(ref.begin(), ref.end());
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	
}

template <size_t N>
void initializer_test_string()
{
	{
		std::vector<std::string> ref({});
		segmented_list<std::string, N> segmented_list0({});
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	{
		std::vector<std::string> ref({ "a", "b" });
		segmented_list<std::string,N> segmented_list0({ "a", "b" });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	{
		std::vector<std::string> ref({ "a", "b", "c" });
		segmented_list<std::string, N> segmented_list0({ "a", "b", "c" });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	
}

template <size_t N>
void initializer_test()
{
	{
		std::vector<int> ref({});
		segmented_list<int, N> segmented_list0({});
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}

	{
		std::vector<int> ref({1});
		segmented_list<int, N> segmented_list0({ 1 });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}

	{
		std::vector<int> ref({ 1,2 });
		segmented_list<int, N> segmented_list0({ 1,2 });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}

	{
		std::vector<int> ref({ 1,2,3 });
		segmented_list<int, N> segmented_list0({ 1,2,3 });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}

	{
		std::vector<int> ref({ 1,2,3,4 });
		segmented_list<int, N> segmented_list0({ 1,2,3,4 });
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
}


template <typename I, typename T>
I last_item_less_than_or_equal_to(I begin, I end, const T& val)
{
	I it = upper_bound(begin, end, val);

	if (it != begin) {
		it--; // not at end of array so rewind to previous item
	}
	else {
		it = end; // no items before this point, so return end()
	}
	return it;
}

template <typename I>
I first_item_greater_than_or_equal_to(I begin, I end, int val)
{
	I it = upper_bound(begin, end, val);

	if (it != begin) {
		if (*(it - 1) == val)
			it--;
	}
	return it;
}

template<typename T, size_t N>
void binary_search_test()
{
	{
		int randomish_size = 1024 + (rand() % 50) - 25;
		std::vector<int> ref;
		ref.reserve(randomish_size);
		for (int i = 0; i < randomish_size; i++)
		{
			T val = rand();
			ref.emplace_back(val);
		}

		std::sort(ref.begin(), ref.end());

		segmented_list<T, N> segmented_list0;
		std::copy(ref.begin(), ref.end(), 
			std::back_inserter(segmented_list0));
		
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));

		for (int i = 0; i < 1024; i++)
		{
			T val = rand();
			auto ref_first = first_item_greater_than_or_equal_to(ref.begin(), ref.end(), val);
			auto seg_first = first_item_greater_than_or_equal_to(segmented_list0.begin(), segmented_list0.end(), val);
			if (ref_first == ref.end())
			{
				assert(seg_first == segmented_list0.end());
			}
			else
			{
				T a = *ref_first;
				T b = *seg_first;
				assert(a == b);
			}

			auto ref_last = last_item_less_than_or_equal_to(ref.begin(), ref.end(), val);
			auto seg_last = last_item_less_than_or_equal_to(segmented_list0.begin(), segmented_list0.end(), val);
			if (ref_last == ref.end())
			{
				assert(seg_last == segmented_list0.end());
			}
			else
			{
				T a = *ref_last;
				T b = *seg_last;
				assert(a == b);
			}
		}
	}

}

//
template<typename T, size_t N>
void sort_test()
{
	static const int list_size = 1024;
	{// tests are using 1024 because caller will set segment sizes around that boundary
		segmented_list<T, N> segmented_list0;
		std::vector<T> ref;

		for (int i = 0; i < list_size; i++)
		{
			segmented_list0.emplace_back(i);
			ref.emplace_back(i);
		}

		// check distance
		assert(ref.end() - ref.begin() == list_size);
		assert(segmented_list0.end() - segmented_list0.begin() == list_size);

		
		std::sort(ref.begin(), ref.end());
		std::sort(segmented_list0.begin(), segmented_list0.end());
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	

	{
		// sort randomized container
		segmented_list<T, N> segmented_list0;
		std::vector<T> ref;
		for (int i = 0; i < list_size; i++)
		{
			T val = rand();
			segmented_list0.emplace_back(val);
			ref.emplace_back(val);
		}
		
		std::sort(ref.begin(), ref.end());
		std::sort(segmented_list0.begin(), segmented_list0.end());
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));
	}
	
	{
		// fill test
		segmented_list<T, N> segmented_list0(list_size);
		std::vector<T> ref(list_size);
		fill(ref.begin(), ref.end(), 42);
		fill(segmented_list0.begin(), segmented_list0.end(), 42);
		assert(std::equal(ref.begin(), ref.end(), segmented_list0.begin()));

		int ref_count = count(ref.begin(), ref.end(), 42);
		int list_count = count(segmented_list0.begin(), segmented_list0.end(), 42);
		assert(ref_count == list_count);
	}

	{
		segmented_list<T, N> segmented_list0(list_size);
		std::vector<T> ref(list_size);
		for_each(ref.begin(), ref.end(), [](T &n) { n++; });
		for_each(segmented_list0.begin(), segmented_list0.end(), [](T &n) { n++; });
	}
}



template<typename T, size_t N>
void test_random_access_iterators()
{
	segmented_list<T, N> segmented_list0;
	std::vector<T> ref;
	for (int i = 0; i < 1024; i++)
	{
		segmented_list0.emplace_back(i);
		ref.emplace_back(i);
	}

	auto iter1 = segmented_list0.begin();	// check that incrementing by 1 is the same as ++
	auto iter2 = segmented_list0.begin();
	auto iter_ref = ref.begin();
	assert(iter1 == iter2);
	for (int i = 0; i < 1024; i++)
	{
		assert(*iter1 == *iter2);
		assert(*iter_ref == *iter1);
		auto ret1 = iter1++;
		auto ret2 = iter2 +=1;
		assert(ret1 != ret2);
		iter_ref++;
		assert(iter1 == iter2);
	}

	// check stepping forwards by n
	{
		auto iter1 = segmented_list0.begin();	// check that incrementing by 1 is the same as ++
		auto iter_ref = ref.begin();
		for (int i = 0; i < 512; i++)
		{
			assert(*iter_ref == *iter1);
			iter1 += 2;
			iter_ref += 2;
		}

	}

	// check walking backwards 1 at a time
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		auto iter1 = segmented_list0.end();	// check that incrementing by 1 is the same as ++
		auto iter_ref = ref.end();
		for (int i = 0; i < 1024; i++)
		{
			iter1--;
			iter_ref--;
			T a = *iter1;
			T b = *iter_ref;
			assert(a == b);
		}
	}

	// check walking backwards 2 at a time, 
	
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		auto iter1 = segmented_list0.end();	// check that incrementing by 1 is the same as ++
		auto iter_ref = ref.end();
		for (int i = 0; i < 512; i++)
		{
			iter1-=2;
			iter_ref-=2;
			T a = *iter1;
			T b = *iter_ref;
			assert(a == b);
		}
	}
	
	// check walking backwards 2 at a time using + negative values
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		auto iter1 = segmented_list0.end();	// check that incrementing by 1 is the same as ++
		auto iter_ref = ref.end();
		for (int i = 0; i < 512; i++)
		{
			iter1 += -2;
			iter_ref += -2;
			T a = *iter1;
			T b = *iter_ref;
			assert(a == b);
		}
	}

	// check - operator on iterators
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		auto iter1 = segmented_list0.end();	// check that incrementing by 1 is the same as ++
		auto iter_ref = ref.end();
		for (int i = 0; i < 512; i++)
		{
			auto tmp1 = iter1 - 2;
			auto tmpref = iter_ref - 2;
			T a = *tmp1;
			T b = *tmpref;
			assert(a == b);
		}
	}

	// [] operator
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		for (int i = 0; i < 1024; i++)
		{
			T &a = segmented_list0.begin()[i];
			T &b = ref.begin()[i];
			assert(a == b);
		}
	}

	// ordering relations
	// a < b
	{
		segmented_list<T, N> segmented_list0(1024);
		std::vector<T> ref(1024);
		std::iota(segmented_list0.begin(), segmented_list0.end(), 1);
		std::iota(ref.begin(), ref.end(), 1);
		for (int i = 0; i < 1024; i++)
		{
			assert(segmented_list0.begin() < segmented_list0.end());
			assert(ref.begin() < ref.end());

			assert(segmented_list0.end() > segmented_list0.begin());
			assert(ref.end() > ref.begin());

			assert(segmented_list0.end() >= segmented_list0.begin());
			assert(ref.end() >= ref.begin());

			if (i < 1023)
			{
				assert(segmented_list0.begin()[i] < segmented_list0.begin()[i + 1]);
				assert(segmented_list0.begin()[i] <= segmented_list0.begin()[i + 1]);
				assert(ref.begin()[i] < ref.begin()[i + 1]);
				assert(ref.begin()[i] <= ref.begin()[i + 1]);
			}

		}
	}



}

template<typename T, size_t N>
void compare_against_ref()
{
	std::list<T> ref_list;
	segmented_list<T, N> segmented_list0;
	assert(segmented_list0.empty());
	segmented_list<T, N> segmented_list_equiv; // same values as list 0
	segmented_list<T, N> segmented_list1; // list0 with an offset
	segmented_list<T, N> segmented_list_empty1; // empty after every insert
	segmented_list<T, N> segmented_list_empty10; // empty after every 10 insert
	
	for (int i = 0; i < 1024; i++)
	{
		// emplace
		segmented_list0.emplace_back(i);
		segmented_list_equiv.emplace_back(i);
		segmented_list1.emplace_back(i+1);
		segmented_list_empty1.emplace_back(i);
		segmented_list_empty10.emplace_back(i);
		ref_list.emplace_back(i);
		assert(segmented_list0.size() == i + 1);
		assert(ref_list.size() == i + 1);
		assert(std::equal(segmented_list0.begin(), segmented_list0.end(), ref_list.begin()));


		// check clear/empty
		// clear is not ContainerConcept... so wonder which one it is
		assert(!segmented_list_empty1.empty());
		segmented_list_empty1.clear();
		assert(segmented_list_empty1.empty());

		if (i % 10 == 0)
		{
			segmented_list_empty10.clear();
			assert(segmented_list_empty10.empty());
		}
		else
		{
			assert(!segmented_list_empty10.empty());
		}

		// assign
		segmented_list<T, N> segmented_assign;
		segmented_assign = segmented_list0;
		assert(std::equal(segmented_list0.begin(), segmented_list0.end(), ref_list.begin()));

		// copy
		segmented_list<T, N> segmented_copyconstruct(segmented_list0);
		auto iter1 = segmented_copyconstruct.begin();
		auto iter2 = ref_list.begin();
		while (iter2 != ref_list.end())
		{
			T a = *iter1;
			T b = *iter2;
			assert(a == b);
			iter1++;
			iter2++;

		}

		assert(std::equal(segmented_copyconstruct.begin(), segmented_copyconstruct.end(), ref_list.begin()));

		// equality operator
		assert(segmented_list0 == segmented_assign);
		assert(segmented_list0 == segmented_list_equiv);

		// inequality
		bool rc = segmented_list0 != segmented_list1;
		assert(rc);

		// swap
		segmented_list0.swap(segmented_list1);
		// now segmented_list1 should match ref_list
		assert(std::equal(segmented_list1.begin(), segmented_list1.end(), ref_list.begin()));
		// and segmetned_list0 should not match
		assert(!std::equal(segmented_list0.begin(), segmented_list0.end(), ref_list.begin()));
		
		// swap back and check the reverse
		segmented_list0.swap(segmented_list1);
		assert(!std::equal(segmented_list1.begin(), segmented_list1.end(), ref_list.begin()));
		assert(std::equal(segmented_list0.begin(), segmented_list0.end(), ref_list.begin()));


		// global swap function
		::swap(segmented_list0, segmented_list1);
		::swap(segmented_list0, segmented_list1);
		assert(!std::equal(segmented_list1.begin(), segmented_list1.end(), ref_list.begin()));
		assert(std::equal(segmented_list0.begin(), segmented_list0.end(), ref_list.begin()));
	}
}

// evaluates one writer, multi reader threading access to different segment sizes
static void threading_tests()
{
	read_ordering_test_int<1>();
	read_ordering_test_int<2>();
	read_ordering_test_int<3>();
	read_ordering_test_int<1023>();
	read_ordering_test_int<1024>();
	read_ordering_test_int<1025>();
}

// see if const stuff works
template <size_t N>
void const_test()
{
	std::vector<int> v = { 1,2,3 };
	segmented_list<int, N> s = { 1,2,3 };

	const auto viter = v.begin();
	const auto siter = s.begin();	// const iterator from non-const container
	const auto viter2 = v.end();
	const auto siter2 = s.end();
	const int &x = *viter;
	const int &y = *siter;
	assert(x == y);

	{
		const std::vector<int> &vc = v;
		const segmented_list<int, N> &sc = s;

		// iterator from const container should be able to read and move the iterator
		auto viter = vc.begin();
		auto siter = sc.begin();

		// shouldn't compile (uncomment to test)
		//*vc.begin() = 11;
		//*sc.begin() = 12;

		const int &x = *viter;
		const int &y = *siter;
		assert(x == y);

		// i can increment the iterator
		viter++;
		viter += 2;
		viter--;
		viter -= 2;

		siter++;
		siter += 2;
		siter--;
		siter -= 2;

		// I can access randomly
		viter[2];
		siter[2];

		// end is const
		for (auto iter = vc.cbegin(); iter != vc.cend(); iter++) {}
		for (auto iter = sc.cbegin(); iter != sc.cend(); iter++) {}
		for (auto iter = vc.begin();  iter != vc.end(); iter++) {}
		for (auto iter = sc.begin();  iter != sc.end(); iter++) {}

	}
	
	
}

// runs through basic interfaces and checks them
static void basic_behaviour_test()
{
	const_test<10>();

	printf("compare segmented list vs vector constructor\n");
	initializer_test_iter_constructor<1>();
	initializer_test_iter_constructor<2>();
	initializer_test_iter_constructor<5>();

	printf("compare segmented list vs vector initializer where the element_type is string\n");
	initializer_test_string<1>();
	initializer_test_string<2>();
	initializer_test_string<5>();

	printf("compare segmented list vs vector initializer where the element_type is int\n");
	initializer_test<1>();
	initializer_test<2>();
	initializer_test<5>();

	printf("binary search test of segmented list vs vector\n");
	binary_search_test<int, 1>();
	binary_search_test<int, 2>();
	binary_search_test<int, 1022>();
	binary_search_test<int, 1023>();
	binary_search_test<int, 1024>();
	binary_search_test<int, 1025>();

	// sort test of segmented list vs vector
	printf("sort test of segmented list vs vector\n");
	sort_test<int, 1>();
	sort_test<int, 2>();
	sort_test<int, 3>();
	sort_test<int, 4>();
	sort_test<int, 1022>();
	sort_test<int, 1023>();
	sort_test<int, 1024>();
	sort_test<int, 1025>();
	sort_test<int, 2048>();


	printf("catchall test of segmented list vs vector\n");
	compare_against_ref<int, 1>();
	compare_against_ref<int, 2>();
	compare_against_ref<int, 1023>();
	compare_against_ref<int, 1024>();
	compare_against_ref<int, 1025>();

	printf("random access test of segmented list vs vector\n");
	test_random_access_iterators<int, 1>();
	test_random_access_iterators<int, 2>();
	test_random_access_iterators<int, 3>();
	test_random_access_iterators<int, 4>();
	test_random_access_iterators<int, 1022>();
	test_random_access_iterators<int, 1023>();
	test_random_access_iterators<int, 1024>();
	test_random_access_iterators<int, 1025>();

	printf("emplace back tests\n");
	segmented_list<int, 1> l1;
	for (int i = 0; i < 1024; i++)
	{
		l1.emplace_back(i);
		assert(l1.size() == i + 1);
	}

	segmented_list<int, 2> l2;
	for (int i = 0; i < 1024; i++)
	{
		l2.emplace_back(i);
		assert(l2.size() == i + 1);
	}

	segmented_list<int, 1024> l;
	for (int i = 0; i < 1024; i++)
	{
		l.emplace_back(i);
		assert(l.size() == i + 1);
	}

	segmented_list<int, 1024>::iterator s = l.begin();

	//160 bytes in debug
	// 8 bytes in release
	printf("sizeof iterator %d", sizeof(s));
}

static void segmented_list_allocators()
{
	// construct a segmented list with the default allocator
	segmented_list<int, 1024,std::allocator<int>> segmented_list0(1024);

	// construct a segmented list with a slab allocator
	slab a_slab(1024*1024);
	slab_allocator_base::m_temp_slab = &a_slab;
	segmented_list<int, 1024, slab_allocator<int>> segmented_list1(1024, slab_allocator<int>(&a_slab));

	// construct a segmented list with a default allocator
	segmented_list<int, 1024> segmented_list2(1024);

	for (int i = 0; i < 2024; i++)
	{
		segmented_list0.push_back(11);
		segmented_list1.push_back(12);
		segmented_list2.push_back(13);
	}
}


void TEST_SEGMENTED_LIST()
{
	segmented_list_allocators();
	basic_behaviour_test();
	threading_tests();
}
