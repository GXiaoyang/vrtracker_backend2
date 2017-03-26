#include "vr_settings_indexer.h"
#include "platform.h"

const char* const steamvrsection_bool_settings_ary[] =
{
	vr::k_pch_SteamVR_DisplayDebug_Bool,
	vr::k_pch_SteamVR_EnableDistortion_Bool,
	vr::k_pch_SteamVR_SendSystemButtonToAllApps_Bool,
	vr::k_pch_SteamVR_BackgroundUseDomeProjection_Bool,
	vr::k_pch_SteamVR_ShowStage_Bool,
	vr::k_pch_SteamVR_ActivateMultipleDrivers_Bool,
	vr::k_pch_SteamVR_DirectMode_Bool,
	vr::k_pch_SteamVR_UsingSpeakers_Bool,
	vr::k_pch_SteamVR_BaseStationPowerManagement_Bool,
	vr::k_pch_SteamVR_NeverKillProcesses_Bool,
	vr::k_pch_SteamVR_AllowAsyncReprojection_Bool,
	vr::k_pch_SteamVR_AllowReprojection_Bool,
	vr::k_pch_SteamVR_ForceReprojection_Bool,
	vr::k_pch_SteamVR_ForceFadeOnBadTracking_Bool,
	vr::k_pch_SteamVR_ShowMirrorView_Bool,
	vr::k_pch_SteamVR_StartMonitorFromAppLaunch,
	vr::k_pch_SteamVR_StartCompositorFromAppLaunch_Bool,
	vr::k_pch_SteamVR_StartDashboardFromAppLaunch_Bool,
	vr::k_pch_SteamVR_StartOverlayAppsFromDashboard_Bool,
	vr::k_pch_SteamVR_EnableHomeApp,
	vr::k_pch_SteamVR_SetInitialDefaultHomeApp,
	vr::k_pch_SteamVR_RetailDemo_Bool,
};

const char* const steamvrsection_stri_settings_ary[] =
{
	vr::k_pch_SteamVR_RequireHmd_String,
	vr::k_pch_SteamVR_ForcedDriverKey_String,
	vr::k_pch_SteamVR_ForcedHmdKey_String,
	vr::k_pch_SteamVR_DebugProcessPipe_String,
	vr::k_pch_SteamVR_Background_String,
	vr::k_pch_SteamVR_GridColor_String,
	vr::k_pch_SteamVR_PlayAreaColor_String,
	vr::k_pch_SteamVR_MirrorViewGeometry_String,
};

const char* const steamvrsection_floa_settings_ary[] =
{
	vr::k_pch_SteamVR_IPD_Float,
	vr::k_pch_SteamVR_BackgroundCameraHeight_Float,
	vr::k_pch_SteamVR_BackgroundDomeRadius_Float,
	vr::k_pch_SteamVR_SpeakersForwardYawOffsetDegrees_Float,
	vr::k_pch_SteamVR_RenderTargetMultiplier_Float,
	vr::k_pch_SteamVR_IpdOffset_Float,
};

const char* const steamvrsection_int32_settings_ary[] =
{
	vr::k_pch_SteamVR_DisplayDebugX_Int32,
	vr::k_pch_SteamVR_DisplayDebugY_Int32,
	vr::k_pch_SteamVR_LogLevel_Int32,
	vr::k_pch_SteamVR_DirectModeEdidVid_Int32,
	vr::k_pch_SteamVR_DirectModeEdidPid_Int32,
	vr::k_pch_SteamVR_DefaultMirrorView_Int32,
};

const char* const lighthousesection_bool_settings_ary[] =
{
	vr::k_pch_Lighthouse_DisableIMU_Bool,
	vr::k_pch_Lighthouse_DBHistory_Bool
};

const char* const lighthousesection_stri_settings_ary[] =
{
	vr::k_pch_Lighthouse_UseDisambiguation_String,
};

const char* const lighthousesection_int32_settings_ary[] =
{
	vr::k_pch_Lighthouse_DisambiguationDebug_Int32,
	vr::k_pch_Lighthouse_PrimaryBasestation_Int32
};

const char* const nullsection_stri_settings_ary[] =
{
	vr::k_pch_Null_SerialNumber_String,
	vr::k_pch_Null_ModelNumber_String
};

const char* const nullsection_floa_settings_ary[] =
{
	vr::k_pch_Null_SecondsFromVsyncToPhotons_Float,
	vr::k_pch_Null_DisplayFrequency_Float
};

const char* const nullsection_int32_settings_ary[] =
{
	vr::k_pch_Null_WindowX_Int32,
	vr::k_pch_Null_WindowY_Int32,
	vr::k_pch_Null_WindowWidth_Int32,
	vr::k_pch_Null_WindowHeight_Int32,
	vr::k_pch_Null_RenderWidth_Int32,
	vr::k_pch_Null_RenderHeight_Int32
};

