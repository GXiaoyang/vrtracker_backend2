#pragma once

#include "time_containers.h"


struct update_history_visitor 
{
	constexpr update_history_visitor(time_index_t frame_number)
		:	m_frame_number(0),
			m_update_counter(0),
			m_visit_counter(0)
	{}

	time_index_t m_frame_number;
	int m_update_counter;
	int m_visit_counter;

	constexpr bool visit_source_interfaces() const { return true; }
	constexpr bool expand_structure() const { return true; }
	constexpr bool reload_render_models() const { return false; }
	constexpr bool recheck_distortion() const { return false; }

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


