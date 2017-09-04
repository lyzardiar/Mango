#include "TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "engine/object/GameObject.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "IRenderer.h"
#include "../system/GLProgramSystem.h"
#include "../system/Texture2DSystem.h"

RE::TriangleRenderer::TriangleRenderer() {
	material.texture = Texture2DSystem::instance.Add("images/2.png");
	material.program = GLProgramSystem::instance["Default"];
}

void RE::TriangleRenderer::draw(const Affine& viewMat) {
	renderCMD.mat = viewMat;
	renderCMD.material = &material;
	renderCMD.triangles = &triangles;

	renderCMD.Commit();
}

void RE::TriangleRenderer::setColor(Color col) {
	IRenderer::setColor(col);
	auto& verts = triangles.verts;
	auto size = verts.size;
	auto data = verts.data;
	for (decltype(size) i = 0; i < size; ++i) {
		memcpy(data[i].color, &color.r, sizeof(float) * 4);
	}
}
