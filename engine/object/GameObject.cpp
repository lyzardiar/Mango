#include "GameObject.h"
#include "base/Macros.h"



RE::GameObject::GameObject() 
	: transform(*(new Transform()))
{
	init();
}

RE::GameObject::GameObject(const char* name)
	: GameObject()
{
	this->name.assign(name);
}

RE::GameObject::~GameObject() {
	for (auto& comp : _components) {
		SAFE_DELETE(comp);
	}
	_components.clear();
}

void RE::GameObject::init() {
	_components.push_back(&transform);
}

template<typename T>
T& RE::GameObject::AddComponent() {
	static_assert(std::is_convertible<T*, IComponent*>::value, "Invalid Type for GameObject::AddComponent!");

	auto Ttype = typeid(T);
	for (auto& comp : _components) {
		if (Ttype == typeid(*comp)) {
			return *comp;
		}
	}

	T* ret = new T();
	ret->Awake();

	return *ret;
}

void RE::GameObject::Update(float dt) {

}

void RE::GameObject::Render() {

}
