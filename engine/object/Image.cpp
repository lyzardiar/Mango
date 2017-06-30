#include "Image.h"

RE::Image::Image() {
	renderer = new TriangleRenderer();
	renderer->gameObject = this;
}

RE::Image::Image(const char* name) 
	: GameObject(name)
{
	renderer = new TriangleRenderer();
	renderer->gameObject = this;
}

void RE::Image::Render() {
	auto& verts = renderer->triangles.verts;
	for (auto& tri : verts) {
		memcpy(tri.color, &color.r, sizeof(float) * 4);
	}

	renderer->draw();

	GameObject::Render();
}
