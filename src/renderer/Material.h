#pragma once

#include "Texture2D.h"
#include "GLProgram.h"
#include "core/math/Affine.h"

namespace RE {
	class Material {
	public:
		void Apply(const Affine& viewMat);
		void OnGUI();
		UI32 GetID();

		bool operator == (const Material&);
		bool operator != (const Material&);
	public:
		GLProgram*  program = nullptr;
		Texture2D* texture = nullptr;
		BlendFunc blend = BlendFunc::ALPHA_PREMULTIPLIED;
	};
}