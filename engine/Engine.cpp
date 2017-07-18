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

	glEnable(GL_BLEND);

	_fbo = new FrameBuffer();

	Lua.openlibs();
	initLuaEnginie();

	int count = 1;

	root->children.Resize(count);
	for (int i = 0; i < count; ++i) {
		auto img = new Image("image");
		root->AddChild(img);
		auto img2 = new Image("image");
		img->AddChild(img2);
	}

	_isInited = true;
	return true;
}

void RE::Engine::Update(float dt) {
	if (!Lua["Engine"]["Update"].isNilref()) {
		Lua["Engine"]["Update"](dt);
	}
	root->TransferUpdate(dt);
}

void RE::Engine::Render() {
	root->transform.w = (float)camera.size.width;
	root->transform.h = (float)camera.size.height;
	root->transform.x = -root->transform.w / 2.0f;
	root->transform.y = -root->transform.h / 2.0f;
	root->transform.ax = 0;
	root->transform.ay = 0;

	Vec2::SceneSize.width = camera.size.width;
	Vec2::SceneSize.height = camera.size.height;
	Vec2::HalfSceneSize = Vec2::SceneSize / 2.0f;


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

	//root->transform.x += speed;

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

void RE::Engine::SelectObjects(class GameObject** objs, int count) {
	selectedObjs.Clear();
	for (int idx = 0; objs != nullptr && idx < count; ++idx) {
		selectedObjs.Push(objs[idx]);
	}
}

void RE::Engine::initLuaEnginie() {
	RE::LuaWrapper::registerLua();

	if (Lua["Engine"].isNilref())
		Lua["Engine"] = kaguya::NewTable();

	Lua.dofile("scripts/Engine.lua");
}
