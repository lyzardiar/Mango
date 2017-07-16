#include "TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "engine/object/GameObject.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "IRenderer.h"



RE::Texture2D* testTexture() {
	static RE::Texture2D* tex = nullptr;
	tex = tex ? tex : new RE::Texture2D("images/1.png");
	return tex;
}

RE::TriangleRenderer::TriangleRenderer() {
	static PipeLine *pl = new PipeLine();

	material.texture = testTexture();
	material.pipeLine = pl;
}

void RE::TriangleRenderer::draw() {
	IRenderer::draw();
	
	long offset = (long)(triangles.verts.data);
	// vertex
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	// color
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	// texcood
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RE::TriangleRenderer::draw(const Affine& viewMat) {
	IRenderer::draw(viewMat);

	long offset = (long)(triangles.verts.data);
	// vertex
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	// color
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	// texcood
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}