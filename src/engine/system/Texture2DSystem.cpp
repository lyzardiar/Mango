#include "Texture2DSystem.h"
#include "renderer/Texture2D.h"



RE::Texture2DSystem RE::Texture2DSystem::instance;

RE::Texture2D* RE::Texture2DSystem::Add(const char* path) {
	if (textures.count(path) > 0) {
		return textures[path];
	}

	auto tex = new Texture2D(path);
	textures[path] = tex;
	return tex;
}

RE::Texture2D* RE::Texture2DSystem::operator[](const char* path) {
	return textures[path];
}

RE::Texture2D* RE::Texture2DSystem::operator[](const StaticString<128>& path) {
	return textures[path.data];
}

RE::Texture2D* RE::Texture2DSystem::Add(const StaticString<128>& path) {
	return Add(path.data);
}
