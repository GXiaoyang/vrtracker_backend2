#pragma once

#include "vr_types.h"
#include "vr_wrappers_common.h"
#include <algorithm>

namespace vr_result
{
	using namespace vr;

	struct ChaperoneWrapper
	{
		explicit ChaperoneWrapper(IVRChaperone *chapi_in)
			: chapi(chapi_in)
		{}

		SCALAR_WRAP(IVRChaperone, chapi, GetCalibrationState);
		SCALAR_WRAP(IVRChaperone, chapi, AreBoundsVisible);

		inline HmdVector2<bool> GetPlayAreaSize()
		{
			HmdVector2<bool> result;
                        //memset(&result, 0, sizeof(result)); // valgrind
			result.return_code = chapi->GetPlayAreaSize(&result.val.v[0], &result.val.v[1]);
			return result;
		}

		inline HmdQuad<bool> GetPlayAreaRect()
		{
			HmdQuad<bool> result;
                        //memset(&result, 0, sizeof(result)); // valgrind
			result.return_code = chapi->GetPlayAreaRect(&result.val);
			return result;
		}


		inline void GetBoundsColor(
			TMPHMDColorString<>* colors,
			int num_output_colors,
			float fade_distance,
			HmdColor<> *camera_color)
		{
			assert(size_as_int(colors->val.max_size()) >= num_output_colors);
			num_output_colors = std::min(num_output_colors, size_as_int(colors->val.max_size()));
			chapi->GetBoundsColor(colors->val.data(), num_output_colors, fade_distance, &camera_color->val);
			colors->val.resize(num_output_colors);
		}


		IVRChaperone *chapi;
	};


	
}
