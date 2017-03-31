#include "vr_device_properties_indexer.h"
#include "platform.h"

// int32 device properties
#define EXPANDO_DEVICE_PROPERTY_INT32O(x) { vr::Prop_ ## x ## _Int32 , #x }
const PropertiesIndexer::device_property_row device_int32_properties_table[] =
{
	EXPANDO_DEVICE_PROPERTY_INT32O(DeviceClass),
	EXPANDO_DEVICE_PROPERTY_INT32O(DisplayMCType),
	EXPANDO_DEVICE_PROPERTY_INT32O(EdidVendorID),
	EXPANDO_DEVICE_PROPERTY_INT32O(EdidProductID),
	EXPANDO_DEVICE_PROPERTY_INT32O(DisplayGCType),
	EXPANDO_DEVICE_PROPERTY_INT32O(CameraCompatibilityMode),
	EXPANDO_DEVICE_PROPERTY_INT32O(DisplayMCImageWidth),
	EXPANDO_DEVICE_PROPERTY_INT32O(DisplayMCImageHeight),
	EXPANDO_DEVICE_PROPERTY_INT32O(DisplayMCImageNumChannels),
	EXPANDO_DEVICE_PROPERTY_INT32O(Axis0Type),
	EXPANDO_DEVICE_PROPERTY_INT32O(Axis1Type),
	EXPANDO_DEVICE_PROPERTY_INT32O(Axis2Type),
	EXPANDO_DEVICE_PROPERTY_INT32O(Axis3Type),
	EXPANDO_DEVICE_PROPERTY_INT32O(Axis4Type),
	EXPANDO_DEVICE_PROPERTY_INT32O(ControllerRoleHint),
};

#define EXPANDO_DEVICE_PROPERTY_UINT64O(x) { vr::Prop_ ## x ## _Uint64 , #x }
const PropertiesIndexer::device_property_row device_uint64_properties_table[] =
{
	EXPANDO_DEVICE_PROPERTY_UINT64O(HardwareRevision),
	EXPANDO_DEVICE_PROPERTY_UINT64O(FirmwareVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(FPGAVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(VRCVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(RadioVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(DongleVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(CurrentUniverseId),
	EXPANDO_DEVICE_PROPERTY_UINT64O(PreviousUniverseId),
	EXPANDO_DEVICE_PROPERTY_UINT64O(DisplayFirmwareVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(CameraFirmwareVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(DisplayBootloaderVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(DisplayHardwareVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(AudioFirmwareVersion),
	EXPANDO_DEVICE_PROPERTY_UINT64O(SupportedButtons),
	EXPANDO_DEVICE_PROPERTY_UINT64O(ParentDriver),
};

#define EXPANDO_DEVICE_PROPERTY_MAT34O(x) { vr::Prop_ ## x ## _Matrix34 , #x }
const PropertiesIndexer::device_property_row device_mat34_properties_table[] =
{
	EXPANDO_DEVICE_PROPERTY_MAT34O(StatusDisplayTransform),
	EXPANDO_DEVICE_PROPERTY_MAT34O(CameraToHeadTransform),
};

#define EXPANDO_DEVICE_PROPERTY_FLOATO(x) { vr::Prop_ ## x ## _Float , #x }
const PropertiesIndexer::device_property_row device_float_properties_table[] =
{
	EXPANDO_DEVICE_PROPERTY_FLOATO(DeviceBatteryPercentage),
	EXPANDO_DEVICE_PROPERTY_FLOATO(SecondsFromVsyncToPhotons),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayFrequency),
	EXPANDO_DEVICE_PROPERTY_FLOATO(UserIpdMeters),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayMCOffset),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayMCScale),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayGCBlackClamp),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayGCOffset),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayGCScale),
	EXPANDO_DEVICE_PROPERTY_FLOATO(DisplayGCPrescale),
	EXPANDO_DEVICE_PROPERTY_FLOATO(LensCenterLeftU),
	EXPANDO_DEVICE_PROPERTY_FLOATO(LensCenterLeftV),
	EXPANDO_DEVICE_PROPERTY_FLOATO(LensCenterRightU),
	EXPANDO_DEVICE_PROPERTY_FLOATO(LensCenterRightV),
	EXPANDO_DEVICE_PROPERTY_FLOATO(UserHeadToEyeDepthMeters),
	EXPANDO_DEVICE_PROPERTY_FLOATO(ScreenshotHorizontalFieldOfViewDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(ScreenshotVerticalFieldOfViewDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(FieldOfViewLeftDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(FieldOfViewRightDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(FieldOfViewTopDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(FieldOfViewBottomDegrees),
	EXPANDO_DEVICE_PROPERTY_FLOATO(TrackingRangeMinimumMeters),
	EXPANDO_DEVICE_PROPERTY_FLOATO(TrackingRangeMaximumMeters),
};

#define EXPANDO_DEVICE_PROPERTY_BOOLO(x) { vr::Prop_ ## x ## _Bool , #x }
const PropertiesIndexer::device_property_row device_bool_properties_table[] =
{
	// general properties that apply to all device classes
	EXPANDO_DEVICE_PROPERTY_BOOLO(WillDriftInYaw),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DeviceIsWireless),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DeviceIsCharging),
	EXPANDO_DEVICE_PROPERTY_BOOLO(Firmware_UpdateAvailable),
	EXPANDO_DEVICE_PROPERTY_BOOLO(Firmware_ManualUpdate),
	EXPANDO_DEVICE_PROPERTY_BOOLO(BlockServerShutdown),
	EXPANDO_DEVICE_PROPERTY_BOOLO(CanUnifyCoordinateSystemWithHmd),
	EXPANDO_DEVICE_PROPERTY_BOOLO(ContainsProximitySensor),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DeviceProvidesBatteryStatus),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DeviceCanPowerOff),
	EXPANDO_DEVICE_PROPERTY_BOOLO(HasCamera),
	EXPANDO_DEVICE_PROPERTY_BOOLO(Firmware_ForceUpdateRequired),
	EXPANDO_DEVICE_PROPERTY_BOOLO(ViveSystemButtonFixRequired),
	EXPANDO_DEVICE_PROPERTY_BOOLO(ReportsTimeSinceVSync),
	EXPANDO_DEVICE_PROPERTY_BOOLO(IsOnDesktop),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DisplaySuppressed),
	EXPANDO_DEVICE_PROPERTY_BOOLO(DisplayAllowNightMode),
	EXPANDO_DEVICE_PROPERTY_BOOLO(UsesDriverDirectMode),
};

#define EXPANDO_DEVICE_PROPERTY_STRINGO(x) { vr::Prop_ ## x ## _String , #x }
const PropertiesIndexer::device_property_row device_string_properties_table[] =
{
	EXPANDO_DEVICE_PROPERTY_STRINGO(TrackingSystemName),
	EXPANDO_DEVICE_PROPERTY_STRINGO(ModelNumber),
	EXPANDO_DEVICE_PROPERTY_STRINGO(SerialNumber),
	EXPANDO_DEVICE_PROPERTY_STRINGO(RenderModelName),
	EXPANDO_DEVICE_PROPERTY_STRINGO(ManufacturerName),
	EXPANDO_DEVICE_PROPERTY_STRINGO(TrackingFirmwareVersion),
	EXPANDO_DEVICE_PROPERTY_STRINGO(HardwareRevision),
	EXPANDO_DEVICE_PROPERTY_STRINGO(AllWirelessDongleDescriptions),
	EXPANDO_DEVICE_PROPERTY_STRINGO(ConnectedWirelessDongle),
	EXPANDO_DEVICE_PROPERTY_STRINGO(Firmware_ManualUpdateURL),
	EXPANDO_DEVICE_PROPERTY_STRINGO(Firmware_ProgrammingTarget),
	EXPANDO_DEVICE_PROPERTY_STRINGO(DriverVersion),
	EXPANDO_DEVICE_PROPERTY_STRINGO(DisplayMCImageLeft),
	EXPANDO_DEVICE_PROPERTY_STRINGO(DisplayMCImageRight),
	EXPANDO_DEVICE_PROPERTY_STRINGO(DisplayGCImage),
	EXPANDO_DEVICE_PROPERTY_STRINGO(CameraFirmwareDescription),
	EXPANDO_DEVICE_PROPERTY_STRINGO(AttachedDeviceId),
	EXPANDO_DEVICE_PROPERTY_STRINGO(IconPathName),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceOff),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceSearching),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceSearchingAlert),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceReady),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceReadyAlert),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceNotReady),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceStandby),
	EXPANDO_DEVICE_PROPERTY_STRINGO(NamedIconPathDeviceAlertLow),
	EXPANDO_DEVICE_PROPERTY_STRINGO(UserConfigPath),
	EXPANDO_DEVICE_PROPERTY_STRINGO(InstallPath)
};


