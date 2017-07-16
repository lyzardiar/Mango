#include "Material.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"



void RE::Material::Apply(const Affine& viewMat) {
	if (texture != nullptr) {
		texture->Bind();
	}
	if (pipeLine != nullptr) {
		pipeLine->Apply(Engine::instance.camera.matp);
		viewMat.BindToGL(pipeLine->GetMatMHandle());
	}
}
