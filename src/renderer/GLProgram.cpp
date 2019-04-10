

#include "GLProgram.h"
#include <malloc.h>
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"
#include "FrameBuffer.h"
#include "RenderState.h"

const char* RE::GLProgram::ATTRIBUTE_NAME_COLOR		= "a_color";
const char* RE::GLProgram::ATTRIBUTE_NAME_POSITION	= "a_position";
const char* RE::GLProgram::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";

RE::GLProgram::GLProgram() {

}

RE::GLProgram::~GLProgram() {
	reset();
}

bool RE::GLProgram::Init(const char* vert, const char* frag) {
	return Init(Path(vert), Path(frag));
}

bool RE::GLProgram::Init(Path vert, Path frag) {
	reset();

	Data vertdata = FileUtils::getInstance()->GetData(vert);
	Data fragdata = FileUtils::getInstance()->GetData(frag);

	return InitWithBuff((char*)vertdata.getBytes(), (char*)fragdata.getBytes());
}

bool RE::GLProgram::InitWithBuff(const char* vertData, const char* fragData) {
	if (!compile(_vertHandle, GL_VERTEX_SHADER, (const GLchar*)vertData) ||
		!compile(_fragHandle, GL_FRAGMENT_SHADER, (const GLchar*)fragData)) {
		return false;
	}
	_program = glCreateProgram();
	glAttachShader(_program, _vertHandle);
	glAttachShader(_program, _fragHandle);

	return link();
}

bool RE::GLProgram::Apply() {
	glUseProgram(_program);
	return true;
}

bool RE::GLProgram::Apply(float* matp) {
	RenderState::CurrentRenderState->UseProgram(_program);
	glUniformMatrix4fv(_matPHandle, (GLsizei)1, GL_FALSE, matp);
	return true;
}

GLuint RE::GLProgram::GetProgramHandle() {
	return _program;
}

GLuint RE::GLProgram::GetMatPHandle() {
	return _matPHandle;
}

GLuint RE::GLProgram::GetMatMHandle() {
	return _matMHandle;
}

bool RE::GLProgram::compile(GLuint& handle, GLenum type, const GLchar* code) {
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

bool RE::GLProgram::link() {
	GLint status = GL_TRUE;

	glBindAttribLocation(_program, VERTEX_ATTRIB_POSITION,	ATTRIBUTE_NAME_POSITION);
	glBindAttribLocation(_program, VERTEX_ATTRIB_COLOR,		ATTRIBUTE_NAME_COLOR);
	glBindAttribLocation(_program, VERTEX_ATTRIB_TEX_COORD, ATTRIBUTE_NAME_TEX_COORD);

	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		clearProgram();
		puts("GLProgram Link Error.");
	}
	else {
		clearShader();

		_matPHandle = glGetUniformLocation(_program, "MatP");
		_matMHandle = glGetUniformLocation(_program, "MatM");
	}

	return (status == GL_TRUE);
}

std::string RE::GLProgram::getShaderLog(GLuint handle) {
	GLint len = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
	if (len < 1) return "";

	char *buff = (char*)alloca(len);
	glGetShaderInfoLog(handle, len, nullptr, buff);
	std::string ret(buff, len);
	return ret;
}

void RE::GLProgram::reset() {
	clearShader();
	clearProgram();
}

void RE::GLProgram::clearShader() {
	if (_vertHandle) glDeleteShader(_vertHandle); 
	if (_fragHandle) glDeleteShader(_fragHandle);
	_vertHandle = _fragHandle = 0;
}

void RE::GLProgram::clearProgram() {
	if (_program) glDeleteProgram(_program); 
	_program = 0;
}
