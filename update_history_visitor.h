#pragma once
// update_history_visitor:
//  * this is the key comparison class used when querying and updating vr state
//    it is fed to the traverse graph logic by the vr_tracker_updater which then
//    invokes it's visit interfaces

#include "time_containers.h"
#include "vr_types.h"

struct update_history_visitor 
{
	update_history_visitor(time_index_t t)
		:	m_frame_number(t)
	{}
	time_index_t m_frame_number;

	//
	// visit interfaces
	//
	static const bool visit_source_interfaces() { return true; }
	static const bool expand_structure() { return true; }
	static const bool reload_render_models() { return false; }
	static const bool recheck_distortion() { return false; }

	inline void start_group_node(const base::URL &url_name, int group_id_index) {}
	inline void end_group_node(const base::URL &group_id_name, int group_id_index) {}

	// if openvr resizes something, then the visitor should have a chance to resize
	// as well
	template <typename T>
	inline void start_vector(const base::URL &vector_name, T &vec) {}

	template <typename T>
	inline void end_vector(const base::URL &vector_name, T &vec) {}

	template <typename HistoryVectorType, typename ResultType>
	void visit_node(HistoryVectorType &history,  const ResultType &latest_result)
	{
		// add the entry if its new,
		// or the presence has changed
		if (history.empty() || not_equals(history.latest().get_value(), latest_result))
		{
			history.emplace_back(m_frame_number, latest_result);
		}
	}

	template <typename HistoryVectorType>
	void visit_node(HistoryVectorType &history_node) {}
};


