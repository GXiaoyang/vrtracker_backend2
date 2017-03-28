#include "base_serialization.h"
#include <iostream>

// register objects by an id so they can be found for serialization and deserialization
void SerializableRegistry::dump() const
{
	for (auto entry : registered)
	{
		auto url = entry->get_serialization_url();
		auto index = entry->get_serialization_index();
		std::cout << "id: " << index << " url:" << url.get_full_path() << std::endl;
	}
};