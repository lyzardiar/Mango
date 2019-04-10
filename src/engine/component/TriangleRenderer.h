#pragma once
#include "IRenderer.h"
#include "renderer/Triangle.h"
#include "renderer/TriangleCommand.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		ComponentTypeName(TriangleRenderer)

		TriangleRenderer();

		void update();
		void draw(const Affine& viewMat) override;
		virtual void setColor(Color col) override;
		virtual void OnGUI() override;
	public:
		Triangle triangles = Triangle::Default;

		TriangleCommand renderCMD;
	};
}