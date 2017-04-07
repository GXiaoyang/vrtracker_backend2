#include <windows.h>
#include "openvr_dll_client.h"
#include "log.h"

// when opening an openvr dll, the following names will be queried using GetProcAddress
static const char *outer_proc_names[] =
{
	//"LiquidVR",								// {openvr_api.dll!LiquidVR(void)} int(__stdcall*)()
	//"VRCompositorSystemInternal",             //openvr_api.dll!VRCompositorSystemInternal(void)}	int(__stdcall*)()
	//"VRControlPanel",							//openvr_api.dll!VRControlPanel(void)}	int(__stdcall*)()
	//"VRDashboardManager",						//openvr_api.dll!VRDashboardManager(void)}	int(__stdcall*)()
	//"VROculusDirect",							//openvr_api.dll!VROculusDirect(void)}	int(__stdcall*)()
	//"VRRenderModelsInternal",					//openvr_api.dll!VRRenderModelsInternal(void)}	int(__stdcall*)()
	//"VRTrackedCameraInternal",					//openvr_api.dll!VRTrackedCameraInternal(void)}	int(__stdcall*)()
	/*0*/"VR_GetGenericInterface",					//openvr_api.dll!VR_GetGenericInterface(const char *, vr::EVRInitError *)}	int(__stdcall*)()
	/*1*/"VR_GetInitToken",							//openvr_api.dll!VR_GetInitToken(void)}	int(__stdcall*)()
	/*2*/"VR_GetStringForHmdError",					//openvr_api.dll!VR_GetStringForHmdError(vr::EVRInitError)}	int(__stdcall*)()
	/*3*/"VR_GetVRInitErrorAsEnglishDescription",	//openvr_api.dll!VR_GetStringForHmdError(vr::EVRInitError)}	int(__stdcall*)()
	/*4*/"VR_GetVRInitErrorAsSymbol",				//openvr_api.dll!VR_GetVRInitErrorAsSymbol(vr::EVRInitError)}	int(__stdcall*)()
	/*5*/"VR_InitInternal",							//openvr_api.dll!VR_InitInternal(vr::EVRInitError *, vr::EVRApplicationType)}	int(__stdcall*)()
	/*6*/"VR_IsHmdPresent",							//openvr_api.dll!VR_IsHmdPresent(void)}	int(__stdcall*)()
	/*7*/"VR_IsInterfaceVersionValid",				//openvr_api.dll!VR_IsInterfaceVersionValid(const char *)}	int(__stdcall*)()
	/*8*/"VR_IsRuntimeInstalled",					//openvr_api.dll!VR_IsRuntimeInstalled(void)}	int(__stdcall*)()
	/*9*/"VR_RuntimePath",							//openvr_api.dll!VR_RuntimePath(void)}	int(__stdcall*)()
	/*10*/"VR_ShutdownInternal"						//openvr_api.dll!VR_ShutdownInternal(void)}	int(__stdcall*)()
};

