#include "RenderState.h"



RE::RenderState* RE::RenderState::CurrentRenderState = nullptr;

RE::RenderState::RenderState() {
	CurrentRenderState = this;
}

void RE::RenderState::ActiveTexture(GLenum index) {
	if (_activeTexture != index) {
		_activeTexture = index;
		glActiveTexture(index);
	}
}

void RE::RenderState::BindTexture(GLuint id) {
	BindTextureN(0, id);
}

void RE::RenderState::BindTextureN(I8 index, GLuint id) {
	if (_texture2dBinded[index] != id) {
		_texture2dBinded[index] = id;
		ActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}

void RE::RenderState::DeleteTexture(GLuint& id) {
	if (id == 0) return;
	for (int i = 0; i < MAX_TEXTURES; ++i) {
		if (_texture2dBinded[i] == id) {
			_texture2dBinded[i] = -1;
		}
	}
	if (glIsTexture(id) == GL_TRUE) {
		glDeleteTextures(1, &id);
	}
	id = 0;
}

void RE::RenderState::BlendFunc(GLenum src, GLenum dst) {
	if (_blend.src != src || _blend.dst != dst) {
		_blend = { src, dst };
		_isBlending = !(src == GL_ONE && dst == GL_ZERO);
		if (_isBlending) {
			glDisable(GL_BLEND);
		}
		else {
			glEnable(GL_BLEND);
			glBlendFunc(src, dst);
		}
	}
}

void RE::RenderState::UseProgram(GLuint program) {
	if (_program != program) {
		_program = program;
		glUseProgram(_program);
	}
}

void RE::RenderState::DeleteProgram(GLuint program) {
	if (_program == program) {
		_program = GLuint(-1);
	}
	glDeleteProgram(program);
}

void RE::RenderState::Clear() {
	_blend = {0, 0};
	_isBlending = false;
	_program = (GLuint)-1;
	_activeTexture = -1;
	memset(_texture2dBinded, 0xFF, sizeof(GLuint) * MAX_TEXTURES);
}
