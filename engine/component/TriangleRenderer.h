#pragma once
#include "IRenderer.h"
#include "renderer/Triangle.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		ComponentTypeName(TriangleRenderer)

		TriangleRenderer();

		void draw() override;
		void draw(const Affine& viewMat) override;

	public:
		Triangle triangles = Triangle::Default;
	};
}