// after initial lookup, function pointers will be stored into an array using the following indexes
enum
{
	/*0*/indexVR_GetGenericInterface,					//openvr_api.dll!VR_GetGenericInterface(const char *, vr::EVRInitError *)}	int(__stdcall*)()
	/*1*/indexVR_GetInitToken,							//openvr_api.dll!VR_GetInitToken(void)}	int(__stdcall*)()
	/*2*/indexVR_GetStringForHmdError,					//openvr_api.dll!VR_GetStringForHmdError(vr::EVRInitError)}	int(__stdcall*)()
	/*3*/indexVR_GetVRInitErrorAsEnglishDescription,	//openvr_api.dll!VR_GetStringForHmdError(vr::EVRInitError)}	int(__stdcall*)()
	/*4*/indexVR_GetVRInitErrorAsSymbol,				//openvr_api.dll!VR_GetVRInitErrorAsSymbol(vr::EVRInitError)}	int(__stdcall*)()
	/*5*/indexVR_InitInternal,							//openvr_api.dll!VR_InitInternal(vr::EVRInitError *, vr::EVRApplicationType)}	int(__stdcall*)()
	/*6*/indexVR_IsHmdPresent,							//openvr_api.dll!VR_IsHmdPresent(void)}	int(__stdcall*)()
	/*7*/indexVR_IsInterfaceVersionValid,				//openvr_api.dll!VR_IsInterfaceVersionValid(const char *)}	int(__stdcall*)()
	/*8*/indexVR_IsRuntimeInstalled,					//openvr_api.dll!VR_IsRuntimeInstalled(void)}	int(__stdcall*)()
	/*9*/indexVR_RuntimePath,							//openvr_api.dll!VR_RuntimePath(void)}	int(__stdcall*)()
	/*10*/indexVR_ShutdownInternal						//openvr_api.dll!VR_ShutdownInternal(void)}	int(__stdcall*)()
};

// size of the table
static const int NUM_OUTER_PROCS = sizeof(outer_proc_names) / sizeof(outer_proc_names[0]);

// since the funciton pointer table holds generic pointers, they need to be static cast into the following for 
// the compiler to not complain
typedef int(*voidReturnsVoid)();
typedef int(*voidReturnsBool)();
typedef int(*voidReturnsInt)();
typedef char *(intReturnsChar)();

// these are the openvr_api (ie the inner interface) interfaces names that will be requested by calls
// to GetGenericInterface
static const char *inner_cpp_interface_names[] =
{
	"IVRSystem_015",
	"IVRApplications_006",
	"IVRSettings_002",
	"IVRChaperone_003",
	"IVRChaperoneSetup_005",
	"IVRCompositor_020",
	"IVRNotifications_002",
	"IVROverlay_014",
	"IVRRenderModels_005",
	"IVRExtendedDisplay_001",
	"IVRTrackedCamera_003",
	"IVRScreenshots_001",
	"IVRResources_001"
};
static const char *inner_c_interface_names[] =
{
	"FnTable:IVRSystem_015",
	"FnTable:IVRApplications_006",
	"FnTable:IVRSettings_002",
	"FnTable:IVRChaperone_003",
	"FnTable:IVRChaperoneSetup_005",
	"FnTable:IVRCompositor_019",
	"FnTable:IVRNotifications_002",
	"FnTable:IVROverlay_014",
	"FnTable:IVRRenderModels_005",
	"FnTable:IVRExtendedDisplay_001",
	"FnTable:IVRTrackedCamera_003",
	"FnTable:IVRScreenshots_001",
	"FnTable:IVRResources_001"
};

enum
{
	index_IVRSystem,
	index_IVRApplications,
	index_IVRSettings,
	index_IVRChaperone,
	index_IVRChaperoneSetup,
	index_IVRCompositor,
	index_IVRNotifications,
	index_IVROverlay,
	index_IVRRenderModels,
	index_IVRExtendedDisplay,
	index_IVRTrackedCamera,
	index_IVRScreenshots,
	index_IVRResources
};

static const int NUM_INNER_INTERFACES = sizeof(inner_cpp_interface_names) / sizeof(inner_cpp_interface_names[0]);

// utility
static bool QueryInterfaces(FARPROC p, void **interface_table, const char **interface_names)
{
	typedef void* (*VR_GetGenericInterface)(const char *, int *);
	bool rc = true;
	for (int i = 0; i < NUM_INNER_INTERFACES; i++)
	{
		const char *expected_valid_version = interface_names[i];
		int error_code;
		void *handle = (*(VR_GetGenericInterface)p)(expected_valid_version, &error_code);
		if (handle)
		{
			// interface version is valid
			//log_printf("%p %s\n", handle, expected_valid_version);
			interface_table[i] = handle;
		}
		else
		{
			log_printf("got failed handle for version: %s error code: %d\n", expected_valid_version, error_code);
			interface_table[i] = nullptr;
			rc = false;
		}
	}
	return rc;
}

