#pragma once

#include "string2int.h"
#include "openvr_serialization.h"

// setting clients use string sections and string names to find them
class SettingsIndexer
{
public:
	enum SectionSettingType
	{
		SETTING_TYPE_BOOL,
		SETTING_TYPE_STRING,
		SETTING_TYPE_FLOAT,
		SETTING_TYPE_INT32,
		NUM_SETTING_TYPES
	};

	bool AddCustomSetting(const char *section_name_in, SectionSettingType section_type, const char *setting_name_in);

	void Init(
		int num_bool_settings, const char **bool_setting_sections, const char **bool_setting_names,
		int num_int32_settings, const char **int32_setting_sections, const char **int32_setting_names,
		int num_string_settings, const char **string_setting_sections, const char **string_setting_names,
		int num_float_settings, const char **float_setting_sections, const char **float_setting_names);

	void WriteToStream(EncodeStream &s);
	void ReadFromStream(EncodeStream &s);

	// given a section, setting_type and setting name,
	//	* if found, returns the section and setting index for it
	bool GetIndexes(const char *section_name, SectionSettingType setting_type, const char *setting_name,
		int *section_in, int *setting_in)
	{
		bool rc = false;
		auto iter = name2section.find(section_name);
		if (iter != name2section.end())
		{
			int section = iter->second;
			auto field_iter = sections[section].typed_data[setting_type].fieldname2index.find(setting_name);
			if (field_iter != sections[section].typed_data[setting_type].fieldname2index.end())
			{
				*section_in = section;
				*setting_in = field_iter->second;
				rc = true;
			}
		}
		return rc;
	}

	const char *GetSectionName(int section)
	{
		return sections[section].section_name;
	}

	int GetNumSections()
	{
		return (int)sections.size();
	}

	int GetNumFields(const char *section_name, SectionSettingType setting_type)
	{
		int rc = 0;
		auto iter = name2section.find(section_name);
		if (iter != name2section.end())
		{
			int section = iter->second;
			rc = sections[section].typed_data[setting_type].fieldnames.size();
		}
		return rc;
	}
	const char **GetFieldNames(const char *section_name, SectionSettingType setting_type)
	{
		const char **rc = nullptr;
		auto iter = name2section.find(section_name);
		if (iter != name2section.end())
		{
			int section = iter->second;
			rc = &sections[section].typed_data[setting_type].fieldnames.at(0);
		}
		return rc;
	}
private:

	void InitDefault();

	// settings of a type
	struct subtable
	{
		std::vector<const char *> fieldnames;
		string2int fieldname2index;
	};

	struct section_data
	{
		section_data() {}
		section_data(const char *s) : section_name(s) {}
		const char *section_name;
		subtable typed_data[NUM_SETTING_TYPES];
	};


	std::vector < std::string > custom_sections[NUM_SETTING_TYPES];
	std::vector < std::string > custom_settings[NUM_SETTING_TYPES];
	std::vector<section_data> sections;
	string2int name2section;
};
