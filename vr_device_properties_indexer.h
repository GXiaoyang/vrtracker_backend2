#pragma once

#include "vr_properties_indexer.h"

class DevicePropertiesIndexer : public PropertiesIndexer
{
public:
	void Init(
		int num_bool_properties, const char **bool_names, int *bool_values,
		int num_string_properties, const char **string_names, int *string_values,
		int num_uint64_properties, const char **uint64_names, int *uint64_values,
		int num_int32_properties, const char **int32_names, int *int32_values,
		int num_mat34_properties, const char **mat34_names, int *mat34_values,
		int num_float_properties, const char **float_names, int *float_values
	);
	void WriteToStream(EncodeStream &s) const;
	void ReadFromStream(EncodeStream &s);
};
