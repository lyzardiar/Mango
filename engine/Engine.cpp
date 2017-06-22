#include "Engine.h"
#include "object/GameObject.h"
#include "platform/Platform.h"

RE::Engine RE::Engine::instance;

RE::Engine::Engine()
	: L (luaL_newstate())
	, Lua(L)
	, _root(new GameObject("Root"))
{
	Init();
}

RE::Engine::~Engine() {

}

bool RE::Engine::Init() {
	Lua.openlibs();
	initLuaEnginie();

	return true;
}

void RE::Engine::Update(float dt) {
	if (!Lua["Engine"]["Update"].isNilref()) {
		Lua["Engine"]["Update"](dt);
	}
	_root->Update(dt);
}

void RE::Engine::Render() {
	_root->Render();
}

void RE::Engine::Loop() {
	time.Update();

	if (time.elapse >= _interval) {
		Update((float)time.elapse);
		Render();
		time.ResetElapse();
	}
}

void RE::Engine::SetFPS(int fps) {
	_fps = fps;
	_interval = 1.0f / _fps;
}

void RE::Engine::initLuaEnginie() {
	Lua["Engine"] = kaguya::NewTable();

	Lua.dofile("scripts/Engine.lua");
}
