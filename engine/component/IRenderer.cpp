#include "IRenderer.h"

#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/object/GameObject.h"

void RE::IRenderer::draw() {
	draw(gameObject->transform.affine);
}

void RE::IRenderer::draw(const Affine& viewMat) {
	material.Apply(viewMat);
}
