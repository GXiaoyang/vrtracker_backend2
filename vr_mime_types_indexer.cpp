#include "vr_mime_types_indexer.h"
#include <openvr.h>

const char* mime_types[] =
{
	vr::k_pch_MimeType_HomeApp,
	vr::k_pch_MimeType_GameTheater,
};
static const int mime_tbl_size = sizeof(mime_types) / sizeof(mime_types[0]);

MimeTypesIndexer::MimeTypesIndexer()
	:	m_table_size(mime_tbl_size), 
		m_table_ref(mime_types)
{}
