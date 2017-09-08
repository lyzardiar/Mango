#include "Image.h"

RE::Image::Image(const char* name/* = "" */) 
	: GameObject(name)
{
	init();
}

void RE::Image::OnDraw(const Affine& viewMat) {
	transform.rotation += rand() % 10;
	GameObject::OnDraw(viewMat);
}

bool RE::Image::init() {
	renderer = AddComponent<TriangleRenderer>();
	renderer->canRemove = false;
	transform.ax = transform.ay = 0.5f;
	return true;
}
