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
	for (auto& comp : _components) {
		comp->OnDestroy();
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
	child->transform.parent = &transform;
	child->parent = this;
}

void RE::GameObject::RemoveChild(GameObject* child) {
	child->transform.parent = nullptr;
	children.Remove(child);
}

void RE::GameObject::Update(float dt) {
	for (auto& comp : _components) {
		if (comp->isStart) {
			comp->Update(dt);
		}
		else {
			comp->Start();
			comp->isStart = true;
		}
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
	for (auto& comp : _components) {
		auto renderer = dynamic_cast<IRenderer*>(comp);
		if (renderer != nullptr) {
			renderer->draw(viewMat);
		}
	}
}

void RE::GameObject::TransferUpdate(float dt) {
	Update(dt);

	for (int idx = 0; idx < children.size; ++idx) {
		children[idx]->TransferUpdate(dt);
	}
}
