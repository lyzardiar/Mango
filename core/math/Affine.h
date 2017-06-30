#pragma once
#include <cstring>
#include "core/base/Types.h"
#include "renderer/GL/REGL.h"

namespace RE {
	class Affine {
	public:
		union {
			float m[6];
			struct { float a, b, c, d, x, y; };
		};

	public:
		Affine() { *this = Identity; }
		Affine(float va, float vb, float vc, float vd, float vx, float vy) { a = va, b = vb, c = vc, d = vd, x = vx, y = vy; }
		Affine(float* mat) { Set(mat); }

		void BindToGL(int handle) const {
			float mat[16];
			Get(mat);
			glUniformMatrix4fv(handle, (GLsizei)1, GL_FALSE, mat);
		}

		void Get(float* mat) const {
			mat[2] = mat[3] = mat[6] = mat[7] = mat[8] = mat[9] = mat[11] = mat[14] = 0.0f;
			mat[10] = mat[15] = 1.0f;
			mat[0] = a; mat[4] = c; mat[12] = x;
			mat[1] = b; mat[5] = d; mat[13] = y;
		}

		void SetIdentity() {
			*this = Identity;
		}

		void Set(float* mat) {
			a = m[0]; c = m[4]; x = m[12];
			b = m[1]; d = m[5]; y = m[13];
		}

		void Set(float va, float vb, float vc, float vd, float vx, float vy) { a = va, b = vb, c = vc, d = vd, x = vx, y = vy; }

		Affine Invert() {
			float det = 1 / (a * d - b * c);
			return Affine(
				det * d, -det * b, -det * c, det * a,
				det * (c * y - d * x), det * (b * x - a * y)
			);
		}

		Affine Translate(Vec2& pos) {
			return Affine(a, b, c, d, x + a * pos.x + c * pos.y, y + b * pos.x + d * pos.y);
		}

		Affine& TranslateSelf(Vec2& pos) {
			x += a * pos.x + c * pos.y;
			y += b * pos.x + d * pos.y;
			return *this;
		}

		Affine Scale(Vec2& scale) {
			return Affine(a*scale.x, b*scale.x, c*scale.y, d*scale.y, x, y);
		}

		Affine& ScaleSelf(Vec2& scale) {
			a *= scale.x; b *= scale.x;
			c *= scale.y; d *= scale.y;
			return *this;
		}

		Affine Rotate(float angle) {
			float fSin = sin(angle);
			float fCos = cos(angle);

			return Affine(
				a * fCos + c * fSin,
				b * fCos + d * fSin,
				c * fCos - a * fSin,
				d * fCos - b * fSin,
				x,
				y
			);
		}

		Affine& RotateSelf(float angle) {
			float fSin = sin(angle);
			float fCos = cos(angle);

			a = a * fCos + c * fSin;
			b = b * fCos + d * fSin;
			c = c * fCos - a * fSin;
			d = d * fCos - b * fSin;
			return *this;
		}

		Affine operator * (const Affine& af) {
			return Affine(
				a * af.a + b * af.c, a * af.b + b * af.d, //a,b
				c * af.a + d * af.c, c * af.b + d * af.d, //c,d
				x * af.a + y * af.c + af.x,               //x
				x * af.b + y * af.d + af.y				  //y
			);
		}

		Affine operator * (const Affine& af) const {
			return Affine(
				a * af.a + b * af.c, a * af.b + b * af.d, //a,b
				c * af.a + d * af.c, c * af.b + d * af.d, //c,d
				x * af.a + y * af.c + af.x,               //x
				x * af.b + y * af.d + af.y				  //y
			);
		}

		Affine& operator *= (const Affine& af) {
			a = a * af.a + b * af.c;
			b = a * af.b + b * af.d;
			c = c * af.a + d * af.c;
			d = c * af.b + d * af.d;
			x = x * af.a + y * af.c + af.x;
			y = x * af.b + y * af.d + af.y;
			return *this;
		}

		bool operator == (const Affine& af) {
			return a == af.a && b == af.b && c == af.c && d == af.d && x == af.x && y == af.y;
		}

	public:
		static Affine Identity;
	};
}
