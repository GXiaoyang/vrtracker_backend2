#pragma once
// MimeTypesIndexer
//
// MimeTypeName <-> index map
//
#include "platform.h"
#include "string.h"

class MimeTypesIndexer
{
public:
	MimeTypesIndexer();
	inline bool GetIndexForMimeType(const char *pchMimeType, int *index);
	const char *GetNameForIndex(int index) const { return m_table_ref[index]; }
	size_t GetNumMimeTypes() const { return m_table_size;  }
private:
	size_t m_table_size;
	const char **m_table_ref;
};

inline bool MimeTypesIndexer::GetIndexForMimeType(const char *pchMimeType, int *index)
{
	for (int i = 0; i < size_as_int(m_table_size); i++)
	{
		if (strcmp(m_table_ref[i], pchMimeType) == 0)
		{
			*index = i;
			return true;
		}
	}
	return false;
}
