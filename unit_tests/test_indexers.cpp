#include "vr_applications_indexer.h"
#include "test_context.h"
#include <thread>
#include "vr_applications_wrapper.h"

static void reader(ApplicationsIndexer *ai)
{
	while (1)
	{
		for (int i = 0; i < ai->get_num_applications(); i++)
		{
			int invalid = ai->get_index_for_key("ha");
			assert(invalid == -1);

			const char *k = ai->get_key_for_index(i);
			int i2 = ai->get_index_for_key(k);
			assert(i == i2);

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
	}
}

static void updater(ApplicationsIndexer *ai, vr_result::ApplicationsWrapper &wrap)
{
	while (1)
	{
		ai->update(wrap);
	}
}

void TEST_INDEXERS()
{
	test_context context;

	vr_result::ApplicationsWrapper wrap(context.vr_interfaces().appi);

	ApplicationsIndexer ai;
	auto a = std::thread(reader, &ai);
	auto b = std::thread(updater, &ai, wrap);

	a.join();
	b.join();

}