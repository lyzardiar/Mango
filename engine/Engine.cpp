#include "Engine.h"
#include "object/GameObject.h"
#include "platform/Platform.h"
#include "component/Camera.h"
#include "object/Image.h"
#include "renderer/FrameBuffer.h"
#include "system/InputSystem.h"

RE::Engine RE::Engine::instance;

RE::Engine::Engine()
	: L (luaL_newstate())
	, Lua(L)
	, camera(*((new GameObject("Root"))->AddComponent<Camera>()))
	, input(*(new InputSystem()))
{
}

RE::Engine::~Engine() {

}

bool RE::Engine::Init() {
	if (_isInited) return true;
	root = camera.gameObject;

	_fbo = new FrameBuffer();

	Lua.openlibs();
	initLuaEnginie();

	root->transform.ax = 0.5;
	root->transform.ay = 0.5;

	auto img = new Image("image");
	root->AddChild(img);

	_isInited = true;
	return true;
}

void RE::Engine::Update(float dt) {
	if (!Lua["Engine"]["Update"].isNilref()) {
		Lua["Engine"]["Update"](dt);
	}
	root->Update(dt);
}

void RE::Engine::Render() {
	root->transform.w = camera.size.width;
	root->transform.h = camera.size.height;

	root->transform.GetMat(nullptr);

	_fbo->Begin({ 0, camera.size.width, camera.size.height, 0 });

	root->Render(Affine::Identity);

	_fbo->End();
}

void RE::Engine::Loop() {
	Init();
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

GLuint RE::Engine::GetTextureHandle() {
	return _fbo->GetTextureHandle();
}

void RE::Engine::initLuaEnginie() {
	RE::LuaWrapper::registerLua();

	if (Lua["Engine"].isNilref())
		Lua["Engine"] = kaguya::NewTable();

	Lua.dofile("scripts/Engine.lua");
}
