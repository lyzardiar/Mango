#pragma once
#include "GL/REGL.h"
#include <string>

namespace RE {
	class PipeLine {
	public:
		enum {
			VERTEX_ATTRIB_POSITION,
			VERTEX_ATTRIB_COLOR,
			VERTEX_ATTRIB_TEX_COORD
		};
		static const char* ATTRIBUTE_NAME_COLOR;
		static const char* ATTRIBUTE_NAME_POSITION;
		static const char* ATTRIBUTE_NAME_TEX_COORD;
	public:
		PipeLine();
		~PipeLine();

		bool Init(const char* vert, const char* frag);

		bool Apply();

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
	};
}