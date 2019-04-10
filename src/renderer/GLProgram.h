#pragma once
#include "GL/REGL.h"
#include <string>
#include "base/Path.h"
#include "base/Object.h"

namespace RE {
	class GLProgram : public Object {
	public:
		enum {
			VERTEX_ATTRIB_POSITION,
			VERTEX_ATTRIB_COLOR,
			VERTEX_ATTRIB_TEX_COORD,
		};
		static const char* ATTRIBUTE_NAME_COLOR;
		static const char* ATTRIBUTE_NAME_POSITION;
		static const char* ATTRIBUTE_NAME_TEX_COORD;
	public:
		GLProgram();
		~GLProgram();

		bool Init(const char* vert, const char* frag);
		bool Init(Path vert, Path frag);
		bool InitWithBuff(const char* vert, const char* frag);

		void OnGUI();

		bool Apply();
		bool Apply(float* matp);

		GLuint GetProgramHandle();
		GLuint GetMatPHandle();
		GLuint GetMatMHandle();
	protected:
		bool compile(GLuint& handle, GLenum type, const GLchar* code);
		bool link();

		std::string getShaderLog(GLuint handle);
		void reset();
		void clearShader();
		void clearProgram();

	protected:
		GLuint _program = 0;
		GLuint _vertHandle = 0;
		GLuint _fragHandle = 0;
		GLint  _matPHandle = 0;
		GLint  _matMHandle = 0;
	};
}