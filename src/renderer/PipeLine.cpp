#include "PipeLine.h"
#include "IRenderCommand.h"

RE::PipeLine RE::PipeLine::instance;

void RE::PipeLine::AddCommand(class IRenderCommand* cmd) {

	//auto& triangles = *cmd->triangles;
	//cmd->material->Apply(cmd->mat);

	//long offset = (long)(triangles.verts.data);
	//// vertex
	//glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	//// color
	//glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	//// texcood
	//glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	//glDrawElements(GL_TRIANGLES, triangles.indices.size, GL_UNSIGNED_SHORT, &(triangles.indices[0]));

	//return;

	auto& cmdTriangles = *cmd->triangles;

	if (commitMaterial != *cmd->material) {
		Commit();

		commitMaterial = *cmd->material;
	}

	auto& tarVerts = triangles.verts;
	auto& tarInds = triangles.indices;
	auto& cmdVerts = cmdTriangles.verts;
	auto& cmdInds = cmdTriangles.indices;

	auto countVert = tarVerts.size;
	auto& cmdMat = cmd->mat;

	while (tarVerts.size + cmdVerts.size >= tarVerts.capacity) {
		tarVerts.Resize(tarVerts.capacity * 2);
	}
	while (tarInds.size + cmdInds.size >= tarInds.capacity) {
		tarInds.Resize(tarInds.capacity * 2);
	}

	for (int i = 0; i < cmdVerts.size; ++i) {
		Triangle::Vertex& vert = cmdVerts.data[i];

		auto& cur = (tarVerts.data[tarVerts.size++] = vert);

		float x = cmdMat.a * cur.x + cmdMat.c * cur.y + cmdMat.x;
		float y = cmdMat.b * cur.x + cmdMat.d * cur.y + cmdMat.y;
		cur.x = x, cur.y = y;
	}

	auto cmdIndsData = cmdInds.data;
	for (int i = 0; i < cmdInds.size; ++i) {
		tarInds.data[tarInds.size++] = (*cmdIndsData++) + countVert;
	}
}

void RE::PipeLine::Commit() {

	commitMaterial.Apply(Affine::Identity);
	
	long offset = (long)(triangles.verts.data);
	// vertex
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, position) + offset));
	// color
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, color) + offset));
	// texcood
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid *)(offsetof(Triangle::Vertex, tex) + offset));

	glDrawElements(GL_TRIANGLES, triangles.indices.size, GL_UNSIGNED_SHORT, &(triangles.indices[0]));
	
	triangles.indices.Clear();
	triangles.verts.Clear();
}
