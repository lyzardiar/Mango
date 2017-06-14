#pragma once

#include "Material.h"

namespace RE {
	class IRenderer {
	public:
		virtual void draw() = 0;
	};
}