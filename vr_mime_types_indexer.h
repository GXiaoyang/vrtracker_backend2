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

	bool operator == (const MimeTypesIndexer &rhs) const
	{
		// it's all refs to const tables
		return (m_table_size == rhs.m_table_size && m_table_ref == rhs.m_table_ref);
	}

	bool operator != (const MimeTypesIndexer &rhs) const
	{
		return !(*this == rhs);
	}

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
