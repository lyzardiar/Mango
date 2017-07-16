#pragma once

#include "Texture2D.h"
#include "PipeLine.h"
#include "core/math/Affine.h"

namespace RE {
	class Material {
	public:
		void Apply(const Affine& viewMat);
	public:
		PipeLine*  pipeLine = nullptr;
		Texture2D* texture = nullptr;
	};
}