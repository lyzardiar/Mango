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
		virtual void Update(float dt) {}
		virtual void OnDestroy() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}

		virtual const char* TypeName() {
			return "IComponent";
		}

		virtual void OnGUI() {}

	public:
		GameObject* gameObject = nullptr;
		bool canRemove = true;
		bool isStart = false;

	protected:
		
	};
}
