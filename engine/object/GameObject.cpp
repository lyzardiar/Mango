#include "GameObject.h"
#include "base/Macros.h"
#include "engine/component/IRenderer.h"
#include "engine/Engine.h"



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
	$transform = &transform;
	_components.push_back(&transform);
}

void RE::GameObject::Update(float dt) {

}

void RE::GameObject::Render() {
	for (auto& comp : _components) {
		auto renderer = dynamic_cast<IRenderer*>(comp);
		if (renderer != nullptr) {
			renderer->draw();
		}
	}
}
