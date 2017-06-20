#include "FrameBuffer.h"

namespace RE {

	FrameBuffer::FrameBuffer() {

	}

	FrameBuffer::FrameBuffer(UI32 width, UI32 height) {
		Init(width, height);
	}

	FrameBuffer::~FrameBuffer() {
		clear();
	}

	bool FrameBuffer::BindToDefault() {
		//glGenFramebuffers(1, &_fbo);
		//glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);

		return true;
	}

	bool FrameBuffer::Begin(Viewport vp) {
		if (memcmp(&vp, &_viewPort, sizeof(Viewport)) != 0) {
			_viewPort = vp;
			if (_fbo != 0) {
				Init(abs(_viewPort.right - _viewPort.left), abs(_viewPort.top - _viewPort.bottom));
			} else {
				Init(abs(_viewPort.right - _viewPort.left), abs(_viewPort.top - _viewPort.bottom));
			}
		}

		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&_oldFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		return true;
	}

	bool FrameBuffer::End() {
		glBindFramebuffer(GL_FRAMEBUFFER, _oldFbo);
		_oldFbo = 0;

		return true;
	}

	bool FrameBuffer::Init(UI32 width, UI32 height) {
		clear();
		_width = width;
		_height = height;

		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&_oldFbo);

		glGenTextures(1, &_textureHandle);
		glBindTexture(GL_TEXTURE_2D, _textureHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureHandle, 0);

		GLuint uStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (uStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("glCheckFramebufferStatus=%X", uStatus);
			//return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, _oldFbo);
		_oldFbo = 0;
		return true;
	}

	void FrameBuffer::clear() {
		if (_fbo != 0) {
			glDeleteFramebuffers(1, &_fbo);
			_fbo = 0;
		}
		_oldFbo = 0;
		_textureHandle = 0;
		_width = 0;
		_height = 0;
	}

	GLuint RE::FrameBuffer::GetTextureHandle() {
		return _textureHandle;
	}

}