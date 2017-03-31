#include "vr_resources_indexer.h"

void ResourcesIndexer::Init(const char **resource_filenames, const char **initial_resources_dirs, int num_names)
{
	for (int i = 0; i < num_names; i++)
	{
		m_resource_directories.emplace_back(initial_resources_dirs[i]);
		m_resource_filenames.emplace_back(resource_filenames[i]);
	}
}

void ResourcesIndexer::WriteToStream(BaseStream &s) const
{
	int x = 4;
	encode(x, s);
	write_vector_of_strings_to_stream(s, m_resource_directories);
	write_vector_of_strings_to_stream(s, m_resource_filenames);
}

void ResourcesIndexer::ReadFromStream(BaseStream &s)
{
	int x;
	decode(x, s);
	assert(x == 4);
	m_resource_directories.clear();
	m_resource_filenames.clear();
	read_vector_of_strings_from_stream(s, m_resource_directories);
	read_vector_of_strings_from_stream(s, m_resource_filenames);
}