const char* const uisection_bool_settings_ary[] =
{
	vr::k_pch_UserInterface_StatusAlwaysOnTop_Bool,
	vr::k_pch_UserInterface_MinimizeToTray_Bool,
	vr::k_pch_UserInterface_Screenshots_Bool
};

const char* const uisection_int32_settings_ary[] =
{
	vr::k_pch_UserInterface_ScreenshotType_Int
};

const char* const notificationssection_bool_settings_ary[] =
{
	vr::k_pch_Notifications_DoNotDisturb_Bool
};

const char* const keyboardsection_bool_settings_ary[] =
{
	vr::k_pch_Keyboard_Smoothing
};

const char* const keyboardsection_floa_settings_ary[] =
{
	vr::k_pch_Keyboard_ScaleX,
	vr::k_pch_Keyboard_ScaleY,
	vr::k_pch_Keyboard_OffsetLeftX,
	vr::k_pch_Keyboard_OffsetRightX,
	vr::k_pch_Keyboard_OffsetY
};

const char* const keyboardsection_int32_settings_ary[] =
{
	vr::k_pch_Keyboard_TutorialCompletions
};

const char* const perfsection_bool_settings_ary[] =
{
	vr::k_pch_Perf_HeuristicActive_Bool,
	vr::k_pch_Perf_NotifyInHMD_Bool,
	vr::k_pch_Perf_NotifyOnlyOnce_Bool,
	vr::k_pch_Perf_AllowTimingStore_Bool,
	vr::k_pch_Perf_SaveTimingsOnExit_Bool
};

const char* const perfsection_floa_settings_ary[] =
{
	vr::k_pch_Perf_TestData_Float
};

const char* const collisionsection_bool_settings_ary[] =
{
	vr::k_pch_CollisionBounds_GroundPerimeterOn_Bool,
	vr::k_pch_CollisionBounds_CenterMarkerOn_Bool,
	vr::k_pch_CollisionBounds_PlaySpaceOn_Bool
};

const char* const collisionsection_floa_settings_ary[] =
{
	vr::k_pch_CollisionBounds_FadeDistance_Float
};

const char* const collisionsection_int32_settings_ary[] =
{
	vr::k_pch_CollisionBounds_Style_Int32,
	vr::k_pch_CollisionBounds_ColorGammaR_Int32,
	vr::k_pch_CollisionBounds_ColorGammaG_Int32,
	vr::k_pch_CollisionBounds_ColorGammaB_Int32,
	vr::k_pch_CollisionBounds_ColorGammaA_Int32
};

const char* const camerasection_bool_settings_ary[] =
{
	vr::k_pch_Camera_EnableCamera_Bool,
	vr::k_pch_Camera_EnableCameraInDashboard_Bool,
	vr::k_pch_Camera_EnableCameraForCollisionBounds_Bool,
	vr::k_pch_Camera_EnableCameraForRoomView_Bool,
};

const char* const camerasection_int32_settings_ary[] =
{
	vr::k_pch_Camera_BoundsColorGammaR_Int32,
	vr::k_pch_Camera_BoundsColorGammaG_Int32,
	vr::k_pch_Camera_BoundsColorGammaB_Int32,
	vr::k_pch_Camera_BoundsColorGammaA_Int32,
	vr::k_pch_Camera_BoundsStrength_Int32
};

const char* const audiosection_bool_settings_ary[] =
{
	vr::k_pch_audio_VIVEHDMIGain
};

const char* const audiosection_stri_settings_ary[] =
{
	vr::k_pch_audio_OnPlaybackDevice_String,
	vr::k_pch_audio_OnRecordDevice_String,
	vr::k_pch_audio_OnPlaybackMirrorDevice_String,
	vr::k_pch_audio_OffPlaybackDevice_String,
	vr::k_pch_audio_OffRecordDevice_String,
};

const char* const powersection_bool_settings_ary[] =
{
	vr::k_pch_Power_PowerOffOnExit_Bool,
	vr::k_pch_Power_AutoLaunchSteamVROnButtonPress
};

const char* const powersection_floa_settings_ary[] =
{
	vr::k_pch_Power_TurnOffScreensTimeout_Float,
	vr::k_pch_Power_TurnOffControllersTimeout_Float,
	vr::k_pch_Power_ReturnToWatchdogTimeout_Float
};

const char* const dashboardsection_bool_settings_ary[] =
{
	vr::k_pch_Dashboard_EnableDashboard_Bool,
	vr::k_pch_Dashboard_ArcadeMode_Bool
};

