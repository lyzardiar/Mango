#include "Material.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"



void RE::Material::Apply(const Affine& viewMat) {
	glBlendFunc(blend.src, blend.dst);
	if (texture != nullptr) {
		texture->Bind();
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (program != nullptr) {
		program->Apply(Engine::instance.camera.matp);
		viewMat.BindToGL(program->GetMatMHandle());
	}
}

UI32 RE::Material::GetID() {
	return 0;
}
