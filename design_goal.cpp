// rangesplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "range_algorithm.h"
#include "openvr_string_std.h"
#include <shared_mutex>
#include "segmented_list.h"

#if 0
struct TrackedDevicePose_t
{
	HmdMatrix34_t mDeviceToAbsoluteTracking;
	HmdVector3_t vVelocity;				// velocity in tracker space in m/s
	HmdVector3_t vAngularVelocity;		// angular velocity in radians/s (?)
	ETrackingResult eTrackingResult;
	bool bPoseIsValid;

	// This indicates that there is a device connected for this spot in the pose array.
	// It could go from true to false if the user unplugs the device.
	bool bDeviceIsConnected;
};
#endif

#if 0
#include <openvr.h>
#include <vector>
#include <deque>
#include "range.h"
#include "segmented_list.h"

using namespace vr;
using namespace std;
using namespace openvr_string;


// container placement
// 

//
//	time_values.h
// hierarchy of values in time
//

typedef int time_stamp_t;

template <typename A = std::allocator<char>>
struct named
{
	named(const std::string name, const named *parent, const A& alloc = A())
		: 
		m_name(name, alloc),
		m_path(alloc)
	{ 
		if (parent)
		{
			m_path = parent->get_name() + "/" + m_name;
		}
	}
		
	std::string get_name() const { return m_name; };
	std::string get_path() const { return m_path; };

private:
	std::string m_name;
	std::string m_path;
};



template <typename T, template <typename, typename> class Container, typename A = std::allocator<T>>
struct named_container : public named<A>, public Container<T,A>
{
	template<typename... Args>
	named_container(const std::string &name, named *parent, Args&&... args)
		: named(name, parent),
		Container<T, A>(std::forward<Args>(args)...)
	{}
};

template <typename T, typename A = std::allocator<T>>
using named_vector = named_container<T, std::vector, A>;


template <typename T>
struct timestamped
{
	template<typename... Args>
	timestamped(time_stamp_t t, Args&&... args)
		: timestamp(t),
		value(std::forward<Args>(args)...)
	{}

	const T& get_value() const { return value;  }
	time_stamp_t get_timestamp() const { return timestamp; }
private:
	time_stamp_t timestamp;
	T value;
};

template <	typename T,
	template <typename, typename> class Container,
	typename A = std::allocator<T>>
	struct timestamped_values : named<A>
{
	typedef timestamped<T> element_type;
	typedef Container<element_type, A> container_type_t;
	typedef typename container_type_t::iterator iterator_type_t;

	template<typename... Args>
	timestamped_values(const std::string &name, named *parent, Args&&... args)
		:
		named(name, parent),
		container(std::forward<Args>(args)...)
	{}

	bool is_present() const { return !container.empty(); }
	const T& operator()() const { return container.back().get_value(); }
	const timestamped<T>& latest() const { return container.back(); }
	const timestamped<T>& earliest() const { return container.back(); }

	// [start and end)  (IE ARE NOT inclusive of end)
	range<iterator_type_t> get_range(int start, int end)
	{
		return std::range<iterator_type_t>(container.begin(), container.end());
		//return range_intersect(range(container.begin(), container.end(), start, end));
	}

	range<iterator_type_t> get_range()
	{ return std::range<iterator_type_t>(container.begin(), container.end()); }

	container_type_t container;
};


template <typename A = std::allocator<char>>
struct controller_t : named<A>
{
	controller_t(const std::string &name, const named<A> *parent, const A& alloc = A())
		: named(name, parent, alloc), 
		  raw_tracking_pose("raw_tracking_pose", this, alloc)
	{}

	timestamped_values<TrackedDevicePose_t, std::list, A> raw_tracking_pose;
};

template <typename A = std::allocator<T>>
struct vr_session : public named<A>
{
	vr_session(const std::string &name, named *parent, const A& alloc = A())
		: named(name, parent, alloc),
		system("system", (const named<A>*)this, alloc),
		m_timestamps("timestamps", (const named<A>*)this, alloc)
	{}

	struct system_t : named<A>
	{
		system_t(const std::string &name, named *parent, const A& alloc = A())
			: named(name, parent, alloc),
			controllers(name, parent, alloc)
		{}

		named_vector<controller_t<A>, A> controllers;
	} system;

	time_stamp_t get_closest_timestamp(uint64_t val)
	{
		auto iter = last_item_less_than_or_equal_to(m_timestamps.begin(), m_timestamps.end(), val);
		return std::distance(m_timestamps.begin(), m_timestamps.end());
	}

	uint64_t get_time(time_stamp_t i) { return m_timestamps[i]; }

	named_vector<uint64_t> m_timestamps;
};


// nice accessors
template <typename A = std::allocator<T>>
void accessors_main()
{
	vr_session<A> vr("vr", nullptr);

	// I: ideal: return the latest if someone asks for it like this.
	

	// I0: case 0 - what if it's not present?
	TrackedDevicePose_t pose;
	if (vr.system.controllers[0].raw_tracking_pose.is_present())
	{
		pose = vr.system.controllers[0].raw_tracking_pose();
	}

	// II: ask some meta data questions
	std::string name = vr.system.controllers[0].raw_tracking_pose.get_name();
	std::string path = vr.system.controllers[0].raw_tracking_pose.get_path();

	// III: return a timestamped version
	timestamped<TrackedDevicePose_t> a = vr.system.controllers[0].raw_tracking_pose.latest();
	timestamped<TrackedDevicePose_t> b = vr.system.controllers[0].raw_tracking_pose.earliest();


	time_stamp_t t = a.get_timestamp();	// returns an int
	uint64_t tt = vr.get_time(a.get_timestamp()); // returns a time

	// IV: return a range
	auto pose_range = vr.system.controllers[0].raw_tracking_pose.get_range();
	auto r2 = vr.system.controllers[0].raw_tracking_pose.get_range(0,42);

	uint64_t ta;
	uint64_t tb;
	auto range_from_times = vr.system.controllers[0].raw_tracking_pose.get_range(
		vr.get_closest_timestamp(ta), vr.get_closest_timestamp(tb));	// inputs are times
	
	// V: copy a range of timestamped_values into a vector
	std::vector<decltype(pose_range)::value_type> a_copy(pose_range.begin(), pose_range.end());

	// VIII: given a vector of timestamped values, create a vector that holds the corresponding values
	vector<uint64_t> vec;
	vec.reserve(pose_range.size());
	std::transform(pose_range.begin(), pose_range.end(), vec.begin(),
		[&vr](auto &c) { return vr.get_time(c.get_timestamp()); });
		
	// VII: convert to a string (using return value from I)
	to_string(vr.system.controllers[0].raw_tracking_pose());

	
};


int main()
{
	accessors_main<std::allocator<char*>>();

}

#endif