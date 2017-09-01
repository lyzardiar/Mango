#include "TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "engine/object/GameObject.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "IRenderer.h"

RE::TriangleRenderer::TriangleRenderer() {
	static Shader *pl = new Shader();

	material.texture = 0;
	material.shader = pl;
}

void RE::TriangleRenderer::draw(const Affine& viewMat) {
	IRenderer::draw(viewMat);

	long offset = (long)(triangles.verts.data);
	// vertex
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	// color
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	// texcood
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
