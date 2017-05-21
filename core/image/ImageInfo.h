#pragma once

#include "base/Types.h"
#include "base/Data.h"
#include <assert.h>

namespace RE {
	class ImageInfo {
	public:
		enum class PixelFormat {
			NONE,
			RGB888,
			RGBA8888,
			I8,
			AI88,
		};

		void premultipliedAlpha();

	public:		
		Data data;
		UI32 width = 0;
		UI32 height = 0;
		bool valid = false;
		bool isPreMultipliedAlpha = false;
		PixelFormat format = PixelFormat::NONE;
	};
}