const char *const compositor_floa_settings_ary[] =
{
	vr::k_pch_SteamVR_RenderTargetMultiplier_Float,	// discussed here https://www.reddit.com/r/Vive/comments/59go2a/new_option_to_set_rendertargetmultipler_for/?st=izalffbw&sh=289a4f21
};


#define TBL(tablename) tablename, TBL_SIZE(tablename)
const struct section_def_t
{
	const char *const section_name;
	const char * const *bool_settings_ary;
	int bool_size;
	const char * const *stri_settings_ary;
	int stri_size;
	const char *const*floa_settings_ary;
	int floa_size;
	const char *const*int32_settings_ary;
	int int3_size;
} default_section_defs[] =
{
	{
		vr::k_pch_SteamVR_Section,
		TBL(steamvrsection_bool_settings_ary),
		TBL(steamvrsection_stri_settings_ary),
		TBL(steamvrsection_floa_settings_ary),
		TBL(steamvrsection_int32_settings_ary)
	},
	{
		vr::k_pch_Lighthouse_Section,
		TBL(lighthousesection_bool_settings_ary),
		TBL(lighthousesection_stri_settings_ary),
		nullptr, 0,
		TBL(lighthousesection_int32_settings_ary)
	},
	{
		vr::k_pch_Null_Section,
		nullptr,0,
		TBL(nullsection_stri_settings_ary),
		TBL(nullsection_floa_settings_ary),
		TBL(nullsection_int32_settings_ary)
	},

	{
		vr::k_pch_UserInterface_Section,
		TBL(uisection_bool_settings_ary),
		nullptr, 0,
		nullptr, 0,
		TBL(uisection_int32_settings_ary)
	},
	{
		vr::k_pch_Notifications_Section,
		TBL(notificationssection_bool_settings_ary),
		nullptr, 0,
		nullptr, 0,
		nullptr, 0,
	},
	{
		vr::k_pch_Keyboard_Section,
		TBL(keyboardsection_bool_settings_ary),
		nullptr, 0,
		TBL(keyboardsection_floa_settings_ary),
		TBL(keyboardsection_int32_settings_ary)
	},
	{
		vr::k_pch_Perf_Section,
		TBL(perfsection_bool_settings_ary),
		nullptr, 0,
		TBL(perfsection_floa_settings_ary),
		nullptr //perfsection_int32_settings_ary
	},
	{
		vr::k_pch_CollisionBounds_Section,
		TBL(collisionsection_bool_settings_ary),
		nullptr, 0, //collisionsection_string_settings_ary),
		TBL(collisionsection_floa_settings_ary),
		TBL(collisionsection_int32_settings_ary)
	},
	{
		vr::k_pch_Camera_Section,
		TBL(camerasection_bool_settings_ary),
		nullptr, 0,
		nullptr, 0,
		TBL(camerasection_int32_settings_ary)
	},
	{
		vr::k_pch_audio_Section,
		TBL(audiosection_bool_settings_ary),
		TBL(audiosection_stri_settings_ary),
		nullptr, 0,
		nullptr, 0
	},
	{
		vr::k_pch_Power_Section,
		TBL(powersection_bool_settings_ary),
		nullptr, 0,
		TBL(powersection_floa_settings_ary),
		nullptr, 0,
	},
	{
		vr::k_pch_Dashboard_Section,
		TBL(dashboardsection_bool_settings_ary),
		nullptr, 0,
		nullptr, 0,
		nullptr, 0
	},
	{
		"compositor",
		nullptr, 0,
		nullptr, 0,
		TBL(compositor_floa_settings_ary),
		nullptr, 0
	},
};

static const int DEFAULT_SECTIONS_TABLE_SIZE = sizeof(default_section_defs) / sizeof(default_section_defs[0]);


// setting clients use string sections and string names to find them
bool SettingsIndexer::AddCustomSetting(const char *section_name_in, SectionSettingType section_type, const char *setting_name_in)
{
	auto section_iter = name2section.find(section_name_in);
	string2int::iterator field_iter;
	if (section_iter != name2section.end())
	{
		field_iter = sections[section_iter->second].typed_data[section_type].fieldname2index.find(setting_name_in);
	}

	if (section_iter == name2section.end() || field_iter == sections[section_iter->second].typed_data[section_type].fieldname2index.end())
	{
		// do sme
		custom_settings[section_type].emplace_back(setting_name_in);
		custom_sections[section_type].emplace_back(section_name_in);
		const char *setting_name = custom_settings[section_type].back().c_str();
		const char *section_name = custom_sections[section_type].back().c_str();

		int section_index;
		if (section_iter == name2section.end())
		{
			section_index = sections.size();
			name2section.insert({ section_name, section_index });
			sections.emplace_back(section_name);
		}
		else
		{
			section_index = section_iter->second;
		}

		int field_index = sections[section_index].typed_data[section_type].fieldnames.size();
		sections[section_index].typed_data[section_type].fieldnames.push_back(setting_name);
		sections[section_index].typed_data[section_type].fieldname2index.insert({ setting_name, field_index });
	}
	return true;
}

