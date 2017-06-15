#include "FrameBuffer.h"

namespace RE {

	FrameBuffer::FrameBuffer() {

	}

	FrameBuffer::~FrameBuffer() {

	}

	bool FrameBuffer::BindToDefault() {
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);

		return true;
	}

	bool FrameBuffer::Init(UI32 width, UI32 height) {

	}

	GLuint RE::FrameBuffer::GetTextureHandle() {
		if (_texture != nullptr) return _texture->GetHandle();
		return 0;
	}

}