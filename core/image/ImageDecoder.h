#pragma once

#include "base/Types.h"
#include "ImageInfo.h"

namespace RE {

	class ImageDecoder {
	public:
		static ImageInfo decode(std::string& path);

	};
}