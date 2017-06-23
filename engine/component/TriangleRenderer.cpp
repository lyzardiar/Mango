#include "TriangleRenderer.h"
#include "renderer/Triangle.h"
#include "renderer/Texture2D.h"
#include "renderer/PipeLine.h"
#include "core/image/PngDecoder.h"
#include "engine/object/GameObject.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "IRenderer.h"



RE::Texture2D* testTexture() {
	static RE::Texture2D* tex = nullptr;
	if (tex == nullptr) {
		auto imgInfo = RE::Png::Decode("images/1.png");

		tex = new RE::Texture2D(imgInfo.width, imgInfo.height, imgInfo.data.getBytes(), imgInfo.data.getSize());
	}
	return tex;
}

RE::TriangleRenderer::TriangleRenderer() {
	material.texture = testTexture();
	material.pipeLine = new PipeLine();
}

void RE::TriangleRenderer::draw() {
	IRenderer::draw();
	
	Triangle Default = {
		{
			{ 0, 0,	0,		1, 1, 1, 1,		0, 0 },
			{ 100, 0, 0,	1, 1, 1, 1,		1, 0 },
			{ 0, 100, 0,	1, 1, 1, 1,		0, 1 },
			{ 100, 100, 0,	1, 1, 1, 1,		1, 1 },
		},
		{ 0, 1, 2, 3 }
	};
	auto& triangles = Default;

	long offset = (long)&(triangles.verts[0]);
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
