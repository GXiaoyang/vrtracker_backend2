//
// test opening a dll 
//
#include "log.h"
#include "openvr_dll_client.h"

void test_dll_client()
{
	{
		// default construction and destruction
		openvr_dll_client cli;
	}

	{
		static LPCTSTR server_dll = L"openvr_api.dll";
		static LPCTSTR extra_path = L"C:\\projects\\rangesplay\\submodules\\openvr\\bin\\win32";
		
		openvr_dll_client cli;
		cli.open_lib(server_dll, extra_path);

	}
	

	
}

