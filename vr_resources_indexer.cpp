#pragma once

#include "vr_resources_indexer.h"

void ResourcesIndexer::Init(const char **resource_filenames, const char **initial_resources_dirs, int num_names)
{
	for (int i = 0; i < num_names; i++)
	{
		m_resource_directories.emplace_back(initial_resources_dirs[i]);
		m_resource_filenames.emplace_back(resource_filenames[i]);
	}
}

void ResourcesIndexer::WriteToStream(EncodeStream &s)
{
	int x = 4;
	encode(x, s);
	write_string_vector_to_stream(s, m_resource_directories);
	write_string_vector_to_stream(s, m_resource_filenames);
}

void ResourcesIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 4);
	m_resource_directories.clear();
	m_resource_filenames.clear();
	read_string_vector_from_stream(s, m_resource_directories);
	read_string_vector_from_stream(s, m_resource_filenames);
}


