#include "Texture2DSystem.h"
#include "renderer/Texture2D.h"

static unsigned char re_2x2_white_image[] = {
	// RGBA8888
	0xFF, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0x00
};

RE::Texture2DSystem RE::Texture2DSystem::instance;

RE::Texture2DSystem::Texture2DSystem() {
}

void RE::Texture2DSystem::InitDefault() {
	auto tex = new Texture2D();
	tex->InitWithData(2, 2, re_2x2_white_image, 16);
	textures["Default"] = tex;
}

RE::Texture2D* RE::Texture2DSystem::Add(const char* path) {
	return Add(Path(path));
}

RE::Texture2D* RE::Texture2DSystem::Add(const Path& path) {
	auto tex = textures[path];
	if (tex != nullptr) {
		return tex;
	}

	tex = new Texture2D(path);
	textures[path] = tex;
	return tex;
}

RE::Texture2D* RE::Texture2DSystem::operator[](const char* path) {
	return textures[Path(path)];
}

RE::Texture2D* RE::Texture2DSystem::operator[](const Path& path) {
	auto tex = textures[path];
	if (tex == nullptr) {
		return Add(path);
	}
	return tex;
}
