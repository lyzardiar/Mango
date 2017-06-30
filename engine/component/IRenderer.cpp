#include "IRenderer.h"

#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/object/GameObject.h"

void RE::IRenderer::draw() {
	draw(gameObject->transform.affine);
}

void RE::IRenderer::draw(const Affine& viewMat) {
	if (material.texture != nullptr) {
		material.texture->Bind();
	}
	if (material.pipeLine != nullptr) {
		material.pipeLine->Apply(Engine::instance.camera.matp);
		viewMat.BindToGL(glGetUniformLocation(material.pipeLine->GetProgramHandle(), "MatM"));
	}
}
