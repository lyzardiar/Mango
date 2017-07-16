#include "GameObject.h"
#include "base/Macros.h"
#include "engine/component/IRenderer.h"
#include "engine/Engine.h"
#include "base/Utils.h"



RE::GameObject::GameObject() 
	: transform(*(new Transform()))
	, uuid(Utils::GenUUID().data)
	, self(this)
{
	init();
}

RE::GameObject::GameObject(const char* name)
	: GameObject()
{
	self->name.assign(name);
}

RE::GameObject::~GameObject() {
	for (auto& comp : _components) {
		SAFE_DELETE(comp);
	}
	_components.clear();
}

bool RE::GameObject::init() {
	$transform = &transform;
	_components.push_back(&transform);
	return true;
}

void RE::GameObject::AddChild(GameObject* child) {
	children.Push(child);
}

void RE::GameObject::Update(float dt) {

}

void RE::GameObject::Render(const Affine& viewMat) {
	Affine mv = viewMat * transform.affine;
	OnDraw(mv);

	auto size = children.size;
	auto data = children.data;
	for (decltype(size) i = 0; i < size; ++i) {
		data[i]->Render(mv);
	}
}

void RE::GameObject::OnDraw(const Affine& viewMat) {

}
