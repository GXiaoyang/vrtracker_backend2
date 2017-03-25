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

	tracker_test_context context;
	vr_tracker_traverse u;
	u.save_tracker_to_binary_file(&context.get_tracker(), "tracker.bin");
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
