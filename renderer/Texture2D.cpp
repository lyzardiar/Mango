#include "Texture2D.h"



RE::Texture2D::Texture2D() {

}

RE::Texture2D::Texture2D(UI32 width, UI32 height, UI8* data, UI32 len) {
	InitWithData(width, height, data, len);
}

RE::Texture2D::~Texture2D() {
	clear();
}

bool RE::Texture2D::InitWithData(UI32 width, UI32 height, UI8* data, UI32 len) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	clear();

	_width = width;
	_height = height;

	glGenTextures(1, &_handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return true;
}

GLuint RE::Texture2D::GetHandle() {
	return _handle;
}

void RE::Texture2D::clear() {
	if (_handle != 0) {
		glDeleteTextures(1, &_handle);
		_handle = 0;
	}
}
