#pragma once

#include "core/base/String.h"
#include "core/base/Array.h"
#include "platform/FileUtils.h"
#include "engine/component/Transform.h"
#include "engine/component/ScriptComponent.h"

namespace RE {
	class GameObject {
	public:
		GameObject();
		GameObject(const char* name);
		virtual ~GameObject();
		virtual const char* ClassName() { return "GameObject"; }

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
		void RemoveComponent(T* unused = nullptr) {
			static_assert(std::is_convertible<T*, IComponent*>::value, "Invalid Type for GameObject::AddComponent!");

			for (auto iter = _components.begin(); iter != _components.end(); ++iter) {
				auto comp = *iter;
				if (typeid(T) == typeid(*comp)) {
					comp->OnDestroy();
					_components.erase(iter);
					break;
				}
			}
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

		IComponent* AddScriptComponent(const char* path) {
			if (FileUtils::getInstance()->isFileExists(path)) {
				for (auto& comp : _components) {
					if (strcmp(comp->TypeName(), path) == 0) {
						return comp;
					}
				}

				ScriptComponent* scomp = new ScriptComponent(path, this);
				scomp->Awake();
				_components.push_back(scomp);
				return scomp;
			}
			return nullptr;
		}

		void RemoveComponentImt(IComponent* rmComp) {
			for (auto iter = _components.begin(); iter != _components.end(); ++iter) {
				auto comp = *iter;
				if (comp == rmComp) {
					comp->OnDestroy();
					_components.erase(iter);
					break;
				}
			}
		}

		void RemoveScriptComponent(const char* path) {
			for (auto iter = _components.begin(); iter != _components.end(); ++iter) {
				auto comp = *iter;
				if (strcmp(comp->TypeName(), path) == 0) {
					comp->OnDestroy();
					_components.erase(iter);
					break;
				}
			}
		}

		virtual void AddChild(GameObject* child);
		virtual void RemoveChild(GameObject* child);

		virtual void Update(float dt);
		virtual void Render(const Affine& viewMat);
		virtual void OnDraw(const Affine& viewMat);

		virtual void OnGUI();

		void TransferUpdate(float dt);
	public:
		Transform& transform;
		// export for Lua
		Transform* $transform;
		GameObject* parent = nullptr;
		
		std::string name;
		StaticString<65> uuid;
		Array<GameObject*> children;

	protected:
		virtual bool init();

	protected:
		std::vector<IComponent*> _components;
	};
}