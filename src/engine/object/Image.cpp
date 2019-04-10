#include "Image.h"
#include "engine/system/Texture2DSystem.h"

RE::Image::Image(const char* name/* = "" */) 
	: GameObject(name)
{
	init();
}

void RE::Image::OnDraw(const Affine& viewMat) {
	GameObject::OnDraw(viewMat);
}

void RE::Image::setTexture(const char* path) {
	renderer->material.texture = Texture2DSystem::instance.Add(path);
}

bool RE::Image::init() {
	renderer = AddComponent<TriangleRenderer>();
	renderer->canRemove = false;
	transform.ax = transform.ay = 0.5f;
	return true;
}
