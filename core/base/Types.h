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