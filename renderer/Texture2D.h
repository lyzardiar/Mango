#pragma once

#include "core/base/Types.h"
#include "renderer/GL/REGL.h"

namespace RE {
	class Texture2D {
	public:
		Texture2D();
		Texture2D(const char* filepath);
		Texture2D(UI32 width, UI32 height, UI8* data, UI32 len);
		~Texture2D();

		bool InitWithData(UI32 width, UI32 height, UI8* data, UI32 len);
		bool InitWithFile(const char* filepath);
		GLuint GetHandle();

		void Bind();

	public:
		StaticString<128> path;

	protected:
		void clear();

	protected:
		GLuint _handle = 0;
		UI32 _width = 0;
		UI32 _height = 0;

	};
}