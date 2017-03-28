//
// tracker_decode_visitor: reads entire tracker state from a stream
//
#pragma once
#include "time_containers.h"
#include "vr_types.h"

struct tracker_decode_visitor 
{
	tracker_decode_visitor()
	{}
	EncodeStream *m_stream;
	SerializableRegistry *registry;		// register objects by an id so they can be found for serialization and deserialization

	//
	// visit interfaces
	//
	static const bool visit_source_interfaces() { return false; }
	static const bool spawn_children() { return false; }
	static const bool reload_render_models() { return false; }
	static const bool recheck_distortion() { return false; }

	inline void start_group_node(const base::URL &url_name, int group_id_index) {}
	inline void end_group_node(const base::URL &group_id_name, int group_id_index) {}

	template <typename T>
	inline void start_vector(const base::URL &vector_name, T &vec) 
	{
		vec.decode_size(*m_stream);
	}

	template <typename T>
	inline void end_vector(const base::URL &vector_name, T &vec) {}

	template <typename HistoryVectorType, typename ResultType>
	void visit_node(const HistoryVectorType &history, const ResultType &latest_result) 
	{
		assert(0); 
	}

	template <typename HistoryVectorType>
	void visit_node(HistoryVectorType &history_node) 
	{
		history_node.decode(*m_stream);
	}

	template <typename ParentVectorType>
	void spawn_child(ParentVectorType &parent_vector, const std::string &child_name)
	{
		base::URL child_url(parent_vector.make_url_for_child(child_name));
		parent_vector.emplace_back(child_url, registry);
	}
};


