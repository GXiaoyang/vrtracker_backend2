#pragma once

#include "vr_string_indexer.h"

void StringIndexer::WriteToStream(EncodeStream &s)
{
	int x = 88;
	encode(x, s);
	write_string_vector_to_stream(s, keys);
}


void StringIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 88);
	keys.clear();
	keys2index.clear();
	read_string_vector_from_stream(s, keys);
	for (int i = 0; i < (int)keys.size(); i++)
	{
		keys2index.insert({ keys[i].c_str(), i });
	}
}

void StringIndexer::maybe_swap_present_indexes(std::vector<int> * v)
{
	if (*v != present_indexes)
	{
		present_index_lock.lock(); // writer lock
		present_indexes.swap(*v);
		present_index_lock.unlock();
	}
}

