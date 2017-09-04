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

	auto countVert = triangles.verts.size;
	auto& cmdMat = cmd->mat;

	for (int i = 0; i < cmdTriangles.verts.size; ++i) {
		auto& vert = cmdTriangles.verts[i];

		auto& cur = triangles.verts.Push(cmdTriangles.verts[i]);

		Vec2& pos = *(Vec2*)&cur.position;
		pos = cmdMat.Apply(pos);
	}

	//triangles.verts.Push(cmdTriangles.verts);

	for (int i = 0; i < cmdTriangles.indices.size; ++i) {
		triangles.indices.Push(cmdTriangles.indices[i] + countVert);
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
