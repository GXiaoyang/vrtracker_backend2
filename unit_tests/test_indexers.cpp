#include "vr_applications_indexer.h"
#include "test_context.h"
#include <thread>
#include "vr_applications_wrapper.h"
#include <iostream>

std::atomic<int> reader_done;

static void reader(ApplicationsIndexer *ai)
{
	int application_sets_read = 0;
	while (application_sets_read < 1000)
	{
		if (ai->get_num_applications() > 0)
			application_sets_read++;

		for (int i = 0; i < ai->get_num_applications(); i++)
		{
			int invalid = ai->get_index_for_key("ha");
			assert(invalid == -1);

			const char *k = ai->get_key_for_index(i);
			int i2 = ai->get_index_for_key(k);
			assert(i == i2);
		}

		ai->read_lock_present_indexes();
		auto &v = ai->get_present_indexes();
		for (auto iter = v.begin(); iter != v.end(); iter++)
		{
			const char *k = ai->get_key_for_index(*iter);
			int i2 = ai->get_index_for_key(k);
			assert(*iter == i2);
		}
		ai->read_unlock_present_indexes();
	}
	reader_done = 1;
}

static void updater(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	while (!reader_done)
	{
		ai->update(wrap);
	}
}

void test_multi_threading(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	auto a = std::thread(reader, ai);
	auto b = std::thread(updater, ai, wrap);

	a.join();
	b.join();
}

void lookup_perf_test(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	int counter = 0;
	ai->update(wrap);
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	
	for (int i = 0; i < 5000; i++)
	{
		for (int i = 0; i < ai->get_num_applications(); i++)
		{
			const char *k = ai->get_key_for_index(i);
			int i2 = ai->get_index_for_key(k);
			counter += i2;
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "lookups took " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< "ms.\n";

	printf("%d\n", counter);
}

void TEST_INDEXERS()
{
	test_context context;

	vr_result::ApplicationsWrapper wrap(context.vr_interfaces().appi);
	ApplicationsIndexer ai;
	lookup_perf_test(&ai, wrap);
	lookup_perf_test(&ai, wrap);
	lookup_perf_test(&ai, wrap);
	test_multi_threading(&ai, wrap);
	

}