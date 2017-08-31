#pragma once

#include "ImageInfo.h"

namespace RE {
	class Png {
	public:
		static ImageInfo Decode(const char* path);
		static ImageInfo Decode(std::string& path);
		static ImageInfo Decode(UI8* data, UI32 len);
	};
}