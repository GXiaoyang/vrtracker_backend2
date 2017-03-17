#include "vr_system_cursor.h"
#include "tracker_test_context.h"
#include "vr_cursor_context.h"
#include "vr_tracker_updater.h"
#include <set>

static void do_read(tracker_test_context *test_context, int unique_reads)
{
	// create a cursor
	CursorContext cursor_context(&test_context->get_tracker());
	VRSystemCursor system(&cursor_context);

	while (cursor_context.GetCurrentFrame() != 0)
	{
		std::this_thread::yield();
		cursor_context.ChangeFrame(0);
	}

	uint32_t width;
	uint32_t height;
	std::set<int> unique_read_set;
	while (size_as_int(unique_read_set.size()) < unique_reads)
	{
		time_index_t frame = std::numeric_limits<int>::max();
		cursor_context.ChangeFrame(frame);
		system.GetRecommendedRenderTargetSize(&width, &height);
		int read_frame = cursor_context.GetCurrentFrame();
		unique_read_set.insert(read_frame);
		std::this_thread::yield();
	}
}

std::atomic<int> readers_done;
static void writer_thread(tracker_test_context *test_context, bool parallel)
{
	vr_tracker_updater u;
	while (!readers_done)
	{
		if (parallel)
		{
			u.update_tracker_parallel(&test_context->get_tracker(), &test_context->raw_vr_interfaces());
		}
		else
		{
			u.update_tracker_sequential(&test_context->get_tracker(), &test_context->raw_vr_interfaces());
		}
		
		std::this_thread::yield();
	}
}

static void launch_readers(tracker_test_context *test_context, int unique_reads)
{
	std::vector<std::thread> threads;

	for (int i = 0; i < 10; i++)
	{
		threads.emplace_back(do_read, test_context, unique_reads);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
	readers_done = 1;
}

// test that multiple cursors can run simultaneously
// test that seek time is linear or better
void TEST_SYSTEM_CURSOR()
{
	tracker_test_context test_context;
	test_context.ForceInitAll(); // make sure it's setup before splitting into separate threads
	std::thread writer(writer_thread, &test_context, true);
	launch_readers(&test_context, 10);
	writer.join();
	
}
