#include "ScriptComponent.h"
#include "engine/Engine.h"
#include "engine/object/GameObject.h"
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"

#ifdef _WIN32
#include "game/editor/manager/ScriptManager.h"
#endif


RE::ScriptComponent::ScriptComponent(Path path, class GameObject* go) {
	this->path = path;
	this->gameObject = go;

	auto bn = strrchr(path.data.c_str(), '/');	
	if (bn != nullptr) {
		this->baseName = bn+1;
	}
	else {
		this->baseName = path.data;
	}
	
	Class = Load(path);

	auto func = Class.getField<kaguya::LuaFunction>("new");
	if (!func.isNilref()) {
		Class = func(Class, gameObject);
	}
	else {
		Class = kaguya::LuaTable();
	}

	setupFuncs();

#ifdef _WIN32
	ScriptManager::instance.Add(this);
#endif
}

RE::ScriptComponent::~ScriptComponent() {
#ifdef _WIN32
	ScriptManager::instance.Remove(this);
#endif
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
	if (!isStart) {
		Start();
		isStart = true;
		return;
	}

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

	Class = kaguya::LuaTable();
	setupFuncs();
}

void RE::ScriptComponent::OnEnable() {
	isEnabled = true;
	auto& func = luaFuncs[FuncType::ONENABLE];
	if (!func.isNilref()) {
		func(Class);
	}
}

void RE::ScriptComponent::OnDisable() {
	isEnabled = false;
	auto& func = luaFuncs[FuncType::ONDISABLE];
	if (!func.isNilref()) {
		func(Class);
	}
}

const char* RE::ScriptComponent::TypeName() {
	return path.data.c_str();
}

void RE::ScriptComponent::Reload() {
	kaguya::LuaTable cls = Load(path);

	if (cls.isNilref()) return;
	
	auto keys = cls.keys<std::string>();

	Clear();

	for (auto& key : keys) {
		if (Class[key].isNilref()) {
			Class[key] = cls[key];
		}
		else {
			switch (cls[key].type())
			{
			case LUA_TSTRING: {
				break;
			}
			case LUA_TBOOLEAN: {
				break;
			}
			case LUA_TNUMBER: {
				break;
			}
			case LUA_TFUNCTION: {
				Class[key] = cls[key];
				break;
			}
			case LUA_TNIL: {
				break;
			}
			default:
				break;
			}
		}
	}

	setupFuncs();
}

kaguya::LuaTable RE::ScriptComponent::Load(Path& path) {
	Data data = FileUtils::getInstance()->GetData(path);

	auto& state = Engine::instance.Lua;
	auto func = state.loadstring((char*)data.getBytes());

	if (func.isNilref()) return kaguya::LuaTable();

	kaguya::LuaTable cls = func.call<kaguya::LuaTable>();
	return cls;
}

void RE::ScriptComponent::Clear() {

}

void RE::ScriptComponent::setupFuncs() {
	luaFuncs[FuncType::AWAKE] = Class.getField<kaguya::LuaFunction>("Awake");
	luaFuncs[FuncType::START] = Class.getField<kaguya::LuaFunction>("Start");
	luaFuncs[FuncType::UPDATE] = Class.getField<kaguya::LuaFunction>("Update");
	luaFuncs[FuncType::ONDESTROY] = Class.getField<kaguya::LuaFunction>("OnDestroy");
	luaFuncs[FuncType::ONENABLE] = Class.getField<kaguya::LuaFunction>("OnEnable");
	luaFuncs[FuncType::ONDISABLE] = Class.getField<kaguya::LuaFunction>("OnDisable");
}
