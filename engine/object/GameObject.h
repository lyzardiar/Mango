#pragma once

#include "core/base/Types.h"
#include "platform/FileUtils.h"
#include "engine/component/Transform.h"
#include "engine/component/ScriptComponent.h"

namespace RE {
	class GameObject {
	public:
		GameObject();
		GameObject(const char* name);
		virtual ~GameObject();

	public:
		template<typename T>
		T* AddComponent(T* unused = nullptr) {
			static_assert(std::is_convertible<T*, IComponent*>::value, "Invalid Type for GameObject::AddComponent!");

			for (auto& comp : _components) {
				if (typeid(T) == typeid(*comp)) {
					return (T*)comp;
				}
			}

			T* ret = new T();
			ret->gameObject = this;
			ret->Awake();

			_components.push_back(ret);

			return ret;
		}

		template<typename T>
		T* GetComponent() {
			for (auto& comp : _components) {
				if (typeid(T) == typeid(*comp)) {
					return (T*)comp;
				}
			}
			return nullptr;
		}

		IComponent* GetComponent(const char* name) {
			if (name == nullptr) return nullptr;
			for (auto& comp : _components) {
				if (strcmp(comp->TypeName(), name) == 0) {
					return comp;
				}
			}
			return nullptr;
		}

		IComponent* AddComponent(const char* path) {
			if (FileUtils::getInstance()->isFileExists(path)) {
				for (auto& comp : _components) {
					if (strcmp(comp->TypeName(), path) == 0) {
						return comp;
					}
				}

				ScriptComponent* scomp = new ScriptComponent(path);
				scomp->gameObject = this;
				scomp->Awake();
				_components.push_back(scomp);
				return scomp;
			}
			return nullptr;
		}

		void Update(float dt);
		void Render();

	public:
		Transform& transform;
		Transform* $transform;

		std::string name;

	protected:
		void init();

	protected:
		std::vector<IComponent*> _components;

	};
}