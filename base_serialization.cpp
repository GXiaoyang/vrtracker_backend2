#include "base_serialization.h"
#include <iostream>


SerializableRegistry::SerializableRegistry(const SerializableRegistry &rhs)
	: 
#ifdef _DEBUG
	map(rhs.map),
#endif
	registered(rhs.registered)
{}

SerializableRegistry &SerializableRegistry::operator=(const SerializableRegistry& rhs)
{
#ifdef _DEBUG
	map = rhs.map;
#endif
	registered = rhs.registered;
	return *this;
}

bool SerializableRegistry::operator==(const SerializableRegistry &rhs) const
{
	if (this == &rhs)
		return true;
	size_t my_size = registered.size();
	size_t rhs_size = rhs.registered.size();
	if (my_size != rhs_size)
		return false;

	auto iter_a = registered.begin();
	auto iter_b = rhs.registered.begin();
	// check that the urls match
	while (iter_a != registered.end())
	{
		RegisteredSerializable *a = *iter_a;
		RegisteredSerializable *b = *iter_b;
		if (a->get_serialization_url() != b->get_serialization_url())
			return false;
		++iter_a;
		++iter_b;
	}
	return true;
}

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