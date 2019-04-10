#include "Material.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/system/Texture2DSystem.h"
#include "engine/system/GLProgramSystem.h"



RE::Material::Material() {
	texture = Texture2DSystem::instance.Add("Default");
	program = GLProgramSystem::instance["Default"];
	if (program) program->Retain();
	if (texture) texture->Retain();
}

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

void RE::Material::SetGLProgram(const char* name) {
	if (program) program->Release();
}

void RE::Material::SetTexturePath(const char* name) {

}
