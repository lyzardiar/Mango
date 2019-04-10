#pragma once

#include "renderer/GL/REGL.h"
#include "core/base/String.h"
#include "core/base/Object.h"
#include "core/base/Path.h"

namespace RE {
	class Texture2D : public Object {
	public:
		Texture2D();
		Texture2D(Path filepath);
		Texture2D(UI32 width, UI32 height, UI8* data, UI32 len);
		~Texture2D();

		bool InitWithData(UI32 width, UI32 height, UI8* data, UI32 len);
		bool InitWithFile(Path filepath);
		GLuint GetHandle();

		void Bind();

		void OnGUI();
	public:
		Path path;
		Vec2 size;

	protected:
		void clear();

	protected:
		GLuint _handle = 0;
	};
}