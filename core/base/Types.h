#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <stack>

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

namespace RE {

	class Vec2 {
	public:
		union { float x; float u; float width; };
		union { float y; float v; float height; };

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

		void Set(float v1, float v2) { x = v1, y = v2; }

	public:
		static Vec2 Zero;
		static Vec2 One;
		static Vec2 Left;
		static Vec2 Top;
		static Vec2 Right;
		static Vec2 Bottom;
		static Vec2 Center;
	};

	class Vec2i : public Vec2 {
	public:
		union { int x; int u; int width; };
		union { int y; int v; int height; };

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

	public:
		static Rect Zero;

	};

	template<int N>
	class StaticString {
	public:
		StaticString() {  }
		StaticString(const char* buff) {
			memcpy(data, buff, strlen(buff));
		}
		StaticString(const char* buff, int len) {
			memcpy(data, buff, len);
		}
		StaticString(std::string& rhs) {
			memcpy(data, rhs.c_str(), rhs.size());
		}

		bool operator == (const StaticString& rhs) {
			return strcmp(data, rhs.data) == 0;
		}
		bool operator == (const char* buff) {
			return strcmp(data, buff) == 0;
		}
		bool operator == (std::string& rhs) {
			return strcmp(data, rhs.c_str()) == 0;
		}

		bool operator != (const StaticString& rhs) {
			return strcmp(data, rhs.data) != 0;
		}
		bool operator != (const char* buff) {
			return strcmp(data, buff) != 0;
		}
		bool operator != (std::string& rhs) {
			return strcmp(data, rhs.c_str()) != 0;
		}
	public:
		char data[N] = {0};
	};
}