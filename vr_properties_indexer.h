// PropertiesIndexer -	
//		BaseClass for device and application property names and enums
//		Use with the wrapper classes to do the actual queries
#pragma once
#include <unordered_map>
#include <vector>
#include "openvr_serialization.h"

class PropertiesIndexer
{
public:
	enum PropertySettingType
	{
		PROP_BOOL,
		PROP_STRING,
		PROP_UINT64,
		PROP_INT32,
		PROP_MAT34,
		PROP_FLOAT,
		NUM_PROP_TYPES,
	};

	void AddCustomProperties(
		int num_bool_properties, const char **bool_names, int *bool_enum_values,
		int num_string_properties, const char **string_names, int *string_enum_values,
		int num_uint64_properties, const char **uint64_names, int *uint64_enum_values,
		int num_int32_properties, const char **int32_names, int *int32_enum_values,
		int num_mat34_properties, const char **mat34_names, int *mat34_enum_values,
		int num_float_properties, const char **float_names, int *float_enum_values
	);

	// spawns new name
	void AddCustomProperty(PropertySettingType prop_type, const char *name, int val);

	inline bool GetIndexForEnum(PropertySettingType setting_type, int enum_val, int *index)
	{
		bool rc = false;
		auto iter = enum2index[setting_type].find(enum_val);
		if (iter != enum2index[setting_type].end())
		{
			*index = iter->second;
			rc = true;
		}
		return rc;
	}

	inline int GetNumPropertiesOfType(PropertySettingType setting_type)
	{
		return enum2index[setting_type].size();
	}

	inline int GetEnumVal(PropertySettingType setting_type, int index)
	{
		if (index < default_property_table[setting_type].size)
			return default_property_table[setting_type].rows[index].enum_val;
		else
			return custom_enum_values[setting_type][index - default_property_table[setting_type].size];
	}
	
	inline const char* GetName(PropertySettingType setting_type, int index)
	{
		if (index < default_property_table[setting_type].size)
			return default_property_table[setting_type].rows[index].name;
		else
			return custom_names[setting_type][index - default_property_table[setting_type].size].c_str();
	}

	void WriteToStream(BaseStream &s) const;
	void ReadFromStream(BaseStream &s);

	bool operator==(const PropertiesIndexer &rhs) const
	{
		for (int i = 0; i < NUM_PROP_TYPES; i++)
		{
			if (default_property_table[i].rows != rhs.default_property_table[i].rows) // it's a pointer compare but this should be ok since it's a compile time table pointer
			{
				return false;
			}
			if (default_property_table[i].size != rhs.default_property_table[i].size)
			{
				return false;
			}
			if (enum2index[i] != rhs.enum2index[i])
				return false;
			if (custom_names[i] != rhs.custom_names[i])
				return false;
			if (custom_enum_values[i] != rhs.custom_enum_values[i])
				return false;
		}
		return true;
	}

	bool operator!=(const PropertiesIndexer &rhs) const
	{
		return !(*this == rhs);
	}

	
	// configuration table
	struct device_property_row
	{
		int enum_val;
		const char *name;
	};

protected:
	// i want properties to have names
	// i want them to have indexes into node arrays
	// properties have enum values associated with them
	// properties don't have sections
	void Init(
		const device_property_row *bool_properties, int num_bool_properties,
		const device_property_row *string_properties, int num_string_properties,
		const device_property_row *uint64_properties, int num_uint64_properties,
		const device_property_row *int32_properties, int num_int32_properties,
		const device_property_row *mat34_properties, int num_mat34_properties,
		const device_property_row *float_properties, int num_float_properties
	);

private:
	void HashDefaultRows(PropertySettingType setting_type);
	void AddCustomPropertiesArray(PropertySettingType setting_type, int num, const char **names, int *values);

	// there are default properties loaded at construction time and stored in the following table:
	struct
	{
		const device_property_row *rows;
		int size;
	} default_property_table[NUM_PROP_TYPES];

	std::unordered_map<int, int> enum2index[NUM_PROP_TYPES];
	std::vector<std::string> custom_names[NUM_PROP_TYPES];
	std::vector<int> custom_enum_values[NUM_PROP_TYPES];
};
