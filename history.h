#pragma once
#include "typehelper.h"
#include "range.h"
#include <vector>

// allocator
// results
// history     wrappers
// schema, abstract traversal
// serialization cursors, traversal,gui
// control_api, cursor_api, 

// ideal schema
//History<HMDPoseData, std::deque>	pose_data;
//History<HMDPoseData, std::list>	properties;

// frame data is data that has a frame number associated with it.  E.g.
// frame range is a range of frames (begin and end iterators on things with frame numbers associated with them)
namespace FrameData
{
	template <typename T>
	struct Record
	{
		Record() = default;
		Record(int frame_number_in, const T &result_in)
			: frame_number(frame_number_in),
			result(result_in)
		{}
		
		template<typename... Args>
		Record(int frame_number_in, Args&&... args)
			: frame_number(frame_number_in),
			result(std::forward<Args>(args)...)
		{}
		int	frame_number;
		T	result;
	};
}



// requirements
// things that rarely change should be forward lists
// things that change alot should be segmented lists. e.g. per frame data should be stored in buckets of 100 each

// code that uses the history nodes needs iterators
//
// for the big / cursor based ones:
//	what I did before is that they would just 
//		1. instantiate the ::iter_type of the container
//      2. they would keep a copy of the associated frame.
//
// for the gui ones:
// there is the cached_iterator template:
//		iterator_type iter_cur;	// initialized in start_iterator() and updated in get_next
//		int start_frame_id;
//		int cache_start_frame_id;
//		int cache_end_frame_id;
//		bool cache_rc;
//  which was designed to 
//		* quickly answer the question: do changes exist in this interval.
//		* quickly walk though changes in the interval
//      * when changing the interval, try and reuse the old cursor information.  
//		
//		* relies on the knowledge that it's a sequence and that the Entry values are monotonically increasing.
//  
//

template<typename T, template <typename, typename> class Container>
class History2
{
public:
	typedef Container<FrameData::Record<T>, std::allocator<FrameData::Record<T>>> container_type;
	typedef typename Container<FrameData::Record<T>, std::allocator<FrameData::Record<T>>>::iterator iterator_type;
	typedef std::range<iterator_type> range_type; 

public:
	container_type values;
	const char *name;

public:
	History2() : name(nullptr)
	{}

	range_type range() { return range_type(values.begin(), values.end()); }
	iterator_type begin() { return values.begin(); }
	iterator_type end() { return values.end(); }

	bool empty() const { return values.empty(); }
	const T & latest() const { return values.front(); }

	template<typename... Args>
	void emplace_new_entry(Args&&... args)
	{
		values.emplace_back(std::forward<Args>(args)...);
	}
	
	range_type interval_query(int frame_a, int frame_b)
	{
		return range_intersect(range(), frame_a, frame_b);
	}

	iterator_type queryLTE(int frame)
	{
		FrameData::Record<T> dummy;
		dummy.frame_number = frame;
		return last_item_less_than_or_equal_to(values.begin(), values.end(), dummy, 
			[](const auto &lhs, const auto &rhs) -> bool { return lhs.frame_number < rhs.frame_number; });
	}

};


