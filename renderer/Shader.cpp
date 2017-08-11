

#include "Shader.h"
#include <malloc.h>
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"
#include "FrameBuffer.h"

GLuint RE::Shader::CurProgram = 0;

const char* RE::Shader::ATTRIBUTE_NAME_COLOR = "a_color";
const char* RE::Shader::ATTRIBUTE_NAME_POSITION = "a_position";
const char* RE::Shader::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";

RE::Shader::Shader() {
	Init("shaders/default.vert", "shaders/default.frag");
}

RE::Shader::~Shader() {
	reset();
}

bool RE::Shader::Init(const char* vert, const char* frag) {
	Data vertdata = FileUtils::getInstance()->getData(vert);
	Data fragdata = FileUtils::getInstance()->getData(frag);

	reset();

	if (!compile(_vertHandle, GL_VERTEX_SHADER, (const GLchar*)vertdata.getBytes()) ||
		!compile(_fragHandle, GL_FRAGMENT_SHADER, (const GLchar*)fragdata.getBytes())) {
		return false;
	}
	_program = glCreateProgram();
	glAttachShader(_program, _vertHandle);
	glAttachShader(_program, _fragHandle);
	
	return link();
}

bool RE::Shader::Apply() {
	glUseProgram(_program);
	return true;
}

bool RE::Shader::Apply(float* matp) {
	if (CurProgram != _program) {
		CurProgram = _program;
		glUseProgram(_program);
		glUniformMatrix4fv(_matPHandle, (GLsizei)1, GL_FALSE, matp);
	}
	return true;
}

GLuint RE::Shader::GetProgramHandle() {
	return _program;
}

GLuint RE::Shader::GetMatPHandle() {
	return _matPHandle;
}

GLuint RE::Shader::GetMatMHandle() {
	return _matMHandle;
}

bool RE::Shader::compile(GLuint& handle, GLenum type, const GLchar* code) {
	GLint status = 0;
	handle = glCreateShader(type);
	CHECK_GL_ERROR_DEBUG();
	glShaderSource(handle, 1, &code, nullptr);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	if (status == 0) {
		std::string errlog = getShaderLog(handle);
		puts(errlog.c_str());
		return false;
	}
	return status == GL_TRUE;
}

bool RE::Shader::link() {
	GLint status = GL_TRUE;

	glBindAttribLocation(_program, VERTEX_ATTRIB_POSITION, ATTRIBUTE_NAME_POSITION);
	glBindAttribLocation(_program, VERTEX_ATTRIB_COLOR, ATTRIBUTE_NAME_COLOR);
	glBindAttribLocation(_program, VERTEX_ATTRIB_TEX_COORD, ATTRIBUTE_NAME_TEX_COORD);

	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		clearProgram();
		puts("Shader Link Error.");
	}
	else {
		clearShader();

		_matPHandle = glGetUniformLocation(_program, "MatP");
		_matMHandle = glGetUniformLocation(_program, "MatM");
	}

	return (status == GL_TRUE);
}

std::string RE::Shader::getShaderLog(GLuint handle) {
	GLint len = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
	if (len < 1) return "";

	char *buff = (char*)alloca(len);
	glGetShaderInfoLog(handle, len, nullptr, buff);
	std::string ret(buff, len);
	return ret;
}

void RE::Shader::reset() {
	clearShader();
	clearProgram();
}

void RE::Shader::clearShader() {
	if (_vertHandle) glDeleteShader(_vertHandle); 
	if (_fragHandle) glDeleteShader(_fragHandle);
	_vertHandle = _fragHandle = 0;
}

void RE::Shader::clearProgram() {
	if (_program) glDeleteProgram(_program); 
	_program = 0;
}
