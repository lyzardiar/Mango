#pragma once

#include "core/base/Types.h"
#include "renderer/GL/REGL.h"

namespace RE {
	class Texture2D {
	public:
		Texture2D();
		Texture2D(UI32 width, UI32 height, UI8* data, UI32 len);
		~Texture2D();

		bool InitWithData(UI32 width, UI32 height, UI8* data, UI32 len);
		GLuint GetHandle();

		void Bind();

	protected:
		void clear();

	protected:
		GLuint _handle = 0;
		UI32 _width = 0;
		UI32 _height = 0;
	};
}