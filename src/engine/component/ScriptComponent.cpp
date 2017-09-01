#include "ScriptComponent.h"
#include "engine/Engine.h"
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"


RE::ScriptComponent::ScriptComponent(const char* path, class GameObject* go) {
	this->path = path;
	this->gameObject = go;

	auto bn = strrchr(path, '/');	
	if (bn != nullptr) {
		this->baseName = bn+1;
	}
	else {
		this->baseName = path;
	}
	

	Data data = FileUtils::getInstance()->getData(path);

	auto& state = Engine::instance.Lua;
	auto func = state.loadstring((char*)data.getBytes());
	
	Class = func.call<kaguya::LuaTable>();
	func = Class.getField<kaguya::LuaFunction>("new");
	if (!func.isNilref()) {
		Class = func(Class, go);

		luaFuncs[FuncType::AWAKE]		= Class.getField<kaguya::LuaFunction>("Awake");
		luaFuncs[FuncType::START]		= Class.getField<kaguya::LuaFunction>("Start");
		luaFuncs[FuncType::UPDATE]		= Class.getField<kaguya::LuaFunction>("Update");
		luaFuncs[FuncType::ONDESTROY]	= Class.getField<kaguya::LuaFunction>("OnDestroy");
		luaFuncs[FuncType::ONENABLE]	= Class.getField<kaguya::LuaFunction>("OnEnable");
		luaFuncs[FuncType::ONDISABLE]	= Class.getField<kaguya::LuaFunction>("OnDisable");
	}
}

RE::ScriptComponent::~ScriptComponent() {
	
}

void RE::ScriptComponent::Awake() {
	auto& func = luaFuncs[FuncType::AWAKE];
	if (!func.isNilref()) {
		func(Class);
	}
}

void RE::ScriptComponent::Start() {
	auto& func = luaFuncs[FuncType::START];
	if (!func.isNilref()) {
		func(Class);
	}
}

void RE::ScriptComponent::Update(float dt) {
	auto& func = luaFuncs[FuncType::UPDATE];
	if (!func.isNilref()) {
		func(Class, dt);
	}
}

void RE::ScriptComponent::OnDestroy() {
	auto& func = luaFuncs[FuncType::ONDESTROY];
	if (!func.isNilref()) {
		func(Class);
	}
}

void RE::ScriptComponent::OnEnable() {
	auto& func = luaFuncs[FuncType::ONENABLE];
	if (!func.isNilref()) {
		func(Class);
	}
}

void RE::ScriptComponent::OnDisable() {
	auto& func = luaFuncs[FuncType::ONDISABLE];
	if (!func.isNilref()) {
		func(Class);
	}
}

const char* RE::ScriptComponent::TypeName() {
	return path.c_str();
}

void RE::ScriptComponent::Reload() {

}
