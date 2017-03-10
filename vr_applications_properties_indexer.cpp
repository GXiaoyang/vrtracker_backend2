#pragma once

#include "vr_applications_properties_indexer.h"

#define EXPANDO_APP_PROPERTY_UINT64O(x) { vr::VRApplicationProperty_ ## x ## _Uint64, #x }
const PropertiesIndexer::device_property_row application_uint64_properties_table[] =
{
	EXPANDO_APP_PROPERTY_UINT64O(LastLaunchTime)
};

#define EXPANDO_APP_PROPERTY_BOOLO(x) { vr::VRApplicationProperty_ ## x ## _Bool, #x }
const PropertiesIndexer::device_property_row application_bool_properties_table[] =
{
	EXPANDO_APP_PROPERTY_BOOLO(IsDashboardOverlay),
	EXPANDO_APP_PROPERTY_BOOLO(IsTemplate),
	EXPANDO_APP_PROPERTY_BOOLO(IsInstanced),
	EXPANDO_APP_PROPERTY_BOOLO(IsInternal),
};

#define EXPANDO_APP_PROPERTY_STRINGO(x) { vr::VRApplicationProperty_ ## x ## _String, #x }
const PropertiesIndexer::device_property_row application_string_properties_table[] =
{
	EXPANDO_APP_PROPERTY_STRINGO(Name),
	EXPANDO_APP_PROPERTY_STRINGO(LaunchType),
	EXPANDO_APP_PROPERTY_STRINGO(WorkingDirectory),
	EXPANDO_APP_PROPERTY_STRINGO(BinaryPath),
	EXPANDO_APP_PROPERTY_STRINGO(Arguments),
	EXPANDO_APP_PROPERTY_STRINGO(URL),
	EXPANDO_APP_PROPERTY_STRINGO(Description),
	EXPANDO_APP_PROPERTY_STRINGO(NewsURL),
	EXPANDO_APP_PROPERTY_STRINGO(ImagePath),
	EXPANDO_APP_PROPERTY_STRINGO(Source),
};

void ApplicationsPropertiesIndexer::Init()
{
#define TBL_SIZE(t) (sizeof(t)/sizeof(t[0]))

	PropertiesIndexer::Init(
		(const device_property_row*)application_bool_properties_table, TBL_SIZE(application_bool_properties_table),
		(const device_property_row*)application_string_properties_table, TBL_SIZE(application_string_properties_table),
		(const device_property_row*)application_uint64_properties_table, TBL_SIZE(application_uint64_properties_table),
		nullptr, 0,
		nullptr, 0,
		nullptr, 0);
}

void ApplicationsPropertiesIndexer::WriteToStream(EncodeStream &s)
{
	int x = 33;
	encode(x, s);
	PropertiesIndexer::WriteToStream(s);
}

void ApplicationsPropertiesIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 33);
	Init();
	PropertiesIndexer::ReadFromStream(s);
}
