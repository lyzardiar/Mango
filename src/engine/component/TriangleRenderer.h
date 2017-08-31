#pragma once
#include "IRenderer.h"
#include "renderer/Triangle.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		ComponentTypeName(TriangleRenderer)

		TriangleRenderer();

		void draw(const Affine& viewMat) override;
		virtual void setColor(Color col) override;
		
		virtual void OnGUI() override;
	public:
		Triangle triangles = Triangle::Default;
	};
}