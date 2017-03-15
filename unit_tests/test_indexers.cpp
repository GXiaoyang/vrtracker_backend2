#include "vr_applications_indexer.h"
#include "test_context.h"
#include <thread>
#include "vr_applications_wrapper.h"
#include <iostream>

std::atomic<int> app_reader_done;

static void app_reader(ApplicationsIndexer *ai)
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
	app_reader_done = 1;
}

static void app_updater(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	while (!app_reader_done)
	{
		ai->update_presence_and_size(wrap);
	}
}

void app_test_multi_threading(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	auto a = std::thread(app_reader, ai);
	auto b = std::thread(app_updater, ai, wrap);

	a.join();
	b.join();
}

void app_lookup_perf_test(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	int counter = 0;
	ai->update_presence_and_size(wrap);
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


std::atomic<int> overlay_reader_done;
static void overlay_reader(OverlayIndexer *ai)
{
#if 0
	int sets_read = 0;
	while (sets_read < 1000)
	{
		if (ai->get_num_overlays() > 0)
		{
			sets_read++;
		}
			

		for (int i = 0; i < ai->get_num_overlays(); i++)
		{
			int invalid = ai->get_index_for_key("ha");
			assert(invalid == -1);

			const char *k = ai->get_overlay_key_for_index()
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
	overlay_reader_done = 1;
#endif
}

static void overlay_updater(OverlayIndexer *ai, vr_result::OverlayWrapper &wrap)
{
	//while (!overlay_reader_done)
	{
	//	ai->update(wrap);
	}
}

void overlay_test_multi_threading(OverlayIndexer *ai, vr_result::OverlayWrapper &wrap)
{
	auto a = std::thread(overlay_reader, ai);
	auto b = std::thread(overlay_updater, ai, wrap);

	a.join();
	b.join();
}

void TEST_INDEXERS()
{
	test_context context;

	{
		OverlayIndexer oi;
		vr_result::OverlayWrapper wrap(context.vr_interfaces().ovi);
		overlay_test_multi_threading(&oi, wrap);
	}
	


	vr_result::ApplicationsWrapper wrap(context.vr_interfaces().appi);
	ApplicationsIndexer ai;
	app_lookup_perf_test(&ai, wrap);
	app_test_multi_threading(&ai, wrap);

	

}