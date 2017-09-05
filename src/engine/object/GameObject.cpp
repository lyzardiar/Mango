#include "GameObject.h"
#include "base/Macros.h"
#include "engine/component/IRenderer.h"
#include "engine/Engine.h"
#include "base/Utils.h"



RE::GameObject::GameObject() 
	: transform(*(new Transform()))
	, uuid(Utils::GenUUID().data)
{
	init();
}

RE::GameObject::GameObject(const char* name)
	: GameObject()
{
	this->name.assign(name);
}

RE::GameObject::~GameObject() {
	auto size = _components.size;
	for (int idx = 0; idx < size; ++idx) {
		auto comp = _components[idx];
		comp->OnDestroy();
		SAFE_DELETE(comp);
	}
	_components.Clear();
}

bool RE::GameObject::init() {
	$transform = &transform;
	_components.Push(&transform);
	return true;
}

void RE::GameObject::AddChild(GameObject* child) {
	children.Push(child);
	child->transform.parent = &transform;
	child->parent = this;
}

void RE::GameObject::RemoveChild(GameObject* child) {
	child->transform.parent = nullptr;
	children.Remove(child);
}

void RE::GameObject::Update(float dt) {
	int size = _components.size;
	auto comp = _components.data + 1;
	while (size-- > 1) {
		(*comp++)->Update(dt);
	}
}

void RE::GameObject::Render(const Affine& viewMat) {
	transform.GetMat(nullptr);
	transform.affineMV = transform.affine * viewMat;
	OnDraw(transform.affineMV);

	auto size = children.size;
	auto data = children.data;
	for (decltype(size) i = 0; i < size; ++i) {
		data[i]->Render(transform.affineMV);
	}
}

void RE::GameObject::OnDraw(const Affine& viewMat) {
	int size = _components.size;
	auto comp = _components.data;
	while (size-- > 0) {
		(*comp++)->draw(viewMat);
	}
}

void RE::GameObject::TransferUpdate(float dt) {
	Update(dt);

	for (int idx = 0; idx < children.size; ++idx) {
		children[idx]->TransferUpdate(dt);
	}
}
