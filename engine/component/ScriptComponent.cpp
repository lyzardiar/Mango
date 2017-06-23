#include "ScriptComponent.h"
#include "engine/Engine.h"
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"


RE::ScriptComponent::ScriptComponent(const char* path) {
	this->path = path;

	Data data = FileUtils::getInstance()->getData(path);

	auto& state = Engine::instance.Lua;
	auto func = state.loadstring((char*)data.getBytes());
	
	Class = func.call<kaguya::LuaTable>();
	func = Class.getField<kaguya::LuaFunction>("new");
	if (!func.isNilref()) {
		Class = func(gameObject);
	}
}

void RE::ScriptComponent::Awake() {
	auto func = Class.getField<kaguya::LuaFunction>("Awake");
	if (!func.isNilref()) {
		func();
	}
}

void RE::ScriptComponent::Start() {

}

void RE::ScriptComponent::Update() {

}

void RE::ScriptComponent::OnDestroy() {

}

void RE::ScriptComponent::OnEnable() {

}

void RE::ScriptComponent::OnDisable() {

}

const char* RE::ScriptComponent::TypeName() {
	return path.c_str();
}
