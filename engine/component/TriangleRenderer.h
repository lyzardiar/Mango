#pragma once
#include "IRenderer.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		ComponentTypeName(TriangleRenderer)

		TriangleRenderer();

		void draw() override;
	};
}