void DevicePropertiesIndexer::Init
(
	int num_bool_properties, const char **bool_names, int *bool_values,
	int num_string_properties, const char **string_names, int *string_values,
	int num_uint64_properties, const char **uint64_names, int *uint64_values,
	int num_int32_properties, const char **int32_names, int *int32_values,
	int num_mat34_properties, const char **mat34_names, int *mat34_values,
	int num_float_properties, const char **float_names, int *float_values
)
{
	PropertiesIndexer::Init(
		device_bool_properties_table, TBL_SIZE(device_bool_properties_table),
		device_string_properties_table, TBL_SIZE(device_string_properties_table),
		device_uint64_properties_table, TBL_SIZE(device_uint64_properties_table),
		device_int32_properties_table, TBL_SIZE(device_int32_properties_table),
		device_mat34_properties_table, TBL_SIZE(device_mat34_properties_table),
		device_float_properties_table, TBL_SIZE(device_float_properties_table));

	PropertiesIndexer::AddCustomProperties(
		num_bool_properties, bool_names, bool_values,
		num_string_properties, string_names, string_values,
		num_uint64_properties, uint64_names, uint64_values,
		num_int32_properties, int32_names, int32_values,
		num_mat34_properties, mat34_names, mat34_values,
		num_float_properties, float_names, float_values
	);
}

void DevicePropertiesIndexer::WriteToStream(BaseStream &s) const
{
	int x = 33;
	encode(x, s);
	PropertiesIndexer::WriteToStream(s);
}

void DevicePropertiesIndexer::ReadFromStream(BaseStream &s)
{
	PropertiesIndexer::Init(
		device_bool_properties_table, TBL_SIZE(device_bool_properties_table),
		device_string_properties_table, TBL_SIZE(device_string_properties_table),
		device_uint64_properties_table, TBL_SIZE(device_uint64_properties_table),
		device_int32_properties_table, TBL_SIZE(device_int32_properties_table),
		device_mat34_properties_table, TBL_SIZE(device_mat34_properties_table),
		device_float_properties_table, TBL_SIZE(device_float_properties_table));

	// now read...

	int x;
	decode(x, s);
	assert(x == 33);
	PropertiesIndexer::ReadFromStream(s);
}

