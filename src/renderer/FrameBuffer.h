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
		Texture2D* GetTexture();

		bool Begin(Viewport vp);
		bool End();

		static UI32 CurWidth;
		static UI32 CurHeight;

	protected:
		bool Init(UI32 width, UI32 height);
		void clear();

	protected:
		GLuint _fbo		= 0;
		GLuint _oldFbo	= 0;
		UI32 _width		= 0;
		UI32 _height	= 0;

		Texture2D* _texture = nullptr;
		Viewport _viewPort;
	};
}
