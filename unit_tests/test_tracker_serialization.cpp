// testserialization
//

#include "tracker_test_context.h"
#include "vr_tracker_traverse.h"

using namespace vr;

void test_tracker_serialization()
{
	using namespace vr;
	using namespace vr_result;
	using std::range;
	
	log_printf("start of test_tracker_serialization\n");
	tracker_test_context::reset_globals();

	vr_tracker_traverse u;
	{
		tracker_test_context contexta;
		assert(contexta.get_tracker() == contexta.get_tracker());
	}

	{
		tracker_test_context contexta;
		tracker_test_context contextb;
		assert(contexta.get_tracker() == contextb.get_tracker());
	}

	{
		log_printf("serializing empty context\n");
		// no updates
		tracker_test_context contexta;
		u.save_tracker_to_binary_file(&contexta.get_tracker(), "c:\\temp\\tracker0.bin");

		tracker_test_context contextb;
		u.load_tracker_from_binary_file(&contextb.get_tracker(), "c:\\temp\\tracker0.bin");
		assert(contexta.get_tracker() == contextb.get_tracker());
	}

	tracker_test_context::reset_globals();
	{
		log_printf("serializing one update\n");
		// one update
		tracker_test_context contexta;
		u.update_tracker_parallel(&contexta.get_tracker(), &contexta.raw_vr_interfaces());
		assert(u.save_tracker_to_binary_file(&contexta.get_tracker(), "c:\\temp\\tracker1.bin"));
		//contexta.get_tracker().m_state_registry.dump();
		
		tracker_test_context contextb;
		assert(u.load_tracker_from_binary_file(&contextb.get_tracker(), "c:\\temp\\tracker1.bin"));
		assert(contexta.get_tracker() == contextb.get_tracker());
	}
	tracker_test_context::reset_globals();
	{
		log_printf("serializing 10 updates\n");
		// ten updates ... 100mb with no textures... 600mb with textures
		tracker_test_context contexta;
		for (int i = 0; i < 10; i++)
		{
			u.update_tracker_parallel(&contexta.get_tracker(), &contexta.raw_vr_interfaces());
		}
		log_printf("writing\n");
		u.save_tracker_to_binary_file(&contexta.get_tracker(), "c:\\temp\\tracker10.bin");

		log_printf("reading\n");
		tracker_test_context contextb;
		u.load_tracker_from_binary_file(&contextb.get_tracker(), "c:\\temp\\tracker10.bin");
		assert(contexta.get_tracker() == contextb.get_tracker());
	}
	tracker_test_context::reset_globals();
	log_printf("done test_tracker_serialization\n");
}

// i can log object changes and probably find them pretty easily
// what about object construction/insertion/spawn

void stream_frame(int frame)
{
	//vector <int> ids_for_frame;
	//write frame id
	//write number of chunks
#if 0
	for (id : frame)
	{
		ObjectIf *p = tracker.getObjectForId();
		p->encode(s);
	}
#endif


}

#if 0
// load frame
{

	read frame
		// find the object
		//
		// update the object
		p->decode(s);
}
#endif
