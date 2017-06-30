#include "Image.h"

RE::Image::Image() {
	init();
}

RE::Image::Image(const char* name) 
	: Image()
{
	self->name.assign(name);
}

void RE::Image::OnDraw(const Affine& viewMat) {
	auto& verts = renderer->triangles.verts;
	for (auto& tri : verts) {
		memcpy(tri.color, &color.r, sizeof(float) * 4);
	}

	renderer->draw(viewMat);
}

bool RE::Image::init() {
	renderer = new TriangleRenderer();
	renderer->gameObject = self;

	transform.ax = transform.ay = 0.5f;

	return true;
}
