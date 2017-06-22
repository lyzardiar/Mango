#pragma once
#include "IComponent.h"
#include "core/math/Affine.h"

namespace RE {
	class Transform : public IComponent {
	public:
		Transform() : position(Vec2::Zero) {}

		void GetMat(float* mat) {
			float cf = cosf(roration);
			float sf = sinf(roration);

			if (anchor != Vec2::Zero) {
				x += cf * -anchor.x * size.width * scale.x + -sf * -anchor.y * size.height * scale.y;
				y += sf * -anchor.x * size.width * scale.x + cf * -anchor.y * size.height * scale.y;
			}

			_affine.Set(cf * scale.x, sf * scale.x, -sf * scale.y, cf * scale.y, x, y);
			_affine.Get(mat);
		}

	public:
		union {
			struct { float x; float y; };
			Vec2 position;
		};
		float roration = 0.0f;
		Vec2 scale = Vec2::One;
		Vec2 size = Vec2(100.0f, 100.0f);
		Vec2 anchor = Vec2::Zero;

	protected:
		Affine _affine;
	};
}