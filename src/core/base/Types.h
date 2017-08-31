#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>

typedef char I8;
typedef short I16;
typedef int I32;
typedef long long I64;

typedef unsigned char UI8;
typedef unsigned short UI16;
typedef unsigned int UI32;
typedef unsigned long long UI64;

typedef float F32;
typedef double F64;

#define Min(a, b) (a) > (b) ? (b) : (a)
#define Max(a, b) (a) > (b) ? (a) : (b)

namespace RE {

	class Vec2 {
	public:
		union { float x; float u; float width; float w; };
		union { float y; float v; float height; float h; };

		Vec2() { x = y = 0.0f; }
		Vec2(float a, float b) : x(a), y(b) {}
		Vec2 operator + (const Vec2& other) { return Vec2(x + other.x, y + other.y); }
		Vec2 operator - (const Vec2& other) { return Vec2(x - other.x, y - other.y); }
		Vec2 operator * (float s) { return Vec2(x * s, y * s); }
		Vec2 operator / (float s) { return Vec2(x / s, y / s); }
		Vec2& operator += (const Vec2& other) { x += other.x; y += other.y; return *this; }
		Vec2& operator -= (const Vec2& other) { x -= other.x; y -= other.y; return *this; }
		Vec2& operator *= (float s) { x *= s; y *= s; return *this; }
		Vec2& operator /= (float s) { x /= s; y /= s; return *this; }
		bool operator != (const Vec2& rhv) { return x != rhv.x || y != rhv.y; }
		bool operator == (const Vec2& rhv) { return x == rhv.x && y == rhv.y; }

		Vec2& operator = (const Vec2& rhs) { x = rhs.x; y = rhs.y; return *this; }

		void Set(float v1, float v2) { x = v1, y = v2; }

	public:
		static Vec2 Zero;
		static Vec2 One;
		static Vec2 Left;
		static Vec2 Top;
		static Vec2 Right;
		static Vec2 Bottom;
		static Vec2 Center;
		static Vec2 SceneSize;
		static Vec2 HalfSceneSize;
	};

	class Vec2i {
	public:
		union { int x; int u; int width; int w; };
		union { int y; int v; int height; int h; };

		Vec2i() { x = y = 0; }
		Vec2i(int a, int b) : x(a), y(b) {}

	};

	class Tex2 : public Vec2 {};

	class Color {
	public:
		union { float x; float r; };
		union { float y; float g; };
		union { float z; float b; };
		union { float w; float a; };

		Color() { x = y = z = w = 1.0f; }
		Color(float v1, float v2, float v3, float v4) { x = v1, y = v2, z = v3, w = v4; }
		void Set(float v1, float v2, float v3, float v4) { x = v1, y = v2, z = v3, w = v4; }

	public:
		static Color Black;
		static Color White;
		static Color Yellow;
		static Color Red;
		static Color Green;
		static Color Blue;
	};

	class Rect {
	public:
		union {
			struct {
				Vec2 origion; Vec2 size;
			};
			struct { float x, y, width, height; };
		};

		Rect(float vx, float vy, float vw, float vh) : x(vx), y(vy), width(vw), height(vh) { }

		bool Contains(const Vec2& pos) {
			float minx = x;
			float maxx = x + width;
			float miny = y;
			float maxy = y + height;

			if (minx > maxx) std::swap(minx, maxx);
			if (miny > maxy) std::swap(miny, maxy);

			return pos.x >= minx && pos.x <= maxx && pos.y >= miny && pos.y <= maxy;
		}
	public:
		static Rect Zero;

	};

	struct BlendFunc {
		UI32 src;
		UI32 dst;

		BlendFunc() : src(0), dst(0) {}
		BlendFunc(UI32 $src, UI32 $dst)
			: src($src)
			, dst($dst)
		{ }

		bool operator == (const BlendFunc& other) {
			return src == other.src && dst == other.dst;
		}

		static const BlendFunc ALPHA_PREMULTIPLIED;
		static const BlendFunc ALPHA_NON_PREMULTIPLIED;
		static const BlendFunc ADDITIVE;
		static const BlendFunc DISABLE;
	};
}