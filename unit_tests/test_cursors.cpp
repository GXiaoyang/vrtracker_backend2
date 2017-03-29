#include "vr_system_cursor.h"
#include "vr_compositor_cursor.h"
#include "tracker_test_context.h"
#include "vr_cursor_context.h"
#include "vr_tracker_traverse.h"
#include <set>

static void do_read(tracker_test_context *test_context, int unique_reads)
{
	// create a cursor
	CursorContext cursor_context(&test_context->get_tracker());
	VRSystemCursor system(&cursor_context);

	// wait until there is some data to read
	while (cursor_context.GetCurrentFrame() != 0)
	{
		std::this_thread::yield();
		cursor_context.ChangeFrame(0);
	}

	// keep reading until we've read unique_reads items:
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
	vr_tracker_traverse u;
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
static void test_simultaneous_cursors(tracker_test_context *test_context)
{
	std::thread writer(writer_thread, test_context, true);
	launch_readers(test_context, 10);
	writer.join();
}


template <typename TOwner, float(TOwner::*func)()>
float time_seek(
	CursorContext &cursor_context, 
	TOwner *p,
	time_index_t from_index, time_index_t to_index
	)
{
	float accum = 0;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		cursor_context.ChangeFrame(from_index);
		accum += (p->*func)();
		cursor_context.ChangeFrame(to_index);
		accum += (p->*func)();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log_printf("seek to and from indexes %d, %d took %lld us.\n", from_index, to_index,
		std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	return accum;
}

// test that seek time is linear or better
static void test_seek_time(tracker_test_context *test_context)
{
	// update 1000 frames
	// measure time from 0->10
	// measure time from 0->100
	// measure time from 0->1000

	CursorContext cursor_context(&test_context->get_tracker());
	VRCompositorCursor compi(&cursor_context);
	vr_tracker_traverse u;
	int target_number_of_frames = 1000;
	log_printf("collecting %d frames\n", target_number_of_frames);

	
	while (cursor_context.GetCurrentFrame() < target_number_of_frames)
	{
		u.update_tracker_parallel(&test_context->get_tracker(), &test_context->raw_vr_interfaces());
		cursor_context.ChangeFrame(target_number_of_frames);
		if (cursor_context.GetCurrentFrame() % 100 == 0)
		{
			log_printf("%d\n", cursor_context.GetCurrentFrame());
		}
	}

	float accum = 0;
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining>(cursor_context, &compi, 0, 0);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining>(cursor_context, &compi, 500, 500);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining>(cursor_context, &compi, 200, 300);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining>(cursor_context, &compi, 0, 1000);

	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining2>(cursor_context, &compi, 0, 0);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining2>(cursor_context, &compi, 500, 500);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining2>(cursor_context, &compi, 200, 300);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining2>(cursor_context, &compi, 0, 1000);

	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining3>(cursor_context, &compi, 0, 0);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining3>(cursor_context, &compi, 500, 500);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining3>(cursor_context, &compi, 200, 300);
	accum += time_seek<VRCompositorCursor, &VRCompositorCursor::GetFrameTimeRemaining3>(cursor_context, &compi, 0, 1000);

	//as far as the reader goes, I'm guessing the 500,500 testcase is the most important. alot of stuff doesnt change and so
	//the queries are going to be choosing the same choice over and over
	//the best cache for 500,500 case is where cache return if its an exact match or lower bound
}


void TEST_SYSTEM_CURSOR()
{
	tracker_test_context test_context;
	test_context.ForceInitAll(); // make sure it's setup before splitting into separate threads

	test_seek_time(&test_context);   
	test_simultaneous_cursors(&test_context);
}
