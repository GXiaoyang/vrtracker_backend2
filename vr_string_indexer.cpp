#include "vr_string_indexer.h"


StringIndexer::StringIndexer(const StringIndexer &rhs)
	:
	keys(rhs.keys),
	keys2index(rhs.keys2index),
	live_indexes(rhs.live_indexes)
{
	updated_size = keys.size();
}

StringIndexer &StringIndexer::operator=(const StringIndexer &rhs)
{
	keys = rhs.keys;
	keys2index = rhs.keys2index;
	live_indexes = rhs.live_indexes;
	updated_size = keys.size();
	return *this;
}

void StringIndexer::WriteToStream(BaseStream &s) const
{
	int size = updated_size;
	s.write_to_stream(&size, sizeof(size));

	write_vector_of_strings_to_stream(s, keys);
	s.forward_container_out_to_stream(live_indexes);
}

void StringIndexer::ReadFromStream(BaseStream &s)
{
	int size;
	s.read_from_stream(&size, sizeof(size));
	updated_size = size;

	keys.clear();
	keys2index.clear();
	read_vector_of_strings_from_stream(s, keys);

	s.forward_container_from_stream(live_indexes);

	// rebuild hash
	for (int i = 0; i < size_as_int(keys.size()); i++)
	{
		keys2index.insert({ keys[i].c_str(), i });
	}
}

void StringIndexer::maybe_swap_live_indexes(std::vector<int> * v)
{
	if (*v != live_indexes)
	{
		live_index_lock.lock(); // writer lock
		live_indexes.swap(*v);
		live_index_lock.unlock();
	}
}