void SettingsIndexer::InitDefault()
{
	int num_sections = DEFAULT_SECTIONS_TABLE_SIZE;
	sections.resize(num_sections);
	for (int i = 0; i < DEFAULT_SECTIONS_TABLE_SIZE; i++)
	{
		const section_def_t *def = &default_section_defs[i];
		name2section[def->section_name] = i;
		sections[i].section_name = def->section_name;
		sections[i].typed_data[SETTING_TYPE_BOOL].fieldnames.resize(def->bool_size);
		for (int j = 0; j < def->bool_size; j++)
		{
			const char *field_name = def->bool_settings_ary[j];
			sections[i].typed_data[SETTING_TYPE_BOOL].fieldnames[j] = field_name;
			sections[i].typed_data[SETTING_TYPE_BOOL].fieldname2index[field_name] = j;
		}

		sections[i].typed_data[SETTING_TYPE_STRING].fieldnames.resize(def->stri_size);
		for (int j = 0; j < def->stri_size; j++)
		{
			const char *field_name = def->stri_settings_ary[j];
			sections[i].typed_data[SETTING_TYPE_STRING].fieldnames[j] = field_name;
			sections[i].typed_data[SETTING_TYPE_STRING].fieldname2index[field_name] = j;
		}

		sections[i].typed_data[SETTING_TYPE_FLOAT].fieldnames.resize(def->floa_size);
		for (int j = 0; j < def->floa_size; j++)
		{
			const char *field_name = def->floa_settings_ary[j];
			sections[i].typed_data[SETTING_TYPE_FLOAT].fieldnames[j] = field_name;
			sections[i].typed_data[SETTING_TYPE_FLOAT].fieldname2index[field_name] = j;
		}

		sections[i].typed_data[SETTING_TYPE_INT32].fieldnames.resize(def->int3_size);
		for (int j = 0; j < def->int3_size; j++)
		{
			const char *field_name = def->int32_settings_ary[j];
			sections[i].typed_data[SETTING_TYPE_INT32].fieldnames[j] = field_name;
			sections[i].typed_data[SETTING_TYPE_INT32].fieldname2index[field_name] = j;
		}
	}
}


void SettingsIndexer::Init(
	int num_bool_settings, const char **bool_setting_sections, const char **bool_setting_names,
	int num_int32_settings, const char **int32_setting_sections, const char **int32_setting_names,
	int num_string_settings, const char **string_setting_sections, const char **string_setting_names,
	int num_float_settings, const char **float_setting_sections, const char **float_setting_names)
{
	InitDefault();
	for (int i = 0; i < num_bool_settings; i++)
	{
		AddCustomSetting(bool_setting_sections[i], SectionSettingType::SETTING_TYPE_BOOL, bool_setting_names[i]);
	}
	for (int i = 0; i < num_int32_settings; i++)
	{
		AddCustomSetting(int32_setting_sections[i], SectionSettingType::SETTING_TYPE_INT32, int32_setting_names[i]);
	}
	for (int i = 0; i < num_string_settings; i++)
	{
		AddCustomSetting(string_setting_sections[i], SectionSettingType::SETTING_TYPE_STRING, string_setting_names[i]);
	}
	for (int i = 0; i < num_float_settings; i++)
	{
		AddCustomSetting(float_setting_sections[i], SectionSettingType::SETTING_TYPE_FLOAT, float_setting_names[i]);
	}
}

void SettingsIndexer::WriteToStream(EncodeStream &s) const
{
	int x = 33;
	encode(x, s);
	for (int i = 0; i < NUM_SETTING_TYPES; i++)
	{
		write_vector_of_strings_to_stream(s, custom_sections[i]);
		write_vector_of_strings_to_stream(s, custom_settings[i]);
	}
}

void SettingsIndexer::ReadFromStream(EncodeStream &s)
{
	int x;
	decode(x, s);
	assert(x == 33);
	InitDefault();
	std::vector<std::string> tmp_sections;
	std::vector<std::string> tmp_settings;

	for (int i = 0; i < NUM_SETTING_TYPES; i++)
	{
		tmp_sections.clear();
		tmp_settings.clear();
		read_vector_of_strings_from_stream(s, tmp_sections);
		read_vector_of_strings_from_stream(s, tmp_settings);

		for (int j = 0; j < size_as_int(tmp_sections.size()); j++)
		{
			AddCustomSetting(tmp_sections[j].c_str(), SectionSettingType(i), tmp_settings[j].c_str());
		}
	}
}

