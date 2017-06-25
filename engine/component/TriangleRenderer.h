#pragma once
#include "IRenderer.h"
#include "renderer/Triangle.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		ComponentTypeName(TriangleRenderer)

		TriangleRenderer();

		void draw() override;

	public:
		Triangle triangles;
	};
}