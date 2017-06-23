#pragma once

#include "base/Types.h"
#include <vector>

struct Triangle {
	struct Vertex {
		float position[3];
		float color[4];
		float tex[2];
	};

	std::vector<Vertex> verts = std::vector<Vertex>(4);
	std::vector<UI16> indices = std::vector<UI16>(4);

	static Triangle Default;
};