#pragma once
#include "Triangle.h"
#include "Material.h"

namespace RE {
	class IRenderCommand {
	public:
		virtual void Commit() = 0;

	public:
		Triangle*	triangles;
		Material*	material;
		Affine		mat;
	};
}
