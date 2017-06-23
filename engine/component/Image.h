#pragma once
#include "IComponent.h"
#include "engine/component/TriangleRenderer.h"

namespace RE {
	class Image : public IComponent {
	public:
		ComponentTypeName(Awake)
		Image();

	public:

		virtual void Awake() override;

	public:
		IRenderer* renderer = nullptr;
	};
}