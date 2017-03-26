#pragma once
#include "vr_properties_indexer.h"

// ApplicationsPropertiesIndexer
// * Customized version of a PropertiesIndexer to answer appl
class ApplicationsPropertiesIndexer : public PropertiesIndexer
{
public:
	void Init();
	void WriteToStream(EncodeStream &s) const;
	void ReadFromStream(EncodeStream &s);
};
