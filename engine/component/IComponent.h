#pragma once

#include "core/base/Types.h"

#define ComponentTypeName(name) \
	virtual const char* TypeName() override { \
		return "\"" #name "\""; \
	}

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

		virtual const char* TypeName() {
			return "IComponent";
		}

	public:
		GameObject* gameObject = nullptr;
		bool canRemove = true;

	protected:
		bool _isStart = false;
		
	};
}
