#pragma once

#include "core/base/Types.h"
#include "renderer/GL/REGL.h"
#include "renderer/Texture2D.h"

namespace RE {
	struct Viewport {
		int left = 0;
		int right = 0;
		int top = 0;
		int bottom = 0;
	};

	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(UI32 width, UI32 height);
		~FrameBuffer();


	public:
		bool BindToDefault();
		GLuint GetTextureHandle();
		bool Begin(Viewport vp);
		bool End();

	protected:
		bool Init(UI32 width, UI32 height);
		void clear();

	protected:
		GLuint _fbo		= 0;
		GLuint _oldFbo	= 0;
		UI32 _width		= 0;
		UI32 _height	= 0;

		GLuint _textureHandle = 0;
		Viewport _viewPort;
	};
}
