#pragma once

#include "time_containers.h"


struct update_history_visitor 
{
	update_history_visitor(time_index_t t)
		:	m_frame_number(t),
			m_update_counter(0),
			m_visit_counter(0)
	{}

	time_index_t m_frame_number;
	int m_update_counter;
	int m_visit_counter;

	static const bool visit_source_interfaces() { return true; }
	static const bool expand_structure() { return true; }
	static const bool reload_render_models() { return false; }
	static const bool recheck_distortion() { return false; }

	inline void start_group_node(const base::URL &url_name, int group_id_index)
	{}

	inline void end_group_node(const base::URL &group_id_name, int group_id_index)
	{}

	// this should give the history half and the openvr half
	// because if openvr resizes something, then the history visitor should have a chance to resize
	// as well
	template <typename T>
	inline void start_vector(const base::URL &vector_name, T &vec)
	{}

	template <typename T>
	inline void end_vector(const base::URL &vector_name, T &vec)
	{}

	template <typename HistoryVectorType, typename ResultType>
	void visit_node(HistoryVectorType &history,  ResultType &latest_result)
	{
		// add the entry if its new,
		// or the presence has changed
		if (history.empty() || not_equals(history.latest().get_value(), latest_result))
		{
			history.emplace_back(m_frame_number, latest_result);
		}
	}

	template <typename HistoryVectorType>
	void visit_node(HistoryVectorType &history_node)
	{
	}
};


