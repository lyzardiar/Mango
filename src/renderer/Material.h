#pragma once

#include "Texture2D.h"
#include "Shader.h"
#include "core/math/Affine.h"

namespace RE {
	class Material {
	public:
		void Apply(const Affine& viewMat);
		void OnGUI();
	public:
		Shader*  shader = nullptr;
		Texture2D* texture = nullptr;
		BlendFunc blend = BlendFunc::ALPHA_PREMULTIPLIED;
	};
}