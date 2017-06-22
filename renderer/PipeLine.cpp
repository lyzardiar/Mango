

#include "PipeLine.h"
#include <malloc.h>
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"
#include "FrameBuffer.h"

const char* RE::PipeLine::ATTRIBUTE_NAME_COLOR = "a_color";
const char* RE::PipeLine::ATTRIBUTE_NAME_POSITION = "a_position";
const char* RE::PipeLine::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";

RE::PipeLine::PipeLine() {
	Init("shaders/default.vert", "shaders/default.frag");
}

RE::PipeLine::~PipeLine() {
	reset();
}

bool RE::PipeLine::Init(const char* vert, const char* frag) {
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

bool RE::PipeLine::Apply()
{
	glUseProgram(_program);

	float left = 0;
	float right = (float)FrameBuffer::CurWidth;
	float bottom = 0;
	float top = (float)FrameBuffer::CurHeight;

	float zNearPlane = -1000;
	float zFarPlane = 1000;

	GLfloat matp[16] = {0};
	memset(matp, 0, sizeof(GLfloat) * 16);
	matp[0] = 2 / (right - left);
	matp[5] = 2 / (top - bottom);
	matp[10] = 2 / (zNearPlane - zFarPlane);

	matp[12] = (left + right) / (left - right);
	matp[13] = (top + bottom) / (bottom - top);
	matp[14] = (zNearPlane + zFarPlane) / (zNearPlane - zFarPlane);
	matp[15] = 1;

	auto matpHandle = glGetUniformLocation(_program, "MatP");
	glUniformMatrix4fv(matpHandle, (GLsizei)1, GL_FALSE, matp);

	return true;
}

GLuint RE::PipeLine::GetProgramHandle() {
	return _program;
}

bool RE::PipeLine::compile(GLuint& handle, GLenum type, const GLchar* code) {
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

bool RE::PipeLine::link() {
	GLint status = GL_TRUE;

	glBindAttribLocation(_program, VERTEX_ATTRIB_POSITION, ATTRIBUTE_NAME_POSITION);
	glBindAttribLocation(_program, VERTEX_ATTRIB_COLOR, ATTRIBUTE_NAME_COLOR);
	glBindAttribLocation(_program, VERTEX_ATTRIB_TEX_COORD, ATTRIBUTE_NAME_TEX_COORD);

	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		clearProgram();
		puts("PipeLine Link Error.");
	}
	else {
		clearShader();
	}

	return (status == GL_TRUE);
}

std::string RE::PipeLine::getShaderLog(GLuint handle) {
	GLint len = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
	if (len < 1) return "";

	char *buff = (char*)alloca(len);
	glGetShaderInfoLog(handle, len, nullptr, buff);
	std::string ret(buff, len);
	return ret;
}

void RE::PipeLine::reset() {
	clearShader();
	clearProgram();
}

void RE::PipeLine::clearShader() {
	if (_vertHandle) glDeleteShader(_vertHandle); 
	if (_fragHandle) glDeleteShader(_fragHandle);
	_vertHandle = _fragHandle = 0;
}

void RE::PipeLine::clearProgram() {
	if (_program) glDeleteProgram(_program); 
	_program = 0;
}
