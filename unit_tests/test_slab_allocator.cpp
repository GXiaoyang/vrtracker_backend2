// test_result
// * unit test for slab allocator
//

#include "slab_allocator.h"
#include "log.h"
#include <memory.h>
#include <vector>
#include <thread>

void allocator(slab* s, int num_allocs, int alloc_size, char my_id)
{
	std::vector<void *> mine; 
	mine.reserve(num_allocs);
	for (int i = 0; i < num_allocs; i++)
	{
		//void *ptr = malloc(alloc_size);
		void *ptr = s->slab_alloc(alloc_size);

		memset(ptr, my_id, alloc_size);
		mine.push_back(ptr);
	}
	void *ref = malloc(alloc_size);
	memset(ref, my_id, alloc_size);
	for (void * ptr : mine)
	{
		assert(memcmp(ptr, ref, alloc_size) == 0);
	}
	free(ref);
}

void allocate_from_multiple_threads()
{
	int page_size = 1000000;
	int num_threads = 10;
#ifdef _DEBUG
	int allocs_per_thread = 1000;
	int size_per_alloc = 10000;
#else
	int allocs_per_thread = 100000;
	int size_per_alloc = 1000;
#endif
	slab s(page_size);
	std::vector<std::thread*> threads;
	for (int i = 0; i < num_threads; i++)
	{
		std::thread *t = new std::thread(allocator, &s, allocs_per_thread, size_per_alloc, (char)i);
		threads.push_back(t);
	}

	for (auto thread : threads)
	{
		thread->join();
	}
	int page_count = std::distance(s.pages.begin(), s.pages.end());
	int expected_page_count = num_threads * allocs_per_thread * size_per_alloc / page_size;
	assert(page_count == expected_page_count);
}

void TEST_SLAB_ALLOCATOR()
{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		allocate_from_multiple_threads();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		log_printf("multi threaded allocations took %lld ms.\n",
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	
}

