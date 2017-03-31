//
// tracker_id_fixer : traverse the graph and make sure everyone has the correct id
//
#pragma once
#include "time_containers.h"
#include "vr_types.h"
#include <unordered_map>

struct tracker_id_fixer_visitor 
{
	tracker_id_fixer_visitor()
	{}

	std::unordered_map<std::string, serialization_id> url2id;
	SerializableRegistry *registry;

	//
	// visit interfaces
	//
	static const bool visit_source_interfaces() { return false; }
	static const bool spawn_children() { return false; }
	static const bool reload_render_models() { return false; }
	static const bool recheck_distortion() { return false; }

	inline void start_group_node(const base::URL &url_name, int group_id_index) {}
	inline void end_group_node(const base::URL &group_id_name, int group_id_index) {}

	serialization_id get_id(const std::string &url)
	{
		auto iter = url2id.find(url);
		if (iter == url2id.end())
		{
			// assert - 
			assert(0);
		}
		return iter->second;
	}

	inline void start_vector(const base::URL &vector_name, RegisteredSerializable &vec) 
	{
		serialization_id id = get_id(vec.get_serialization_url().get_full_path());
		vec.set_serialization_index(id);
		registry->Register(&vec, id);
	}

	template <typename T>
	inline void end_vector(const base::URL &vector_name, T &vec) {}

	template <typename HistoryVectorType, typename ResultType>
	void visit_node(const HistoryVectorType &history, const ResultType &latest_result) 
	{
		assert(0); 
	}

	void visit_node(RegisteredSerializable &node)
	{
		serialization_id id = get_id(node.get_serialization_url().get_full_path());
		node.set_serialization_index(id);
		registry->Register(&node, id);
	}

	template <typename ParentVectorType> void spawn_child(ParentVectorType &vector, const std::string &child_name) 
	{
		assert(0);
	}
};


