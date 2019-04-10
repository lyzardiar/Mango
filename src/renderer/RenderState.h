#pragma once

#include "renderer/GL/REGL.h"
#include "core/base/Types.h"

namespace RE {
	class RenderState {
	public:
		static RenderState* CurrentRenderState;

		const static int MAX_TEXTURES = 16;
	public:
		RenderState();

		void ActiveTexture(GLenum index);
		
		void BindTexture(GLuint id);
		void BindTextureN(I8 index, GLuint id);

		void DeleteTexture(GLuint& id);

		void BlendFunc(GLenum src, GLenum dst);

		void UseProgram(GLuint program);
		void DeleteProgram(GLuint program);
		
		void Clear();
	protected:
		RE::BlendFunc _blend;
		bool _isBlending = false;
		GLuint _program = (GLuint)-1;
		GLenum _activeTexture = -1;
		GLuint _texture2dBinded[MAX_TEXTURES] = { (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
	};
}