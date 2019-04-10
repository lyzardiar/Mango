#pragma once

#include "core/base/Types.h"
#include "core/image/ImageInfo.h"
#include "core/base/Path.h"

namespace RE {

	class ImageDecoder {
	public:
		static ImageInfo decode(Path& path);

	};
}