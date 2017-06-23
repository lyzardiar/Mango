#pragma once

#include "Texture2D.h"
#include "PipeLine.h"

namespace RE {
	class Material {
	public:
		PipeLine*  pipeLine = nullptr;
		Texture2D* texture = nullptr;
	};
}