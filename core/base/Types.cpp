#include "Types.h"

namespace RE {
	Vec2 Vec2::Zero = Vec2(0.0f, 0.0f);
	Vec2 Vec2::One = Vec2(1.0f, 1.0f);
	Vec2 Vec2::Left = Vec2(0.0f, 0.5f);
	Vec2 Vec2::Top = Vec2(0.5f, 1.0f);
	Vec2 Vec2::Right = Vec2(1.0f, 0.5f);
	Vec2 Vec2::Bottom = Vec2(0.5f, 0.0f);
	Vec2 Vec2::Center = Vec2(0.5f, 0.5f);
	
	Vec2 Vec2::SceneSize;
	Vec2 Vec2::HalfSceneSize;

	Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
	Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color Color::Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
	Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
	Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
	Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);

	Rect Rect::Zero = Rect(0.0f, 0.0f, 0.0f, 0.0f);
}

