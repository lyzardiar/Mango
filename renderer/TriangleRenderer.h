#pragma once

#include "IRenderer.h"

namespace RE {
	class TriangleRenderer : public IRenderer {
	public:
		TriangleRenderer() {}

		void draw() override;
	};
}