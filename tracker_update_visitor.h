#pragma once
// tracker_update_visitor:
//  * this is the key comparison class used when querying and updating vr state
//    it is fed to the traverse graph logic by the vr_tracker_updater which then
//    invokes it's visit interfaces

#include "time_containers.h"
#include "vr_types.h"

struct tracker_update_visitor 
{
	tracker_update_visitor(time_index_t t)
		:	m_frame_number(t),
			updated_nodes(registry->GetNumRegistered())
	{}
	time_index_t m_frame_number;
	SerializableRegistry *registry;
	VRBitset updated_nodes;
	std::vector<std::pair<serialization_id, std::string>> spawn; //log any new spawned objects during this update

	//
	// visit interfaces
	//
	static const bool visit_source_interfaces() { return true; }
	static const bool spawn_children() { return true; }
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
			updated_nodes.set(history.get_serialization_index());
		}
	}

	template <typename ParentVectorType>
	void spawn_child(ParentVectorType &parent_vector, const std::string &child_name)
	{
		base::URL child_url(parent_vector.make_url_for_child(child_name));
		parent_vector.emplace_back(child_url, registry);
		spawn.emplace_back(parent_vector.get_serialization_index(), child_name);
	}

	template <typename HistoryVectorType>
	void visit_node(HistoryVectorType &history_node) {}
};


