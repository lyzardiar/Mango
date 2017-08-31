#pragma once
#include "IComponent.h"
#include "renderer/Material.h"
#include "math/Affine.h"

namespace RE {
	class IRenderer : public IComponent {
	public:
		virtual void draw(const Affine& viewMat);
		virtual void setColor(Color col);
	public:
		Material material;
		Color color = Color::White;
	};
}