#pragma once

#include "core/base/Types.h"
#include "renderer/GL/REGL.h"
#include "renderer/Texture2D.h"

namespace RE {
	struct Viewport {
		float left = 0.0f;
		float right = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;
	};

	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();


	public:
		bool BindToDefault();
		GLuint GetTextureHandle();

	protected:
		bool Init(UI32 width, UI32 height);

	protected:
		GLuint _fbo = 0;
		GLuint _oldFbo = 0;
		UI32 _width = 0;
		UI32 _height = 0;

		Texture2D* _texture = nullptr;
	};
}
