#pragma once
#include "IComponent.h"
#include "renderer/Material.h"

namespace RE {
	class IRenderer : public IComponent {
	public:
		virtual void draw();

	public:
		Material material;
	};
}