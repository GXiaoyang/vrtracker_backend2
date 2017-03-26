#include "vr_string_indexer.h"

void StringIndexer::WriteToStream(EncodeStream &s) const
{
	int x = 88;
	encode(x, s);
	write_vector_of_strings_to_stream(s, keys);
}


void StringIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 88);
	keys.clear();
	keys2index.clear();
	read_vector_of_strings_from_stream(s, keys);
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

