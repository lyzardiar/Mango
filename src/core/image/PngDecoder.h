#pragma once

#include "core/image/ImageInfo.h"
#include "core/base/Path.h"

namespace RE {
	class Png {
	public:
		static ImageInfo Decode(const char* path);
		static ImageInfo Decode(Path& path);
		static ImageInfo Decode(UI8* data, UI32 len);
	};
}