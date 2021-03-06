#pragma once

#include "Texture2D.h"
#include "GLProgram.h"
#include "core/math/Affine.h"

namespace RE {
	class Material {
	public:
		Material();

		void Apply(const Affine& viewMat);
		void OnGUI();
		UI32 GetID();

		inline bool operator != (const Material& rhs) {
			return !(*this == rhs);
		}
		inline bool operator == (const Material& rhs) {
			if (program == rhs.program && blend == rhs.blend && texture == rhs.texture) {
				return true;
			}
			return false;
		}

		void SetGLProgram(const char* name);
		void SetTexturePath(const char* name);

	public:
		GLProgram*  program = nullptr;
		Texture2D* texture = nullptr;
		BlendFunc blend = BlendFunc::ALPHA_PREMULTIPLIED;
	};
}