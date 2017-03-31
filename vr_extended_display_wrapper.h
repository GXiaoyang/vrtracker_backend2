#pragma once

#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct ExtendedDisplayWrapper
	{
		explicit ExtendedDisplayWrapper(IVRExtendedDisplay *edi_in)
			: edi(edi_in)
		{}

		inline WindowBounds<> GetWindowBounds()
		{
			WindowBounds<> ret;
			edi->GetWindowBounds(&ret.val.i[0], &ret.val.i[1], &ret.val.u[0], &ret.val.u[1]);
			return ret;
		}

		inline ViewPort<> GetEyeOutputViewport(vr::EVREye eye)
		{
			ViewPort<> ret;
			edi->GetEyeOutputViewport(eye, &ret.val.u[0], &ret.val.u[1], &ret.val.u[2], &ret.val.u[3]);
			return ret;
		}
		IVRExtendedDisplay *edi;
	};	
}
