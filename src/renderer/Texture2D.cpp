#include "Texture2D.h"
#include "image/PngDecoder.h"
#include "engine/system/Texture2DSystem.h"
#include "RenderState.h"

RE::Texture2D::Texture2D() {

}

RE::Texture2D::Texture2D(Path filepath) {
	InitWithFile(filepath);
}

RE::Texture2D::Texture2D(UI32 width, UI32 height, UI8* data, UI32 len) {
	InitWithData(width, height, data, len);
}

RE::Texture2D::~Texture2D() {
	clear();
}

bool RE::Texture2D::InitWithData(UI32 width, UI32 height, UI8* data, UI32 len) {
	clear();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	size.width = static_cast<float>(width);
	size.height = static_cast<float>(height);

	glGenTextures(1, &_handle);
	RenderState::CurrentRenderState->BindTexture(_handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return true;
}

bool RE::Texture2D::InitWithFile(Path filepath) {
	clear();

	path = filepath;
	auto imgInfo = RE::Png::Decode(filepath);
	if (imgInfo.valid)
		return InitWithData(imgInfo.width, imgInfo.height, imgInfo.data.getBytes(), imgInfo.data.getSize());

	_handle = 0;
	size.width = 1;
	size.height = 1;
	return false;
}

GLuint RE::Texture2D::GetHandle() {
	return _handle;
}

void RE::Texture2D::Bind() {
	RenderState::CurrentRenderState->BindTexture(_handle);
}

void RE::Texture2D::clear() {
	RenderState::CurrentRenderState->DeleteTexture(_handle);
}
