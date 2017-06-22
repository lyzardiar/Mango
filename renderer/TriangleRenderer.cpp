#include "TriangleRenderer.h"
#include "Triangle.h"
#include "Texture2D.h"
#include "image/PngDecoder.h"
#include "engine/object/GameObject.h"



GLuint testTexture() {
	static RE::Texture2D* tex = nullptr;
	if (tex == nullptr) {
		auto imgInfo = RE::Png::Decode("images/1.png");

		tex = new RE::Texture2D(imgInfo.width, imgInfo.height, imgInfo.data.getBytes(), imgInfo.data.getSize());
	}
	return tex->GetHandle();
}

void RE::TriangleRenderer::draw()
{
	static auto pl = PipeLine();
	pl.Apply();


	static GameObject* obj = new GameObject("Test");

	float matm[16];

	obj->transform.x = 300;
	obj->transform.y = 300;

	obj->transform.scale.x = 2;
	obj->transform.scale.y = 2;

	obj->transform.roration = -60.0f/180 * 3.14159f;

	obj->transform.GetMat(matm);

	auto matmHandle = glGetUniformLocation(pl.GetProgramHandle(), "MatM");
	glUniformMatrix4fv(matmHandle, (GLsizei)1, GL_FALSE, matm);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, testTexture());

	if (true) {
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
}
