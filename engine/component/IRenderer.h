#pragma once
#include "IComponent.h"
#include "renderer/Material.h"
#include "math/Affine.h"

namespace RE {
	class IRenderer : public IComponent {
	public:
		virtual void draw();
		virtual void draw(const Affine& viewMat);

	public:
		Material material;
	};
}