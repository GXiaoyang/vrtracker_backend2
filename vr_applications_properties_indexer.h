#pragma once
#include "vr_properties_indexer.h"

class ApplicationsPropertiesIndexer : public PropertiesIndexer
{
public:
	void Init();
	void WriteToStream(EncodeStream &s);
	void ReadFromStream(EncodeStream &s);
};