// internal state of the openvr_dll_client class
struct openvr_dll_client::imp 
{
private:
	HMODULE module;
	FARPROC proc_table[NUM_OUTER_PROCS];
	void *cpp_interface_table[NUM_INNER_INTERFACES];
	void *c_interface_table[NUM_INNER_INTERFACES];
	bool InitializeInnerInterfaces();

public:
	imp();
	~imp();

	bool open_lib(LPCTSTR library_name, LPCTSTR extra_path);
	bool close_lib();

	void *GetCPPInterfaceTable() { return cpp_interface_table; }
	void *GetCInterfaceTable() { return c_interface_table; }

	void call_ShutdownInternal();
	bool call_IsInterfaceVersionValid(const char *interface_name);
	bool call_VRInit();
	bool call_IsRuntimeInstalled();
	bool call_IsHmdPresent();
	const char *call_RuntimePath();
	int call_GetInitToken();
};

openvr_dll_client::imp::imp()
{
	memset(this, 0, sizeof(this));
}

openvr_dll_client::imp::~imp()
{
	close_lib();
}

bool openvr_dll_client::imp::open_lib(LPCTSTR library_name, LPCTSTR extra_path)
{
	module = LoadLibraryEx(library_name, NULL, 0);
	if (!module)
	{
		wprintf(L"could not find %s in default path\n", library_name);
		DLL_DIRECTORY_COOKIE c = AddDllDirectory(extra_path);
		if (!c)
		{
			wprintf(L"could not add %s\n", extra_path);
		}
		else
		{
			module = LoadLibraryEx(library_name, NULL, LOAD_LIBRARY_SEARCH_USER_DIRS);
			if (!module)
			{
				wprintf(L"could not find (or load!) %s in extra path %s\n", library_name, extra_path);
			}
			else
			{
				wprintf(L"found it after adding extra path %s\n", extra_path);
			}
		}
	}
	if (module)
	{
		bool got_procs = true;
		for (int i = 0; i < NUM_OUTER_PROCS; i++)
		{
			proc_table[i] = GetProcAddress(module, outer_proc_names[i]);
			if (!proc_table[i])
			{
				log_printf("failed to GetProcAddress for %s\n", outer_proc_names[i]);
				got_procs = false;
			}
		}
		if (got_procs)
		{
			bool rc = call_VRInit();
			if (rc)
			{   // warmup to fail early 
				call_IsRuntimeInstalled();
				call_RuntimePath();
				call_IsHmdPresent();
				call_GetInitToken();
				// initialize the actual openvr interfaces
				rc = InitializeInnerInterfaces();
			}
			return rc;
		}
	}
	return false;
}

bool openvr_dll_client::imp::close_lib()
{
	bool rc = true;
	if (module)
	{
		call_ShutdownInternal();
		BOOL free_rc = FreeLibrary(module);
		if (free_rc)
		{
			//log_printf("freelibrary: success\n");
			return true;
		}
		else
		{
			log_printf("freelibary: fail %d\n", GetLastError());
			return false;
		}
		module = nullptr;
	}
	return rc;
}

void openvr_dll_client::imp::call_ShutdownInternal()
{
	if (proc_table[indexVR_ShutdownInternal])
	{
		FARPROC p = *proc_table[indexVR_ShutdownInternal];
		(*(voidReturnsVoid)p)();
	}
}

// use GetGenericInterfaces to populate the openvr_api tables
bool openvr_dll_client::imp::InitializeInnerInterfaces()
{
	FARPROC p = *proc_table[indexVR_GetGenericInterface];
	bool rc = true;
	bool rc1 = QueryInterfaces(p, &cpp_interface_table[0], &inner_cpp_interface_names[0]);
	bool rc2 = QueryInterfaces(p, &c_interface_table[0], &inner_c_interface_names[0]);
	return rc1 && rc2;
}

