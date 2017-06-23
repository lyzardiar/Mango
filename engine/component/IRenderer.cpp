#include "IRenderer.h"

#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/object/GameObject.h"


void RE::IRenderer::draw() {
	if (material.texture != nullptr) {
		material.texture->Bind();
	}
	if (material.pipeLine != nullptr) {
		material.pipeLine->Apply(Engine::instance.camera.matp);
		gameObject->transform.BindToGL(glGetUniformLocation(material.pipeLine->GetProgramHandle(), "MatM"));
	}
}
