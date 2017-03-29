#pragma once
#include "openvr_serialization.h"

class ResourcesIndexer
{
public:
	ResourcesIndexer() {}

	bool operator == (const ResourcesIndexer &rhs) const
	{
		if (&rhs == this)
			return true;
		return (m_resource_directories == rhs.m_resource_directories) &&
			(m_resource_filenames == rhs.m_resource_filenames);
	}

	bool operator != (const ResourcesIndexer &rhs) const
	{
		return !(*this == rhs);
	}

	void Init(const char **resource_filenames, const char **initial_resources_dirs, int num_names);
	void WriteToStream(EncodeStream &s) const;
	void ReadFromStream(EncodeStream &s);

	const char *get_filename_for_index(int index, int *fname_size)
	{
		*fname_size = size_as_int(m_resource_filenames[index].size()) + 1;
		return m_resource_filenames[index].c_str();
	}

	const char * get_directoryname_for_index(int index, int *dname_size)
	{
		*dname_size = size_as_int(m_resource_directories[index].size()) + 1;
		return m_resource_directories[index].c_str();
	}

	int get_num_resources() {
		return size_as_int(m_resource_filenames.size());
	}

	std::vector<std::string> m_resource_directories;
	std::vector<std::string> m_resource_filenames;
};


