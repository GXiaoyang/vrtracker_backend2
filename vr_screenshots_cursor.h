#pragma once
#include "openvr_cppstub.h"
struct CursorContext;

struct VRScreenshotsCursor : public VRScreenshotsCppStub
{
	explicit VRScreenshotsCursor(CursorContext *context) {}
};
