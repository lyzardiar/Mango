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

void RE::GameObject::init() {
	$transform = &transform;
	_components.push_back(&transform);
}

void RE::GameObject::AddChild(GameObject* child) {
	children.push_back(child);
}

void RE::GameObject::Update(float dt) {

}

void RE::GameObject::Render(const Affine& viewMat) {
	Affine mv = viewMat * transform.affine;
	OnDraw(mv);
	for (auto child : children) {
		child->Render(mv);
	}
}

void RE::GameObject::OnDraw(const Affine& viewMat) {

}
