#pragma once

#include "core/base/Types.h"
#include "engine/component/Transform.h"

namespace RE {
	class GameObject {
	public:
		GameObject();
		GameObject(const char* name);
		virtual ~GameObject();

	public:
		template<typename T>
		T& AddComponent();

		void Update(float dt);
		void Render();

	public:
		Transform& transform;
		std::string name;

	protected:
		void init();

	protected:
		std::vector<IComponent*> _components;
	};

}