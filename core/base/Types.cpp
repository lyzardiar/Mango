#include "Types.h"
#include "renderer/GL/REGL.h"

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



	const BlendFunc BlendFunc::ALPHA_PREMULTIPLIED = BlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	const BlendFunc BlendFunc::ALPHA_NON_PREMULTIPLIED = BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const BlendFunc BlendFunc::ADDITIVE = BlendFunc(GL_SRC_ALPHA, GL_ONE);

	const BlendFunc BlendFunc::DISABLE = BlendFunc(GL_ONE, GL_ZERO);

}

