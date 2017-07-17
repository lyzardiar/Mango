#include "Engine.h"
#include "object/GameObject.h"
#include "platform/Platform.h"
#include "component/Camera.h"
#include "object/Image.h"
#include "renderer/FrameBuffer.h"
#include "system/InputSystem.h"
#include "renderer/Texture2D.h"
#include "base/Array.h"

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

	Array<int> arr(10);
	for (int i = 0; i < 10; ++i) {
		arr.Push(i);
	}
	arr.Remove(5);
	for (int i = 0; i < arr.size; ++i) {
		Log("=> %d", arr.data[i]);
	}

	_fbo = new FrameBuffer();

	Lua.openlibs();
	initLuaEnginie();

	int count = 1;

	root->transform.ax = 0.5;
	root->transform.ay = 0.5;
	root->children.Resize(count);
	for (int i = 0; i < count; ++i) {
		auto img = new Image("image");
		root->AddChild(img);
	}

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
	root->transform.w = (float)camera.size.width;
	root->transform.h = (float)camera.size.height;

	root->transform.GetMat(nullptr);

	Texture2D::CurHandle = 0;
	PipeLine::CurProgram = 0;

	_fbo->Begin({ 0, camera.size.width, camera.size.height, 0 });

	root->Render(Affine::Identity);

	_fbo->End();
}

bool RE::Engine::Loop(float dt)
{
	Init();
	time.Update();

	static int speed = 2;
	if (root->transform.x > 500) speed = -2;
	if (root->transform.x < 0) speed = 2;

	root->transform.x += speed;

	Update(dt);
	Render();
	time.ResetElapse();

	return true;
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
