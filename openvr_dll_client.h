#pragma once

//
// opens a dll and queries for openvr interfaces
//

#include "openvr_broker.h"
#include <windows.h>

class openvr_dll_client
{
public:
	openvr_dll_client();
	~openvr_dll_client();

	openvr_dll_client(openvr_dll_client&) = delete; // can't copy module handles
	openvr_dll_client &operator=(openvr_dll_client&) = delete;

	bool open_lib(LPCTSTR library_name, LPCTSTR extra_path);
	bool is_open() { return internals != nullptr; }
	bool close_lib();
	openvr_broker::open_vr_interfaces *cpp_interfaces();
	openvr_broker::open_vr_interfaces_void *c_interfaces();
private:
	struct imp;
	imp *internals;
};
