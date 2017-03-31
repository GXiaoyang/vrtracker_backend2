#pragma once


#include "vr_types.h"
#include "vr_wrappers_common.h"

namespace vr_result
{
	using namespace vr;

	struct ResourcesWrapper
	{
		explicit ResourcesWrapper(IVRResources *resi_in)
			: resi(resi_in)
		{}

		TMPString<> & GetFullPath(const char *filename, const char *directory, TMPString<> *result)
		{
			query_vector_rccount(result, resi, &IVRResources::GetResourceFullPath, filename, directory);
			return *result;
		}

		uint32_t GetImageData(const char *joinedfilename, uint8_t **ret)
		{
			uint32_t image_size = resi->LoadSharedResource(joinedfilename, nullptr, 0);
			if (image_size > 0)
			{
				char *buf = (char *)malloc(image_size);
				if (buf)
				{
					image_size = resi->LoadSharedResource(joinedfilename, buf, image_size);
				}
				*ret = (uint8_t *)buf;
			}
			else
			{
				*ret = nullptr;
			}
			return image_size;
		}

		void FreeImageData(uint8_t *data)
		{
			if (data)
			{
				free(data);
			}
		}

		IVRResources *resi;
	};
}
