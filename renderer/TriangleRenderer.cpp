#include "TriangleRenderer.h"
#include "Triangle.h"



void RE::TriangleRenderer::draw()
{
	static auto pl = PipeLine();
	pl.Apply();

	auto& triangles = Triangle::Default;

	long offset = (long)&triangles;

	// vertex
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	// color
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	// texcood
	glEnableVertexAttribArray(PipeLine::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(PipeLine::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
