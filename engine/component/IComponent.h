#pragma once

#include "core/base/Types.h"

namespace RE {

	class GameObject;

	class IComponent {
		friend class GameObject;

	public:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDestroy() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}

		virtual std::string TypeName() {
			return typeid(*this).name();
		}

	public:
		GameObject* gameObject = nullptr;

	protected:
		bool _isStart = false;
		
	};
}
