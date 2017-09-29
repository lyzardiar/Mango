#include "FrameBuffer.h"
#include "image/PngDecoder.h"
#include "base/Macros.h"

namespace RE {

	UI32 FrameBuffer::CurWidth;
	UI32 FrameBuffer::CurHeight;

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

	RE::Texture2D* FrameBuffer::GetTexture() {
		return _texture;
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

		CurWidth = _width;
		CurHeight = _height;
		
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&_oldFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glViewport(0, 0, _width, _height);
		
		glClearColor(0.15f, 0.15f, 0.15f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		return true;
	}

	bool FrameBuffer::End() {
		glBindFramebuffer(GL_FRAMEBUFFER, _oldFbo);
		_oldFbo = 0;

		return true;
	}

	bool FrameBuffer::Init(UI32 width, UI32 height) {
		printf("Rebuild FBO: %d %d\n", width, height);
		clear();
		_width = width;
		_height = height;

		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&_oldFbo);
		
		_texture = new Texture2D(width, height, nullptr, 0);
		
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->GetHandle(), 0);

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
		_width = 0;
		_height = 0;
		SAFE_DELETE(_texture);
	}

	GLuint RE::FrameBuffer::GetTextureHandle() {
		if (_texture != nullptr) return _texture->GetHandle();
		return 0;
	}

}