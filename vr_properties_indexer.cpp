#include "platform.h"
#include "vr_properties_indexer.h"

void PropertiesIndexer::Init(
	const device_property_row *bool_properties, int num_bool_properties,
	const device_property_row *string_properties, int num_string_properties,
	const device_property_row *uint64_properties, int num_uint64_properties,
	const device_property_row *int32_properties, int num_int32_properties,
	const device_property_row *mat34_properties, int num_mat34_properties,
	const device_property_row *float_properties, int num_float_properties
)
{
	default_property_table[PROP_BOOL].rows = bool_properties;
	default_property_table[PROP_BOOL].size = num_bool_properties;
	HashDefaultRows(PROP_BOOL);

	default_property_table[PROP_STRING].rows = string_properties;
	default_property_table[PROP_STRING].size = num_string_properties;
	HashDefaultRows(PROP_STRING);

	default_property_table[PROP_UINT64].rows = uint64_properties;
	default_property_table[PROP_UINT64].size = num_uint64_properties;
	HashDefaultRows(PROP_UINT64);

	default_property_table[PROP_INT32].rows = int32_properties;
	default_property_table[PROP_INT32].size = num_int32_properties;
	HashDefaultRows(PROP_INT32);

	default_property_table[PROP_MAT34].rows = mat34_properties;
	default_property_table[PROP_MAT34].size = num_mat34_properties;
	HashDefaultRows(PROP_MAT34);

	default_property_table[PROP_FLOAT].rows = float_properties;
	default_property_table[PROP_FLOAT].size = num_float_properties;
	HashDefaultRows(PROP_FLOAT);
}

void PropertiesIndexer::HashDefaultRows(PropertySettingType setting_type)
{
	enum2index[setting_type].reserve(default_property_table[setting_type].size);

	for (int i = 0; i < default_property_table[setting_type].size; i++)
	{
		enum2index[setting_type].insert({ (int)default_property_table[setting_type].rows[i].enum_val, i });
	}
}

void PropertiesIndexer::AddCustomPropertiesArray(PropertySettingType setting_type, int num, const char **names, int *values)
{
	for (int i = 0; i < num; i++)
	{
		AddCustomProperty(setting_type, names[i], values[i]);
	}
}

void PropertiesIndexer::AddCustomProperty(PropertySettingType prop_type, const char *name, int val)
{
	assert(enum2index[prop_type].find(val) == enum2index[prop_type].end());
	custom_names[prop_type].push_back(name);
	custom_enum_values[prop_type].push_back(val);
	int index = enum2index[prop_type].size();
	enum2index[prop_type].insert({ val, index });
}

void PropertiesIndexer::AddCustomProperties(
	int num_bool_properties, const char **bool_names, int *bool_values,
	int num_string_properties, const char **string_names, int *string_values,
	int num_uint64_properties, const char **uint64_names, int *uint64_values,
	int num_int32_properties, const char **int32_names, int *int32_values,
	int num_mat34_properties, const char **mat34_names, int *mat34_values,
	int num_float_properties, const char **float_names, int *float_values
)
{
	AddCustomPropertiesArray(PROP_BOOL, num_bool_properties, bool_names, bool_values);
	AddCustomPropertiesArray(PROP_STRING, num_string_properties, string_names, string_values);
	AddCustomPropertiesArray(PROP_UINT64, num_uint64_properties, uint64_names, uint64_values);
	AddCustomPropertiesArray(PROP_INT32, num_int32_properties, int32_names, int32_values);
	AddCustomPropertiesArray(PROP_MAT34, num_mat34_properties, mat34_names, mat34_values);
	AddCustomPropertiesArray(PROP_FLOAT, num_float_properties, float_names, float_values);
}

void PropertiesIndexer::WriteToStream(EncodeStream &s)
{
	for (int i = 0; i < NUM_PROP_TYPES; i++)
	{
		write_string_vector_to_stream(s, custom_names[i]);
		write_int_vector_to_stream(s, custom_enum_values[i]);
	}
}

void PropertiesIndexer::ReadFromStream(EncodeStream &s)
{
	for (int setting_type = 0; setting_type < NUM_PROP_TYPES; setting_type++)
	{
		assert(custom_names[setting_type].size() == 0); // assume init has been called and properties are empty - otherwise check load and init seq.
		assert(custom_enum_values[setting_type].size() == 0); // assume init has been called and properties are empty - otherwise check load and init seq.

		read_string_vector_from_stream(s, custom_names[setting_type]);
		read_int_vector_from_stream(s, custom_enum_values[setting_type]);

		enum2index[setting_type].reserve(enum2index[setting_type].size() + custom_enum_values[setting_type].size());
		for (int i = 0; i < size_as_int(custom_enum_values[setting_type].size()); i++)
		{
			int index = enum2index[setting_type].size();
			enum2index[setting_type].insert({ (int)custom_enum_values[setting_type][i], index });
		}
	}
}
