#pragma once
#include "IComponent.h"
#include "core/math/Affine.h"
#include "renderer/GL/REGL.h"

namespace RE {
	class Transform : public IComponent {
	public:
		ComponentTypeName(Transform)

		Transform()
			: position(Vec2::Zero)
			, scale(Vec2::One)
			, size(100.0f, 100.0f)
			, anchor(Vec2::Zero)
		{
			canRemove = false;
		}

		void GetMat(float* mat) {
			float rad = rotation / 180.0f * 3.14159565358f;
			float cf = cosf(rad);
			float sf = sinf(rad);
			float tx = x;
			float ty = y;
			if (anchor != Vec2::Zero) {
				tx += cf * -ax * w * sx + -sf * -ay * h * sy;
				ty += sf * -ax * w * sx + cf  * -ay * h * sy;
			}

			affine.Set(cf * sx, sf * sx, -sf * sy, cf * sy, tx, ty);
			mat ? affine.Get(mat) : void();
		}

		void BindToGL(int handle) {
			float matm[16];
			GetMat(matm);
			glUniformMatrix4fv(handle, (GLsizei)1, GL_FALSE, matm);
		}

		Affine operator * (const Affine& rhs) {
			return affine * rhs;
		}

	public:
		union {
			struct { float x; float y; };
			Vec2 position;
		};
		union {
			struct { float sx; float sy; };
			Vec2 scale;
		};
		union {
			struct { float w; float h; };
			Vec2 size;
		};
		union {
			struct { float ax; float ay; };
			Vec2 anchor;
		};

		float rotation = 0.0f;

		Affine affine;
	};
}