bool openvr_dll_client::imp::call_IsInterfaceVersionValid(const char *interface_name)
{
	typedef int(*VR_IsInterfaceVersionValid)(const char *);
	FARPROC p = *proc_table[indexVR_IsInterfaceVersionValid];
	int valid = (*(VR_IsInterfaceVersionValid)p)(interface_name);
	if (valid)
		return true;
	else
		return false;
}

bool openvr_dll_client::imp::call_VRInit()
{
	typedef int(*VR_InitInternal)(int *, int);
	int error;
	FARPROC p = *proc_table[indexVR_InitInternal];
	int token = (*(VR_InitInternal)p)(&error, 1);
	if (error == 0)
	{
		//log_printf("VR_Init success\n");
		return true;
	}
	else
	{
		log_printf("VR_Init fail %d\n", error);
		return false;
	}
}

bool openvr_dll_client::imp::call_IsRuntimeInstalled()
{
	FARPROC p = *proc_table[indexVR_IsRuntimeInstalled];
	int runtime_installed = (*(voidReturnsBool)p)();
	if (runtime_installed)
	{
		// interface version is valid
		//log_printf("runtime_installed: true\n");
	}
	else
	{
		log_printf("runtime_installed: false\n");
	}
	return (runtime_installed != 0);
}

bool openvr_dll_client::imp::call_IsHmdPresent()
{
	FARPROC p = *proc_table[indexVR_IsHmdPresent];
	int hmd = (*(voidReturnsBool)p)();
	if (hmd)
	{
		// interface version is valid
		//log_printf("hmd_present: true\n");
	}
	else
	{
		log_printf("hmd_present: false\n");
	}
	return (hmd != 0);
}

const char *openvr_dll_client::imp::call_RuntimePath()
{
	typedef char *(*typedefRuntimePath)();
	FARPROC p = *proc_table[indexVR_RuntimePath];
	char *runtime_path = (*(typedefRuntimePath)p)();
	if (runtime_path)
	{
		// interface version is valid
		//log_printf("runtime_path: %s\n", runtime_path);
	}
	else
	{
		log_printf("runtime_path: null\n");
	}
	return runtime_path;
}

int openvr_dll_client::imp::call_GetInitToken()
{
	FARPROC p = *proc_table[indexVR_RuntimePath];
	int init_token = (*(voidReturnsInt)p)();
	//log_printf("init_token: %d\n", init_token);
	return init_token;
}

// external interface - creates the pimpl object
openvr_dll_client::openvr_dll_client()
	: internals(nullptr)
{}

openvr_dll_client::~openvr_dll_client()
{
	if (internals)
		delete internals;
}

bool openvr_dll_client::open_lib(LPCTSTR library_name, LPCTSTR extra_path)
{
	internals = new openvr_dll_client::imp;
	bool open_rc = internals->open_lib(library_name, extra_path);
	if (!open_rc)
	{
		delete internals;
		internals = nullptr;
	}
	return open_rc;
}

openvr_broker::open_vr_interfaces *openvr_dll_client::cpp_interfaces()
{
	if (internals)
	{
		openvr_broker::open_vr_interfaces *ret = (openvr_broker::open_vr_interfaces *)internals->GetCPPInterfaceTable();
		return ret;
	}
	else
		return nullptr;
}

openvr_broker::open_vr_interfaces_void *openvr_dll_client::c_interfaces()
{
	if (internals)
	{
		openvr_broker::open_vr_interfaces_void *ret = (openvr_broker::open_vr_interfaces_void *)internals->GetCInterfaceTable();
		return ret;
	}
	else
		return nullptr;
}

bool openvr_dll_client::close_lib()
{
	bool rc = internals->close_lib();
	delete internals;
	internals = nullptr;
	return rc;
}
